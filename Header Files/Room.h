#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <iostream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class Room {
private:
    int roomNumber;
    string roomType;
    double pricePerNight;
    bool isAvailable;

    static int nextRoomNumber;

public:
    // Constructors
    Room();
    Room(const string& type, double price, bool available = true);
        
    // Getters
    int getRoomNumber() const;
    string getRoomType() const;
    double getPrice() const;
    bool getAvailability() const;

    // Setters
    void setRoomType(const string& type);
    void setPrice(double price);
    void setAvailability(bool available);

    // JSON
    json toJSON() const;
    static Room fromJSON(const json& j);

    // Operator overloads
    friend ostream& operator<<(ostream& os, const Room& r);
    friend istream& operator>>(istream& is, Room& r);
};

#endif
