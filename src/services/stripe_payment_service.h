
#pragma once
#include "payment_service.h"

class StripePaymentService : public PaymentService {
public:
    StripePaymentService(const std::string& api_key, QObject* parent = nullptr);
    QFuture<bool> processPaymentAsync(double amount, const std::string& payment_details) override;
private:
    std::string api_key;
};
