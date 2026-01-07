#include "CustomerRepository.h"
#include <fstream>
#include <iostream>

using namespace std;

CustomerRepository::CustomerRepository(const std::string& file)
    : filename(file) {
}

// ------------------------- LOAD -------------------------

void CustomerRepository::load() {
    //Empty the vector (so we don’t mix old data with new).
    customers.clear();

    //Try to open "customers.json"
    ifstream inFile(filename);
    if (!inFile) {
        // file doesn’t exist yet – first run → just keep empty vector
        return;
    }

    //Read the entire JSON file into a JSON array
    json j;
    inFile >> j;

    //Convert each JSON object → Customer object:
    for (const auto& item : j) {
        customers.push_back(Customer::fromJSON(item));
    }
}
 
// ------------------------- SAVE -------------------------(writes vector → file)

void CustomerRepository::save() const {

    //Create an empty JSON array:
    json j = json::array();


    /*For each customer in memory (in the customer vector):
    toJSON() converts it to JSON format
    then pushes it into the array*/

    for (const auto& c : customers) {
        j.push_back(c.toJSON());
    }

    /*If "customers.json" does NOT exist → it is created.
    If it exists → it is overwritten with new data.*/

    ofstream outFile(filename);

    /*dump(4) means “pretty print with indentation = 4 spaces”
    The file becomes perfect readable JSON*/

    outFile << j.dump(4);   // pretty print
}

// ------------------------- OPERATIONS -------------------------

void CustomerRepository::addCustomer(const Customer& c) {
    customers.push_back(c);
}

bool CustomerRepository::removeById(int id) {
    for (auto it = customers.begin(); it != customers.end(); ++it) {
        if (it->getId() == id) {
            customers.erase(it);
            return true;
        }
    }
    return false;
}

Customer* CustomerRepository::findById(int id) {
    for (Customer& c : customers) {
        if (c.getId() == id) {
            return &c;
        }
    }
    return nullptr;
}

const vector<Customer>& CustomerRepository::getAll() const {
    return customers;
}

bool CustomerRepository::editCustomerById() {
    int id;
    cout << "Enter customer ID to edit: ";
    cin >> id;
    cin.ignore(1000, '\n');   // clear rest of line just in case

    Customer* c = findById(id);
    if (!c) {
        cout << "Customer not found.\n";
        return false;
    }

    cout << "\n--- Current Customer Details ---\n";
    cout << *c << "\n";

    cout << "What would you like to edit?\n";
    cout << "1. First Name\n";
    cout << "2. Last Name\n";
    cout << "3. Email\n";
    cout << "4. Phone\n";
    cout << "5. Birthdate\n";
    cout << "0. Cancel\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
        cin.clear();               // 1) clear the error flag
        cin.ignore(1000, '\n');    // 2) throw away whatever the user typed
        cout << "Invalid input. Please enter a number.\n";
        return false;              // 3) go back to the top of the loop
    }


    if (choice == 0) {
        cout << "Edit cancelled.\n";
        return false;
    }

    string newValue;
    cout << "Enter new value (NO spaces, use '-' if needed): ";
    cin >> newValue;          // reads only up to first space
    cin.ignore(1000, '\n');   // throw away the rest if they typed spaces

    switch (choice) {
    case 1: c->setFirstName(newValue); break;
    case 2: c->setLastName(newValue); break;
    case 3: c->setEmail(newValue); break;
    case 4: c->setPhone(newValue); break;
    case 5: c->setBirthDate(newValue); break;
    default:
        cout << "Invalid option.\n";
        return false;
    }

    save();

    cout << "\nCustomer updated successfully.\n";
    cout << "\n--- Updated Customer Details ---\n";
    cout << *c << "\n";

    return true;
}

void CustomerRepository::printAll() const {
    if (customers.empty()) {
        cout << "No customers found.\n";
        return;
    }
    for (const auto& c : customers) {
        cout << c;   // uses your operator<<
    }
}
