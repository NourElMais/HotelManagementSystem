#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;


class Customer {
private:
    static int nextId;   // static counter

    int id;
    string birthDate;
    string firstName;
    string lastName;
    string email;
    string phone;

public:
    Customer();
    Customer(const string& birthDate,
        const string& firstName,
        const string& lastName,
        const string& email,
        const string& phone);

   

    // Getters
    int getId() const;
    string getBirthDate() const;
    string getFirstName() const;
    string getLastName() const;
    string getEmail() const;
    string getPhone() const;

    // Setters
    void setBirthDate(const string& birthDate);
    void setFirstName(const string& firstName);
    void setLastName(const string& lastName);
    void setEmail(const string& email);
    void setPhone(const string& phone);

    // Operators
    friend ostream& operator<<(ostream& os, const Customer& c);
    friend istream& operator>>(istream& is, Customer& c);

    
    // JSON conversion
    json toJSON() const;
    static Customer fromJSON(const json& j);

};

#endif
