# StorageEngine

A custom, memory-managed in-memory key-value database engine built from scratch in C++. 

## About This Project
This is a **beginner-level project** developed to explore the fundamentals of database architecture. The goal was to build a functional storage engine from the ground up, bypassing standard library containers to gain a deeper, practical understanding of C++ memory management, pointer manipulation, and data structure design.

## Technical Approach
* **Custom Indexing:** Implemented a **Separate-Chaining Hash Table** to manage key-value pairs.
* **Memory Management:** Focused on raw pointer usage and manual memory allocation/deallocation.
* **Learning Objective:** This project served as an exercise in building reliable software infrastructure and handling common data structure challenges like collision resolution.

## Features
* **CRUD Operations:** Supports `SET`, `GET`, and `DEL` for record management.
* **Persistence:** Includes `SAVE` functionality to write the database state to a flat file.
* **CLI Interface:** An interactive command-line tool for real-time interaction with the engine.
* **Diagnostics:** Includes a `STATS` command to monitor hash table bucket distribution.

## Commands
| Command | Description |
| :--- | :--- |
| `SET [key] [age] [major]` | Adds or updates a record. |
| `GET [key]` | Retrieves record data. |
| `DEL [key]` | Removes a record. |
| `STATS` | Displays database metrics/health. |
| `SAVE` | Persists state to disk. |
| `EXIT` | Safely saves and exits. |
| `COMMANDS` | Print the list of commands. |

## Why I Built This
As I continue my journey into systems programming, I wanted to move beyond high-level abstractions. This project helped me bridge the gap between theoretical knowledge of hash tables and the reality of implementing them in production-grade C++. 

---
*Developed as a learning project to master low-level C++.*
