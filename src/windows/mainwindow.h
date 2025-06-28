#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include "../services/database_service.h"
#include "../services/cart_service.h"
#include "../services/payment_service.h"

class CartWindow;
class AddProductWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(DatabaseService* db_service, PaymentService* payment_service, CartService* cart_service,
               int user_id, bool is_guest, bool is_admin, QWidget* parent = nullptr);
private slots:
    void addToCart();
    void openCart();
    void openAddProduct();
    void searchProducts();
    void onCategoryChanged(int index);
    void onProductsLoaded(const std::vector<Product>& products);
    void onCategoriesLoaded(const std::vector<std::pair<std::string, int>>& categories);
    void onError(const QString& error);
private:
    DatabaseService* db_service;
    PaymentService* payment_service;
    CartService* cart_service;
    int user_id;
    bool is_guest;
    bool is_admin;
    QComboBox* categoryCombo;
    QLineEdit* searchEdit;
    QTableWidget* productTable;
    QPushButton* addCartButton;
    QPushButton* viewCartButton;
    QPushButton* addProductButton;
    CartWindow* cartWindow;
    AddProductWindow* addProductWindow;
};
