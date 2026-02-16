#include "include/gui/Mainwindow.h"
#include "include/gui/Canvas.h"
#include <QToolBar>
#include <QAction>
#include <QColorDialog>
#include <QSpinBox>
#include <QLabel>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(800, 600);                                            // Set default window size
    canvas = new Canvas(this);                                   // Initialize drawing area
    setCentralWidget(canvas);                                    // Set canvas as main widget

    createMenus();                                               // Initialize menu bar

    currentFile = "";                                            // Reset current filename

    // --- TOOLBAR SETUP ---
    QToolBar *toolbar = addToolBar("Tools");                     // Create main toolbar
    toolbar->setIconSize(QSize(32, 32));                         // Set larger icon size
    toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);         // Hide text labels

    auto addTool = [&](const QString &name, const QString &iconPath, ToolType type) {
        QAction *act = toolbar->addAction(QIcon(iconPath), name);// Create action with icon
        act->setToolTip(name);                                   // Set hover text
        connect(act, &QAction::triggered, [this, type]() {       // Link action to slot
            canvas->setTool(type);                               // Change active tool
        });
    };

    addTool("Select",  ":/resources/icons/cursor.png" , ToolType::Select);
    toolbar->addSeparator(); toolbar->addSeparator();            // Add visual spacer

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
            canvas->setStrokeColor(color.name().toStdString());  // Update stroke color
        }
    });

    // Fill Color
    QAction *actFillColor = toolbar->addAction(QIcon(":/resources/icons/fill_color.png"), "Fill Color");
    connect(actFillColor, &QAction::triggered, [this]() {
        QColor color = QColorDialog::getColor(Qt::white, this, "Select Fill Color");
        if (color.isValid()) {
            canvas->setFillColor(color.name().toStdString());    // Update fill color
        }
    });

    // No Fill
    QAction *actNoFill = toolbar->addAction("No Fill");
    connect(actNoFill, &QAction::triggered, [this]() {
        canvas->setFillColor("none");                            // Set transparent fill
    });

    toolbar->addSeparator(); toolbar->addSeparator();

    // Width Spinner
    toolbar->addWidget(new QLabel(" Width: "));                  // Add label
    QSpinBox *spinWidth = new QSpinBox();
    spinWidth->setRange(1, 20);                                  // Set min/max width
    spinWidth->setValue(2);                                      // Set default width
    toolbar->addWidget(spinWidth);                               // Add spinner to toolbar

    connect(spinWidth, QOverload<int>::of(&QSpinBox::valueChanged), 
    [this](int newVal) {
        canvas->setStrokeWidth(newVal);                          // Update stroke width
    });

    // Radius Spinner
    toolbar->addWidget(new QLabel(" R: "));                      // Add label
    QSpinBox *spinR = new QSpinBox();
    spinR->setRange(0, 100);                                     // Set min/max radius
    spinR->setValue(20);                                         // Set default radius
    toolbar->addWidget(spinR);                                   // Add spinner to toolbar

    connect(spinR, QOverload<int>::of(&QSpinBox::valueChanged), 
    [this](int val) {
        canvas->setcurrentRadius(val);                           // Update corner radius
    });
}

MainWindow::~MainWindow() {
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave())                                             // Check for unsaved changes
        event->accept();                                         // Close window
    else
        event->ignore();                                         // Cancel close
}