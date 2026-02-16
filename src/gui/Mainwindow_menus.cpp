#include "include/gui/Mainwindow.h"
#include "include/gui/Canvas.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>

void MainWindow::createMenus() {
    // 1. File Menu
    QMenu *fileMenu = menuBar()->addMenu("&File");

    QAction *newAct = new QAction("&New", this);
    newAct->setShortcut(QKeySequence::New); 
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);

    QAction *openAct = new QAction("&Open", this);
    openAct->setShortcut(QKeySequence::Open); 
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(openAct);

    QAction *saveAct = new QAction("&Save As...", this);
    saveAct->setShortcut(QKeySequence::Save); 
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);
    fileMenu->addAction(saveAct);

    fileMenu->addSeparator();

    QAction *exitAct = new QAction("E&xit", this);
    exitAct->setShortcut(QKeySequence::Quit); 
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
    fileMenu->addAction(exitAct);

    // 2. Edit Menu
    QMenu *editMenu = menuBar()->addMenu("&Edit");
    
    QAction *cutAct = editMenu->addAction("Cu&t");
    cutAct->setShortcut(QKeySequence::Cut); 
    connect(cutAct, &QAction::triggered, canvas, &Canvas::cut);

    QAction *copyAct = editMenu->addAction("&Copy");
    copyAct->setShortcut(QKeySequence::Copy); 
    connect(copyAct, &QAction::triggered, canvas, &Canvas::copy);

    QAction *pasteAct = editMenu->addAction("&Paste");
    pasteAct->setShortcut(QKeySequence::Paste); 
    connect(pasteAct, &QAction::triggered, canvas, &Canvas::paste);

    // 3. Undo/Redo (Direct on Bar)
    QAction *undoAct = new QAction(QIcon(":/resources/icons/undo.png"), "", this);
    undoAct->setShortcut(QKeySequence::Undo);
    undoAct->setToolTip("Undo (Ctrl+Z)"); 
    connect(undoAct, &QAction::triggered, canvas, &Canvas::undo);
    menuBar()->addAction(undoAct); 

    QAction *redoAct = new QAction(QIcon(":/resources/icons/redo.png"), "", this);
    redoAct->setShortcut(QKeySequence::Redo);
    redoAct->setToolTip("Redo (Ctrl+Y)");
    connect(redoAct, &QAction::triggered, canvas, &Canvas::redo);
    menuBar()->addAction(redoAct);
}