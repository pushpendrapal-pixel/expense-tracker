
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <sstream>
#include <limits>
#include <string>
#include "ExpenseManager.h"

using namespace std;
string toLowerCase(string text) {
    for (int i = 0; i < text.length(); i++) {
        text[i] = tolower(static_cast<unsigned char>(text[i]));
    }
    return text;
}

// Constructor
ExpenseManager::ExpenseManager() {
    count = 0;
    loadExpenses();
}

// Add a new expense
void ExpenseManager::addExpense() {
    if (count >= 100) {
        cout << "Expense limit reached!\n";
        return;
    }

    double amount;
    string category, date, description;

    cout << "Enter amount: Rs. ";
    cin >> amount;

    if (cin.fail() || amount <= 0) {
        cout << "Invalid amount!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter category: ";
    getline(cin, category);

    cout << "Enter date (YYYY-MM-DD): ";
    getline(cin, date);

    cout << "Enter description: ";
    getline(cin, description);

    
int newId = 1;

for (int i = 0; i < count; i++) {
    if (expenses[i].getId() >= newId) {
        newId = expenses[i].getId() + 1;
    }
}

expenses[count] = Expense(
    newId, amount, category, date, description
);

count++;
saveExpenses();


    cout << "Expense added successfully!\n"; 
}

// Display all expenses
void ExpenseManager::viewExpenses() const {
    if (count == 0) {
        cout << "No expenses recorded yet.\n";
        return;
    }

    cout << "\n===== ALL EXPENSES =====\n";

    for (int i = 0; i < count; i++) {
        cout << "\nExpense " << i + 1 << "\n";
        expenses[i].display();
    }
}

// Calculate total amount
double ExpenseManager::calculateTotal() const {
    double total = 0.0;

    for (int i = 0; i < count; i++) {
        total += expenses[i].getAmount();
    }

    return total;
}

void ExpenseManager::categorySummary() const {
    if (count == 0) {
        cout << "No expenses recorded yet.\n";
        return;
    }

    bool processed[100] = {false};

    cout << "\n===== CATEGORY SUMMARY =====\n";

    for (int i = 0; i < count; i++) {
        if (processed[i]) {
            continue;
        }

        double categoryTotal = 0.0;

        for (int j = i; j < count; j++) {
            if (expenses[i].getCategory() ==
                expenses[j].getCategory()) {

                categoryTotal += expenses[j].getAmount();
                processed[j] = true;
            }
        }

        cout << expenses[i].getCategory()
             << ": Rs. " << categoryTotal << endl;
    }

    cout << "----------------------------\n";
    cout << "Total Expenses: Rs. "
         << calculateTotal() << endl;
}

void ExpenseManager::searchByCategory() const {
    if (count == 0) {
        cout << "No expenses recorded yet.\n";
        return;
    }

    string searchCategory;
    bool found = false;

    cout << "Enter category to search: ";
    cin>> ws;
    getline(cin, searchCategory);

    cout << "\n===== EXPENSES IN "
         << searchCategory << " =====\n";

    for (int i = 0; i < count; i++) {
       if (toLowerCase(expenses[i].getCategory()) ==
            toLowerCase(searchCategory)) { 
            expenses[i].display();
            cout << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No expenses found in this category.\n";
    }
}

void ExpenseManager::monthlyExpenseSummary() const {
    if (count == 0) {
        cout << "No expenses recorded yet.\n";
        return;
    }

    string month;
    double monthlyTotal = 0.0;
    int expensesFound = 0;

    cout << "Enter month (YYYY-MM): ";
    cin >> ws;
    getline(cin, month);

    if (month.length() != 7 || month[4] != '-') {
        cout << "Invalid format! Use YYYY-MM (example: 2026-10).\n";
        return;
    }

    cout << "\n===== MONTHLY EXPENSE SUMMARY: "
         << month << " =====\n";

    for (int i = 0; i < count; i++) {
        string expenseDate = expenses[i].getDate();

        if (expenseDate.length() >= 7 &&
            expenseDate.substr(0, 7) == month) {

            expenses[i].display();
            cout << endl;

            monthlyTotal += expenses[i].getAmount();
            expensesFound++;
        }
    }

    if (expensesFound == 0) {
        cout << "No expenses found for this month.\n";
    } else {
        cout << "Number of expenses: " << expensesFound << endl;
        cout << "Total spent in " << month
             << ": Rs. " << monthlyTotal << endl;
    }
}



void ExpenseManager::loadExpenses() {
    ifstream file("expenses.txt");

    if (!file.is_open()) {
        return;
    }

    string line;

    while (getline(file, line) && count < 100) {
        stringstream ss(line);

        string idText, amountText;
        string category, date, description;

        if (!getline(ss, idText, '|') ||
            !getline(ss, amountText, '|') ||
            !getline(ss, category, '|') ||
            !getline(ss, date, '|') ||
            !getline(ss, description)) {
            continue;
        }

        stringstream idStream(idText);
        stringstream amountStream(amountText);

        int id;
        double amount;

        if (!(idStream >> id) ||
            !(amountStream >> amount) ||
            id <= 0 || amount < 0) {
            continue;
        }

        expenses[count] = Expense(
            id, amount, category, date, description
        );

        count++;
    }

    file.close();

    cout << count << " expense(s) loaded.\n";
}


void ExpenseManager::saveExpenses() const {
    ofstream file("expenses.txt");

    if (!file.is_open()) {
        cout << "Error: Could not save expenses.\n";
        return;
    }

    file << fixed << setprecision(2);

    for (int i = 0; i < count; i++) {
        file << expenses[i].getId() << '|'
             << expenses[i].getAmount() << '|'
             << expenses[i].getCategory() << '|'
             << expenses[i].getDate() << '|'
             << expenses[i].getDescription() << '\n';
    }

    file.close();
}

void ExpenseManager::deleteExpense() {
    if (count == 0) {
        cout << "No expenses to delete.\n";
        return;
    }

    int id;
    cout << "Enter expense ID to delete: ";
    cin >> id;

    int index = -1;

    for (int i = 0; i < count; i++) {
        if (expenses[i].getId() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Expense ID not found.\n";
        return;
    }

    for (int i = index; i < count - 1; i++) {
        expenses[i] = expenses[i + 1];
    }

    count--;

    saveExpenses();

    cout << "Expense deleted successfully!\n";
}


void ExpenseManager::editExpense() {
    if (count == 0) {
        cout << "No expenses available to edit.\n";
        return;
    }

    int id;
    cout << "Enter expense ID to edit: ";

    if (!(cin >> id)) {
        cout << "Invalid ID. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int index = -1;

    for (int i = 0; i < count; i++) {
        if (expenses[i].getId() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "Expense ID not found.\n";
        return;
    }

    double amount;
    string category, date, description;

    cout << "Enter new amount: Rs. ";

    if (!(cin >> amount) || amount < 0) {
        cout << "Invalid amount.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter new category: ";
    getline(cin, category);

    cout << "Enter new date (YYYY-MM-DD): ";
    getline(cin, date);

    cout << "Enter new description: ";
    getline(cin, description);

    expenses[index] = Expense(
        id, amount, category, date, description
    );

    saveExpenses();

    cout << "Expense updated successfully!\n";
}

void ExpenseManager::compareCategorySpending() const {
    if (count == 0) {
        cout << "No expenses available to compare.\n";
        return;
    }

    string categories[100];
    double totals[100] = {0};
    int categoryCount = 0;
    double grandTotal = 0;

    for (int i = 0; i < count; i++) {
        grandTotal += expenses[i].getAmount();

        string category = expenses[i].getCategory();
        int j;

        for (j = 0; j < categoryCount; j++) {
            if (categories[j] == category) {
                break;
            }
        }

        if (j < categoryCount) {
            totals[j] += expenses[i].getAmount();
        } else if (categoryCount < 100) {
            categories[categoryCount] = category;
            totals[categoryCount] = expenses[i].getAmount();
            categoryCount++;
        }
    }

    cout << "\n===== CATEGORY-WISE SPENDING COMPARISON =====\n";

    cout << fixed << setprecision(2);

    for (int i = 0; i < categoryCount; i++) {
        double percentage = (totals[i] / grandTotal) * 100;

        cout << "\nCategory: " << categories[i] << '\n';
        cout << "Amount: Rs. " << totals[i] << '\n';
        cout << "Percentage: " << percentage << "%\n";

        cout << "Spending: ";

        int bars = static_cast<int>(percentage / 2);

        for (int j = 0; j < bars; j++) {
            cout << '#';
        }

        cout << '\n';
    }

    cout << "\nTotal Spending: Rs. " << grandTotal << '\n';
}




