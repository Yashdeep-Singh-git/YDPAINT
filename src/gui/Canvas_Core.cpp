#include "include/gui/Canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget *parent) : QWidget(parent), currentTool(ToolType::Select), isDrawing(false) {
    currentStrokeWidth = 2;
    currentStrokeColor = "black";
    currentFillColor = "none";
    currentRadius = 20;
    currentFreehand = nullptr;
    selectedShape = nullptr; 
    this->setTool(ToolType::Select);
    interactionMode = InteractionMode::None;
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::StrongFocus);
    saveState();
}
void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for (const auto& shape : shapes) {
        shape->draw(painter);
    }
    if (selectedShape) {
        QRect bounds = selectedShape->getBounds();
        QPen selPen(Qt::blue, 2, Qt::DashLine);
        painter.setPen(selPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(bounds.adjusted(-5, -5, 5, 5));
        painter.setBrush(Qt::blue);
        painter.drawRect(bounds.right() + 5 - 6, bounds.bottom() + 5 - 6, 6, 6);
    }

    if (isDrawing && previewShape) {
        painter.setPen(Qt::DashLine); 
        previewShape->draw(painter);
    }
}
void Canvas::addShape(std::unique_ptr<Object> shape) {
    shapes.push_back(std::move(shape));
    update(); 
}

void Canvas::clearShapes() {
    shapes.clear();
    selectedShape = nullptr;
    currentFreehand = nullptr;
    previewShape = nullptr;
    isDrawing = false;
    while (!undoStack.empty()) undoStack.pop();
    while (!redoStack.empty()) redoStack.pop();
    update();
}

void Canvas::bringToFront(int index) {
    if (index >= shapes.size() - 1) return; 
    auto temp = std::move(shapes[index]);
    shapes.erase(shapes.begin() + index);
    shapes.push_back(std::move(temp));
    selectedShape = shapes.back().get();
}

std::vector<std::unique_ptr<Object>>& Canvas::getShapes() { 
    return shapes; 
}

void Canvas::setTool(ToolType t) {
    currentTool = t;
}

void Canvas::setStrokeColor(const std::string& color) { 
    currentStrokeColor = color; 
    if (selectedShape) {
        saveState();
        selectedShape->setStrokeColor(color);
        update(); 
    }
}

void Canvas::setFillColor(const std::string& color) { 
    currentFillColor = color; 
    if (selectedShape) {
        saveState();
        selectedShape->setFillColor(color);
        update();
    }
}

void Canvas::setStrokeWidth(int w) { 
    currentStrokeWidth = w; 
    if (selectedShape) {
        saveState();
        selectedShape->setStrokeWidth(w);
        update();
    }
}

void Canvas::setcurrentRadius(int r) {
    currentRadius = r; 
    if (selectedShape) {
        saveState();
        selectedShape->setCornerRadius(r);
        update(); 
    }
}