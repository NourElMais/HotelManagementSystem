#include "ReservationRepository.h"
#include <fstream>

using namespace std;

ReservationRepository::ReservationRepository(const string& file)
    : filename(file) {
}


// ------------------------- LOAD -------------------------

void ReservationRepository::load() {
    reservations.clear();

    ifstream inFile(filename);
    if (!inFile) {
        // file doesn't exist yet → first run
        return;
    }

    json j;
    inFile >> j;

    for (const auto& item : j) {
        reservations.push_back(Reservation::fromJSON(item));
    }
}


// ------------------------- SAVE -------------------------

void ReservationRepository::save() const {
    json j = json::array();

    for (const auto& r : reservations) {
        j.push_back(r.toJSON());
    }

    ofstream outFile(filename);
    outFile << j.dump(4);   // pretty print
}


// ------------------------- OPERATIONS -------------------------

void ReservationRepository::addReservation(const Reservation& r) {
    reservations.push_back(r);
}

bool ReservationRepository::cancelById(int reservationID) {
    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if (it->getReservationID() == reservationID) {
            reservations.erase(it);
            return true;
        }
    }
    return false;
}

Reservation* ReservationRepository::findById(int reservationID) {
    for (auto& r : reservations) {
        if (r.getReservationID() == reservationID) {
            return &r;
        }
    }
    return nullptr;
}

const vector<Reservation>& ReservationRepository::getAll() const {
    return reservations;
}

void ReservationRepository::printAll() const {
    if (reservations.empty()) {
        cout << "No reservations found.\n";
        return;
    }
    for (const auto& r : reservations) {
        cout << r;   // uses operator<< from Reservation
    }
}
