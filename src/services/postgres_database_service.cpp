#include "postgres_database_service.h"
#include <QtConcurrent/QtConcurrent>
#include <bcrypt.h>
using namespace std;

PostgresDatabaseService::PostgresDatabaseService(const std::string& conn_str, QObject* parent)
    : DatabaseService(parent), conn(conn_str) {
    if (!conn.is_open()) emit error("Database connection failed");
}

QFuture<std::vector<Product>> PostgresDatabaseService::getProductsAsync(int category_id, const std::string& search) {
    return QtConcurrent::run([this, category_id, search]() -> std::vector<Product> {
        std::vector<Product> products;
        try {
            pqxx::work txn(conn);
            std::string query = "SELECT id, name, price, stock, category_id FROM products WHERE 1=1";
            if (category_id > 0) query += " AND category_id = " + std::to_string(category_id);
            if (!search.empty()) query += " AND name ILIKE '%" + txn.esc(search) + "%'";
            pqxx::result res = txn.exec(query);
            for (const auto& row : res) {
                products.push_back({row[0].as<int>(), row[1].as<std::string>(),
                                   row[2].as<double>(), row[3].as<int>(), row[4].as<int>()});
            }
        } catch (const std::exception& e) {
            emit error(QString::fromStdString(e.what()));
        }
        return products;
    });
}

QFuture<std::vector<std::pair<std::string, int>>> PostgresDatabaseService::getCategoriesAsync() {
    return QtConcurrent::run([this]() -> std::vector<std::pair<std::string, int>> {
        std::vector<std::pair<std::string, int>> categories;
        try {
            pqxx::work txn(conn);
            pqxx::result res = txn.exec("SELECT name, id FROM categories");
            for (const auto& row : res) {
                categories.emplace_back(row[0].as<std::string>(), row[1].as<int>());
            }
        } catch (const std::exception& e) {
            emit error(QString::fromStdString(e.what()));
        }
        return categories;
    });
}

QFuture<bool> PostgresDatabaseService::addToCartAsync(int user_id, int product_id, int quantity, bool is_guest) {
    return QtConcurrent::run([this, user_id, product_id, quantity, is_guest]() -> bool {
        try {
            pqxx::work txn(conn);
            txn.exec_params("INSERT INTO cart (user_id, product_id, quantity) VALUES ($1, $2, $3)",
                           is_guest ? 0 : user_id, product_id, quantity);
            txn.commit();
            return true;
        } catch (const std::exception& e) {
            emit error(QString::fromStdString(e.what()));
            return false;
        }
    });
}

QFuture<bool> PostgresDatabaseService::placeOrderAsync(int user_id, bool is_guest) {
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
                               is_guest ? 0 : user_id, product_id, quantity, price * quantity, "Pending");
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

QFuture<bool> PostgresDatabaseService::verifyUserAsync(const std::string& username, const std::string& password, bool& is_admin) {
    return QtConcurrent::run([this, username, password, &is_admin]() -> bool {
        try {
            pqxx::work txn(conn);
            auto res = txn.exec_params1("SELECT password, is_admin FROM users WHERE username = $1", username);
            if (res.empty()) return false;
            is_admin = res[1].as<bool>();
            return bcrypt_checkpw(password.c_str(), res[0].as<std::string>().c_str()) == 0;
        } catch (const std::exception& e) {
            emit error(QString::fromStdString(e.what()));
            return false;
        }
    });
}

QFuture<bool> PostgresDatabaseService::addProductAsync(const std::string& name, double price, int stock, int category_id) {
    return QtConcurrent::run([this, name, price, stock, category_id]() -> bool {
        try {
            pqxx::work txn(conn);
            txn.exec_params("INSERT INTO products (name, price, stock, category_id) VALUES ($1, $2, $3, $4)",
                           name, price, stock, category_id);
            txn.commit();
            return true;
        } catch (const std::exception& e) {
            emit error(QString::fromStdString(e.what()));
            return false;
        }
    });
}