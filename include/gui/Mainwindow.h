#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "include/gui/Canvas.h" 

class MainWindow : public QMainWindow {
    Q_OBJECT // Required for Qt signals/slots

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow(); 

private:
    Canvas* canvas; // The drawing area
};

#endif // MAINWINDOW_H