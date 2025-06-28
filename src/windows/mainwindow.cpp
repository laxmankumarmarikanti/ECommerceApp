#include "mainwindow.h"
#include "cartwindow.h"
#include "addproductwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFutureWatcher>

MainWindow::MainWindow(DatabaseService* db_service, PaymentService* payment_service, CartService* cart_service,
                       int user_id, bool is_guest, bool is_admin, QWidget* parent)
    : QMainWindow(parent), db_service(db_service), payment_service(payment_service), cart_service(cart_service),
      user_id(user_id), is_guest(is_guest), is_admin(is_admin), cartWindow(nullptr), addProductWindow(nullptr) {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    categoryCombo = new QComboBox(this);
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("Search products...");
    productTable = new QTableWidget(0, 3, this);
    productTable->setHorizontalHeaderLabels({"Name", "Price", "Stock"});
    addCartButton = new QPushButton("Add to Cart", this);
    viewCartButton = new QPushButton("View Cart", this);
    addProductButton = new QPushButton("Add Product", this);
    addProductButton->setVisible(is_admin);

    layout->addWidget(categoryCombo);
    layout->addWidget(searchEdit);
    layout->addWidget(productTable);
    layout->addWidget(addCartButton);
    layout->addWidget(viewCartButton);
    layout->addWidget(addProductButton);

    connect(addCartButton, &QPushButton::clicked, this, &MainWindow::addToCart);
    connect(viewCartButton, &QPushButton::clicked, this, &MainWindow::openCart);
    connect(addProductButton, &QPushButton::clicked, this, &MainWindow::openAddProduct);
    connect(searchEdit, &QLineEdit::returnPressed, this, &MainWindow::searchProducts);
    connect(categoryCombo, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::onCategoryChanged);
    connect(db_service, &DatabaseService::error, this, &MainWindow::onError);

    auto cat_future = db_service->getCategoriesAsync();
    connect(new QFutureWatcher<std::vector<std::pair<std::string, int>>>(this),
            &QFutureWatcher<std::vector<std::pair<std::string, int>>>::finished, this, [=]() {
                onCategoriesLoaded(cat_future.result());
            });

    auto prod_future = db_service->getProductsAsync();
    connect(new QFutureWatcher<std::vector<Product>>(this), &QFutureWatcher<std::vector<Product>>::finished,
            this, [=]() { onProductsLoaded(prod_future.result()); });
}

void MainWindow::onCategoriesLoaded(const std::vector<std::pair<std::string, int>>& categories) {
    categoryCombo->addItem("All Categories", 0);
    for (const auto& [name, id] : categories) {
        categoryCombo->addItem(QString::fromStdString(name), id);
    }
}

void MainWindow::onProductsLoaded(const std::vector<Product>& products) {
    productTable->setRowCount(products.size());
    for (size_t i = 0; i < products.size(); ++i) {
        productTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(products[i].name)));
        productTable->setItem(i, 1, new QTableWidgetItem(QString::number(products[i].price)));
        productTable->setItem(i, 2, new QTableWidgetItem(QString::number(products[i].stock)));
    }
}

void MainWindow::addToCart() {
    int row = productTable->currentRow();
    if (row >= 0) {
        int product_id = row + 1; // Assuming IDs start at 1
        auto future = cart_service->addToCartAsync(user_id, product_id, 1, is_guest);
        connect(new QFutureWatcher<bool>(this), &QFutureWatcher<bool>::finished, this, [=]() {
            if (future.result()) {
                QMessageBox::information(this, "Success", "Added to cart");
            } else {
                QMessageBox::warning(this, "Error", "Failed to add to cart");
            }
        });
    } else {
        QMessageBox::warning(this, "Error", "Select a product");
    }
}

void MainWindow::openCart() {
    if (!cartWindow) {
        cartWindow = new CartWindow(db_service, payment_service, cart_service, user_id, is_guest, this);
    }
    cartWindow->raise();
}

void MainWindow::openAddProduct() {
    if (!addProductWindow) {
        addProductWindow = new AddProductWindow(db_service, this);
    }
    addProductWindow->show();
    addProductWindow->raise();
}

void MainWindow::searchProducts() {
    int category_id = categoryCombo->currentData().toInt();
    auto future = db_service->getProductsAsync(category_id, searchEdit->text().toStdString());
    connect(new QFutureWatcher<std::vector<Product>>(this),
            &QFutureWatcher<std::vector<Product>>::finished,
            this, [=]() { onProductsLoaded(future.result()); });
}

void MainWindow::onCategoryChanged(int /*index*/) {
    searchProducts();
}

void MainWindow::onError(const QString& error) {
    QMessageBox::warning(this, "Error", error);
}
