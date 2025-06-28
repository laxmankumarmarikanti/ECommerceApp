#include "database_cart_service.h"
#include <QtConcurrent/QtConcurrent>
#include <pqxx/types.hxx> // <-- Add this line


DatabaseCartService::DatabaseCartService(const std::string& conn_str, QObject* parent)
    : CartService(parent), conn(conn_str) {
    if (!conn.is_open()) emit error("Database connection failed");
}

QFuture<bool> DatabaseCartService::addToCartAsync(int user_id, int product_id, int quantity, bool is_guest) {
    return QtConcurrent::run([this, user_id, product_id, quantity, is_guest]() -> bool {
        try {
            pqxx::work txn(conn);
            txn.exec_params("INSERT INTO cart (user_id, product_id, quantity) VALUES ($1, $2, $3)",
                is_guest ? nullptr : &user_id, product_id, quantity);
            txn.commit();
            return true;
        } catch (const std::exception& e) {
            emit error(QString::fromStdString(e.what()));
            return false;
        }
    });
}

QFuture<std::vector<std::pair<int, int>>> DatabaseCartService::getCartAsync(int user_id, bool is_guest) {
    return QtConcurrent::run([this, user_id, is_guest]() -> std::vector<std::pair<int, int>> {
        std::vector<std::pair<int, int>> cart;
        try {
            pqxx::work txn(conn);
            auto res = txn.exec_params("SELECT product_id, quantity FROM cart WHERE user_id IS " +
                                       std::string(is_guest ? "NULL" : "NOT NULL AND user_id = $1"), user_id);
            for (const auto& row : res) {
                cart.emplace_back(row[0].as<int>(), row[1].as<int>());
            }
        } catch (const std::exception& e) {
            emit error(QString::fromStdString(e.what()));
        }
        return cart;
    });
}

QFuture<bool> DatabaseCartService::placeOrderAsync(int user_id, bool is_guest) {
    return QtConcurrent::run([this, user_id, is_guest]() -> bool {
        try {
            pqxx::work txn(conn);
            auto cart = txn.exec_params("SELECT product_id, quantity FROM cart WHERE user_id IS " +
                                       std::string(is_guest ? "NULL" : "NOT NULL AND user_id = $1"), user_id);
            for (const auto& row : cart) {
                int product_id = row[0].as<int>();
                int quantity = row[1].as<int>();
                auto prod = txn.exec_params1("SELECT price, stock FROM products WHERE id = $1", product_id);
                double price = prod[0].as<double>();
                int stock = prod[1].as<int>();
                if (stock < quantity) return false;
                txn.exec_params("INSERT INTO orders (user_id, product_id, quantity, total_price, payment_status) "
                "VALUES ($1, $2, $3, $4, $5)",
                is_guest ? nullptr : &user_id, product_id, quantity, price * quantity, "Pending");
                txn.exec_params("UPDATE products SET stock = stock - $1 WHERE id = $2", quantity, product_id);
            }
            txn.exec_params("DELETE FROM cart WHERE user_id IS " +
                           std::string(is_guest ? "NULL" : "NOT NULL AND user_id = $1"), user_id);
            txn.commit();
            return true;
        } catch (const std::exception& e) {
            emit error(QString::fromStdString(e.what()));
            return false;
        }
    });
}
