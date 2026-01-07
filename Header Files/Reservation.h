#pragma once
#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <iostream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Reservation {
private:
    int reservationID;
    int customerID;
    int roomNumber;
    string checkInDate;
    string checkOutDate;
    double totalPrice;
    string requestedRoomType;

    static int nextReservationID;

public:
    // Constructors
    Reservation();
    Reservation(int custID, int roomNum, const string& checkIn, const string& checkOut, double price);

    // Getters
    int getReservationID() const;
    int getCustomerID() const;
    int getRoomNumber() const;
    string getCheckInDate() const;
    string getCheckOutDate() const;
    double getTotalPrice() const;
    string getRequestedRoomType() const;

    // Setters
    void setCustomerID(int id);
    void setRoomNumber(int num);
    void setCheckInDate(const string& date);
    void setCheckOutDate(const string& date);
    void setTotalPrice(double price);
    void setRequestedRoomType(const string& type);

    //calculations
    int calculateNights() const;
    void calculateTotalPrice(double pricePerNight);

    // JSON handling
    json toJSON() const;
    static Reservation fromJSON(const json& j);

    // Operator overloads
    friend ostream& operator<<(ostream& os, const Reservation& r);
    friend istream& operator>>(istream& is, Reservation& r);
};

// validation helpers
bool isValidDate(const string& date);

#endif
