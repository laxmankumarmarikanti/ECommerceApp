CREATE TABLE users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password VARCHAR(255) NOT NULL, -- Hashed with bcrypt
    is_admin BOOLEAN DEFAULT FALSE
);

CREATE TABLE categories (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);

CREATE TABLE products (
    id SERIAL PRIMARY KEY,
    category_id INTEGER REFERENCES categories(id),
    name VARCHAR(255) NOT NULL,
    price DECIMAL(10, 2) NOT NULL,
    stock INTEGER NOT NULL
);

CREATE TABLE cart (
    id SERIAL PRIMARY KEY,
    user_id INTEGER, -- NULL for guest
    product_id INTEGER REFERENCES products(id),
    quantity INTEGER NOT NULL
);

CREATE TABLE orders (
    id SERIAL PRIMARY KEY,
    user_id INTEGER, -- NULL for guest
    product_id INTEGER REFERENCES products(id),
    quantity INTEGER NOT NULL,
    total_price DECIMAL(10, 2) NOT NULL,
    payment_status VARCHAR(50),
    order_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE INDEX idx_cart_user_id ON cart(user_id);
CREATE INDEX idx_product_category ON products(category_id);

-- Sample data
INSERT INTO categories (name) VALUES ('Electronics'), ('Clothing');
INSERT INTO products (category_id, name, price, stock) VALUES
(1, 'Laptop', 999.99, 10),
(1, 'Phone', 499.99, 20),
(2, 'T-Shirt', 19.99, 50);
INSERT INTO users (username, password, is_admin) VALUES
('admin', '$2a$12$K5X5z3Z7z3Z7z3Z7z3Z7z3Z7z3Z7z3Z7z3Z7z3Z7z3Z7', TRUE),
('testuser', '$2a$12$K5X5z3Z7z3Z7z3Z7z3Z7z3Z7z3Z7z3Z7z3Z7z3Z7z3Z7', FALSE);