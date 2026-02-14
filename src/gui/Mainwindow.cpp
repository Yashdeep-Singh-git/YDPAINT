#include "include/gui/Mainwindow.h"
#include "include/models/Circle.h" // Needed to create the test circle
#include "include/models/Rectangle.h"   // Needed to create the test rectangle
#include "include/gui/Canvas.h"
#include "include/utils/svg_parser.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(800, 600);
    canvas = new Canvas(this);
    setCentralWidget(canvas);

    // --- NEW LOADING LOGIC ---
    
    // 1. Create a dummy SVG file to test (Manual test)
    // You should create a file named "test.svg" in your build folder with this content:
    // <circle cx="100" cy="100" r="40" fill="green" stroke="black" stroke-width="2" />
    // <rect x="200" y="200" width="100" height="50" fill="yellow" stroke="red" stroke-width="4" />

    // 2. Parse the file
    // Note: ensure "test.svg" is in the folder where you run the executable!
    auto shapes = SVGParser::parseFile("./../sample.svg");

    // 3. Add loaded shapes to canvas
    for (auto& s : shapes) {
        canvas->addShape(std::move(s));
    }
}

MainWindow::~MainWindow() {
    // Canvas is automatically deleted by QMainWindow because it's a child widget.
}