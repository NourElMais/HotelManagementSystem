#include "Room.h"
#include <string>
#include <cctype>

using namespace std;

int Room::nextRoomNumber = 1;

// ------------------- Constructors -------------------

Room::Room()
    : roomNumber(nextRoomNumber++),
    roomType("Unknown"),
    pricePerNight(0.0),
    isAvailable(true) {
}

Room::Room(const string& type, double price, bool available)
    : roomNumber(nextRoomNumber++),
    roomType(type),
    pricePerNight(price),
    isAvailable(available) {
}
//------------------validation---------------------
bool isValidRoomType(string& type) {
    for (char& c : type) {
        c = tolower(c);
    }
    return type == "single" ||
        type == "double" ||
        type == "suite"  ||
        type == "deluxe" ||
        type == "family";
}

bool isValidPrice(double price) {
    return price > 0 && price < 2000;  
}

bool isValidAvailabilityInput(const string& s) {
    string lower = s;
    for (char& c : lower) c = tolower(c);

    return (lower == "yes" || lower == "no");
}

// ------------------- Getters -------------------

int Room::getRoomNumber() const { return roomNumber; }
string Room::getRoomType() const { return roomType; }
double Room::getPrice() const { return pricePerNight; }
bool Room::getAvailability() const { return isAvailable; }


// ------------------- Setters -------------------

void Room::setRoomType(const string& type) { roomType = type; }
void Room::setPrice(double price) { pricePerNight = price; }
void Room::setAvailability(bool available) { isAvailable = available; }


// ------------------- JSON Conversion -------------------

json Room::toJSON() const {
    return json{
        {"roomNumber", roomNumber},
        {"roomType", roomType},
        {"pricePerNight", pricePerNight},
        {"isAvailable", isAvailable}
    };
}

Room Room::fromJSON(const json& j) {
    Room r;

    r.roomNumber = j.at("roomNumber").get<int>();
    r.roomType = j.at("roomType").get<string>();
    r.pricePerNight = j.at("pricePerNight").get<double>();
    r.isAvailable = j.at("isAvailable").get<bool>();

    if (r.roomNumber >= nextRoomNumber)
        nextRoomNumber = r.roomNumber + 1;

    return r;
}


// ------------------- Operator << (print) -------------------

ostream& operator<<(ostream& os, const Room& r) {
    os << "\n----------------------------------------\n";
    os << "              ROOM DETAILS              \n";
    os << "----------------------------------------\n";
    os << "Room Number   : " << r.roomNumber << "\n";
    os << "Room Type     : " << r.roomType << "\n";
    os << "Price / Night : " << r.pricePerNight << " $\n";
    os << "Available     : " << (r.isAvailable ? "Yes" : "No") << "\n";
    os << "----------------------------------------\n";
    return os;
}


// ------------------- Operator >> (input) -------------------

istream& operator>>(istream& is, Room& r) {
    cout << "\nCreating Room with Room Number: " << r.roomNumber << "\n";

    do {
        cout << "Enter room type (Single/Double/Suite/Deluxe/Family): ";
        is >> r.roomType;

        if (!isValidRoomType(r.roomType))
            cout << "Invalid room type. Try again.\n";

    } while (!isValidRoomType(r.roomType));
    
    do {
        cout << "Enter price per night: ";
        is >> r.pricePerNight;

        if (!isValidPrice(r.pricePerNight))
            cout << "Invalid price. Must be > 0.\n";

    } while (!isValidPrice(r.pricePerNight));

    string availInput;

    do {
        cout << "Is it available? (yes / no): ";
        cin >> availInput;

        if (!isValidAvailabilityInput(availInput)) {
            cout << "Invalid input. Please type 'yes' or 'no'.\n";
        }

    } while (!isValidAvailabilityInput(availInput));

    // convert to boolean
    for (char& c : availInput) c = tolower(c);
    r.isAvailable = (availInput == "yes");

    return is;
}
