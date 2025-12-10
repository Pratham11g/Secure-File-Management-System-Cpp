# Secure File Management System (C++)

This is a **Secure File Management System** implemented in **C++**, designed as part of the Operating Systems (CSE316) coursework at Lovely Professional University.

The system provides:
- Password-based authentication  
- Optional Two-Factor Authentication (OTP)  
- Secure file encryption (XOR-based)  
- Access control (owner + shared users)  
- Secure sharing of files  
- File metadata  
- Malware simulation check  
- Full console-based interface (no external libraries required)

---

## 🚀 Features

### 🔐 Authentication
- User registration  
- Password hashing (simple hash)  
- Login system  
- Two-Factor Authentication (OTP)  

### 🔏 File Security
- XOR encryption + decryption  
- Secure upload  
- Secure read  
- Metadata stored in RAM (owner, size, filename)  
- Owner-based access control  
- File sharing with specific users  

### ⚠️ Threat Detection
- Simple malware simulation using hash matching  
- Blocks upload of files matching known malicious hash  

### 🖥️ Console Interface
- Easy to use menu  
- Fully runs inside VS Code terminal  
- No external dependencies  

---

## 🛠️ Installation & Usage

### 1️⃣ Clone the Repository
