create database tms;
use tms;

CREATE TABLE admin (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL
);

INSERT INTO admin (username, password) VALUES ('admin', 'admin123');


CREATE TABLE vehicles (
    vehicle_id INT AUTO_INCREMENT PRIMARY KEY,
    vehicle_number VARCHAR(20) NOT NULL UNIQUE,
    vehicle_type VARCHAR(50),
    status ENUM('available', 'assigned', 'maintenance') DEFAULT 'available'
);

CREATE TABLE drivers (
    driver_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    license_number VARCHAR(50) NOT NULL UNIQUE,
    phone VARCHAR(20)
);

CREATE TABLE orders (
    order_id INT AUTO_INCREMENT PRIMARY KEY,
    customer_name VARCHAR(100) NOT NULL,
    destination VARCHAR(255) NOT NULL,
    cargo_details TEXT,
    status VARCHAR(50) DEFAULT 'Pending'
);

ALTER TABLE orders
ADD COLUMN driver_id INT,
ADD COLUMN vehicle_id INT,
ADD FOREIGN KEY (driver_id) REFERENCES drivers(driver_id),
ADD FOREIGN KEY (vehicle_id) REFERENCES vehicles(vehicle_id);


ALTER TABLE admin ADD COLUMN role ENUM('superadmin', 'admin') DEFAULT 'admin';

INSERT INTO admin (username, password, role) VALUES ('superadmin', 'pass', 'superadmin');