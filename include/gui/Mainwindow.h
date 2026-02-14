#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include "include/gui/Canvas.h" 

class MainWindow : public QMainWindow {
    Q_OBJECT // Required for Qt signals/slots

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow(); 

protected:
    void closeEvent(QCloseEvent *event);

private:
    Canvas* canvas; // The drawing area
    std::string currentFile;
};

#endif // MAINWINDOW_H