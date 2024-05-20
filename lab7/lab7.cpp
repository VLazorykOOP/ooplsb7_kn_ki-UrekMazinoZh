#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Vehicle {
private:
    string regNumber; 
    string model; 
    bool isAvailable; 

public:
    Vehicle(string regNum = "", string model = "") : regNumber(regNum), model(model), isAvailable(true) {}

    void saveToFile(ofstream& outFile) {
        outFile << regNumber << "," << model << "," << (isAvailable ? "1" : "0") << endl;
    }

    static Vehicle readFromFile(ifstream& inFile) {
        string regNum, model, avail;
        getline(inFile, regNum, ',');
        getline(inFile, model, ',');
        getline(inFile, avail);
        Vehicle vehicle(regNum, model);
        vehicle.isAvailable = (avail == "1");
        return vehicle;
    }

    void display() {
        cout << "Рег. номер: " << regNumber << ", Модель: " << model << ", Доступний: " << (isAvailable ? "Так" : "Ні") << endl;
    }

    string getRegNumber() { return regNumber; }
    string getModel() { return model; }
    bool getIsAvailable() { return isAvailable; }
    void setAvailability(bool availability) { isAvailable = availability; }
};

class Driver {
private:
    string name; 
    bool isWorking;

public:
    Driver(string name) : name(name), isWorking(true) {}

    void makeRepairRequest(Vehicle& vehicle) {
        cout << "Заявка на ремонт відправлена для автомобіля: " << vehicle.getRegNumber() << endl;
    }

    void completeTrip() {
        cout << "Рейс виконано водієм: " << name << endl;
    }

    void setIsWorking(bool working) { isWorking = working; }
    bool getIsWorking() { return isWorking; }
    string getName() { return name; }
};

class Dispatcher {
private:
    vector<Vehicle> vehicles; 
    vector<Driver> drivers;

public:
    void addVehicle(const Vehicle& vehicle) {
        vehicles.push_back(vehicle);
    }

    void addDriver(const Driver& driver) {
        drivers.push_back(driver);
    }

    void assignTrip(string driverName, string regNumber) {
        for (auto& driver : drivers) {
            if (driver.getName() == driverName && driver.getIsWorking()) {
                for (auto& vehicle : vehicles) {
                    if (vehicle.getRegNumber() == regNumber && vehicle.getIsAvailable()) {
                        vehicle.setAvailability(false);
                        cout << "Рейс призначено для водія: " << driverName << " на автомобіль: " << regNumber << endl;
                        return;
                    }
                }
            }
        }
        cout << "Не вдалось призначити рейс для водія: " << driverName << endl;
    }

    void sidelineDriver(string driverName) {
        for (auto& driver : drivers) {
            if (driver.getName() == driverName) {
                driver.setIsWorking(false);
                cout << "Водій " << driverName << " відсторонений від роботи." << endl;
                return;
            }
        }
    }

    void showAllVehicles() {
        for (auto& vehicle : vehicles) {
            vehicle.display();
        }
    }

    void saveVehiclesToFile(ofstream& outFile) {
        for (auto& vehicle : vehicles) {
            vehicle.saveToFile(outFile);
        }
    }
};

void displayMenu() {
    cout << "\n*** Автобаза Меню ***\n";
    cout << "1. Додати автомобіль\n";
    cout << "2. Призначити рейс\n";
    cout << "3. Відсторонити водія\n";
    cout << "4. Показати всі автомобілі\n";
    cout << "5. Завершити роботу\n";
    cout << "Ваш вибір: ";
}

int main() {
    Dispatcher dispatcher;
    string regNum, model, driverName;
    int choice;

    dispatcher.addDriver(Driver("Іван"));
    dispatcher.addDriver(Driver("Петро"));
    dispatcher.addDriver(Driver("Олександр"));
    dispatcher.addDriver(Driver("Василь"));
    dispatcher.addDriver(Driver("Михайло"));
    dispatcher.addDriver(Driver("Олег"));
    dispatcher.addDriver(Driver("Андрій"));
    dispatcher.addDriver(Driver("Сергій"));
    dispatcher.addDriver(Driver("Дмитро"));
    dispatcher.addDriver(Driver("Олексій"));

    ifstream inFile("vehicles.txt");
    while (inFile.good()) {
        Vehicle vehicle = Vehicle::readFromFile(inFile);
        if (inFile) { 
            dispatcher.addVehicle(vehicle);
        }
    }
    inFile.close();

    do {
        displayMenu();
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "Введіть реєстраційний номер: ";
            cin >> regNum;
            cout << "Введіть модель: ";
            cin >> model;
            dispatcher.addVehicle(Vehicle(regNum, model));
            break;
        case 2: 
            cout << "Введіть ім'я водія: ";
            cin >> driverName;
            cout << "Введіть реєстраційний номер автомобіля: ";
            cin >> regNum;
            dispatcher.assignTrip(driverName, regNum);
            break;
        case 3:
            cout << "Введіть ім'я водія для відсторонення: ";
            cin >> driverName;
            dispatcher.sidelineDriver(driverName);
            break;
        case 4:
            dispatcher.showAllVehicles();
            break;
        }
    } while (choice != 5);

    ofstream outFile("vehicles.txt");
    dispatcher.saveVehiclesToFile(outFile);
    outFile.close();

    return 0;
}


/* file: vehicles.txt
AA1234BX,Tesla Model S,1
BC9876MX,Toyota Corolla,1
XX0011XX,Ford Focus,1
YY2244YY,Chevrolet Bolt,0
ZZ5566ZZ,Nissan Leaf,0
AA7890CC,Hyundai Sonata,0
BB3344DD,Volkswagen Golf,1
CC5678EE,Mercedes-Benz C-Class,0
DD9012FF,BMW 3 Series,1
EE3456GG,Audi A4,1
*/