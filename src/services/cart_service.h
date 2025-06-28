#pragma once
#include <QObject>
#include <QFuture>
#include <vector>
#include <string>

class CartService : public QObject {
    Q_OBJECT
public:
    explicit CartService(QObject* parent = nullptr) : QObject(parent) {}
    virtual QFuture<bool> addToCartAsync(int user_id, int product_id, int quantity, bool is_guest) = 0;
    virtual QFuture<std::vector<std::pair<int, int>>> getCartAsync(int user_id, bool is_guest) = 0;
    virtual QFuture<bool> placeOrderAsync(int user_id, bool is_guest) = 0;
signals:
    void error(const QString& message);
};