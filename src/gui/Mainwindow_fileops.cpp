#include "include/gui/Mainwindow.h"
#include "include/gui/Canvas.h"
#include "include/utils/svg_parser.h"
#include <QFileDialog>
#include <QMessageBox>

void MainWindow::openFile() {
    if (!maybeSave())
        return;

    QString fileName = QFileDialog::getOpenFileName(
        this, "Open SVG", "", "SVG Files (*.svg)"
    );
    if (fileName.isEmpty())
        return;
    canvas->clearShapes();
    auto loadedShapes = SVGParser::parseFile(fileName.toStdString());
    for (auto& s : loadedShapes) {
        canvas->addShape(std::move(s));
    }
    currentFile = fileName.toStdString();
}

void MainWindow::saveFile() {
    // Open Save Dialog
    QString fileName = QFileDialog::getSaveFileName(this, "Save SVG", "", "SVG Files (*.svg)");

    if (fileName.isEmpty()) return; 

    // Call our existing helper
    SVGParser::saveFile(fileName.toStdString(), canvas->getShapes());

    // Update current file path
    currentFile = fileName.toStdString();
}

void MainWindow::newFile() {
    if (!maybeSave())
        return;

    canvas->clearShapes();
    currentFile = "";
}

bool MainWindow::maybeSave() {
    // If canvas is empty → nothing to save
    if (canvas->getShapes().empty())
        return true;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this,
        "Save Changes?",
        "Do you want to save the current document before proceeding?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
    );

    if (reply == QMessageBox::Save) {
        // If file already has name → overwrite
        if (!currentFile.empty()) {
            SVGParser::saveFile(currentFile, canvas->getShapes());
        }
        else {
            saveFile();  // Open Save dialog
            if (currentFile.empty())
                return false;  // User cancelled Save dialog
        }
        return true;
    }
    else if (reply == QMessageBox::Discard) {
        return true;
    }
    else {
        return false;  // Cancel
    }
}