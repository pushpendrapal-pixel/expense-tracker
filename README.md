
# Expense Tracker

A C++ and Next.js expense management application developed as a college group project.

## Features

- Add, view, edit, and delete expenses
- Calculate total spending
- View category-wise expense summaries
- Search expenses by category
- Store expense records in a text file
- View spending through a web dashboard
- Connect the Next.js frontend to a C++ HTTP API

## Technology Stack

- C++ — core expense management logic
- Object-Oriented Programming — classes and objects
- File Handling — text-file storage
- Next.js and JavaScript — web dashboard
- HTTP API — communication between frontend and backend
- Git and GitHub — version control

## Project Structure

- `main.cpp` — console menu
- `Expense.cpp` and `Expense.h` — expense class
- `ExpenseManager.cpp` and `ExpenseManager.h` — expense operations
- `ExpenseAPI.cpp` — HTTP API
- `expense-tracker-web/` — Next.js frontend

## How to Run

### C++ Console Application

Compile the C++ source files required for the console version, then run the generated executable.

### Web Dashboard

Open the `expense-tracker-web` directory and install dependencies:

```bash
npm install
npm run dev
```

Start the C++ API separately using the project's API build instructions. The dashboard uses the local API during development.

## Academic Project

Developed as a group project at SRM University-AP, Andhra Pradesh.
