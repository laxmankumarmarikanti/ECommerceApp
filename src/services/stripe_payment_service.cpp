
#include "stripe_payment_service.h"
#include <curl/curl.h>
#include <QtConcurrent/QtConcurrent>

StripePaymentService::StripePaymentService(const std::string& api_key, QObject* parent)
    : PaymentService(parent), api_key(api_key) {}

QFuture<bool> StripePaymentService::processPaymentAsync(double amount, const std::string& payment_details) {
    return QtConcurrent::run([this, amount, payment_details]() -> bool {
        CURL* curl = curl_easy_init();
        if (!curl) {
            emit error("Failed to initialize CURL");
            return false;
        }
        std::string post_data = "amount=" + std::to_string(static_cast<int>(amount * 100)) +
                               "¤cy=usd&source=" + payment_details + "&description=Order Payment";
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.stripe.com/v1/charges");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, (api_key + ":").c_str());
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res != CURLE_OK) {
            emit error(QString::fromStdString(curl_easy_strerror(res)));
            return false;
        }
        return true;
    });
}
