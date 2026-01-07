#include "Reservation.h"
#include <cctype>
#include <chrono>

int Reservation::nextReservationID = 1;


// ---------------- VALIDATION ----------------
bool isLeapYear(int year) {
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    if (year % 4 == 0) return true;
    return false;
}

int daysInMonth(int year, int month) {
    int monthDays[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    int days = monthDays[month - 1];
    if (month == 2 && isLeapYear(year)) {
        days = 29;
    }
    return days;
}
int getCurrentYear() {
    time_t t = time(nullptr);

    tm nowStruct;
    localtime_s(&nowStruct, &t);  

    return nowStruct.tm_year + 1900;
}

bool isValidDate(const string& date) {
    if (date.size() != 10) return false;

    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            if (date[i] != '-') return false;
        }
        else {
            if (!isdigit(static_cast<unsigned char>(date[i]))) return false;
        }
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (year < getCurrentYear()) {
        return false;
    }
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    // 🔹 Use existing daysInMonth() to validate day for that month/year
    int maxDay = daysInMonth(year, month);   // uses isLeapYear internally
    if (day > maxDay) return false;          // rejects Feb 30, Apr 31, etc.

    return true;
}


bool isCheckOutAfterOrEqual(const string& checkIn, const string& checkOut) {
    return checkOut >= checkIn;   // because format is YYYY-MM-DD
}



// ------------------- Constructors -------------------

Reservation::Reservation()
    : reservationID(nextReservationID++),
    customerID(0),
    roomNumber(0),
    checkInDate(""),
    checkOutDate(""),
    totalPrice(0.0) {
}


Reservation::Reservation(int custID, int roomNum, const string& checkIn, const string& checkOut, double price)
    : reservationID(nextReservationID++),
    customerID(custID),
    roomNumber(roomNum),
    checkInDate(checkIn),
    checkOutDate(checkOut),
    totalPrice(price) {
}


// ------------------- Getters -------------------

int Reservation::getReservationID() const { return reservationID; }
int Reservation::getCustomerID() const { return customerID; }
int Reservation::getRoomNumber() const { return roomNumber; }
string Reservation::getCheckInDate() const { return checkInDate; }
string Reservation::getCheckOutDate() const { return checkOutDate; }
double Reservation::getTotalPrice() const { return totalPrice; }
string Reservation::getRequestedRoomType() const { return requestedRoomType; }


// ------------------- Setters -------------------

void Reservation::setCustomerID(int id) { customerID = id; }
void Reservation::setRoomNumber(int num) { roomNumber = num; }
void Reservation::setCheckInDate(const string& date) { checkInDate = date; }
void Reservation::setCheckOutDate(const string& date) { checkOutDate = date; }
void Reservation::setTotalPrice(double price) { totalPrice = price; }
void Reservation::setRequestedRoomType(const string& s) { requestedRoomType = s; }


// ------------------- JSON Conversion -------------------

json Reservation::toJSON() const {
    return json{
        {"reservationID", reservationID},
        {"customerID", customerID},
        {"roomNumber", roomNumber},
        {"checkInDate", checkInDate},
        {"checkOutDate", checkOutDate},
        {"totalPrice", totalPrice}
    };
}

Reservation Reservation::fromJSON(const json& j) {
    Reservation r;

    r.reservationID = j.at("reservationID").get<int>();
    r.customerID = j.at("customerID").get<int>();
    r.roomNumber = j.at("roomNumber").get<int>();
    r.checkInDate = j.at("checkInDate").get<string>();
    r.checkOutDate = j.at("checkOutDate").get<string>();
    r.totalPrice = j.at("totalPrice").get<double>();

    if (r.reservationID >= nextReservationID)
        nextReservationID = r.reservationID + 1;

    return r;
}
// ------------------- Nights Calculations -------------------


int Reservation::calculateNights() const {
    int y1 = stoi(checkInDate.substr(0, 4));
    int m1 = stoi(checkInDate.substr(5, 2));
    int d1 = stoi(checkInDate.substr(8, 2));

    int y2 = stoi(checkOutDate.substr(0, 4));
    int m2 = stoi(checkOutDate.substr(5, 2));
    int d2 = stoi(checkOutDate.substr(8, 2));

    int nights = 0;

    while (y1 < y2 || (y1 == y2 && (m1 < m2 || (m1 == m2 && d1 < d2)))) {
        int dim = daysInMonth(y1, m1); // days in current month 
        d1 = d1 + 1;
        if (d1 > dim) {
            d1 = 1;
            m1 = m1 + 1;
            if (m1 > 12) {
                m1 = 1;
                y1 = y1 + 1;
            }
        }
        nights = nights + 1;
    }
    return nights;
}

// ------------------- Total Price -------------------
void Reservation::calculateTotalPrice(double pricePerNight) {
    int nights = calculateNights();
    totalPrice = nights * pricePerNight;
    setTotalPrice(totalPrice);
}

// ------------------- Operator << -------------------
ostream& operator<<(ostream& os, const Reservation& r) {
    os << "\n----------------------------------------\n";
    os << "           RESERVATION DETAILS          \n";
    os << "------------------------------------------\n";
    os << "Reservation ID : " << r.reservationID << "\n";
    os << "Customer ID    : " << r.customerID << "\n";
    os << "Room Number    : " << r.roomNumber << "\n";
    os << "Check-In Date  : " << r.checkInDate << "\n";
    os << "Check-Out Date : " << r.checkOutDate << "\n";
    os << "Total Price    : " << r.totalPrice << " $\n";
    os << "----------------------------------------\n";
    return os;
}


// ------------------- Operator >> -------------------

istream& operator>>(istream& is, Reservation& r) {
    cout << "\nCreating Reservation with ID: " << r.reservationID << "\n";


    string inDate, outDate;

    // ---- Input dates with format + order check ----
    do {
        // Check-in
        do {
            cout << "Enter check-in date (YYYY-MM-DD): ";
            is >> inDate;

            if (!isValidDate(inDate)) {
                cout << "Invalid date format or year (must be >= 2025). Try again.\n";
            }
        } while (!isValidDate(inDate));

        // Check-out
        do {
            cout << "Enter check-out date (YYYY-MM-DD): ";
            is >> outDate;

            if (!isValidDate(outDate)) {
                cout << "Invalid date format or year (must be >= "<< getCurrentYear()<<").Try again.\n";
            }
        } while (!isValidDate(outDate));

        if (!isCheckOutAfterOrEqual(inDate, outDate)) {
            cout << "Check-out date must be greater than or equal to check-in date.\n";
        }

    } while (!isCheckOutAfterOrEqual(inDate, outDate));

    r.checkInDate = inDate;
    r.checkOutDate = outDate;


    string typeInput;
    bool validType = false;

    do {
        cout << "Enter room type (Single/Double/Suite/Deluxe/Family): ";
        is >> typeInput;

        string lower = typeInput;
        for (char& ch : lower) {
            ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
        }

        if (lower == "single" ||
            lower == "double" ||
            lower == "suite" ||
            lower == "deluxe" ||
            lower == "family") {

            r.requestedRoomType = lower;
            validType = true;
        }
        else {
            cout << "Invalid room type. Please try again.\n";
        }

    } while (!validType);
    return is;
}
