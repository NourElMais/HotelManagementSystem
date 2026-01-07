#include "Customer.h"
#include <string>
#include <iostream>
#include <cctype>   // for isalpha, isdigit

using namespace std;

// static counter
int Customer::nextId = 1;


// ------------------------- JSON CONVERSION ----------------------------

json Customer::toJSON() const {
    return json{
        {"id", id},
        {"birthDate", birthDate},
        {"firstName", firstName},
        {"lastName", lastName},
        {"email", email},
        {"phone", phone}
    };
}

Customer Customer::fromJSON(const json& j) {
    Customer c;  // temporary object, id will be overwritten

    c.id = j.at("id").get<int>();
    c.birthDate = j.at("birthDate").get<std::string>();
    c.firstName = j.at("firstName").get<std::string>();
    c.lastName = j.at("lastName").get<std::string>();
    c.email = j.at("email").get<std::string>();
    c.phone = j.at("phone").get<std::string>();

    // keep nextId in sync with loaded IDs
    if (c.id >= nextId) {
        nextId = c.id + 1;
    }

    return c;
}


// -------------------- VALIDATION FUNCTIONS --------------------

bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (char ch : name) {
        if (!isalpha(static_cast<unsigned char>(ch)) && ch != '-')
            return false;
    }
    return true;
}

bool isValidBirthDate(const string& date) {
    // Format must be YYYY-MM-DD → length = 10
    if (date.size() != 10) return false;

    // Checking structure
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return false;
        }
        else {
            if (!isdigit(date[i])) return false;
        }
    }

    // Extract numbers
    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    // ----------- Year check -----------
    if (year <= 1915) return false; 
    if (year > 2025) return false;

    // ----------- Month check -----------
    if (month < 1 || month > 12) return false;

    // ----------- Day check -----------
    if (day < 1 || day > 31) return false; 

    return true;
}


bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0 || atPos == email.size() - 1)
        return false;

    size_t dotPos = email.find('.', atPos);
    if (dotPos == string::npos || dotPos == atPos + 1 || dotPos == email.size() - 1)
        return false;

    return true;
}

bool isValidPhone(const string& phone) {
    if (phone.empty()) return false;

    int digitCount = 0;

    for (size_t i = 0; i < phone.size(); ++i) {
        char ch = phone[i];

        if (i == 0 && ch == '+') {
            // allow leading +
            continue;
        }
        else if (ch == ' ' || ch == '-') {
            // allow spaces and dashes for readability
            continue;
        }
        else if (isdigit(static_cast<unsigned char>(ch))) {
            ++digitCount;
        }
        else {
            // anything else is invalid
            return false;
        }
    }

    // 7–15 digits works for Lebanese and most international numbers
    return digitCount >= 7 && digitCount <= 15;
}
// --------------------------- CONSTRUCTORS ----------------------------
Customer::Customer()
    : id(nextId++),
    birthDate(""),
    firstName(""),
    lastName(""),
    email(""),
    phone("") {
}

Customer::Customer(const string& birthDate,
    const string& firstName,
    const string& lastName,
    const string& email,
    const string& phone)
    : id(nextId++),
    birthDate(birthDate),
    firstName(firstName),
    lastName(lastName),
    email(email),
    phone(phone) {
}

// ----------------------------- GETTERS -------------------------------

int Customer::getId() const { return id; }
string Customer::getBirthDate() const { return birthDate; }
string Customer::getFirstName() const { return firstName; }
string Customer::getLastName() const { return lastName; }
string Customer::getEmail() const { return email; }
string Customer::getPhone() const { return phone; }

// ----------------------------- SETTERS -------------------------------

void Customer::setBirthDate(const string& s) { birthDate = s; }
void Customer::setFirstName(const string& s) { firstName = s; }
void Customer::setLastName(const string& s) { lastName = s; }
void Customer::setEmail(const string& s) { email = s; }
void Customer::setPhone(const string& s) { phone = s; }

// ----------------------- OUTPUT OPERATOR << --------------------------

ostream& operator<<(ostream& os, const Customer& c) {
    os << "\n----------------------------------------\n";
    os << "            CUSTOMER DETAILS            \n";
    os << "----------------------------------------\n";
    os << "Customer ID   : " << c.id << "\n";
    os << "Name          : " << c.firstName << " " << c.lastName << "\n";
    os << "Birth Date    : " << c.birthDate << "\n";
    os << "Email         : " << c.email << "\n";
    os << "Phone         : " << c.phone << "\n";
    os << "----------------------------------------\n";
    return os;
}

// ------------------------ INPUT OPERATOR >> --------------------------

istream& operator>>(istream& is, Customer& c) {
    cout << "\nCreating Customer with ID: " << c.id << "\n";

    string input;

    // First name
    do {
        cout << "Enter first name (letters, - only): ";
        is >> input;
        if (!isValidName(input)) {
            cout << "Invalid first name format. Use letters and '-' only.\n";
        }
    } while (!isValidName(input));
    c.firstName = input;


    // Last name
    do {
        cout << "Enter last name (letters, - only): ";
        is >> input;
        if (!isValidName(input)) {
            cout << "Invalid last name format. Use letters and '-' only.\n";
        }
    } while (!isValidName(input));
    c.lastName = input;

    // Birth date
    do {
        cout << "Enter birth date (YYYY-MM-DD): ";
        is >> input;
        if (!isValidBirthDate(input)) {
            cout << "Invalid date format. Please use YYYY-MM-DD.\n";
        }
    } while (!isValidBirthDate(input));
    c.birthDate = input;

    // Email
    do {
        cout << "Enter email: ";
        is >> input;
        if (!isValidEmail(input)) {
            cout << "Invalid email format. Please enter a valid email.\n";
        }
    } while (!isValidEmail(input));
    c.email = input;

    // Phone
    do {
        cout << "Enter phone number (can start with +, digits, spaces, -): ";
        is >> input;
        if (!isValidPhone(input)) {
            cout << "Invalid phone format. Please try again.\n";
        }
    } while (!isValidPhone(input));
    c.phone = input;

    return is;
}
