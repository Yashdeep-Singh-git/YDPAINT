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