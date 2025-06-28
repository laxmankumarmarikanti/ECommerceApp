#pragma once
#include "database_service.h"
#include <pqxx/pqxx>


class PostgresDatabaseService : public DatabaseService {
    Q_OBJECT
public:
    PostgresDatabaseService(const std::string& conn_str, QObject* parent = nullptr);
    QFuture<std::vector<Product>> getProductsAsync(int category_id = 0, const std::string& search = "") override;
    QFuture<std::vector<std::pair<std::string, int>>> getCategoriesAsync() override;
    QFuture<bool> addToCartAsync(int user_id, int product_id, int quantity, bool is_guest) override;
    QFuture<bool> placeOrderAsync(int user_id, bool is_guest) override;
    QFuture<bool> verifyUserAsync(const std::string& username, const std::string& password, bool& is_admin) override;
    QFuture<bool> addProductAsync(const std::string& name, double price, int stock, int category_id) override;
private:
    pqxx::connection conn;
};
