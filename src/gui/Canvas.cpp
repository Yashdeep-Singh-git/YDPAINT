    #include "include/gui/Canvas.h"
    #include <QPainter>

    // ... (keep constructor) ...

    Canvas::Canvas(QWidget *parent) : QWidget(parent) {
        // Optional: Set a specific background color if you want
        // setPalette(QPalette(Qt::white));
        // setAutoFillBackground(true);
    }

    void Canvas::addShape(std::unique_ptr<Object> shape) {
        // 1. Take ownership of the shape and save it
        shapes.push_back(std::move(shape));
        
        // 2. Trigger a screen refresh (calls paintEvent internally)
        update(); 
    }

    // Getter for the parser to access shapes
    std::vector<std::unique_ptr<Object>>& Canvas::getShapes() { 
        return shapes; 
    }

    
    void Canvas::paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // Makes lines smooth

        // 3. The "Polymorphic" Loop
        // The Canvas doesn't know if it's a Circle or Rect, it just calls draw()
        for (const auto& shape : shapes) {
            shape->draw(painter); 
        }
    }