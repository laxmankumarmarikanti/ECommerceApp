
#pragma once
#include "user_service.h"
#include <pqxx/pqxx>

class DatabaseUserService : public UserService {
public:
    DatabaseUserService(const std::string& conn_str, QObject* parent = nullptr);
    QFuture<bool> verifyUserAsync(const std::string& username, const std::string& password, bool& is_admin) override;
private:
    pqxx::connection conn;
};
