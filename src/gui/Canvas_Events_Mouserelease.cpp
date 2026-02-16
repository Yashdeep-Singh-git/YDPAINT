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

