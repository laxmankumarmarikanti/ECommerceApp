#include "cartwindow.h"
#include "orderwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFutureWatcher>

CartWindow::CartWindow(DatabaseService* db_service, PaymentService* payment_service, CartService* cart_service,
                       int user_id, bool is_guest, QWidget* parent)
    : QMainWindow(parent), db_service(db_service), payment_service(payment_service), cart_service(cart_service),
      user_id(user_id), is_guest(is_guest), orderWindow(nullptr) {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    cartList = new QListWidget(this);
    checkoutButton = new QPushButton("Checkout", this);
    layout->addWidget(cartList);
    layout->addWidget(checkoutButton);

    connect(checkoutButton, &QPushButton::clicked, this, &CartWindow::checkout);
    connect(db_service, &DatabaseService::error, this, &CartWindow::onError);
    connect(cart_service, &CartService::error, this, &CartWindow::onError);
    connect(payment_service, &PaymentService::error, this, &CartWindow::onError);

    auto future = cart_service->getCartAsync(user_id, is_guest);
    connect(new QFutureWatcher<std::vector<std::pair<int, int>>>(this),
            &QFutureWatcher<std::vector<std::pair<int, int>>>::finished, this, [=]() {
                onCartLoaded(future.result());
            });
}

void CartWindow::onCartLoaded(const std::vector<std::pair<int, int>>& cart) {
    cartList->clear();
    for (const auto& [product_id, quantity] : cart) {
        auto future = db_service->getProductsAsync(product_id);
        connect(new QFutureWatcher<std::vector<Product>>(this), &QFutureWatcher<std::vector<Product>>::finished,
                this, [=]() {
                    if (!future.result().empty()) {
                        QString item = QString("%1 (Qty: %2)")
                                          .arg(QString::fromStdString(future.result()[0].name))
                                          .arg(quantity);
                        cartList->addItem(item);
                    }
                });
    }
}

void CartWindow::checkout() {
    if (cartList->count() == 0) {
        QMessageBox::warning(this, "Error", "Cart is empty");
        return;
    }
    double total = 100.00; // Simplified; calculate from cart in production
    auto payment_future = payment_service->processPaymentAsync(total, "tok_visa");
    connect(new QFutureWatcher<bool>(this), &QFutureWatcher<bool>::finished, this, [=]() {
        if (payment_future.result()) {
            auto order_future = cart_service->placeOrderAsync(user_id, is_guest);
            connect(new QFutureWatcher<bool>(this), &QFutureWatcher<bool>::finished, this, [=]() {
                if (order_future.result()) {
                    if (!orderWindow) {
                        orderWindow = new OrderWindow(this);
                    }
                    orderWindow->show();
                    orderWindow->raise();
                    cartList->clear();
                } else {
                    QMessageBox::warning(this, "Error", "Failed to place order");
                }
            });
        }
    });
}

void CartWindow::onError(const QString& error) {
    QMessageBox::warning(this, "Error", error);
}
