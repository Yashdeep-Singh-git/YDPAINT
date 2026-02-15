#include "include/gui/Canvas.h"
#include "include/models/Circle.h"
#include "include/models/Rectangle.h"
#include "include/models/Line.h"
#include "include/models/Hexagon.h"
#include "include/models/Freehand.h"
#include "include/models/Text.h"
#include <QPainter>
#include <cmath>
#include <QInputDialog>



Canvas::Canvas(QWidget *parent) : QWidget(parent), currentTool(ToolType::Select), isDrawing(false) {
    
    currentStrokeWidth = 2;
    currentStrokeColor = "black";
    currentFillColor = "none";
    currentRadius = 20;
    
    currentFreehand = nullptr;
    
    selectedShape = nullptr; // selection
    this->setTool(ToolType::Select);
    interactionMode = InteractionMode::None;
    
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void Canvas::bringToFront(int index) {
    if (index >= shapes.size() - 1) return; 
    auto temp = std::move(shapes[index]);

    shapes.erase(shapes.begin() + index);

    shapes.push_back(std::move(temp));
    selectedShape = shapes.back().get();
}
void Canvas::setTool(ToolType t) {
    currentTool = t;
}

void Canvas::setStrokeWidth(int w) { currentStrokeWidth = w; }
void Canvas::setStrokeColor(const std::string& color) { currentStrokeColor = color; }
void Canvas::setFillColor(const std::string& color) { currentFillColor = color; }
void Canvas::setcurrentRadius(int r) {currentRadius = r; }
void Canvas::clearShapes() {
    shapes.clear();

    selectedShape = nullptr;
    currentFreehand = nullptr;
    previewShape = nullptr;

    isDrawing = false;

    update();
}

void Canvas::addShape(std::unique_ptr<Object> shape) {
    shapes.push_back(std::move(shape));
    
    update(); 
}

// 1. MOUSE PRESS: Start the drawing
void Canvas::mousePressEvent(QMouseEvent *event) {
    if ((event->button() == Qt::LeftButton) || (event->button() == Qt::RightButton)) {
        startPoint = event->pos();
        lastmousepos = event->pos();

        if (currentTool == ToolType::Select) {
            selectedShape = nullptr; // Deselect first
            interactionMode = InteractionMode::None;

            // Loop Backwards (Top to Bottom)
            
            for (int i=shapes.size()-1; i>=0; i--) {
                if (shapes[i]->isHit(event->pos())) {
                    selectedShape = (&shapes[i])->get(); // Get raw pointer
                    interactionMode = InteractionMode::Moving;
                    bringToFront(i);
                    QRect bounds = selectedShape->getBounds();
                    isDrawing = true;
                    update();
                    break; // Stop looking once we found one
                }
            }
            update(); // Redraw to show selection box
            return;
        }

        if (currentTool == ToolType::Text) {
            bool ok;
            QString text = QInputDialog::getText(this, "Add Text", "Enter text:", QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {
                // Create Text Object
                // Note: Ensure your class name matches (TextObj vs Text)
                auto t = std::make_unique<TextObj>(
                    startPoint.x(), startPoint.y(), 
                    text.toStdString(), currentStrokeColor, 20
                );
                shapes.push_back(std::move(t));
                update();
            }
            return; // Done, don't trigger drawing logic
        }

        // --- NEW: FREEHAND TOOL LOGIC ---
        if (currentTool == ToolType::Freehand) {
            std::vector<QPoint> initialPoints = {startPoint};
            auto poly = std::make_unique<Freehand>(initialPoints, currentStrokeColor, currentStrokeWidth);
            
            // 1. Save reference to currentFreehand so we can add points in mouseMove
            currentFreehand = poly.get();
            
            // 2. Move ownership to main list immediately
            shapes.push_back(std::move(poly));
            
            isDrawing = true;
            return; // Return early
        }

        isDrawing = true;
        previewShape = nullptr; // Reset preview
    }
}

// 2. MOUSE MOVE: Update the preview shape (Rubber Band Effect)
void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons()) && isDrawing) {
        currentPoint = event->pos();


        // --- NEW: FREEHAND UPDATE ---
        if (currentTool == ToolType::Freehand && currentFreehand) {
            currentFreehand->addPoint(currentPoint);
            update();
            return; // Skip the rest
        }

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
        else if (currentTool == ToolType::Select && selectedShape) {
            QPoint currentPos = event->pos();
            int dx = currentPos.x() - lastmousepos.x();
            int dy = currentPos.y() - lastmousepos.y();

            if (event->buttons() & Qt::LeftButton) {
                // Left Click Drag = MOVE
                selectedShape->move(dx, dy);
            } 
            if (event->buttons() & Qt::RightButton) {
                // Right Click Drag = RESIZE (Simple shortcut implementation)
                selectedShape->resize(dx, dy);
            }

            lastmousepos = currentPos; // Update for next frame
            update(); // Redraw
        }

        update(); // Trigger paintEvent to redraw
    }
}


// 3. MOUSE RELEASE: Finalize the shape
void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if ( isDrawing) {
        // Move the preview shape into the real list

        if (currentTool == ToolType::Freehand) {
            currentFreehand = nullptr; // Stop tracking
            isDrawing = false;
            update();
            return;
        }

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

    if (selectedShape) {
        QRect bounds = selectedShape->getBounds();
        
        // Style: Dashed Blue Line
        QPen selPen(Qt::blue, 2, Qt::DashLine);
        painter.setPen(selPen);
        painter.setBrush(Qt::NoBrush);
        
        // Draw Rect with some padding
        painter.drawRect(bounds.adjusted(-5, -5, 5, 5));

        // Draw Resize Handle (Small square at bottom right)
        painter.setBrush(Qt::blue);
        painter.drawRect(bounds.right() + 5 - 6, bounds.bottom() + 5 - 6, 6, 6);
    }

    // Draw the "ghost" shape while dragging
    if (isDrawing && previewShape) {
        // Use a dashed line for preview effect
        painter.setPen(Qt::DashLine); 
        previewShape->draw(painter);
    }
}