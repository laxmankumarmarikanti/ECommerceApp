
#pragma once
#include "cart_service.h"
#include <pqxx/pqxx>

class DatabaseCartService : public CartService {
public:
    DatabaseCartService(const std::string& conn_str, QObject* parent = nullptr);
    QFuture<bool> addToCartAsync(int user_id, int product_id, int quantity, bool is_guest) override;
    QFuture<std::vector<std::pair<int, int>>> getCartAsync(int user_id, bool is_guest) override;
    QFuture<bool> placeOrderAsync(int user_id, bool is_guest) override;
private:
    pqxx::connection conn;
};
