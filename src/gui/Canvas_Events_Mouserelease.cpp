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

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if ( isDrawing) {
        if (currentTool == ToolType::Freehand) {
            currentFreehand = nullptr;                                   // Stop tracking polyline
            isDrawing = false;                                           // End drawing state
            update();                                                    // Redraw canvas
            return;                                                      // Exit early
        }
        if (previewShape) {
            saveState();                                                 // Save history before commit
            shapes.push_back(std::move(previewShape));                   // Move preview to main list
            previewShape = nullptr;                                      // Reset preview pointer
        }
        isDrawing = false;                                               // End drawing state
        update();                                                        // Redraw final state
    }
}