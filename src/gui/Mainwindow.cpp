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

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(800, 600);
    canvas = new Canvas(this);
    setCentralWidget(canvas);
    currentFile = "./../sample.svg";

    // --- TOOLBAR SETUP ---
    QToolBar *toolbar = addToolBar("Tools");

    // 1. SHAPE TOOLS
    QAction *actCircle = toolbar->addAction("Circle");
    connect(actCircle, &QAction::triggered, [this]() {
        canvas->setTool(ToolType::Circle);
    });

    QAction *actRect = toolbar->addAction("Rectangle");
    connect(actRect, &QAction::triggered, [this]() {
        canvas->setTool(ToolType::Rectangle);
    });
    
    QAction *actRound = toolbar->addAction("Rounded Rect");
    connect(actRound, &QAction::triggered, [this]() { 
        canvas->setTool(ToolType::RoundedRect); 
    });

    QAction *actLine = toolbar->addAction("Line");
    connect(actLine, &QAction::triggered, [this]() { 
        canvas->setTool(ToolType::Line); 
    });

    QAction *actHex = toolbar->addAction("Hexagon");
    connect(actHex, &QAction::triggered, [this]() { 
        canvas->setTool(ToolType::Hexagon); 
    });

    // --- NEW: FREEHAND TOOL ---
    QAction *actFree = toolbar->addAction("Freehand");
    connect(actFree, &QAction::triggered, [this]() { 
        canvas->setTool(ToolType::Freehand); 
    });

    // --- NEW: TEXT TOOL ---
    QAction *actText = toolbar->addAction("Text");
    connect(actText, &QAction::triggered, [this]() { 
        canvas->setTool(ToolType::Text); 
    });

    toolbar->addSeparator(); // Visual spacer

    // 2. STROKE COLOR BUTTON
    QAction *actStrokeColor = toolbar->addAction("Stroke Color");
    connect(actStrokeColor, &QAction::triggered, [this]() {
        QColor color = QColorDialog::getColor(Qt::black, this, "Select Stroke Color");
        if (color.isValid()) {
            canvas->setStrokeColor(color.name().toStdString());
        }
    });

    // 3. FILL COLOR BUTTON
    QAction *actFillColor = toolbar->addAction("Fill Color");
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

    toolbar->addSeparator();

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
    auto loadedShapes = SVGParser::parseFile(currentFile);
    for(auto& s : loadedShapes) {
        canvas->addShape(std::move(s));
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    SVGParser::saveFile("./../output.svg", canvas->getShapes()); 
    event->accept(); 
}

MainWindow::~MainWindow() {
}