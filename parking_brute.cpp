#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <limits>

class ParkingTicket {
public:
    int ticketId;
    std::string licensePlate;
    time_t entryTime;
    time_t exitTime;
    bool paid;
};

class ParkingSystem {
private:
    std::vector<ParkingTicket> activeTickets;
    std::vector<ParkingTicket> completedTickets;
    double hourlyRate = 2.50;

    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    double calculateCharge(time_t entry, time_t exit) {
        double hours = difftime(exit, entry) / 3600.0;
        return hours * hourlyRate;
    }

public:
    void vehicleEntry() {
        ParkingTicket ticket;
        ticket.ticketId = activeTickets.size() + 1;
        ticket.paid = false;
        ticket.entryTime = time(nullptr);

        std::cout << "\nEnter license plate: ";
        clearInput();
        std::getline(std::cin, ticket.licensePlate);

        activeTickets.push_back(ticket);
        std::cout << "Ticket issued. ID: " << ticket.ticketId << "\n";
    }

    void vehicleExit() {
        if (activeTickets.empty()) {
            std::cout << "\nNo active vehicles!\n";
            return;
        }

        int ticketId;
        std::cout << "\nEnter ticket ID: ";
        while (!(std::cin >> ticketId) {
            std::cout << "Invalid ID. Try again: ";
            clearInput();
        }

        for (auto it = activeTickets.begin(); it != activeTickets.end(); ++it) {
            if (it->ticketId == ticketId) {
                it->exitTime = time(nullptr);
                double charge = calculateCharge(it->entryTime, it->exitTime);

                std::cout << "License: " << it->licensePlate << "\n";
                std::cout << "Parking duration: " 
                          << std::fixed << std::setprecision(1) 
                          << (difftime(it->exitTime, it->entryTime)/3600.0 
                          << " hours\n";
                std::cout << "Amount due: $" << charge << "\n";

                std::cout << "Mark as paid? (1=Yes, 0=No): ";
                int paid;
                while (!(std::cin >> paid) {
                    std::cout << "Invalid input. Try again: ";
                    clearInput();
                }
                it->paid = (paid == 1);

                completedTickets.push_back(*it);
                activeTickets.erase(it);
                std::cout << "Vehicle exited.\n";
                return;
            }
        }
        std::cout << "Ticket ID not found!\n";
    }

    void displayReport() {
        std::cout << "\nPARKING REPORT\n";
        std::cout << std::setw(8) << "Ticket" 
                  << std::setw(12) << "License" 
                  << std::setw(10) << "Hours" 
                  << std::setw(8) << "Paid" 
                  << std::setw(10) << "Amount\n";

        double totalRevenue = 0;
        for (const auto& ticket : completedTickets) {
            double hours = difftime(ticket.exitTime, ticket.entryTime)/3600.0;
            double charge = calculateCharge(ticket.entryTime, ticket.exitTime);
            if (ticket.paid) totalRevenue += charge;

            std::cout << std::setw(8) << ticket.ticketId 
                      << std::setw(12) << ticket.licensePlate 
                      << std::setw(10) << std::fixed << std::setprecision(1) << hours 
                      << std::setw(8) << (ticket.paid ? "Yes" : "No") 
                      << std::setw(10) << "$" << charge << "\n";
        }
        std::cout << "\nTOTAL REVENUE: $" << totalRevenue << "\n";
    }

    void run() {
        while (true) {
            std::cout << "\nPARKING MANAGEMENT\n"
                      << "1. Vehicle Entry\n"
                      << "2. Vehicle Exit\n"
                      << "3. Daily Report\n"
                      << "4. Exit\n"
                      << "Choose: ";
            
            int choice;
            if (std::cin >> choice) {
                switch (choice) {
                    case 1: vehicleEntry(); break;
                    case 2: vehicleExit(); break;
                    case 3: displayReport(); break;
                    case 4: return;
                    default: std::cout << "Invalid choice!\n";
                }
            } else {
                std::cout << "Enter a number!\n";
                clearInput();
            }
        }
    }
};

int main() {
    ParkingSystem system;
    system.run();
    return 0;
}
