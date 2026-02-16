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
    while (!redoStack.empty()) {
        redoStack.pop();
    }
    undoStack.push(cloneVector(shapes));
}

void Canvas::undo() {
    if (undoStack.empty()) return;
    redoStack.push(cloneVector(shapes));
    shapes = std::move(undoStack.top());
    undoStack.pop();
    selectedShape = nullptr;
    update();
}

void Canvas::redo() {
    if (redoStack.empty()) return;
    undoStack.push(cloneVector(shapes));
    shapes = std::move(redoStack.top());
    redoStack.pop();
    selectedShape = nullptr;
    update();
}

void Canvas::copy() {
    if (selectedShape) {
        clipboard = selectedShape->clone();
    }
}

void Canvas::cut() {
    if (selectedShape) {
        saveState(); 
        copy(); 
        auto it = std::find_if(shapes.begin(), shapes.end(), 
            [this](const std::unique_ptr<Object>& s) { return s.get() == selectedShape; });
        if (it != shapes.end()) {
            shapes.erase(it);
            selectedShape = nullptr; 
            update();
        }
    }
}

void Canvas::paste() {
    if (clipboard) {
        saveState(); 
        auto newShape = clipboard->clone();
        newShape->move(20, 20);
        selectedShape = newShape.get();
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
    selectedShape = nullptr; 
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
void Canvas::mousePressEvent(QMouseEvent *event) {
    if ((event->button() == Qt::LeftButton) || (event->button() == Qt::RightButton)) {
        startPoint = event->pos();
        lastmousepos = event->pos();
        if (currentTool == ToolType::Select) {
            selectedShape = nullptr; 
            interactionMode = InteractionMode::None;
            for (int i=shapes.size()-1; i>=0; i--) {
                if (shapes[i]->isHit(event->pos())) {
                    selectedShape = (&shapes[i])->get(); 
                    interactionMode = InteractionMode::Moving;
                    saveState();
                    bringToFront(i);
                    QRect bounds = selectedShape->getBounds();
                    isDrawing = true;
                    update();
                    break; 
                }
            }
            update();
            return;
        }

        if (currentTool == ToolType::Text) {
            bool ok;
            QString text = QInputDialog::getText(this, "Add Text", "Enter text:", QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {
                auto t = std::make_unique<TextObj>(
                    startPoint.x(), startPoint.y(), 
                    text.toStdString(), currentStrokeColor, 20
                );
                saveState();
                shapes.push_back(std::move(t));
                update();
            }
            return; 
        }

        if (currentTool == ToolType::Freehand) {
            std::vector<QPoint> initialPoints = {startPoint};
            auto poly = std::make_unique<Freehand>(initialPoints, currentStrokeColor, currentStrokeWidth);
            currentFreehand = poly.get();
            saveState();
            shapes.push_back(std::move(poly));
            isDrawing = true;
            return; 
        }

        isDrawing = true;
        previewShape = nullptr; 
    }
}
void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons()) && isDrawing) {
        currentPoint = event->pos();
        if (currentTool == ToolType::Freehand && currentFreehand) {
            currentFreehand->addPoint(currentPoint);
            update();
            return; 
        }
        int dx = currentPoint.x() - startPoint.x();
        int dy = currentPoint.y() - startPoint.y();
        if (currentTool == ToolType::Circle) {
            int radius = static_cast<int>(std::sqrt(dx*dx + dy*dy));
            previewShape = std::make_unique<Circle>(startPoint.x(), startPoint.y(), radius, currentStrokeColor, currentFillColor, currentStrokeWidth);
        } 
        else if (currentTool == ToolType::Rectangle) {
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
                selectedShape->move(dx, dy);
            } 
            if (event->buttons() & Qt::RightButton) {
                selectedShape->resize(dx, dy);
            }
            lastmousepos = currentPos; 
            update(); 
        }
        update(); 
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if ( isDrawing) {
        if (currentTool == ToolType::Freehand) {
            currentFreehand = nullptr;
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

std::vector<std::unique_ptr<Object>>& Canvas::getShapes() { 
    return shapes; 
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

void Canvas::keyPressEvent(QKeyEvent *event) {
    bool ctrlPressed = event->modifiers() & Qt::ControlModifier;
    if (ctrlPressed && event->key() == Qt::Key_Z) {
        undo();
    }
    else if (ctrlPressed && (event->key() == Qt::Key_Y || 
            (event->key() == Qt::Key_Z && (event->modifiers() & Qt::ShiftModifier)))) {
        redo();
    }
    else if (ctrlPressed && event->key() == Qt::Key_C) {
        copy();
    }
    else if (ctrlPressed && event->key() == Qt::Key_V) {
        paste();
    }
    else if (ctrlPressed && event->key() == Qt::Key_X) {
        cut();
    }
    else if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        if (selectedShape) {
            saveState();
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
        QWidget::keyPressEvent(event);
    }
}