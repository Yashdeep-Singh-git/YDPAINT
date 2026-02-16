#include "include/gui/Canvas.h"

Canvas::State Canvas::cloneVector(const State& source) {
    State destination;
    destination.reserve(source.size());
    for (const auto& shape : source) {
        destination.push_back(shape->clone());
    }
    return destination;
}

void Canvas::saveState() {
    while (!redoStack.empty()) {
        redoStack.pop();
    }
    undoStack.push(cloneVector(shapes));
}

void Canvas::undo() {
    if (undoStack.empty()) return;
    redoStack.push(cloneVector(shapes));
    shapes = std::move(undoStack.top());
    undoStack.pop();
    selectedShape = nullptr;
    update();
}

void Canvas::redo() {
    if (redoStack.empty()) return;
    undoStack.push(cloneVector(shapes));
    shapes = std::move(redoStack.top());
    redoStack.pop();
    selectedShape = nullptr;
    update();
}

void Canvas::copy() {
    if (selectedShape) {
        clipboard = selectedShape->clone();
    }
}

void Canvas::cut() {
    if (selectedShape) {
        saveState(); 
        copy(); 
        auto it = std::find_if(shapes.begin(), shapes.end(), 
            [this](const std::unique_ptr<Object>& s) { return s.get() == selectedShape; });
        if (it != shapes.end()) {
            shapes.erase(it);
            selectedShape = nullptr; 
            update();
        }
    }
}

void Canvas::paste() {
    if (clipboard) {
        saveState(); 
        auto newShape = clipboard->clone();
        newShape->move(20, 20);
        selectedShape = newShape.get();
        shapes.push_back(std::move(newShape));
        update();
    }
}