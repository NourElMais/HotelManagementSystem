#pragma once
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include "RoomRepository.h"
#include "ReservationRepository.h"
using namespace std;

class Scheduler {
private:
    RoomRepository& roomRepo;
    ReservationRepository& resRepo;

    /*
       Interval overlap check:
       - Dates are stored as "YYYY-MM-DD"
       - This format lets us compare date strings directly
       - If one interval ends before the other starts → no overlap
       - Otherwise → overlap exists
    */
    bool intervalsOverlap(const string& s1, const string& e1,
        const string& s2, const string& e2) const;

public:
    /*
       Scheduler needs access to repositories:
       - roomRepo → to get list of rooms (tracks)
       - resRepo → to check existing reservations per room
       Using references avoids copying and keeps everything synced.
    */
    Scheduler(RoomRepository& roomRepo, ReservationRepository& resRepo);

    /*
       Left-Edge-style room assignment:
       - Scan rooms in increasing roomNumber order (like tracks)
       - Pick the first room with NO overlapping reservation
       - This greedy choice minimizes number of rooms used
       - Return the roomNumber, or -1 if none is free
    */
    int findAvailableRoom(const string& checkIn, const string& checkOut, const string& desiredType) const;

};

#endif
