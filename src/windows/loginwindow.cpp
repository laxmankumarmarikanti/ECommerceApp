#include "loginwindow.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFutureWatcher>
#include <QLineEdit>
#include <QPushButton>

LoginWindow::LoginWindow(UserService* user_service, DatabaseService* db_service, QWidget* parent)
    : QMainWindow(parent), user_service(user_service), db_service(db_service), mainWindow(nullptr) {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    usernameEdit = new QLineEdit(centralWidget);
    passwordEdit = new QLineEdit(centralWidget);
    loginButton = new QPushButton("Login", centralWidget);
    guestButton = new QPushButton("Guest Checkout", centralWidget);

    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(guestButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::login);
    connect(guestButton, &QPushButton::clicked, this, &LoginWindow::guestCheckout);
    connect(user_service, &UserService::error, this, &LoginWindow::onError);
}

void LoginWindow::login() {
    bool is_admin = false;
    auto future = user_service->verifyUserAsync(usernameEdit->text().toStdString(), passwordEdit->text().toStdString(), is_admin);
    connect(new QFutureWatcher<bool>(this), &QFutureWatcher<bool>::finished, this, [=]() {
        onLoginResult(future.result(), is_admin);
    });
}

void LoginWindow::guestCheckout() {
    if (!mainWindow) {
        mainWindow = new MainWindow(db_service, nullptr, nullptr, 0, true, false, this);
    }
    mainWindow->show();
    hide();
}

void LoginWindow::onLoginResult(bool success, bool is_admin) {
    if (success) {
        if (!mainWindow) {
            mainWindow = new MainWindow(db_service, nullptr, nullptr, 1, false, is_admin, this);
        }
        mainWindow->show();
        hide();
    } else {
        QMessageBox::warning(this, "Error", "Invalid credentials");
    }
}

void LoginWindow::onError(const QString& error) {
    QMessageBox::warning(this, "Error", error);
}
