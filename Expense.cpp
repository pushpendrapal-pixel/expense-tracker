
#include <iostream>
#include "Expense.h"

using namespace std;


// Default constructor
Expense::Expense() {
    id = 0;
    amount = 0.0;
    category = "";
    date = "";
    description = "";
}
// Parameterized constructor
Expense::Expense(int id, double amount, string category,
                 string date, string description) {
    this->id = id;
    this->amount = amount;
    this->category = category;
    this->date = date;
    this->description = description;
}

// Display all expense details
void Expense::display() const {
    cout << "Expense ID: " << id << endl;
    cout << "Amount: Rs. " << amount << endl;
    cout << "Category: " << category << endl;
    cout << "Date: " << date << endl;
    cout << "Description: " << description << endl;
}

// Getter functions
int Expense::getId() const {
    return id;
}

double Expense::getAmount() const {
    return amount;
}

string Expense::getCategory() const {
    return category;
}

string Expense::getDate() const {
    return date;
}

string Expense::getDescription() const {
    return description;
}
