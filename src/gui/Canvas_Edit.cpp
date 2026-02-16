#include "include/gui/Canvas.h"

Canvas::State Canvas::cloneVector(const State& source) {
    State destination;
    destination.reserve(source.size());                  // Optimization: Pre-allocate memory
    for (const auto& shape : source) {
        destination.push_back(shape->clone());           // Deep copy each shape
    }
    return destination;                                  // Return independent copy
}

void Canvas::saveState() {
    while (!redoStack.empty()) {
        redoStack.pop();                                 // New action clears Redo history
    }
    undoStack.push(cloneVector(shapes));                 // Save snapshot to Undo stack
}

void Canvas::undo() {
    if (undoStack.empty()) return;                       // Nothing to undo
    redoStack.push(cloneVector(shapes));                 // Save current state to Redo
    shapes = std::move(undoStack.top());                 // Restore previous state
    undoStack.pop();                                     // Remove used state
    selectedShape = nullptr;                             // Deselect to avoid crashes
    update();                                            // Redraw canvas
}

void Canvas::redo() {
    if (redoStack.empty()) return;                       // Nothing to redo
    undoStack.push(cloneVector(shapes));                 // Save current state to Undo
    shapes = std::move(redoStack.top());                 // Restore future state
    redoStack.pop();                                     // Remove used state
    selectedShape = nullptr;                             // Clear selection
    update();                                            // Redraw canvas
}

void Canvas::copy() {
    if (selectedShape) {
        clipboard = selectedShape->clone();              // Deep copy to clipboard
    }
}

void Canvas::cut() {
    if (selectedShape) {
        saveState();                                     // Save history before change
        copy();                                          // Copy to clipboard first
        auto it = std::find_if(shapes.begin(), shapes.end(), 
            [this](const std::unique_ptr<Object>& s) { return s.get() == selectedShape; });
        if (it != shapes.end()) {
            shapes.erase(it);                            // Remove shape from canvas
            selectedShape = nullptr;                     // Clear selection
            update();                                    // Redraw canvas
        }
    }
}

void Canvas::paste() {
    if (clipboard) {
        saveState();                                     // Save history before change
        auto newShape = clipboard->clone();              // Clone from clipboard
        newShape->move(20, 20);                          // Offset position slightly
        selectedShape = newShape.get();                  // Select the new shape
        shapes.push_back(std::move(newShape));           // Add to canvas list
        update();                                        // Redraw canvas
    }
}