-- 创建 employees 表
CREATE TABLE employees (
    employee_id SMALLINT PRIMARY KEY,
    first_name VARCHAR(255) NOT NULL,
    last_name VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL,
    phone VARCHAR(50),
    hire_date DATE NOT NULL,
    manager_id SMALLINT,
    job_title VARCHAR(255) NOT NULL
);

-- 创建 orders 表
CREATE TABLE orders (
    order_id SMALLINT PRIMARY KEY,
    customer_id SMALLINT NOT NULL,
    salesman_id SMALLINT NOT NULL,
    order_date DATE NOT NULL
);

-- 创建 products 表
CREATE TABLE products (
    product_id SMALLINT PRIMARY KEY,
    product_name VARCHAR(255) NOT NULL,
    description VARCHAR(2000),
    standard_cost DECIMAL(9, 2) NOT NULL,
    list_price DECIMAL(9, 2) NOT NULL,
    category_id SMALLINT NOT NULL
);

-- 创建 customers 表
CREATE TABLE customers (
    customer_id SMALLINT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    address VARCHAR(255),
    website VARCHAR(255),
    credit_limit DECIMAL(8, 2)
);

-- 创建 contact 表
CREATE TABLE contacts (
    contact_id SMALLINT PRIMARY KEY,
    first_name VARCHAR(255) NOT NULL,
    last_name VARCHAR(255) NOT NULL,
    email VARCHAR(255) NOT NULL,
    phone VARCHAR(20),
    customer_id SMALLINT NOT NULL
);

-- 创建 product_categories 表
CREATE TABLE product_categories (
    category_id SMALLINT PRIMARY KEY,
    category_name VARCHAR(255) NOT NULL
);

/*
-- 创建 inventories 表
CREATE TABLE inventories (
    product_id SMALLINT NOT NULL,
    warehouse_id SMALLINT NOT NULL,
    quantity DECIMAL(8, 2) NOT NULL,
    PRIMARY KEY (product_id, warehouse_id)
);

-- 创建 warehouses 表
CREATE TABLE warehouses (
    warehouse_id SMALLINT PRIMARY KEY,
    warehouse_name VARCHAR(255) NOT NULL,
    location_id SMALLINT NOT NULL
);

-- 创建 locations 表
CREATE TABLE locations (
    location_id SMALLINT PRIMARY KEY,
    address VARCHAR(255) NOT NULL,
    postal_code VARCHAR(20),
    city VARCHAR(50) NOT NULL,
    state VARCHAR(50),
    country_id CHAR(2) NOT NULL
);

-- 创建 countries 表
CREATE TABLE countries (
    country_id CHAR(2) PRIMARY KEY,
    country_name VARCHAR(40) NOT NULL,
    region_id SMALLINT NOT NULL
);

-- 创建 regions 表
CREATE TABLE regions (
    region_id SMALLINT PRIMARY KEY,
    region_name VARCHAR(50) NOT NULL
);
*/    