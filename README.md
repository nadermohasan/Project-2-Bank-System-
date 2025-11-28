# 🏦 Bank Management System (C++ Console Application)

A complete **Bank Management System** built using **C++**, designed to
manage clients, users, and financial transactions with a **file-based
storage system** and **role-based access control**.

------------------------------------------------------------------------

## 📁 Project Features

### ✅ Client Management

-   Add new clients
-   Delete existing clients
-   Update client information
-   Search for clients by account number
-   Display all clients in a formatted table

### 💰 Transactions

-   Deposit money
-   Withdraw money
-   Prevent overdraft
-   Show total balances of all clients

### 👤 User Management

-   Add new users
-   Delete users
-   Update user data
-   Search users
-   List all users

### 🔐 Authentication System

-   Secure login system
-   Username & password validation

### 🛡 Permissions System

Each user has specific permissions stored as **bitmasks**.

------------------------------------------------------------------------

## 🗂 Data Storage

  File Name     Description
  ------------- --------------------------------
  Clients.txt   Stores all clients data
  Users.txt     Stores all users & permissions

------------------------------------------------------------------------

## ⚙️ Technologies Used

-   C++
-   File Handling (fstream)
-   Vectors
-   Structs
-   Enums
-   Bitwise Permissions

------------------------------------------------------------------------

## 🚀 How to Run

### Compile

g++ main.cpp -o bank

### Run

./bank

------------------------------------------------------------------------

## 🔑 Default Admin

Add this to Users.txt: admin\|admin123\|-1

------------------------------------------------------------------------

## 👨‍💻 Author

Developed for educational purposes.

------------------------------------------------------------------------
