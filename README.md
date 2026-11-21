# Parking Lot Simulator
**Course:** Data Structures Lab (CS2001)  
**Institution:** FAST National University of Computer & Emerging Sciences (FAST-NUCES), Karachi Campus  
**Section:** BAI-3A  

### Group Members
* **Member 1:** Mohammad Taha (`25K-0097`)
* **Member 2:** Abdul Haseeb (`25K-0065`)
* **Member 3:** Izaan Aamir (`25K-0059`)

---

## Project Overview
The **Parking Lot Simulator** is a console-based vehicle management and slot-allocation system built in C++. In compliance with Data Structures lab constraints, all core data structures—**Min-Heap**, **Hash Table (Separate Chaining)**, **FIFO Queue**, and **LIFO Stack**—are implemented completely from scratch using classes, dynamic arrays, and raw pointers without relying on C++ Standard Template Library (STL) containers.

### Core Features
* **Nearest-Slot Allocation (O(log N)):** Uses a custom Min-Heap to always assign the vacant parking bay closest to the main entrance.
* **Fast License Plate Lookup & Billing (O(1) Average):** Uses a custom Hash Table with separate chaining to store active vehicles, prevent duplicate entries, and generate itemized checkout receipts based on vehicle type (`BIKE`: Rs. 50/hr, `CAR`: Rs. 100/hr, `SUV`: Rs. 150/hr).
* **Automated Overflow Waiting Line (O(1)):** Holds incoming vehicles in a pointer-based FIFO Queue when the lot reaches 100% capacity and automatically promotes waiting cars into vacated slots upon checkout.
* **Gate Entry Undo & Audit Log (O(1)):** Uses LIFO Stacks to roll back erroneous gate check-ins and maintain a chronological audit trail of check-ins, auto-assignments, and checkouts.
* **Smart Input Normalization:** Automatically converts lowercase inputs to uppercase and parses 24-hour timestamps whether entered with spaces (`09 30`), colons (`09:30`), or no separator (`0930`).

---

## Repository Structure
* `project.cpp` — Complete C++ source code containing all custom data structures (`MinHeap`, `HashTable`, `WaitQueue`, `ActionStack`), automated test suite, and interactive console menu.
* `test_cases.txt` — Standalone test input file covering normal workflows and boundary/edge cases (single-slot capacity, empty-lot checkout/undo, duplicate plates, overflow queue promotion).
* `README.md` — Project documentation, build commands, and usage guide.

---

## How to Build & Run

### Prerequisites
* A C++ compiler supporting C++17 or later (`g++`, `clang++`, or MSVC).

### 1. Compilation (Terminal / VS Code)
```bash
g++ -std=c++17 project.cpp -o parking_simulator
