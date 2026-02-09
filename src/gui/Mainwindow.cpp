#include "include/gui/Mainwindow.h"
#include "include/models/Circle.h" // Needed to create the test circle
#include "include/models/Rectangle.h"   // Needed to create the test rectangle

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 1. Setup the Window UI
    resize(800, 600); 
    setWindowTitle("Vector Editor - Test Phase"); 

    // 2. Create the Canvas and place it in the center
    canvas = new Canvas(this);
    setCentralWidget(canvas);

    // --- TEMPORARY TEST CODE (Verify your Backend works!) ---
    
    // Test 1: A Blue Circle
    // Params: cx, cy, radius, stroke, fill, stroke_width
    auto c = std::make_unique<Circle>(200, 200, 50, "black", "blue", 3);
    
    // Test 2: A Red Rectangle
    // Params: x, y, width, height, stroke, fill, stroke_width
    auto r = std::make_unique<Rectangle>(400, 100, 100, 60, "black", "red", 3);

    // 3. Hand them over to the Canvas (Transfer ownership)
    canvas->addShape(std::move(c));
    canvas->addShape(std::move(r));

    // -------------------------------------------------------
}

MainWindow::~MainWindow() {
    // Canvas is automatically deleted by QMainWindow because it's a child widget.
}