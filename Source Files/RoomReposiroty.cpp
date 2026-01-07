#include "RoomRepository.h"
#include <fstream>
#include <iostream>

using namespace std;

// Constructor
RoomRepository::RoomRepository(const string& file)
    : filename(file) {
}


// ------------------------- LOAD -------------------------

void RoomRepository::load() {
    rooms.clear();

    ifstream inFile(filename);
    if (!inFile) {
        // file doesn't exist yet → first run
        return;
    }

    json j;
    inFile >> j;

    for (const auto& item : j) {
        rooms.push_back(Room::fromJSON(item));
    }
}


// ------------------------- SAVE -------------------------

void RoomRepository::save() const {
    json j = json::array();

    for (const auto& r : rooms) {
        j.push_back(r.toJSON());
    }

    ofstream outFile(filename);
    outFile << j.dump(4);   // pretty print
}


// ------------------------- OPERATIONS -------------------------

void RoomRepository::addRoom(const Room& r) {
    rooms.push_back(r);
}

bool RoomRepository::removeByNumber(int number) {
    for (auto it = rooms.begin(); it != rooms.end(); ++it) {
        if (it->getRoomNumber() == number) {
            rooms.erase(it);
            return true;
        }
    }
    return false;
}

Room* RoomRepository::findByNumber(int number) {
    for (auto& r : rooms) {
        if (r.getRoomNumber() == number) {
            return &r;
        }
    }
    return nullptr;
}

const vector<Room>& RoomRepository::getAll() const {
    return rooms;
}

bool RoomRepository::editRoomByNumber() {
    int number;
    cout << "Enter room number to edit: ";
    cin >> number;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid room number.\n";
        return false;
    }

    cin.ignore(1000, '\n'); 

    Room* r = findByNumber(number);
    if (!r) {
        cout << "Room not found.\n";
        return false;
    }

    cout << "\n--- Current Room Details ---\n";
    cout << *r << "\n";

    cout << "What would you like to edit?\n";
    cout << "1. Room Type\n";
    cout << "2. Price per Night\n";
    cout << "3. Availability\n";
    cout << "0. Cancel\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Please enter a number.\n";
        return false;
    }

    if (choice == 0) {
        cout << "Edit cancelled.\n";
        return false;
    }

    cin.ignore(1000, '\n'); 

    switch (choice) {
    case 1: {
        string newType;
        cout << "Enter new room type (NO spaces, use '-' if needed): ";
        cin >> newType;
        cin.ignore(1000, '\n');
        r->setRoomType(newType);
        break;
    }
    case 2: {
        double newPrice;
        cout << "Enter new price per night: ";
        cin >> newPrice;

        if (cin.fail() || newPrice <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid price.\n";
            return false;
        }

        cin.ignore(1000, '\n');
        r->setPrice(newPrice);
        break;
    }
    case 3: {
        int availChoice;
        cout << "Is the room available? (1 = Yes, 0 = No): ";
        cin >> availChoice;

        if (cin.fail() || (availChoice != 0 && availChoice != 1)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid availability choice.\n";
            return false;
        }

        cin.ignore(1000, '\n');
        r->setAvailability(availChoice == 1);
        break;
    }
    default:
        cout << "Invalid option.\n";
        return false;
    }

    save();

    cout << "\nRoom updated successfully.\n";
    cout << "\n--- Updated Room Details ---\n";
    cout << *r << "\n";

    return true;
}


void RoomRepository::printAll() const {
    if (rooms.empty()) {
        cout << "No rooms found.\n";
        return;
    }
    for (const auto& r : rooms) {
        cout << r;  // uses operator<<
    }
}
