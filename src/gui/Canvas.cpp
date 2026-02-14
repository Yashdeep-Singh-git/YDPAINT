#include "include/gui/Canvas.h"
#include "include/models/Circle.h"
#include "include/models/Rectangle.h"
#include "include/models/Line.h"
#include "include/models/Hexagon.h"
#include <QPainter>
#include <cmath>



Canvas::Canvas(QWidget *parent) : QWidget(parent), currentTool(ToolType::Select), isDrawing(false) {
    
    currentStrokeWidth = 2;
    currentStrokeColor = "black";
    currentFillColor = "none";
    currentRadius = 20;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void Canvas::setTool(ToolType t) {
    currentTool = t;
}

void Canvas::setStrokeWidth(int w) { currentStrokeWidth = w; }
void Canvas::setStrokeColor(const std::string& color) { currentStrokeColor = color; }
void Canvas::setFillColor(const std::string& color) { currentFillColor = color; }
void Canvas::setcurrentRadius(int r) {currentRadius = r; }

void Canvas::addShape(std::unique_ptr<Object> shape) {
    // 1. Take ownership of the shape and save it
    shapes.push_back(std::move(shape));
    
    // 2. Trigger a screen refresh (calls paintEvent internally)
    update(); 
}

// 1. MOUSE PRESS: Start the drawing
void Canvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        startPoint = event->pos();
        isDrawing = true;
        previewShape = nullptr; // Reset preview
    }
}

// 2. MOUSE MOVE: Update the preview shape (Rubber Band Effect)
void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && isDrawing) {
        currentPoint = event->pos();

        // Calculate geometry
        int dx = currentPoint.x() - startPoint.x();
        int dy = currentPoint.y() - startPoint.y();

        if (currentTool == ToolType::Circle) {
            // Radius is distance between start and current
            int radius = static_cast<int>(std::sqrt(dx*dx + dy*dy));
            // Create a temporary circle for preview
            previewShape = std::make_unique<Circle>(startPoint.x(), startPoint.y(), radius, currentStrokeColor, currentFillColor, currentStrokeWidth);
        } 
        else if (currentTool == ToolType::Rectangle) {
            // Standardize (handle dragging backwards/upwards)
            int x = std::min(startPoint.x(), currentPoint.x());
            int y = std::min(startPoint.y(), currentPoint.y());
            int w = std::abs(dx);
            int h = std::abs(dy);
            previewShape = std::make_unique<Rectangle>(x, y, w, h, currentStrokeColor, currentFillColor, currentStrokeWidth);
        }
        else if (currentTool == ToolType::RoundedRect){
            int x = std::min(startPoint.x(), currentPoint.x());
            int y = std::min(startPoint.y(), currentPoint.y());
            int w = std::abs(dx);
            int h = std::abs(dy);
            previewShape = std::make_unique<Rectangle>(x, y, w, h, currentStrokeColor, currentFillColor, currentStrokeWidth , currentRadius , currentRadius);
        }
        else if (currentTool == ToolType::Line){
            previewShape = std::make_unique<Line> (startPoint.x() , startPoint.y() , currentPoint.x(), currentPoint.y(), currentStrokeColor, currentStrokeWidth);
        }
        else if (currentTool == ToolType::Hexagon){
            int dx = currentPoint.x() - startPoint.x();
            int dy = currentPoint.y() - startPoint.y();
            int radius = static_cast<int>(std::sqrt(dx*dx + dy*dy));
            double angle = qRadiansToDegrees(qAtan2(dy , dx));
            previewShape = std::make_unique<Hexagon>( startPoint.x(), startPoint.y(), radius, angle , currentStrokeColor, currentFillColor, currentStrokeWidth );
        }

        update(); // Trigger paintEvent to redraw
    }
}

// 3. MOUSE RELEASE: Finalize the shape
void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && isDrawing) {
        // Move the preview shape into the real list
        if (previewShape) {
            shapes.push_back(std::move(previewShape));
            previewShape = nullptr;
        }
        isDrawing = false;
        update();
    }
}

// Getter for the parser to access shapes
std::vector<std::unique_ptr<Object>>& Canvas::getShapes() { 
    return shapes; 
}


// 4. PAINT EVENT: Draw existing shapes + preview
void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw all saved shapes
    for (const auto& shape : shapes) {
        shape->draw(painter);
    }

    // Draw the "ghost" shape while dragging
    if (isDrawing && previewShape) {
        // Use a dashed line for preview effect
        painter.setPen(Qt::DashLine); 
        previewShape->draw(painter);
    }
}