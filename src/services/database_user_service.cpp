
#include "database_user_service.h"
#include <QtConcurrent/QtConcurrent>
#include <bcrypt.h>

DatabaseUserService::DatabaseUserService(const std::string& conn_str, QObject* parent)
    : UserService(parent), conn(conn_str) {
    if (!conn.is_open()) emit error("Database connection failed");
}

QFuture<bool> DatabaseUserService::verifyUserAsync(const std::string& username, const std::string& password, bool& is_admin) {
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