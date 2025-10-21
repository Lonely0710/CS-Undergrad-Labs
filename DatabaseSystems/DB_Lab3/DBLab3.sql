-- 创建 Customer 表
CREATE TABLE Customer (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    phone VARCHAR(15) UNIQUE NOT NULL,
    UNIQUE INDEX idx_customer_phone (phone)  -- 唯一索引
) ENGINE=InnoDB;

-- 创建 Orders 表
CREATE TABLE Orders (
    order_id INT AUTO_INCREMENT PRIMARY KEY,
    customer_id INT NOT NULL,
    order_date DATETIME NOT NULL,
    FOREIGN KEY (customer_id) REFERENCES Customer(id) ON DELETE CASCADE,
    INDEX idx_order_date_customer (order_date, customer_id)  
) ENGINE=InnoDB;

-- 创建 Payment 表
CREATE TABLE Payment (
    payment_id INT AUTO_INCREMENT PRIMARY KEY,
    order_id INT NOT NULL,
    amount DECIMAL(10,2) NOT NULL CHECK (amount > 0),
    method VARCHAR(50) NOT NULL,
    UNIQUE INDEX idx_order_payment (order_id),  -- 确保订单唯一支付
    FOREIGN KEY (order_id) REFERENCES `Orders`(order_id) ON DELETE CASCADE
) ENGINE=InnoDB;

-- 创建 Menu 表
CREATE TABLE Menu (
    menu_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    price DECIMAL(10,2) NOT NULL CHECK (price > 0),
    category VARCHAR(50) NOT NULL DEFAULT 'Uncategorized',
    UNIQUE INDEX idx_menu_name (name)  -- 唯一索引
) ENGINE=InnoDB;

-- 创建 OrderItem 表
CREATE TABLE OrderItem (
    order_id INT NOT NULL,
    menu_id INT NOT NULL,
    quantity INT NOT NULL CHECK (quantity > 0),
    PRIMARY KEY (order_id, menu_id),
    FOREIGN KEY (order_id) REFERENCES `Orders`(order_id) ON DELETE CASCADE,
    FOREIGN KEY (menu_id) REFERENCES Menu(menu_id) ON DELETE CASCADE
) ENGINE=InnoDB;
