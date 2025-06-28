#pragma once
#include <QObject>
#include <QFuture>
#include <vector>
#include <string>
#include "user_service.h"

struct Product {
    int id;
    std::string name;
    double price;
    int stock;
    int category_id;
};

class DatabaseService : public QObject {
    Q_OBJECT
public:
    explicit DatabaseService(QObject* parent = nullptr) : QObject(parent) {}
    virtual QFuture<std::vector<Product>> getProductsAsync(int category_id = 0, const std::string& search = "") = 0;
    virtual QFuture<std::vector<std::pair<std::string, int>>> getCategoriesAsync() = 0;
    virtual QFuture<bool> addToCartAsync(int user_id, int product_id, int quantity, bool is_guest) = 0;
    virtual QFuture<bool> placeOrderAsync(int user_id, bool is_guest) = 0;
    virtual QFuture<bool> verifyUserAsync(const std::string& username, const std::string& password, bool& is_admin) = 0;
    virtual QFuture<bool> addProductAsync(const std::string& name, double price, int stock, int category_id) = 0;
signals:
    void error(const QString& message);
};

