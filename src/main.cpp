// main.cpp
#include "windows/loginwindow.h"
#include "services/postgres_database_service.h"
#include "services/stripe_payment_service.h"
#include "services/database_cart_service.h"
#include "services/database_user_service.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    auto db_service = new PostgresDatabaseService("host=localhost port=5432 dbname=ecommerce_db user=laxman password=laxman");
    auto payment_service = new StripePaymentService("sk_test_your_stripe_key");
    auto cart_service = new DatabaseCartService("host=localhost port=5432 dbname=ecommerce_db user=laxman password=laxman");
    auto user_service = new DatabaseUserService("host=localhost port=5432 dbname=ecommerce_db user=laxman password=laxman");
    LoginWindow window(user_service, db_service);
    window.show();
    return app.exec();
}