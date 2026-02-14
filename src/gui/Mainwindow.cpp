#include "include/gui/Mainwindow.h"
#include "include/models/Circle.h" // Needed to create the test circle
#include "include/models/Rectangle.h"   // Needed to create the test rectangle
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

    QAction *actCircle = toolbar->addAction("Circle");
    connect(actCircle, &QAction::triggered, [this]() {
        canvas->setTool(ToolType::Circle);
    });

    QAction *actRect = toolbar->addAction("Rectangle");
    connect(actRect, &QAction::triggered, [this]() {
        canvas->setTool(ToolType::Rectangle);
    });
    
    toolbar->addSeparator(); // Visual spacer

    // 1. STROKE COLOR BUTTON
    QAction *actStrokeColor = toolbar->addAction("Stroke Color");
    connect(actStrokeColor, &QAction::triggered, [this]() {
        // Open color picker
        QColor color = QColorDialog::getColor(Qt::black, this, "Select Stroke Color");
        if (color.isValid()) {
            // Convert QColor to Hex String (e.g., "#FF0000")
            canvas->setStrokeColor(color.name().toStdString());
        }
    });

    // 2. FILL COLOR BUTTON
    QAction *actFillColor = toolbar->addAction("Fill Color");
    connect(actFillColor, &QAction::triggered, [this]() {
        QColor color = QColorDialog::getColor(Qt::white, this, "Select Fill Color");
        if (color.isValid()) {
            canvas->setFillColor(color.name().toStdString());
        }
    });

    // 3. NO FILL BUTTON (Optional but useful)
    QAction *actNoFill = toolbar->addAction("No Fill");
    connect(actNoFill, &QAction::triggered, [this]() {
        canvas->setFillColor("none");
    });

    toolbar->addSeparator();

    // 4. THICKNESS SPINNER
    toolbar->addWidget(new QLabel(" Width: ")); // Label
    QSpinBox *spinWidth = new QSpinBox();
    spinWidth->setRange(1, 20); // Thickness from 1 to 20
    spinWidth->setValue(2);     // Default value
    toolbar->addWidget(spinWidth);

    // Connect spinner to canvas
    connect(spinWidth, QOverload<int>::of(&QSpinBox::valueChanged), 
    [this](int newVal) {
        canvas->setStrokeWidth(newVal);
    });

    // --- LOAD FILE ---
    // Try to load existing file, or start fresh
    auto loadedShapes = SVGParser::parseFile(currentFile);
    for(auto& s : loadedShapes) {
        canvas->addShape(std::move(s));
    }
}

// --- AUTO-SAVE ON CLOSE ---
// Add "void closeEvent(QCloseEvent *event) override;" to MainWindow.h
void MainWindow::closeEvent(QCloseEvent *event) {
    // Save everything before the window closes
    SVGParser::saveFile("./../output.svg", canvas->getShapes());  // Saving it temporarily in output.svg, later change
    
    event->accept(); // Allow the window to close
}

MainWindow::~MainWindow() {
}