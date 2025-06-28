# ECommerce
## Files
```
ECommerceApp/
├── src/
│   ├── main.cpp                    # Entry point, initializes services and LoginWindow
│   ├── services/
│   │   ├── database_service.h      # DatabaseService interface
│   │   ├── postgres_database_service.h  # PostgresDatabaseService implementation
│   │   ├── postgres_database_service.cpp
│   │   ├── payment_service.h       # PaymentService interface
│   │   ├── stripe_payment_service.h  # StripePaymentService implementation
│   │   ├── stripe_payment_service.cpp
│   │   ├── cart_service.h          # CartService interface
│   │   ├── database_cart_service.h  # DatabaseCartService implementation
│   │   ├── database_cart_service.cpp
│   │   ├── user_service.h          # UserService interface
│   │   ├── database_user_service.h  # DatabaseUserService implementation
│   │   ├── database_user_service.cpp
│   ├── windows/
│   │   ├── loginwindow.h          # LoginWindow class
│   │   ├── loginwindow.cpp
│   │   ├── mainwindow.h           # MainWindow class
│   │   ├── mainwindow.cpp
│   │   ├── cartwindow.h           # CartWindow class
│   │   ├── cartwindow.cpp
│   │   ├── orderwindow.h          # OrderWindow class
│   │   ├── orderwindow.cpp
│   │   ├── addproductwindow.h     # AddProductWindow class
│   │   ├── addproductwindow.cpp
├── resources/
│   ├── ECommerceApp.qrc           # Qt resource file (for icons/images, if used)
│   ├── ui/
│       ├── loginwindow.ui         # UI file for LoginWindow (Qt Designer)
│       ├── mainwindow.ui          # UI file for MainWindow
│       ├── cartwindow.ui          # UI file for CartWindow
│       ├── orderwindow.ui         # UI file for OrderWindow
│       ├── addproductwindow.ui    # UI file for AddProductWindow
├── ECommerceApp.pro               # Qt project file
├── README.md                      # Project documentation
├── LICENSE                        # License file (e.g., MIT, optional)
├── build/                         # Build directory (generated)
├── scripts/
│   ├── setup_db.sql               # SQL script for PostgreSQL schema and sample data
```
## Class Structure
```
+----------------+       +----------------+       +----------------+       +----------------+       +----------------+
|   LoginWindow  |       |   MainWindow   |       |   CartWindow   |       |   OrderWindow  |       |AddProductWindow|
+----------------+       +----------------+       +----------------+       +----------------+       +----------------+
| login()        |       | addToCart()    |       | checkout()     |       | showOrder()    |       | addProduct()   |
| guestCheckout()|       | openCart()     |       |                |       |                |       |                |
|                |       | search()       |       |                |       |                |       |                |
|                |       | addProduct()   |       |                |       |                |       |                |
+----------------+       +----------------+       +----------------+       +----------------+       +----------------+
         |                        |                        |                        |                        |
         +------------------------+------------------------+------------------------+------------------------+
                                  v                        v                        v                        v
+------------------+       +------------------+       +------------------+       +------------------+
| DatabaseService  |       | PaymentService   |       | CartService      |       | UserService      |
| (Interface)      |       | (Interface)      |       | (Interface)      |       | (Interface)      |
+------------------+       +------------------+       +------------------+       +------------------+
| getProducts()    |       | processPayment() |       | addToCart()      |       | verifyUser()     |
| addProduct()     |       |                  |       | getCart()        |       |                  |
| addToCart()      |       |                  |       | placeOrder()     |       |                  |
| placeOrder()     |       |                  |       |                  |       |                  |
+------------------+       +------------------+       +------------------+       +------------------+
         |                        |                        |                        |
         v                        v                        v                        v
+------------------+       +------------------+       +------------------+       +------------------+
| PostgresDatabase |       | StripePayment    |       | DatabaseCart     |       | DatabaseUser     |
| Service          |       | Service          |       | Service          |       | Service          |
+------------------+       +------------------+       +------------------+       +------------------+
         |                        |                        |                        |
         v                        v                        v                        v
+------------------+       +------------------+       +------------------+       +------------------+
| PostgreSQL DB    |       | Stripe API       |       | PostgreSQL DB    |       | PostgreSQL DB    |
+------------------+       +------------------+       +------------------+       +------------------+

```

### Build
```
make clean
qmake 
make
```