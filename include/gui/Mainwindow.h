#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include "include/gui/Canvas.h" 

class MainWindow : public QMainWindow {
    Q_OBJECT // Required for Qt signals/slots

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow(); 

protected:
    void closeEvent(QCloseEvent *event);

private:
    Canvas* canvas;
    std::string currentFile;
    void openFile();
    void saveFile();
    void createMenus();
    void newFile();
    bool maybeSave();
};

#endif 