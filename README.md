# High-Performance Hash Table Chatting Platform

[![Language](https://img.shields.io/badge/C%2B%2B-17%20%2F%2020-00599C?logo=c%2B%2B&logoColor=white)](https://isocpp.org)
[![Data Structure](https://img.shields.io/badge/Data%20Structure-Hash%20Table%20(Chaining)-orange)](#-data-structure-architecture)
[![Complexity](https://img.shields.io/badge/Search%20Time-O(1)%20Average-brightgreen)](#-algorithmic-complexity)
[![License](https://img.shields.io/badge/License-MIT-lightgrey.svg)](LICENSE)

A high-performance, in-memory console chat and messaging architecture written in **C++**. This project implements a custom **Hash Table data structure with separate chaining collision resolution** to manage chat rooms, message histories, and user sessions without relying on standard STL associative containers (`std::unordered_map`).

Developed as part of the Systems & Data Structures curriculum at **Misr International University (MIU)** by **Younss Yahya** and **Mohanad**.

---

## 🏛️ Data Structure Architecture

The core messaging engine is powered by an in-memory hash table of size $m = 100$ with singly linked buckets to resolve hash collisions:

```
Hash Table (Node* hashTable[100])
 ┌─────┐
 │  0  │ ──► [ Node: "general" | Messages: 12 ] ──► [ Node: "gaming" | Messages: 4 ] ──► nullptr
 ├─────┤
 │  1  │ ──► nullptr
 ├─────┤
 │  2  │ ──► [ Node: "dev-team" | Messages: 45 ] ──► nullptr
 ├─────┤
 │ ... │
 ├─────┤
 │ 99  │ ──► [ Node: "cybersecurity" | Messages: 89 ] ──► nullptr
 └─────┘
```

### 1. Hash Function Formulation
Keys (chat room names) are mapped into integer bucket indices using modular character ASCII summation:

$$h(k) = \left( \sum_{i=0}^{|k|-1} \text{ASCII}(k_i) \right) \pmod m$$

Where:
- $k$ is the string key representing the chat room identifier.
- $|k|$ is the character length of the key.
- $m = 100$ is the bucket array capacity (`TABLE_SIZE`).

### 2. Collision Handling: Separate Chaining
When two distinct chat names hash to the identical slot ($h(k_1) = h(k_2)$), the platform inserts the new `Node` into the linked bucket list at `hashTable[position]`, ensuring zero data loss and dynamic channel allocation.

---

## 📊 Algorithmic Complexity

| Operation | Average Case | Worst Case (Full Collision) | Notes |
| :--- | :---: | :---: | :--- |
| **Hash Computation** | $O(|k|)$ | $O(|k|)$ | Linear with respect to key length |
| **Insert Chat Room** | $O(1)$ | $O(n)$ | Direct bucket insertion |
| **Find Chat Room** | $O(1)$ | $O(n)$ | Linked list bucket traversal |
| **Delete Chat Room** | $O(1)$ | $O(n)$ | Pointer detachment and deallocation |
| **Append Message** | $O(1)$ | $O(1)$ | Direct vector `push_back` on found node |
| **Space Complexity** | $O(n + m)$ | $O(n + m)$ | $m = 100$ slots + $n$ active channels |

---

## ✨ Core Features

1. **User Authentication Subsystem**:
   - Secure sign up and credential verification against persistent flat-file storage (`users.txt`).
   - Session isolation tracking `currentUser`.
2. **Dynamic Chat Room Management**:
   - Create and erase channels dynamically in memory.
   - Enumerate all active conversations across hash buckets.
3. **Temporal Message Dispatch**:
   - Timestamps automatically generated using `<chrono>` and `<ctime>` formatted as `YYYY-MM-DD HH:MM:SS`.
   - Message structures tracking `content`, `sender`, and `timestamp`.
4. **State Persistence**:
   - `save()`: Serializes all hash table nodes, channels, and message transcripts into persistent storage.
   - `load()`: Reconstructs the complete hash table and linked buckets from storage on system initialization.
5. **Memory Safety**:
   - Explicit destructor `~ChatPlatform()` traversing all 100 buckets and freeing heap-allocated `Node` pointers to prevent memory leaks.

---

## 💻 CLI Interface & Menu System

```text
=== Main Menu ===
1. Sign Up
2. Sign In
3. Exit

=== Authenticated User Menu ===
1. New Chat
2. Delete Chat
3. Send Message
4. Show All Chats
5. Show Chat Messages
6. Save Chats
7. Load Chats
8. Log Out
```

---

## 🛠️ Compilation & Execution

### Prerequisites
- C++14/C++17/C++20 compliant compiler (`g++`, `clang++`, or MSVC)

### Build with GCC / MinGW:
```bash
cd "Chatting Platform"
g++ -O2 -std=c++17 ChatPlatform1.cpp -o chatting_app
./chatting_app
```

### Build with Clang:
```bash
cd "Chatting Platform"
clang++ -O2 -std=c++17 ChatPlatform1.cpp -o chatting_app
./chatting_app
```

### Build with MSVC:
```cmd
cd "Chatting Platform"
cl.exe /O2 /std:c++17 ChatPlatform1.cpp /Fe:chatting_app.exe
chatting_app.exe
```

---

## 📁 Repository Structure

```
Chatting-Platform/
├── Chatting Platform/
│   ├── ChatPlatform1.h       # Hash table definitions, struct Message, struct Node, class ChatPlatform
│   └── ChatPlatform1.cpp     # Hash table implementation, file I/O, and interactive CLI main()
├── .gitignore                # Ignore build artifacts (*.exe, *.obj, *.o, users.txt)
├── LICENSE                   # MIT License
└── README.md                 # Complete documentation
```

---

## 👥 Contributors
- **Younss Yahya** ([@youunss](https://github.com/youunss)) — Architecture, hash table lifecycle, and documentation.
- **Mohanad** ([@Honda-2005](https://github.com/Honda-2005)) — UI console flow, session persistence, and maintenance.
