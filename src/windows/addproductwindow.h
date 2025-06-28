#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "../services/database_service.h"

class AddProductWindow : public QMainWindow {
    Q_OBJECT
public:
    AddProductWindow(DatabaseService* db_service, QWidget* parent = nullptr);
private slots:
    void addProduct();
    void onCategoriesLoaded(const std::vector<std::pair<std::string, int>>& categories);
    void onError(const QString& error);
private:
    DatabaseService* db_service;
    QLineEdit* nameEdit;
    QLineEdit* priceEdit;
    QLineEdit* stockEdit;
    QComboBox* categoryCombo;
    QPushButton* addButton;
};
