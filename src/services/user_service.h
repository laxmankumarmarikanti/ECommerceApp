#pragma once
#include <QObject>
#include <QFuture>
#include <string>

class UserService : public QObject {
    Q_OBJECT
public:
    explicit UserService(QObject* parent = nullptr) : QObject(parent) {}
    virtual QFuture<bool> verifyUserAsync(const std::string& username, const std::string& password, bool& is_admin) = 0;
signals:
    void error(const QString& message);
};