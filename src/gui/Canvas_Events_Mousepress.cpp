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
        startPoint = event->pos();                                       // Record click position
        lastmousepos = event->pos();                                     // Sync last position
        if (currentTool == ToolType::Select) {
            selectedShape = nullptr;                                     // Reset selection
            interactionMode = InteractionMode::None;                     // Reset mode
            for (int i=shapes.size()-1; i>=0; i--) {                     // Iterate Top-to-Bottom
                if (shapes[i]->isHit(event->pos())) {
                    selectedShape = (&shapes[i])->get();                 // Store clicked shape
                    interactionMode = InteractionMode::Moving;           // Set move mode
                    saveState();                                         // Save history
                    bringToFront(i);                                     // Move shape to top
                    QRect bounds = selectedShape->getBounds();           // Get shape bounds
                    isDrawing = true;                                    // Enable interaction
                    update();                                            // Redraw selection
                    break;                                               // Stop at top shape
                }
            }
            update();                                                    // Redraw canvas
            return;
        }

        if (currentTool == ToolType::Text) {
            bool ok;
            QString text = QInputDialog::getText(this, "Add Text", "Enter text:", QLineEdit::Normal, "", &ok);
            if (ok && !text.isEmpty()) {                                 // Validate input
                auto t = std::make_unique<TextObj>(
                    startPoint.x(), startPoint.y(), 
                    text.toStdString(), currentStrokeColor, 20
                );
                saveState();                                             // Save history
                shapes.push_back(std::move(t));                          // Add text to list
                update();                                                // Redraw
            }
            return;                                                      // Exit text tool logic
        }

        if (currentTool == ToolType::Freehand) {
            std::vector<QPoint> initialPoints = {startPoint};            // Initialize points
            auto poly = std::make_unique<Freehand>(initialPoints, currentStrokeColor, currentStrokeWidth);
            currentFreehand = poly.get();                                // Track active polyline
            saveState();                                                 // Save history
            shapes.push_back(std::move(poly));                           // Add to list immediately
            isDrawing = true;                                            // Enable drawing state
            return; 
        }

        isDrawing = true;                                                // Start shape preview
        previewShape = nullptr;                                          // Reset ghost shape
    }
}