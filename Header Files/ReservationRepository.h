#pragma once
#ifndef RESERVATION_REPOSITORY_H
#define RESERVATION_REPOSITORY_H

#include <vector>
#include <string>
#include <iostream>
#include "Reservation.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

class ReservationRepository {
private:
    vector<Reservation> reservations;
    string filename;

public:
    ReservationRepository(const string& file = "reservations.json");

    // File operations
    void load();          // read from reservations.json
    void save() const;    // write to reservations.json

    void addReservation(const Reservation& r);
    bool cancelById(int reservationID);
    Reservation* findById(int reservationID);
    const vector<Reservation>& getAll() const;

    void printAll() const;
};

#endif
