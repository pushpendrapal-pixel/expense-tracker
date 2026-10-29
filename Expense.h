
#ifndef EXPENSE_H
#define EXPENSE_H

#include <string>
using namespace std;

class Expense {
private:
    int id;
    double amount;
    string category;
    string date;
    string description;

public:
Expense();
    // Parameterized constructor
    Expense(int id, double amount, string category,
            string date, string description);

    // Display function
    void display() const;

    // Getter functions
    int getId() const;
    double getAmount() const;
    string getCategory() const;
    string getDate() const;
    string getDescription() const;
};

#endif
