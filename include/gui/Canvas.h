#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <memory>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include "include/models/Object.h"

class Canvas : public QWidget {
    // ... (keep your existing Q_OBJECT and constructor) ...

public:
    // New Function: Allows MainWindow to push a new shape into the canvas
    void addShape(std::unique_ptr<Object> shape);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    // The Container: Holds all the shapes that need to be drawn
    std::vector<std::unique_ptr<Object>> shapes;
};


#endif