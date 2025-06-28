#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include "../services/database_service.h"

class MainWindow;

class LoginWindow : public QMainWindow {
    Q_OBJECT
public:
    LoginWindow(UserService* user_service, DatabaseService* db_service, QWidget* parent = nullptr);
private slots:
    void login();
    void guestCheckout();
    void onLoginResult(bool success, bool is_admin);
    void onError(const QString& error);
private:
    UserService* user_service;
    DatabaseService* db_service;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;
    QPushButton* guestButton;
    MainWindow* mainWindow;
};

