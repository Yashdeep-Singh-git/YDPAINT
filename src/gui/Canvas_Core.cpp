#include "include/gui/Canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget *parent) : QWidget(parent), currentTool(ToolType::Select), isDrawing(false) {
    currentStrokeWidth = 2;              // Default stroke width
    currentStrokeColor = "black";        // Default stroke color
    currentFillColor = "none";           // Default fill color
    currentRadius = 20;                  // Default corner radius
    currentFreehand = nullptr;           // Reset freehand pointer
    selectedShape = nullptr;             // No shape selected initially
    this->setTool(ToolType::Select);     // Default tool is Select
    interactionMode = InteractionMode::None;
    setBackgroundRole(QPalette::Base);   // Set background style
    setAutoFillBackground(true);         // Enable auto-fill
    setFocusPolicy(Qt::StrongFocus);     // Enable keyboard focus
    saveState();                         // Save initial blank state
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // Smooth edges

    for (const auto& shape : shapes) {
        shape->draw(painter);            // Draw all stored shapes
    }

    if (selectedShape) {
        QRect bounds = selectedShape->getBounds();
        QPen selPen(Qt::blue, 2, Qt::DashLine);
        painter.setPen(selPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(bounds.adjusted(-5, -5, 5, 5)); // Draw selection box

        painter.setBrush(Qt::blue);
        painter.drawRect(bounds.right() + 5 - 6, bounds.bottom() + 5 - 6, 6, 6); // Draw resize handle
    }

    if (isDrawing && previewShape) {
        painter.setPen(Qt::DashLine);    // Draw preview as dashed
        previewShape->draw(painter);     // Draw the ghost shape
    }
}

void Canvas::addShape(std::unique_ptr<Object> shape) {
    shapes.push_back(std::move(shape));  // Add new shape to list
    update();                            // Trigger repaint
}

void Canvas::clearShapes() {
    shapes.clear();                      // Remove all shapes
    selectedShape = nullptr;             // Clear selection
    currentFreehand = nullptr;           // Reset freehand tool
    previewShape = nullptr;              // Clear preview
    isDrawing = false;                   // Stop drawing state
    while (!undoStack.empty()) undoStack.pop(); // Clear undo history
    while (!redoStack.empty()) redoStack.pop(); // Clear redo history
    update();                            // Refresh canvas
}

void Canvas::bringToFront(int index) {
    if (index >= shapes.size() - 1) return; // Already at top
    auto temp = std::move(shapes[index]);   // Move out of vector
    shapes.erase(shapes.begin() + index);   // Remove empty spot
    shapes.push_back(std::move(temp));      // Push to back (top)
    selectedShape = shapes.back().get();    // Update selection pointer
}

std::vector<std::unique_ptr<Object>>& Canvas::getShapes() { 
    return shapes;                       // Return reference to shapes
}

void Canvas::setTool(ToolType t) {
    currentTool = t;                     // Update active tool
}

void Canvas::setStrokeColor(const std::string& color) { 
    currentStrokeColor = color;          // Set default color
    if (selectedShape) {
        saveState();                     // Save history before change
        selectedShape->setStrokeColor(color); // Apply to selected
        update();                        // Repaint
    }
}

void Canvas::setFillColor(const std::string& color) { 
    currentFillColor = color;            // Set default fill
    if (selectedShape) {
        saveState();                     // Save history
        selectedShape->setFillColor(color); // Apply to selected
        update();                        // Repaint
    }
}

void Canvas::setStrokeWidth(int w) { 
    currentStrokeWidth = w;              // Set default width
    if (selectedShape) {
        saveState();                     // Save history
        selectedShape->setStrokeWidth(w); // Apply to selected
        update();                        // Repaint
    }
}

void Canvas::setcurrentRadius(int r) {
    currentRadius = r;                   // Set default radius
    if (selectedShape) {
        saveState();                     // Save history
        selectedShape->setCornerRadius(r); // Apply to selected
        update();                        // Repaint
    }
}