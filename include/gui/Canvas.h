#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <memory>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include "include/models/Object.h"

enum class ToolType {
    Select,
    Circle,
    Rectangle
};

class Canvas : public QWidget {
    Q_OBJECT // Required by Qt for signals/slots to work

public:
    // Constructor: Takes a parent widget (default is null)
    explicit Canvas(QWidget *parent = nullptr);

    // Set the current tool (called from MainWindow)
    void setTool(ToolType t);

    // Function to add a new shape to the canvas
    void addShape(std::unique_ptr<Object> shape);

    // Getter for the parser to access shapes
    std::vector<std::unique_ptr<Object>>& getShapes();

    void setStrokeWidth(int w);
    void setStrokeColor(const std::string& color);
    void setFillColor(const std::string& color);

protected:
    // The "paint" function called automatically by Qt
    void paintEvent(QPaintEvent *event) override;

    // 2. Override Mouse Events
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // The container for all your shape objects
    std::vector<std::unique_ptr<Object>> shapes;

    // 3. State Variables for Interaction
    ToolType currentTool;
    bool isDrawing;
    QPoint startPoint;    // Where you first clicked
    QPoint currentPoint;  // Where the mouse is right now

    int currentStrokeWidth;
    std::string currentStrokeColor;
    std::string currentFillColor;
    
    // A temporary pointer to hold the shape currently being drawn
    std::unique_ptr<Object> previewShape;
};

#endif