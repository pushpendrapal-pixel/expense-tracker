

#ifndef EXPENSEMANAGER_H
#define EXPENSEMANAGER_H

#include "Expense.h"

class ExpenseManager {
private:
    Expense expenses[100];
    int count;

    void saveExpenses() const;
    void loadExpenses();

public:
    ExpenseManager();
    void deleteExpense();
    void editExpense();

    void addExpense();
    void viewExpenses() const;
    double calculateTotal() const;
    void categorySummary() const;
    void searchByCategory() const;
    void monthlyExpenseSummary() const;
    void compareCategorySpending() const;
};

#endif
