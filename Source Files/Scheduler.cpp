#include "Scheduler.h"
#include "Room.h"
#include "Reservation.h"
using namespace std;

//const
Scheduler::Scheduler(RoomRepository& roomRepo, ReservationRepository& resRepo)
    : roomRepo(roomRepo), resRepo(resRepo) {
}

//check for overlap
bool Scheduler::intervalsOverlap(const string& s1, const string& e1,
    const string& s2, const string& e2) const {
    //end date is less than start date => no overlap
    if (e1 < s2 || e2 < s1) return false;
    return true;
}

/* GREEDY left edge style room selection :
room = track
try placing reserv in first room if not in second if not....*/

int Scheduler::findAvailableRoom(const string& checkIn, const string& checkOut,const string& desiredType) const {
    // auto to create a ref + figures out type
    const auto& rooms = roomRepo.getAll();
    const auto& reservations = resRepo.getAll();

    if (rooms.empty()) return -1;   //edge case: no rooms in system

    //lowercase to ignore casing
    string type = desiredType;
    for (char& c : type) c = tolower(c);

    // try rooms in ascending roomNumber order
    for (const auto& room : rooms) {
        // skip unavailable (maybe manually for reno or smthg)
        if (!room.getAvailability()) continue;

        //check if room matches requested
        string roomType = room.getRoomType();
        for (char& c : roomType) c = tolower(c);
        if (roomType != type) continue;

        bool conflict = false;
        // check res for this room
        for (const auto& existing : reservations) {
            // only resv of this room needed to compare
            if (existing.getRoomNumber() != room.getRoomNumber())
                continue;
            //if overlap => cannot use room
            if (intervalsOverlap(existing.getCheckInDate(),
                existing.getCheckOutDate(),
                checkIn, checkOut)) {
                conflict = true;
                break;
            }
        }
        //if no conflict we reserve it
        if (!conflict) {
            return room.getRoomNumber();
        }
    }
    //no room available
    return -1;
}
