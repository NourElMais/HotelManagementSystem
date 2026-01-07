#pragma once
#ifndef CUSTOMER_REPOSITORY_H
#define CUSTOMER_REPOSITORY_H

#include <vector>
#include <string>
#include "Customer.h"
#include "json.hpp"

using json = nlohmann::json;

class CustomerRepository {
private:
    std::vector<Customer> customers;
    std::string filename;

public:
    // constructor
    CustomerRepository(const std::string& file = "customers.json");

    // file operations
    void load();        // read from JSON file into vector
    void save() const;  // write vector to JSON file

    // basic operations
    void addCustomer(const Customer& c);
    bool removeById(int id);
    Customer* findById(int id);              // returns pointer or nullptr
    const std::vector<Customer>& getAll() const;

    //modification
    bool editCustomerById();

    // (optional) print all
    void printAll() const;
};

#endif
