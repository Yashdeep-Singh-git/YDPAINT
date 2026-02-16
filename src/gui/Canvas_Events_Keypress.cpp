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

void Canvas::keyPressEvent(QKeyEvent *event) {
    bool ctrlPressed = event->modifiers() & Qt::ControlModifier;        // Check for Ctrl modifier
    if (ctrlPressed && event->key() == Qt::Key_Z) {
        undo();                                                         // Handle Undo
    }
    else if (ctrlPressed && (event->key() == Qt::Key_Y || 
            (event->key() == Qt::Key_Z && (event->modifiers() & Qt::ShiftModifier)))) {
        redo();                                                         // Handle Redo (Ctrl+Y or Ctrl+Shift+Z)
    }
    else if (ctrlPressed && event->key() == Qt::Key_C) {
        copy();                                                         // Handle Copy
    }
    else if (ctrlPressed && event->key() == Qt::Key_V) {
        paste();                                                        // Handle Paste
    }
    else if (ctrlPressed && event->key() == Qt::Key_X) {
        cut();                                                          // Handle Cut
    }
    else if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        if (selectedShape) {
            saveState();                                                // Save history before deleting
            auto it = std::find_if(shapes.begin(), shapes.end(), 
                [this](const std::unique_ptr<Object>& s) { return s.get() == selectedShape; });
            if (it != shapes.end()) {
                shapes.erase(it);                                       // Remove shape from list
                selectedShape = nullptr;                                // Clear selection
                update();                                               // Redraw canvas
            }
        }
    }
    else {
        QWidget::keyPressEvent(event);                                  // Pass unhandled keys to base
    }
}