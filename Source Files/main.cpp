#include <iostream>
#include <string>

#include "CustomerRepository.h"
#include "RoomRepository.h"
#include "ReservationRepository.h"
#include "Reservation.h"
#include "Scheduler.h"
#include <fstream>   
#include <conio.h>   

using namespace std;

string usernames[100];
string passwords[100];
int adminCount = 0;

Customer * customerLogin(CustomerRepository & customerRepo) {
    int cid;

    cout << "\n====== CUSTOMER LOGIN ======\n";

    cout << "Enter your Customer ID: ";
    cin >> cid;

    Customer* cptr = customerRepo.findById(cid);
    if (!cptr) {
        cout << "No customer found with this ID.\n";
        return nullptr;
    }

    cout << "\nLogin successful. Welcome, "
        << cptr->getFirstName() << "!\n";
    return cptr;
}

void showCustomerReservations(const Customer& c,
    const ReservationRepository& resRepo) {
    const auto& allRes = resRepo.getAll();
    bool has = false;

    cout << "\n===== YOUR RESERVATIONS =====\n";

    for (const auto& r : allRes) {
        if (r.getCustomerID() == c.getId()) {   
            cout << r;                          
            has = true;
        }
    }

    if (!has) {
        cout << "You do not have any reservations yet!\n";
    }
}

void loadAdmins(const string& filename) {
    ifstream file(filename);

    if (!file) {
        cout << "Could not open admin file.\n";
        return;
    }

    while (file >> usernames[adminCount] >> passwords[adminCount]) {
        adminCount++;
    }
}

string getPasswordHidden() {
    string pass = "";
    char ch;

    while (true) {
        ch = _getch(); 

        if (ch == 13) {  
            cout << endl;
            break;
        }
        else if (ch == 8) { 
            if (!pass.empty()) {
                pass.pop_back();
                cout << "\b \b";
            }
        }
        else {
            pass += ch;
            cout << "*";   
        }
    }

    return pass;
}


bool login() {
    string user, pass;

    cout << "====== ADMIN LOGIN ======\n";

    for (int attempt = 1; attempt <= 3; attempt++) {
        cout << "Username: ";
        cin >> user;

        cout << "Password: ";
        pass = getPasswordHidden();

        
        for (int i = 0; i < adminCount; i++) {
            if (usernames[i] == user && passwords[i] == pass) {
                cout << "\nLogin successful. Welcome, " << user << "!\n";
                return true;
            }
        }

        cout << "Wrong username or password. Try again.\n";
    }

    cout << "\nToo many attempts. Access denied.\n";
    return false;
}


void printInvoice(const Reservation& r,
    CustomerRepository& customerRepo,
    RoomRepository& roomRepo)
{
    cout << "\n========================================\n";
    cout << "              INVOICE                     \n";
    cout << "========================================\n";

    
    cout << "Reservation details:\n";
    cout << r << "\n";

    
    const Customer* cptr = customerRepo.findById(r.getCustomerID());
    if (cptr) {
        cout << "\nCustomer details:\n";
        cout << *cptr << "\n";
    }
    else {
        cout << "\nCustomer details: not found (ID = "
            << r.getCustomerID() << ")\n";
    }

    const Room* roomPtr = roomRepo.findByNumber(r.getRoomNumber());
    if (roomPtr) {
        cout << "\nRoom details:\n";
        cout << *roomPtr << "\n";
    }
    else {
        cout << "\nRoom details: not found (Room = "
            << r.getRoomNumber() << ")\n";
    }

    cout << "----------------------------------------\n";
    cout << "Total price to pay: " << r.getTotalPrice() << " $\n";
    cout << "Thank you for your stay!\n";
    cout << "========================================\n\n";
}

int getAgeFromBirthDate(const string& birthDate, int currentYear) {
    int birthYear = stoi(birthDate.substr(0, 4));
    return currentYear - birthYear;
}


int main() {
   
    CustomerRepository customerRepo("customers.json");
    RoomRepository roomRepo("rooms.json");
    ReservationRepository resRepo("reservations.json");
    Scheduler scheduler(roomRepo, resRepo);

    customerRepo.load();
    roomRepo.load();
    resRepo.load();

    
    loadAdmins("Admins.txt");

    int roleChoice;
    cout << "Login as:\n";
    cout << "1. Admin\n";
    cout << "2. Customer\n";
    cout << "Enter choice: ";
    cin >> roleChoice;

    if (roleChoice == 1) {
        if (adminCount == 0) {
            cout << "No admins found. Exiting.\n";
            return 0;
        }

        if (!login()) {
            return 0; 
        }

        int mainChoice;

        do {
            cout << "\n=====  HOTEL MANAGEMENT SYSTEM =====\n";
            cout << "1. Customer Management\n";
            cout << "2. Room Management\n";
            cout << "3. Reservation Management\n";
            cout << "4. Admin Reports\n";
            cout << "0. Exit\n";
            cout << "Enter choice: ";
            cin >> mainChoice;

            switch (mainChoice) {
            case 1: {
                int cChoice;
                do {
                    cout << "\n===== CUSTOMER MENU =====\n";
                    cout << "1. Add customer\n";
                    cout << "2. View all customers\n";
                    cout << "3. Delete customer by ID\n";
                    cout << "4. Modify customer by ID\n";
                    cout << "0. Back to main menu\n";
                    cout << "Enter choice: ";
                    cin >> cChoice;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input. Please enter a number.\n";
                        continue;
                    }

                    switch (cChoice) {
                    case 1: {
                        Customer c;
                        cin >> c;
                        customerRepo.addCustomer(c);
                        customerRepo.save();
                        break;
                    }
                    case 2:
                        customerRepo.printAll();
                        break;

                    case 3: {
                        int id;
                        cout << "Enter customer ID to delete: ";
                        cin >> id;

                        if (customerRepo.removeById(id)) {
                            cout << "Customer removed.\n";
                            customerRepo.save();
                        }
                        else {
                            cout << "Customer not found.\n";
                        }
                        break;
                    }
                    case 4: {
                        customerRepo.editCustomerById();   // <--- all logic is inside repository
                        break;
                    }
                    case 0:
                        break;

                    default:
                        cout << "Invalid choice! Try again.\n";
                    }

                } while (cChoice != 0);
                break;
            }

                  // ================= ROOM MENU =================
            case 2: {
                int rChoice;
                do {
                    cout << "\n===== ROOM MENU =====\n";
                    cout << "1. Add room\n";
                    cout << "2. View all rooms\n";
                    cout << "3. Delete room by room number\n";
                    cout << "4. Modify room by room number\n";
                    cout << "0. Back to main menu\n";
                    cout << "Enter choice: ";
                    cin >> rChoice;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input. Please enter a number.\n";
                        continue;
                    }

                    switch (rChoice) {
                    case 1: {
                        Room r;
                        cin >> r;
                        roomRepo.addRoom(r);
                        roomRepo.save();
                        break;
                    }
                    case 2:
                        roomRepo.printAll();
                        break;

                    case 3: {
                        int num;
                        cout << "Enter room number: ";
                        cin >> num;

                        if (roomRepo.removeByNumber(num)) {
                            cout << "Room removed.\n";
                            roomRepo.save();
                        }
                        else {
                            cout << "Room not found.\n";
                        }
                        break;
                    }

                    case 4: {
                        roomRepo.editRoomByNumber();
                        break;
                    }
                    case 0:
                        break;

                    default:
                        cout << "Invalid choice! Try again.\n";
                    }

                } while (rChoice != 0);
                break;
            }

                  // ================= RESERVATION MENU =================
            case 3: {
                int resChoice;
                do {
                    cout << "\n===== RESERVATION MENU =====\n";
                    cout << "1. Create reservation\n";
                    cout << "2. View all reservations\n";
                    cout << "3. Cancel reservation by ID\n";
                    cout << "4. Generate invoice by reservation ID\n";
                    cout << "0. Back to main menu\n";
                    cout << "Enter choice: ";
                    cin >> resChoice;

                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid input. Please enter a number.\n";
                        continue;
                    }

                    switch (resChoice) {

                    case 1: {
                        Reservation r;

                        int currentYear = 2025;

                        // --- customer Id ---
                        int cid;
                        Customer* cptr = nullptr;
                        do {
                            cout << "Enter customer ID: ";
                            cin >> cid;

                            cptr = customerRepo.findById(cid);

                            if (!cptr) {
                                cout << "No customer with this ID.\n";
                                continue;
                            }

                            int age = getAgeFromBirthDate(cptr->getBirthDate(), currentYear);
                            if (age < 16) {
                                cout << "Customer must be at least 16 years old to make a reservation.\n";
                                cptr = nullptr;
                            }

                        } while (!cptr);

                        r.setCustomerID(cid);


                        cin >> r;
                        int roomNum = scheduler.findAvailableRoom(
                            r.getCheckInDate(),
                            r.getCheckOutDate(),
                            r.getRequestedRoomType()
                        );
                        if (roomNum == -1) {
                            cout << "No available room for the selected date range.\n";
                            break;
                        }

                        r.setRoomNumber(roomNum);

                        Room* roomPtr = roomRepo.findByNumber(roomNum);
                        if (!roomPtr) {
                            cout << "Room not found for pricing.\n";
                            break;
                        }

                        double pricePerNight = roomPtr->getPrice();
                        r.calculateTotalPrice(pricePerNight);

                        resRepo.addReservation(r);
                        resRepo.save();

                        cout << "Reservation created:\n";
                        cout << r;
                        break;
                    }

                    case 2:
                        resRepo.printAll();
                        break;

                    case 3: {
                        int rid;
                        cout << "Enter reservation ID: ";
                        cin >> rid;

                        if (resRepo.cancelById(rid)) {
                            cout << "Reservation cancelled.\n";
                            resRepo.save();
                        }
                        else {
                            cout << "Reservation not found.\n";
                        }
                        break;
                    }

                    case 4: {
                        int rid;
                        cout << "Enter reservation ID for invoice: ";
                        cin >> rid;

                        const auto& allRes = resRepo.getAll();
                        const Reservation* target = nullptr;

                        for (const auto& rr : allRes) {
                            // adjust method name if different
                            if (rr.getReservationID() == rid) {
                                target = &rr;
                                break;
                            }
                        }

                        if (!target) {
                            cout << "Reservation not found.\n";
                        }
                        else {
                            printInvoice(*target, customerRepo, roomRepo);
                        }
                        break;
                    }

                    case 0:
                        break;

                    default:
                        cout << "Invalid choice.\n";
                    }

                } while (resChoice != 0);
                break;
            }

            case 4: {
                // ================= ADMIN REPORTS =================
                const auto& allCustomers = customerRepo.getAll();
                const auto& allRooms = roomRepo.getAll();
                const auto& allRes = resRepo.getAll();

                int totalCustomers = (int)allCustomers.size();
                int totalRooms = (int)allRooms.size();
                int availableRooms = 0;
                int unavailableRooms = 0;

                for (const auto& room : allRooms) {
                    if (room.getAvailability())
                        availableRooms++;
                    else
                        unavailableRooms++;
                }

                double totalRevenue = 0.0;
                for (const auto& r : allRes) {
                    totalRevenue += r.getTotalPrice();
                }
                cout << "\n------------------------------------------------\n";
                cout << "                  ADMIN REPORT                  \n";
                cout << "------------------------------------------------\n";
                cout << "Total customers         : " << totalCustomers << "\n";
                cout << "Total rooms             : " << totalRooms << "\n";
                cout << "Available rooms         : " << availableRooms << "\n";
                cout << "Unavailable rooms       : " << unavailableRooms << "\n";
                cout << "Total reservations      : " << allRes.size() << "\n";
                cout << "Total revenue (all time): " << totalRevenue << " $\n";
                cout << "------------------------------------------------\n";

                break;
            }

            case 0:
                cout << "Exiting... Goodbye!\n";
                break;

            default:
                cout << "Invalid choice.\n";
            }

        } while (mainChoice != 0);
    }

        else if (roleChoice == 2) {
            // ---------- CUSTOMER FLOW ----------
            Customer* logged = customerLogin(customerRepo);
            if (!logged) {
                // login failed
                return 0;
            }

            // simple customer menu (for now only view reservations)
            int custChoice;
            do {
                cout << "\n===== CUSTOMER PORTAL =====\n";
                cout << "1. View my reservations\n";
                cout << "0. Logout\n";
                cout << "Enter choice: ";
                cin >> custChoice;

                switch (custChoice) {
                case 1:
                    showCustomerReservations(*logged, resRepo);
                    break;
                case 0:
                    cout << "Logging out...\n";
                    break;
                default:
                    cout << "Invalid choice.\n";
                }

            } while (custChoice != 0);
        }
        else {
            cout << "Invalid role choice. Exiting.\n";
        }

        return 0;
    }
