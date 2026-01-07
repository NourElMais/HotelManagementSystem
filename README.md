# Hotel Management System

A comprehensive C++ console application for managing hotel operations, built with modern C++ features and JSON-based data persistence.

## Features

### Admin Portal
- Secure login system with admin credentials
- **Customer Management**: Add, view, edit, and delete customer records
- **Room Management**: Add, view, edit, and delete rooms with pricing information
- **Reservation Management**: Create, view, cancel reservations, and generate invoices
- Automated room scheduling and availability checking based on dates and room types
- Administrative reports showing totals for customers, rooms, revenue, and availability

### Customer Portal
- Customer login using customer ID
- View personal reservation history

## Technical Details
- **Language**: C++17
- **Data Storage**: JSON files using nlohmann/json library
- **Architecture**: Object-oriented design with repository pattern for data management
- **Validation**: Age restrictions for reservations (minimum 16 years)
- **Security**: Hidden password input for admin login
- **Error Handling**: Input validation and robust error management

## Installation & Setup

### Prerequisites
- Visual Studio 2022 (or compatible version) with C++ development tools
- Windows OS

### Steps
1. Clone or download the project files
2. Open `ProjectFiles.sln` in Visual Studio
3. Build the solution (F7 or Build > Build Solution)
4. Run the executable from the Debug or Release folder

### Dependencies
- nlohmann/json.hpp (included in the project)
- Standard C++ libraries

## Usage

### Running the Application
Execute the compiled binary. The application will prompt for login type:

1. **Admin Login**: Requires username and password from `Admins.txt`
2. **Customer Login**: Requires customer ID

### Admin Features
- Manage customers, rooms, and reservations through intuitive menus
- Generate reports and invoices
- All changes are automatically saved to JSON files

### Customer Features
- View existing reservations
- Simple, user-friendly interface

## File Structure
```
ProjectFiles/
├── main.cpp                    # Application entry point
├── Customer.cpp/.h            # Customer entity and logic
├── CustomerRepository.cpp/.h  # Customer data management
├── Room.cpp/.h                # Room entity and logic
├── RoomRepository.cpp/.h      # Room data management
├── Reservation.cpp/.h         # Reservation entity and logic
├── ReservationRepository.cpp/.h # Reservation data management
├── Scheduler.cpp/.h           # Room scheduling logic
├── json.hpp                   # JSON library
├── customers.json             # Customer data storage
├── rooms.json                 # Room data storage
├── reservations.json          # Reservation data storage
└── Admins.txt                 # Admin credentials
```

## Contributing
Feel free to fork this project and submit pull requests for improvements or bug fixes.

## License
This project is open-source. Please check for any licensing requirements.
