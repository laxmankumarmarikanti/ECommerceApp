#include "orderwindow.h"
#include <QVBoxLayout>

OrderWindow::OrderWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    QLabel* label = new QLabel("Order placed successfully! Payment confirmed.", this);
    layout->addWidget(label);
}