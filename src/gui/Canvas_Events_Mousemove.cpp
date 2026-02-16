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

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons()) && isDrawing) {
        currentPoint = event->pos();                                     // Get current mouse position
        if (currentTool == ToolType::Freehand && currentFreehand) {
            currentFreehand->addPoint(currentPoint);                     // Append point to path
            update();                                                    // Redraw immediately
            return; 
        }
        int dx = currentPoint.x() - startPoint.x();                      // Calculate X distance
        int dy = currentPoint.y() - startPoint.y();                      // Calculate Y distance
        if (currentTool == ToolType::Circle) {
            int radius = static_cast<int>(std::sqrt(dx*dx + dy*dy));     // Euclidean distance
            previewShape = std::make_unique<Circle>(startPoint.x(), startPoint.y(), radius, currentStrokeColor, currentFillColor, currentStrokeWidth);
        } 
        else if (currentTool == ToolType::Rectangle) {
            int x = std::min(startPoint.x(), currentPoint.x());          // Normalize Top-Left X
            int y = std::min(startPoint.y(), currentPoint.y());          // Normalize Top-Left Y
            int w = std::abs(dx);                                        // Positive Width
            int h = std::abs(dy);                                        // Positive Height
            previewShape = std::make_unique<Rectangle>(x, y, w, h, currentStrokeColor, currentFillColor, currentStrokeWidth);
        }
        else if (currentTool == ToolType::RoundedRect){
            int x = std::min(startPoint.x(), currentPoint.x());          // Normalize X
            int y = std::min(startPoint.y(), currentPoint.y());          // Normalize Y
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
            int radius = static_cast<int>(std::sqrt(dx*dx + dy*dy));     // Distance is radius
            double angle = qRadiansToDegrees(qAtan2(dy , dx));           // Calculate rotation angle
            previewShape = std::make_unique<Hexagon>( startPoint.x(), startPoint.y(), radius, angle , currentStrokeColor, currentFillColor, currentStrokeWidth );
        }
        else if (currentTool == ToolType::Select && selectedShape) {
            QPoint currentPos = event->pos();
            int dx = currentPos.x() - lastmousepos.x();                  // Delta X since last frame
            int dy = currentPos.y() - lastmousepos.y();                  // Delta Y since last frame

            if (event->buttons() & Qt::LeftButton) {
                selectedShape->move(dx, dy);                             // Move object
            } 
            if (event->buttons() & Qt::RightButton) {
                selectedShape->resize(dx, dy);                           // Resize object
            }
            lastmousepos = currentPos;                                   // Update for next move event
            update();                                                    // Redraw canvas
        }
        update();                                                        // Redraw ghost shape
    }
}