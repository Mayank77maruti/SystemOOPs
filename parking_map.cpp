#include <iostream>
#include <unordered_map>
#include <vector>
#include <iomanip>
#include <ctime>
#include <limits>

class ParkingTicket {
public:
    std::string licensePlate;
    time_t entryTime;
    time_t exitTime;
    bool paid;
};

class ParkingSystem {
private:
    std::unordered_map<int, ParkingTicket> activeTickets;
    std::vector<ParkingTicket> completedTickets;
    int nextTicketId = 1;
    const double hourlyRate = 2.50;

    void clearInput() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    double calculateCharge(time_t entry, time_t exit) const {
        double hours = difftime(exit, entry) / 3600.0;
        return hours * hourlyRate;
    }

    void printTicket(const ParkingTicket& ticket, int id) const {
        double hours = difftime(ticket.exitTime, ticket.entryTime)/3600.0;
        double charge = calculateCharge(ticket.entryTime, ticket.exitTime);
        
        std::cout << std::setw(8) << id 
                  << std::setw(12) << ticket.licensePlate 
                  << std::setw(10) << std::fixed << std::setprecision(1) << hours 
                  << std::setw(8) << (ticket.paid ? "Yes" : "No") 
                  << std::setw(10) << "$" << charge << "\n";
    }

public:
    void vehicleEntry() {
        ParkingTicket ticket;
        ticket.entryTime = time(nullptr);
        ticket.paid = false;

        std::cout << "\nEnter license plate: ";
        clearInput();
        std::getline(std::cin, ticket.licensePlate);

        int currentId = nextTicketId++;
        activeTickets.emplace(currentId, ticket);
        std::cout << "Ticket issued. ID: " << currentId << "\n";
    }

    void vehicleExit() {
        if (activeTickets.empty()) {
            std::cout << "\nNo active vehicles!\n";
            return;
        }

        std::cout << "\nEnter ticket ID: ";
        int ticketId;
        while (!(std::cin >> ticketId)) {
            std::cout << "Invalid ID. Try again: ";
            clearInput();
        }

        auto it = activeTickets.find(ticketId);
        if (it != activeTickets.end()) {
            it->second.exitTime = time(nullptr);
            double charge = calculateCharge(it->second.entryTime, it->second.exitTime);

            std::cout << "License: " << it->second.licensePlate << "\n";
            std::cout << "Parking duration: " 
                      << std::fixed << std::setprecision(1) 
                      << (difftime(it->second.exitTime, it->second.entryTime)/3600.0) 
                      << " hours\n";
            std::cout << "Amount due: $" << charge << "\n";

            std::cout << "Mark as paid? (1=Yes, 0=No): ";
            int paid;
            while (!(std::cin >> paid) {
                std::cout << "Invalid input. Try again: ";
                clearInput();
            }
            it->second.paid = (paid == 1);

            completedTickets.push_back(it->second);
            activeTickets.erase(it);
            std::cout << "Vehicle exited.\n";
        } else {
            std::cout << "Ticket ID not found!\n";
        }
    }

    void displayReport() const {
        std::cout << "\nPARKING REPORT\n";
        std::cout << std::setw(8) << "Ticket" 
                  << std::setw(12) << "License" 
                  << std::setw(10) << "Hours" 
                  << std::setw(8) << "Paid" 
                  << std::setw(10) << "Amount\n";

        double totalRevenue = 0;
        for (const auto& ticket : completedTickets) {
            double charge = calculateCharge(ticket.entryTime, ticket.exitTime);
            if (ticket.paid) totalRevenue += charge;
            printTicket(ticket, &ticket - &completedTickets[0] + 1);
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
