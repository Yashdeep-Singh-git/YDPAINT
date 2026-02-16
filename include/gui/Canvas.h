#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <memory>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <stack>
#include "include/models/Object.h"
#include "include/models/Freehand.h"

enum class ToolType {
    Select,
    Circle,
    Rectangle,
    RoundedRect,
    Line,
    Hexagon,
    Freehand,
    Text
};

enum class InteractionMode {
    None,
    Moving,
    Resizing
};

class Canvas : public QWidget {
    Q_OBJECT 

public:
    explicit Canvas(QWidget *parent = nullptr);
    void setTool(ToolType t);
    void addShape(std::unique_ptr<Object> shape);

    std::vector<std::unique_ptr<Object>>& getShapes();

    void setStrokeWidth(int w);
    void setStrokeColor(const std::string& color);
    void setFillColor(const std::string& color);
    void setcurrentRadius(int r);

    void bringToFront(int i);
    void clearShapes();

    void copy();
    void cut();
    void paste();
    void undo();
    void redo();
    void saveState();
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;


private:
    std::vector<std::unique_ptr<Object>> shapes;
    ToolType currentTool;
    bool isDrawing;
    QPoint startPoint;   
    QPoint currentPoint;  
    Object* selectedShape; 
    QPoint lastmousepos;
    InteractionMode interactionMode;

    int currentStrokeWidth;
    int currentRadius;
    Freehand* currentFreehand;

    std::string currentStrokeColor;
    std::string currentFillColor;
    
    std::unique_ptr<Object> previewShape;
    std::unique_ptr<Object> clipboard;

    using State = std::vector<std::unique_ptr<Object>>;
    
    std::stack<State> undoStack;
    std::stack<State> redoStack;

    State cloneVector(const State& source);
};

#endif