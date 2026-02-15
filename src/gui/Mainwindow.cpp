#include "include/gui/Mainwindow.h"
#include "include/models/Circle.h" 
#include "include/models/Rectangle.h"   
#include "include/gui/Canvas.h"
#include "include/utils/svg_parser.h"
#include <QDebug>
#include <QToolBar>
#include <QAction>
#include <QCloseEvent>
#include <QColorDialog>
#include <QSpinBox>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog> // For opening file explorer
#include <QMessageBox> // For error messages



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(800, 600);
    canvas = new Canvas(this);
    setCentralWidget(canvas);

    createMenus();

    // currentFile = "./../sample.svg";

    // Initialize as empty
    currentFile = ""; 
    

    // --- TOOLBAR SETUP ---
    QToolBar *toolbar = addToolBar("Tools");
    toolbar->setIconSize(QSize(32, 32)); // Make icons a bit larger/nicer
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    auto addTool = [&](const QString &name, const QString &iconPath, ToolType type) {
        // The path starts with ":/" to indicate a Qt Resource
        QAction *act = toolbar->addAction(QIcon(iconPath), name);
        act->setToolTip(name); // Show text when hovering
        connect(act, &QAction::triggered, [this, type]() {
            canvas->setTool(type);
        });
    };

    addTool("Select",  ":/resources/icons/cursor.png" , ToolType::Select);
    toolbar->addSeparator(); toolbar->addSeparator();

    addTool("Circle",  ":/resources/icons/circle.png" , ToolType::Circle);
    addTool("Rectangle",  ":/resources/icons/rectangle.png" , ToolType::Rectangle);
    addTool("Rounded Rectangle",  ":/resources/icons/roundedrectangle.png" , ToolType::RoundedRect);
    addTool("Line",  ":/resources/icons/line.png" , ToolType::Line);
    addTool("Hexagon",  ":/resources/icons/hexagon.png" , ToolType::Hexagon);
    addTool("Free Hand",  ":/resources/icons/freehand.png" , ToolType::Freehand);
    addTool("Text",  ":/resources/icons/text.png" , ToolType::Text);

    toolbar->addSeparator(); toolbar->addSeparator();
    

    // 2. STROKE COLOR BUTTON
    QAction *actStrokeColor = toolbar->addAction(QIcon(":/resources/icons/stroke_color.png"), "Stroke Color");
    connect(actStrokeColor, &QAction::triggered, [this]() {
        QColor color = QColorDialog::getColor(Qt::black, this, "Select Stroke Color");
        if (color.isValid()) {
            canvas->setStrokeColor(color.name().toStdString());
        }
    });

    // 3. FILL COLOR BUTTON
    QAction *actFillColor = toolbar->addAction(QIcon(":/resources/icons/fill_color.png"), "Fill Color");
    connect(actFillColor, &QAction::triggered, [this]() {
        QColor color = QColorDialog::getColor(Qt::white, this, "Select Fill Color");
        if (color.isValid()) {
            canvas->setFillColor(color.name().toStdString());
        }
    });

    // 4. NO FILL BUTTON
    QAction *actNoFill = toolbar->addAction("No Fill");
    connect(actNoFill, &QAction::triggered, [this]() {
        canvas->setFillColor("none");
    });

    toolbar->addSeparator(); toolbar->addSeparator();

    // 5. THICKNESS SPINNER
    toolbar->addWidget(new QLabel(" Width: ")); 
    QSpinBox *spinWidth = new QSpinBox();
    spinWidth->setRange(1, 20); 
    spinWidth->setValue(2);     
    toolbar->addWidget(spinWidth);

    connect(spinWidth, QOverload<int>::of(&QSpinBox::valueChanged), 
    [this](int newVal) {
        canvas->setStrokeWidth(newVal);
    });

    // 6. RADIUS SPINNER
    toolbar->addWidget(new QLabel(" R: "));
    QSpinBox *spinR = new QSpinBox();
    spinR->setRange(0, 100); // Changed start to 0 allows sharp corners
    spinR->setValue(20);
    toolbar->addWidget(spinR);

    connect(spinR, QOverload<int>::of(&QSpinBox::valueChanged), 
    [this](int val) {
        canvas->setcurrentRadius(val);
    });

    // --- LOAD FILE ---
    // auto loadedShapes = SVGParser::parseFile(currentFile);
    // for(auto& s : loadedShapes) {
    //     canvas->addShape(std::move(s));
    // }
}

void MainWindow::closeEvent(QCloseEvent *event) {

    if (maybeSave())
        event->accept();
    else
        event->ignore();
}


MainWindow::~MainWindow() {
}

void MainWindow::createMenus() {
    // 1. Create the "File" Menu
    QMenu *fileMenu = menuBar()->addMenu("&File");

    QAction *newAct = new QAction("&New", this);
    newAct->setShortcut(QKeySequence::New); // Ctrl+N
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);

    // 2. Add "Open" Action
    QAction *openAct = new QAction("&Open", this);
    openAct->setShortcut(QKeySequence::Open); // Ctrl+O
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAct);

    // 3. Add "Save As" Action
    QAction *saveAct = new QAction("&Save As...", this);
    saveAct->setShortcut(QKeySequence::Save); // Ctrl+S
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAct);

    fileMenu->addSeparator();

    // 4. Add "Exit" Action
    QAction *exitAct = new QAction("E&xit", this);
    exitAct->setShortcut(QKeySequence::Quit); // Ctrl+Q
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAct);
}

void MainWindow::openFile() {

    if (!maybeSave())
        return;

    QString fileName = QFileDialog::getOpenFileName(
        this, "Open SVG", "", "SVG Files (*.svg)"
    );

    if (fileName.isEmpty())
        return;

    canvas->clearShapes();

    auto loadedShapes = SVGParser::parseFile(fileName.toStdString());

    for (auto& s : loadedShapes) {
        canvas->addShape(std::move(s));
    }

    currentFile = fileName.toStdString();
}



void MainWindow::saveFile() {
    // Open Save Dialog
    QString fileName = QFileDialog::getSaveFileName(this, "Save SVG", "", "SVG Files (*.svg)");

    if (fileName.isEmpty()) return; // User cancelled

    // Call our existing helper
    SVGParser::saveFile(fileName.toStdString(), canvas->getShapes());

    // Update current file path
    currentFile = fileName.toStdString();
}

void MainWindow::newFile() {

    if (!maybeSave())
        return;

    canvas->clearShapes();
    currentFile = "";
}



bool MainWindow::maybeSave() {

    // If canvas is empty → nothing to save
    if (canvas->getShapes().empty())
        return true;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Save Changes?",
        "Do you want to save the current document before proceeding?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Save) {

        // If file already has name → overwrite
        if (!currentFile.empty()) {
            SVGParser::saveFile(currentFile, canvas->getShapes());
        }
        else {
            saveFile();  // Open Save dialog
            if (currentFile.empty())
                return false;  // User cancelled Save dialog
        }

        return true;
    }
    else if (reply == QMessageBox::Discard) {
        return true;
    }
    else {
        return false;  // Cancel
    }
}

