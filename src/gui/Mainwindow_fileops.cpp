#include "include/gui/Mainwindow.h"
#include "include/gui/Canvas.h"
#include "include/utils/svg_parser.h"
#include <QFileDialog>
#include <QMessageBox>

void MainWindow::openFile() {
    if (!maybeSave())
        return;                                                  // Check for unsaved changes

    QString fileName = QFileDialog::getOpenFileName(
        this, "Open SVG", "", "SVG Files (*.svg)"                // Open file dialog
    );
    if (fileName.isEmpty())
        return;                                                  // User cancelled
    canvas->clearShapes();                                       // Clear existing drawing
    auto loadedShapes = SVGParser::parseFile(fileName.toStdString()); // Parse SVG file
    for (auto& s : loadedShapes) {
        canvas->addShape(std::move(s));                          // Add shapes to canvas
    }
    currentFile = fileName.toStdString();                        // Update current path
}

void MainWindow::saveFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save SVG", "", "SVG Files (*.svg)");
    if (fileName.isEmpty()) return;                              // User cancelled
    SVGParser::saveFile(fileName.toStdString(), canvas->getShapes()); // Save canvas to file
    currentFile = fileName.toStdString();                        // Update current path
}

void MainWindow::newFile() {
    if (!maybeSave())
        return;                                                  // Check for unsaved changes
    canvas->clearShapes();                                       // Reset canvas
    currentFile = "";                                            // Reset filename
}

bool MainWindow::maybeSave() {
    if (canvas->getShapes().empty())
        return true;                                             // Nothing to save
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Save Changes?",
        "Do you want to save the current document before proceeding?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Save) {
        if (!currentFile.empty()) {
            SVGParser::saveFile(currentFile, canvas->getShapes()); // Overwrite existing file
        }
        else {
            saveFile();                                          // Save as new file
            if (currentFile.empty())
                return false;                                    // User cancelled save
        }
        return true;                                             // Proceed
    }
    else if (reply == QMessageBox::Discard) {
        return true;                                             // Discard changes
    }
    else {
        return false;                                            // Cancel operation
    }
}