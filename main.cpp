

#include <iostream>
#include <iomanip>
#include "ExpenseManager.h"
#include <limits>

using namespace std;

int main() {
    ExpenseManager manager;
    int choice;

    cout << fixed << setprecision(2);

    do {
        
       cout << "\n===== EXPENSE TRACKER =====\n";
       cout << "1. Add Expense\n";
       cout << "2. View All Expenses\n";
       cout << "3. Show Total Expenses\n";
       cout << "4. Category-wise Summary\n";
       cout << "5. Delete Expense\n";
       cout << "6. Edit Expense\n";
       cout << "7. Search Expenses by Category\n";
       cout << "8. Monthly Expense Summary\n";
       cout<<  "9. Category-wise Spending Comparison\n";
       cout << "10. Exit\n";
       cout << "Enter your choice: ";

 

        cin >> choice;

        if (cin.fail()) {
            cout << "Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
      
     switch (choice) {
    case 1:
        manager.addExpense();
        break;

    case 2:
        manager.viewExpenses();
        break;

    case 3:
        cout << "Total Expenses: Rs. "
             << manager.calculateTotal() << endl;
        break;

    case 4:
        manager.categorySummary();
        break;

    case 5:
    manager.deleteExpense();
    break;

     
    case 6:
    manager.editExpense();
    break;

   case 7:
    manager.searchByCategory();
    break;

    case 8:
    manager.monthlyExpenseSummary();
    break;

    case 9:
    manager.compareCategorySpending();
    break;

    case 10:
    cout << "Thank you for using Expense Tracker!\n";
    break;

     default:
        cout << "Invalid choice. Try again.\n";
}

        

    } while (choice != 10);

    return 0;
}
