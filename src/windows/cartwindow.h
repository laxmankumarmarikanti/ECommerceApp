#pragma once
#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include "../services/database_service.h"
#include "../services/payment_service.h"
#include "../services/cart_service.h"

class OrderWindow;

class CartWindow : public QMainWindow {
    Q_OBJECT
public:
    CartWindow(DatabaseService* db_service, PaymentService* payment_service, CartService* cart_service,
               int user_id, bool is_guest, QWidget* parent = nullptr);
private slots:
    void checkout();
    void onCartLoaded(const std::vector<std::pair<int, int>>& cart);
    void onError(const QString& error);
private:
    DatabaseService* db_service;
    PaymentService* payment_service;
    CartService* cart_service;
    int user_id;
    bool is_guest;
    QListWidget* cartList;
    QPushButton* checkoutButton;
    OrderWindow* orderWindow;
};
