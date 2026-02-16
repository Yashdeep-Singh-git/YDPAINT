#include "include/gui/Mainwindow.h"
#include "include/gui/Canvas.h"
#include <QToolBar>
#include <QAction>
#include <QColorDialog>
#include <QSpinBox>
#include <QLabel>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(800, 600);
    canvas = new Canvas(this);
    setCentralWidget(canvas);

    createMenus();

    currentFile = ""; 

    // --- TOOLBAR SETUP ---
    QToolBar *toolbar = addToolBar("Tools");
    toolbar->setIconSize(QSize(32, 32));
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    auto addTool = [&](const QString &name, const QString &iconPath, ToolType type) {
        QAction *act = toolbar->addAction(QIcon(iconPath), name);
        act->setToolTip(name);
        connect(act, &QAction::triggered, [this, type]() {
            canvas->setTool(type);
        });
    };

    addTool("Select",  ":/resources/icons/cursor.png" , ToolType::Select);
    toolbar->addSeparator(); toolbar->addSeparator();

    addTool("Circle",  ":/resources/icons/circle.png" , ToolType::Circle);
    addTool("Rectangle",  ":/resources/icons/rectangle.png" , ToolType::Rectangle);
    addTool("Rounded Rectangle",  ":/resources/icons/roundedrectangle.png" , ToolType::RoundedRect);
    addTool("Line",  ":/resources/icons/line.png" , ToolType::Line);
    addTool("Hexagon",  ":/resources/icons/hexagon.png" , ToolType::Hexagon);
    addTool("Free Hand",  ":/resources/icons/freehand.png" , ToolType::Freehand);
    addTool("Text",  ":/resources/icons/text.png" , ToolType::Text);

    toolbar->addSeparator(); toolbar->addSeparator();
    
    // Stroke Color
    QAction *actStrokeColor = toolbar->addAction(QIcon(":/resources/icons/stroke_color.png"), "Stroke Color");
    connect(actStrokeColor, &QAction::triggered, [this]() {
        QColor color = QColorDialog::getColor(Qt::black, this, "Select Stroke Color");
        if (color.isValid()) {
            canvas->setStrokeColor(color.name().toStdString());
        }
    });

    // Fill Color
    QAction *actFillColor = toolbar->addAction(QIcon(":/resources/icons/fill_color.png"), "Fill Color");
    connect(actFillColor, &QAction::triggered, [this]() {
        QColor color = QColorDialog::getColor(Qt::white, this, "Select Fill Color");
        if (color.isValid()) {
            canvas->setFillColor(color.name().toStdString());
        }
    });

    // No Fill
    QAction *actNoFill = toolbar->addAction("No Fill");
    connect(actNoFill, &QAction::triggered, [this]() {
        canvas->setFillColor("none");
    });

    toolbar->addSeparator(); toolbar->addSeparator();

    // Width Spinner
    toolbar->addWidget(new QLabel(" Width: ")); 
    QSpinBox *spinWidth = new QSpinBox();
    spinWidth->setRange(1, 20); 
    spinWidth->setValue(2);     
    toolbar->addWidget(spinWidth);

    connect(spinWidth, QOverload<int>::of(&QSpinBox::valueChanged), 
    [this](int newVal) {
        canvas->setStrokeWidth(newVal);
    });

    // Radius Spinner
    toolbar->addWidget(new QLabel(" R: "));
    QSpinBox *spinR = new QSpinBox();
    spinR->setRange(0, 100); 
    spinR->setValue(20);
    toolbar->addWidget(spinR);

    connect(spinR, QOverload<int>::of(&QSpinBox::valueChanged), 
    [this](int val) {
        canvas->setcurrentRadius(val);
    });
}

MainWindow::~MainWindow() {
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}