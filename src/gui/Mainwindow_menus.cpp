#include "include/gui/Mainwindow.h"
#include "include/gui/Canvas.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu("&File");               // Create File menu

    QAction *newAct = new QAction("&New", this);
    newAct->setShortcut(QKeySequence::New);                      // Ctrl+N shortcut
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile); // Link to newFile()
    fileMenu->addAction(newAct);                                 // Add to menu

    QAction *openAct = new QAction("&Open", this);
    openAct->setShortcut(QKeySequence::Open);                    // Ctrl+O shortcut
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile); // Link to openFile()
    fileMenu->addAction(openAct);                                // Add to menu

    QAction *saveAct = new QAction("&Save As...", this);
    saveAct->setShortcut(QKeySequence::Save);                    // Ctrl+S shortcut
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile); // Link to saveFile()
    fileMenu->addAction(saveAct);                                // Add to menu

    fileMenu->addSeparator();                                    // Visual separator

    QAction *exitAct = new QAction("E&xit", this);
    exitAct->setShortcut(QKeySequence::Quit);                    // Ctrl+Q shortcut
    connect(exitAct, &QAction::triggered, this, &QWidget::close); // Close application
    fileMenu->addAction(exitAct);                                // Add to menu

    QMenu *editMenu = menuBar()->addMenu("&Edit");               // Create Edit menu
    QAction *cutAct = editMenu->addAction("Cu&t");
    cutAct->setShortcut(QKeySequence::Cut);                      // Ctrl+X shortcut
    connect(cutAct, &QAction::triggered, canvas, &Canvas::cut);  // Link to canvas cut

    QAction *copyAct = editMenu->addAction("&Copy");
    copyAct->setShortcut(QKeySequence::Copy);                    // Ctrl+C shortcut
    connect(copyAct, &QAction::triggered, canvas, &Canvas::copy); // Link to canvas copy

    QAction *pasteAct = editMenu->addAction("&Paste");
    pasteAct->setShortcut(QKeySequence::Paste);                  // Ctrl+V shortcut
    connect(pasteAct, &QAction::triggered, canvas, &Canvas::paste); // Link to canvas paste

    QAction *undoAct = new QAction(QIcon(":/resources/icons/undo.png"), "", this);
    undoAct->setShortcut(QKeySequence::Undo);                    // Ctrl+Z shortcut
    undoAct->setToolTip("Undo (Ctrl+Z)");                        // Hover tooltip
    connect(undoAct, &QAction::triggered, canvas, &Canvas::undo); // Link to canvas undo
    menuBar()->addAction(undoAct);                               // Add icon directly to bar

    QAction *redoAct = new QAction(QIcon(":/resources/icons/redo.png"), "", this);
    redoAct->setShortcut(QKeySequence::Redo);                    // Ctrl+Y shortcut
    redoAct->setToolTip("Redo (Ctrl+Y)");                        // Hover tooltip
    connect(redoAct, &QAction::triggered, canvas, &Canvas::redo); // Link to canvas redo
    menuBar()->addAction(redoAct);                               // Add icon directly to bar
}