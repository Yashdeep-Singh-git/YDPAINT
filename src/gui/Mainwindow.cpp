#include "include/gui/Mainwindow.h"
#include "include/models/Circle.h" // Needed to create the test circle
#include "include/models/Rectangle.h"   // Needed to create the test rectangle
#include "include/gui/Canvas.h"
#include "include/utils/svg_parser.h"
#include <QDebug>
#include <QToolBar>
#include <QAction>
#include <QCloseEvent>



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
    SVGParser::saveFile(currentFile, canvas->getShapes());
    
    event->accept(); // Allow the window to close
}

MainWindow::~MainWindow() {
}