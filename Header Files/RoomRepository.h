#pragma once
#ifndef ROOM_REPOSITORY_H
#define ROOM_REPOSITORY_H

#include <vector>
#include <string>
#include "Room.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class RoomRepository {
private:
    vector<Room> rooms;
    string filename;

public:
    RoomRepository(const string& file = "rooms.json");

    // File operations
    void load();      // read from rooms.json
    void save() const; // write to rooms.json

    // Room operations
    void addRoom(const Room& r);
    bool removeByNumber(int number);
    Room* findByNumber(int number);
    const vector<Room>& getAll() const;
    bool editRoomByNumber();

    void printAll() const;
};

#endif
