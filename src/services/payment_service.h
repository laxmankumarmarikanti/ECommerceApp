#pragma once
#include <QObject>
#include <QFuture>
#include <string>

class PaymentService : public QObject {
    Q_OBJECT
public:
    explicit PaymentService(QObject* parent = nullptr) : QObject(parent) {}
    virtual QFuture<bool> processPaymentAsync(double amount, const std::string& payment_details) = 0;
signals:
    void error(const QString& message);
};