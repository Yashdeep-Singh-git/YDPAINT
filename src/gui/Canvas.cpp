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
#include <QKeyEvent>


Canvas::State Canvas::cloneVector(const State& source) {
    State destination;
    destination.reserve(source.size());
    for (const auto& shape : source) {
        destination.push_back(shape->clone());
    }
    return destination;
}

void Canvas::saveState() {
    // 1. Clear Redo Stack (because we branched off into a new history)
    while (!redoStack.empty()) {
        redoStack.pop();
    }

    // 2. Save current shapes to Undo Stack
    undoStack.push(cloneVector(shapes));
}

void Canvas::undo() {
    if (undoStack.empty()) return;

    // 1. Push CURRENT state to Redo Stack (so we can go back)
    redoStack.push(cloneVector(shapes));

    // 2. Restore state from Undo Stack
    shapes = std::move(undoStack.top());
    undoStack.pop();

    // 3. Reset selection (important to avoid crashing)
    selectedShape = nullptr;
    
    update();
}

void Canvas::redo() {
    if (redoStack.empty()) return;

    // 1. Push CURRENT state to Undo Stack
    undoStack.push(cloneVector(shapes));

    // 2. Restore state from Redo Stack
    shapes = std::move(redoStack.top());
    redoStack.pop();

    selectedShape = nullptr;
    
    update();
}

void Canvas::copy() {
    if (selectedShape) {
        // Clone the selected shape to clipboard
        clipboard = selectedShape->clone();
    }
}

void Canvas::cut() {
    if (selectedShape) {
        saveState(); // SAVE before modifying!

        copy(); // Save to clipboard first
        
        // Remove from vector
        // (Find the iterator for selectedShape)
        auto it = std::find_if(shapes.begin(), shapes.end(), 
            [this](const std::unique_ptr<Object>& s) { return s.get() == selectedShape; });
            
        if (it != shapes.end()) {
            shapes.erase(it);
            selectedShape = nullptr; // Clear selection
            update();
        }
    }
}

void Canvas::paste() {
    if (clipboard) {
        saveState(); // SAVE before modifying!

        // Create a NEW copy from the clipboard
        auto newShape = clipboard->clone();
        
        // Offset it slightly so user sees it
        newShape->move(20, 20);
        
        // Select the new shape
        selectedShape = newShape.get();
        
        // Add to canvas
        shapes.push_back(std::move(newShape));
        update();
    }
}

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

    setFocusPolicy(Qt::StrongFocus);
    saveState();
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

void Canvas::setStrokeColor(const std::string& color) { 
    currentStrokeColor = color; // Update default for future shapes
    
    // Update currently selected shape immediately
    if (selectedShape) {
        saveState();
        selectedShape->setStrokeColor(color);
        update(); // Re-paint the screen
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
    currentRadius = r; // Update default for future shapes
    if (selectedShape) {
        saveState();
        selectedShape->setCornerRadius(r);
        update(); // Re-paint
    }
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
                    saveState();
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
                saveState();
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
            saveState();
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
            saveState();
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

void Canvas::keyPressEvent(QKeyEvent *event) {
    
    // Check for Control Modifier (Cmd on Mac, Ctrl on Windows/Linux)
    bool ctrlPressed = event->modifiers() & Qt::ControlModifier;

    // Undo: Ctrl + Z
    if (ctrlPressed && event->key() == Qt::Key_Z) {
        undo();
    }
    // Redo: Ctrl + Y (or Ctrl + Shift + Z)
    else if (ctrlPressed && (event->key() == Qt::Key_Y || 
            (event->key() == Qt::Key_Z && (event->modifiers() & Qt::ShiftModifier)))) {
        redo();
    }
    // Copy: Ctrl + C
    else if (ctrlPressed && event->key() == Qt::Key_C) {
        copy();
    }
    // Paste: Ctrl + V
    else if (ctrlPressed && event->key() == Qt::Key_V) {
        paste();
    }
    // Cut: Ctrl + X
    else if (ctrlPressed && event->key() == Qt::Key_X) {
        cut();
    }
    // Delete: Delete Key (Bonus feature!)
    else if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        if (selectedShape) {
            saveState();
            // Similar logic to cut(), but without copying to clipboard
            auto it = std::find_if(shapes.begin(), shapes.end(), 
                [this](const std::unique_ptr<Object>& s) { return s.get() == selectedShape; });
            if (it != shapes.end()) {
                shapes.erase(it);
                selectedShape = nullptr;
                update();
            }
        }
    }
    else {
        // Pass other keys to base class (e.g. Alt+F4)
        QWidget::keyPressEvent(event);
    }
}