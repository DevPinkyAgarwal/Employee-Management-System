#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

// Employee Class
class Employee {
private:
    int id;
    string name;
    string position;
    double basicSalary;
    double allowances;
    int attendanceDays;
    int totalWorkingDays;

public:
    Employee() : id(0), basicSalary(0), allowances(0), attendanceDays(0), totalWorkingDays(0) {}
    
    Employee(int empId, string empName, string empPosition, double salary, double empAllowances)
        : id(empId), name(empName), position(empPosition), basicSalary(salary), 
          allowances(empAllowances), attendanceDays(0), totalWorkingDays(0) {}
    
    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getPosition() const { return position; }
    double getBasicSalary() const { return basicSalary; }
    double getAllowances() const { return allowances; }
    int getAttendanceDays() const { return attendanceDays; }
    int getTotalWorkingDays() const { return totalWorkingDays; }
    
    // Setters
    void setAttendance(int days) {
        if (days >= 0 && days <= 31) {
            attendanceDays = days;
            totalWorkingDays = 22; // Standard working days per month
        }
    }
    
    void markAttendance(bool present) {
        if (present) {
            attendanceDays++;
        }
        totalWorkingDays++;
    }
    
    // Calculation methods
    double calculateGrossSalary() const {
        return basicSalary + allowances + calculateAttendanceBonus();
    }
    
    double calculateTax() const {
        double grossSalary = calculateGrossSalary();
        double tax = 0;
        
        // Progressive tax calculation
        if (grossSalary > 50000) {
            tax += (grossSalary - 50000) * 0.30;
            grossSalary = 50000;
        }
        if (grossSalary > 25000) {
            tax += (grossSalary - 25000) * 0.20;
            grossSalary = 25000;
        }
        if (grossSalary > 10000) {
            tax += (grossSalary - 10000) * 0.10;
        }
        
        return tax;
    }
    
    double calculateNetSalary() const {
        return calculateGrossSalary() - calculateTax();
    }
    
    double calculateAttendanceBonus() const {
        if (totalWorkingDays == 0) return 0;
        
        double attendanceRate = (double)attendanceDays / totalWorkingDays;
        if (attendanceRate >= 0.95) {
            return basicSalary * 0.05; // 5% bonus for excellent attendance
        } else if (attendanceRate >= 0.90) {
            return basicSalary * 0.02; // 2% bonus for good attendance
        }
        return 0;
    }
    
    // Display methods
    void displayInfo() const {
        cout << "\n=== Employee Information ===" << endl;
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Position: " << position << endl;
        cout << "Basic Salary: $" << fixed << setprecision(2) << basicSalary << endl;
        cout << "Allowances: $" << allowances << endl;
        cout << "Attendance: " << attendanceDays << "/" << totalWorkingDays << " days" << endl;
    }
    
    void displaySalarySlip() const {
        cout << "\n=== Salary Slip ===" << endl;
        cout << "Employee ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Position: " << position << endl;
        cout << "----------------------------------------" << endl;
        cout << "Basic Salary: $" << setw(10) << basicSalary << endl;
        cout << "Allowances: $" << setw(11) << allowances << endl;
        cout << "Attendance Bonus: $" << setw(7) << calculateAttendanceBonus() << endl;
        cout << "Gross Salary: $" << setw(9) << calculateGrossSalary() << endl;
        cout << "Tax Deducted: $" << setw(9) << calculateTax() << endl;
        cout << "----------------------------------------" << endl;
        cout << "Net Salary: $" << setw(11) << calculateNetSalary() << endl;
    }
};

// Payroll System Class
class PayrollSystem {
private:
    vector<Employee> employees;
    int nextEmployeeId;

public:
    PayrollSystem() : nextEmployeeId(1) {
        // Add some sample data for demonstration
        addSampleData();
    }
    
    void addSampleData() {
        employees.push_back(Employee(nextEmployeeId++, "John Smith", "Manager", 5000.0, 1000.0));
        employees.push_back(Employee(nextEmployeeId++, "Jane Doe", "Developer", 4000.0, 800.0));
        employees.push_back(Employee(nextEmployeeId++, "Mike Johnson", "Analyst", 3500.0, 500.0));
        
        // Set sample attendance
        employees[0].setAttendance(20);
        employees[1].setAttendance(18);
        employees[2].setAttendance(22);
    }
    
    // Employee management
    void addEmployee() {
        string name, position;
        double salary, allowances;
        
        cout << "\n=== Add New Employee ===" << endl;
        cout << "Employee ID: " << nextEmployeeId << endl;
        
        cin.ignore();
        cout << "Enter Name: ";
        getline(cin, name);
        
        cout << "Enter Position: ";
        getline(cin, position);
        
        cout << "Enter Basic Salary: $";
        while (!(cin >> salary) || salary < 0) {
            cout << "Invalid input. Please enter a positive number: $";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        cout << "Enter Allowances: $";
        while (!(cin >> allowances) || allowances < 0) {
            cout << "Invalid input. Please enter a positive number: $";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        Employee newEmployee(nextEmployeeId++, name, position, salary, allowances);
        employees.push_back(newEmployee);
        
        cout << "Employee added successfully!" << endl;
    }
    
    void removeEmployee() {
        if (employees.empty()) {
            cout << "No employees to remove!" << endl;
            return;
        }
        
        int id;
        cout << "Enter Employee ID to remove: ";
        cin >> id;
        
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->getId() == id) {
                employees.erase(it);
                cout << "Employee removed successfully!" << endl;
                return;
            }
        }
        cout << "Employee not found!" << endl;
    }
    
    void updateEmployee() {
        if (employees.empty()) {
            cout << "No employees to update!" << endl;
            return;
        }
        
        int id;
        cout << "Enter Employee ID to update: ";
        cin >> id;
        
        Employee* emp = findEmployee(id);
        if (emp) {
            string name, position;
            double salary, allowances;
            
            cin.ignore();
            cout << "Enter new Name (current: " << emp->getName() << "): ";
            getline(cin, name);
            
            cout << "Enter new Position (current: " << emp->getPosition() << "): ";
            getline(cin, position);
            
            cout << "Enter new Basic Salary (current: $" << emp->getBasicSalary() << "): $";
            cin >> salary;
            
            cout << "Enter new Allowances (current: $" << emp->getAllowances() << "): $";
            cin >> allowances;
            
            // Create updated employee
            *emp = Employee(emp->getId(), 
                           name.empty() ? emp->getName() : name, 
                           position.empty() ? emp->getPosition() : position,
                           salary == 0 ? emp->getBasicSalary() : salary,
                           allowances == 0 ? emp->getAllowances() : allowances);
            
            cout << "Employee updated successfully!" << endl;
        } else {
            cout << "Employee not found!" << endl;
        }
    }
    
    void displayAllEmployees() {
        if (employees.empty()) {
            cout << "No employees found!" << endl;
            return;
        }
        
        cout << "\n=== All Employees ===" << endl;
        for (const auto& emp : employees) {
            emp.displayInfo();
        }
    }
    
    Employee* findEmployee(int id) {
        for (auto& emp : employees) {
            if (emp.getId() == id) {
                return &emp;
            }
        }
        return nullptr;
    }
    
    // Attendance management
    void markAttendance() {
        if (employees.empty()) {
            cout << "No employees found!" << endl;
            return;
        }
        
        int id;
        char present;
        
        cout << "Enter Employee ID: ";
        cin >> id;
        
        Employee* emp = findEmployee(id);
        if (emp) {
            cout << "Is employee present today? (y/n): ";
            cin >> present;
            
            emp->markAttendance(tolower(present) == 'y');
            cout << "Attendance marked successfully!" << endl;
        } else {
            cout << "Employee not found!" << endl;
        }
    }
    
    void updateAttendance() {
        if (employees.empty()) {
            cout << "No employees found!" << endl;
            return;
        }
        
        int id, days;
        
        cout << "Enter Employee ID: ";
        cin >> id;
        
        Employee* emp = findEmployee(id);
        if (emp) {
            cout << "Enter number of days present: ";
            cin >> days;
            
            emp->setAttendance(days);
            cout << "Attendance updated successfully!" << endl;
        } else {
            cout << "Employee not found!" << endl;
        }
    }
    
    // Salary and reporting
    void generateSalarySlip() {
        if (employees.empty()) {
            cout << "No employees found!" << endl;
            return;
        }
        
        int id;
        cout << "Enter Employee ID: ";
        cin >> id;
        
        Employee* emp = findEmployee(id);
        if (emp) {
            emp->displaySalarySlip();
        } else {
            cout << "Employee not found!" << endl;
        }
    }
    
    void generatePayrollReport() {
        if (employees.empty()) {
            cout << "No employees found!" << endl;
            return;
        }
        
        cout << "\n=== Payroll Report ===" << endl;
        cout << "==================================================================" << endl;
        cout << setw(5) << "ID" << setw(15) << "Name" << setw(12) << "Gross" 
             << setw(10) << "Tax" << setw(12) << "Net" << endl;
        cout << "==================================================================" << endl;
        
        double totalGross = 0, totalTax = 0, totalNet = 0;
        
        for (const auto& emp : employees) {
            double gross = emp.calculateGrossSalary();
            double tax = emp.calculateTax();
            double net = emp.calculateNetSalary();
            
            cout << setw(5) << emp.getId() << setw(15) << emp.getName().substr(0, 12)
                 << setw(12) << fixed << setprecision(2) << gross
                 << setw(10) << tax << setw(12) << net << endl;
            
            totalGross += gross;
            totalTax += tax;
            totalNet += net;
        }
        
        cout << "==================================================================" << endl;
        cout << setw(32) << "Total:" << setw(12) << totalGross 
             << setw(10) << totalTax << setw(12) << totalNet << endl;
    }
    
    void generateTaxReport() {
        if (employees.empty()) {
            cout << "No employees found!" << endl;
            return;
        }
        
        cout << "\n=== Tax Report ===" << endl;
        cout << "==============================================" << endl;
        cout << setw(5) << "ID" << setw(15) << "Name" << setw(15) << "Taxable Income" 
             << setw(12) << "Tax" << endl;
        cout << "==============================================" << endl;
        
        for (const auto& emp : employees) {
            cout << setw(5) << emp.getId() << setw(15) << emp.getName().substr(0, 12)
                 << setw(15) << fixed << setprecision(2) << emp.calculateGrossSalary()
                 << setw(12) << emp.calculateTax() << endl;
        }
        cout << "==============================================" << endl;
    }
    
    void generateAttendanceReport() {
        if (employees.empty()) {
            cout << "No employees found!" << endl;
            return;
        }
        
        cout << "\n=== Attendance Report ===" << endl;
        cout << "=================================================" << endl;
        cout << setw(5) << "ID" << setw(15) << "Name" << setw(10) << "Present" 
             << setw(10) << "Total" << setw(12) << "Rate" << endl;
        cout << "=================================================" << endl;
        
        for (const auto& emp : employees) {
            double rate = 0;
            if (emp.getTotalWorkingDays() > 0) {
                rate = (double)emp.getAttendanceDays() / emp.getTotalWorkingDays() * 100;
            }
            cout << setw(5) << emp.getId() << setw(15) << emp.getName().substr(0, 12)
                 << setw(10) << emp.getAttendanceDays() << setw(10) << emp.getTotalWorkingDays()
                 << setw(10) << fixed << setprecision(1) << rate << "%" << endl;
        }
        cout << "=================================================" << endl;
    }
    
    void showMenu() {
        int choice;
        
        do {
            cout << "\n=== Employee Payroll Management System ===" << endl;
            cout << "===   Online Compiler Version          ===" << endl;
            cout << "1. Add Employee" << endl;
            cout << "2. Remove Employee" << endl;
            cout << "3. Update Employee" << endl;
            cout << "4. Display All Employees" << endl;
            cout << "5. Mark Attendance" << endl;
            cout << "6. Update Attendance" << endl;
            cout << "7. Generate Salary Slip" << endl;
            cout << "8. Generate Payroll Report" << endl;
            cout << "9. Generate Tax Report" << endl;
            cout << "10. Generate Attendance Report" << endl;
            cout << "11. Display Sample Data" << endl;
            cout << "0. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            
            switch (choice) {
                case 1: addEmployee(); break;
                case 2: removeEmployee(); break;
                case 3: updateEmployee(); break;
                case 4: displayAllEmployees(); break;
                case 5: markAttendance(); break;
                case 6: updateAttendance(); break;
                case 7: generateSalarySlip(); break;
                case 8: generatePayrollReport(); break;
                case 9: generateTaxReport(); break;
                case 10: generateAttendanceReport(); break;
                case 11: displaySampleDataInfo(); break;
                case 0: 
                    cout << "Thank you for using Payroll System!" << endl;
                    break;
                default:
                    cout << "Invalid choice! Please try again." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (choice != 0);
    }
    
    void displaySampleDataInfo() {
        cout << "\n=== Sample Data Information ===" << endl;
        cout << "The system comes with 3 sample employees:" << endl;
        cout << "1. John Smith (Manager) - ID: 1" << endl;
        cout << "2. Jane Doe (Developer) - ID: 2" << endl;
        cout << "3. Mike Johnson (Analyst) - ID: 3" << endl;
        cout << "You can use these IDs to test the features." << endl;
    }
};

// Main function
int main() {
    cout << "=== Employee Payroll Management System ===" << endl;
    cout << "===   Online Compiler Compatible      ===" << endl;
    cout << "==========================================" << endl;
    cout << "Features:" << endl;
    cout << "- Employee Management" << endl;
    cout << "- Attendance Tracking" << endl;
    cout << "- Tax Calculation" << endl;
    cout << "- Salary Processing" << endl;
    cout << "- Comprehensive Reporting" << endl;
    cout << "==========================================" << endl;
    
    PayrollSystem payrollSystem;
    payrollSystem.showMenu();
    
    return 0;
}
