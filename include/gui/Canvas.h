#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <vector>
#include <memory>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include "include/models/Object.h" // (Check if you need "include/" prefix based on CMake)

// --- THIS LINE WAS MISSING ---
class Canvas : public QWidget {
    Q_OBJECT // Required by Qt for signals/slots to work

public:
    // Constructor: Takes a parent widget (default is null)
    explicit Canvas(QWidget *parent = nullptr);

    // Function to add a new shape to the canvas
    void addShape(std::unique_ptr<Object> shape);

    // Getter for the parser to access shapes
    std::vector<std::unique_ptr<Object>>& getShapes();

protected:
    // The "paint" function called automatically by Qt
    void paintEvent(QPaintEvent *event) override;

private:
    // The container for all your shape objects
    std::vector<std::unique_ptr<Object>> shapes;
};
// -----------------------------

#endif // CANVAS_HPP