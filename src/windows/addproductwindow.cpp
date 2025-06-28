#include "addproductwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFutureWatcher>

AddProductWindow::AddProductWindow(DatabaseService* db_service, QWidget* parent)
    : QMainWindow(parent), db_service(db_service) {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Product Name");
    priceEdit = new QLineEdit(this);
    priceEdit->setPlaceholderText("Price");
    stockEdit = new QLineEdit(this);
    stockEdit->setPlaceholderText("Stock");
    categoryCombo = new QComboBox(this);
    addButton = new QPushButton("Add Product", this);

    layout->addWidget(nameEdit);
    layout->addWidget(priceEdit);
    layout->addWidget(stockEdit);
    layout->addWidget(categoryCombo);
    layout->addWidget(addButton);

    connect(addButton, &QPushButton::clicked, this, &AddProductWindow::addProduct);
    connect(db_service, &DatabaseService::error, this, &AddProductWindow::onError);

    auto future = db_service->getCategoriesAsync();
    connect(new QFutureWatcher<std::vector<std::pair<std::string, int>>>(this),
            &QFutureWatcher<std::vector<std::pair<std::string, int>>>::finished, this, [=]() {
                onCategoriesLoaded(future.result());
            });
}

void AddProductWindow::onCategoriesLoaded(const std::vector<std::pair<std::string, int>>& categories) {
    for (const auto& [name, id] : categories) {
        categoryCombo->addItem(QString::fromStdString(name), id);
    }
}

void AddProductWindow::addProduct() {
    QString name = nameEdit->text();
    bool price_ok, stock_ok;
    double price = priceEdit->text().toDouble(&price_ok);
    int stock = stockEdit->text().toInt(&stock_ok);
    int category_id = categoryCombo->currentData().toInt();
    if (name.isEmpty() || !price_ok || !stock_ok || price <= 0 || stock < 0) {
        QMessageBox::warning(this, "Error", "Invalid input");
        return;
    }
    auto future = db_service->addProductAsync(name.toStdString(), price, stock, category_id);
    connect(new QFutureWatcher<bool>(this), &QFutureWatcher<bool>::finished, this, [=]() {
        if (future.result()) {
            QMessageBox::information(this, "Success", "Product added");
            nameEdit->clear();
            priceEdit->clear();
            stockEdit->clear();
        } else {
            QMessageBox::warning(this, "Error", "Failed to add product");
        }
    });
}

void AddProductWindow::onError(const QString& error) {
    QMessageBox::warning(this, "Error", error);
}

