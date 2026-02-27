# 🏥 Smart Hospital OS Simulation (C Language)

A comprehensive Operating System simulation written in C that demonstrates core OS concepts including CPU Scheduling, Memory Management, Virtual Memory (Paging), and Secure File Handling.

This project simulates a hospital operating system environment where processes represent hospital operations, memory is managed using fixed partitions, paging is implemented using FIFO replacement, and patient records are secured using basic encryption.

---

## 🚀 Features

### 🧠 CPU Scheduling
- Priority Scheduling (High Priority Processes)
- Aging Mechanism (Prevents Starvation)
- Round Robin Scheduling (Normal Processes)
- Gantt Chart Visualization
- Waiting Time & Turnaround Time Calculation
- CPU Utilization Estimation

---

### 💾 Memory Management
- Fixed Partition Memory Allocation
- Memory Status Reporting
- Used vs Free Partition Tracking

---

### 📄 Virtual Memory (Paging)
- FIFO Page Replacement Algorithm
- Page Hit & Page Fault Tracking
- Frame Status Display

---

### 🔐 Secure File Vault System
- Simulated Encrypted File Storage
- XOR-based Encryption/Decryption
- File Ownership Validation
- Unauthorized Access Logging
- Security Violation Counter

---

## 🏗 System Architecture Overview

The system simulates:

1. Secure File Vault Initialization  
2. Process Scheduling (Priority + Round Robin)  
3. Memory Allocation (Fixed Partitions)  
4. Virtual Memory Paging Simulation  
5. Final System Performance Report  

All modules are integrated into a unified OS simulation workflow.

---

## 📊 Concepts Implemented

This project demonstrates understanding of:

- Process Control Blocks (PCB)
- Scheduling Algorithms
- Starvation & Aging
- Gantt Chart Simulation
- Fixed Partition Allocation
- Virtual Memory & Page Replacement
- Basic Cryptography (XOR Encryption)
- Security Logging Mechanisms

---

## 🛠 Technologies Used

- C Programming Language
- Standard C Libraries (`stdio.h`, `string.h`)
- ANSI Escape Codes (Terminal Coloring)

---

## ▶ How to Compile & Run

### Using GCC

```bash
gcc smart_hospital_os.c -o hospital
./hospital
```
``` On Windows (MinGW)
gcc smart_hospital_os.c -o hospital.exe
hospital.exe
```
📌 Sample Simulation Flow

Secure medical records are created and encrypted
Unauthorized access attempts are detected
High-priority processes execute first
Aging increases priority to prevent starvation
Normal processes execute using Round Robin
Memory is allocated using fixed partitions
Paging system handles page requests
Final system statistics are displayed

📈 Performance Metrics Reported

Average Waiting Time
Average Turnaround Time
CPU Utilization (Approximate)
Memory Usage Statistics
Page Hits & Page Faults
Security Violations Count

🎓 Academic Purpose

This project was developed as part of Operating Systems learning to simulate core OS components in a structured and practical way.
It combines multiple OS concepts into a single cohesive system simulation.

Author

Muhammad Bilal
BS Software Engineering
