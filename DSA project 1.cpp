#include <iostream>
#include <string>
#include <fstream>
#include <sstream> 
#include <vector>

using namespace std;


class User {
protected:
    string username, password, email;
    int attempts;

public:
    User() : attempts(0) {}

    bool createAccount() {
        if (attempts >= 5) {
            cout << "You have reached the maximum number of attempts. Account creation failed." << endl;
            return false;
        }

        cout << "Enter username: ";
        getline(cin, username); // Allow spaces in username
        cout << "Enter password: ";
        getline(cin, password); // Use getline to read password including spaces
        cout << "Enter email: ";
        getline(cin, email); // Read email

        saveToFile(); // Save data in the new format

        cout << "Your account is successfully created." << endl;
        return true;
    }

    void saveToFile() {
        ofstream file("accounts.txt", ios::app);
        if (file.is_open()) {
            string accountData = username + " | " + password + " | " + email;
            file << accountData << endl;
            file.close();
        }
        else {
            cout << "Unable to open file for saving account data.\n";
        }
    }

    bool login() {
        int attempts = 0; // Reset attempts counter at the start of login attempt

        while (attempts < 5) {
            cout << "Enter username: ";
            string username;
            getline(cin, username); // Allow spaces in username

            ifstream file("accounts.txt");
            string line, storedUsername, storedPassword, storedEmail;
            bool found = false;

            while (getline(file, line)) {
                stringstream ss(line);
                getline(ss, storedUsername, '|');
                getline(ss, storedPassword, '|');
                getline(ss, storedEmail, '|');

                // Trim spaces if necessary
                storedUsername = trim(storedUsername);
                storedPassword = trim(storedPassword);
                storedEmail = trim(storedEmail);

                if (username == storedUsername) {
                    found = true;
                    break;
                }
            }

            file.close();

            if (!found) {
                cout << "Incorrect username. Please try again." << endl;
                attempts++;
                continue;
            }

            int passwordAttempts = 0; // Reset password attempts for each new username

            while (passwordAttempts <= 5) {
                cout << "Enter password: ";
                string password;
                getline(cin, password); // Use getline to read password including spaces

                if (storedPassword == password) {
                    cout << "Login successful.\n" << endl;
                    return true;
                }
                else {
                    cout << "Incorrect password. Please try again." << endl;
                    passwordAttempts++;
                }
            }

            cout << "You have reached the maximum number of attempts for password. Account locked." << endl;
            return false; // Exit if max password attempts reached
        }

        cout << "You have reached the maximum number of attempts for username. Account locked." << endl;
        return false; // Exit if max username attempts reached
    }

    bool signOut() {
        cout << "Enter username to sign out: ";
        getline(cin, username); // Allow spaces in username
        cout << "Enter password: ";
        getline(cin, password); // Use getline to read password including spaces
        cout << "Enter email: ";
        getline(cin, email); // Read email

        ifstream file("accounts.txt");
        ofstream tempFile("temp.txt");
        string line, storedUsername, storedPassword, storedEmail;
        bool found = false;

        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, storedUsername, '|');
            getline(ss, storedPassword, '|');
            getline(ss, storedEmail, '|');

            // Trim spaces if necessary
            storedUsername = trim(storedUsername);
            storedPassword = trim(storedPassword);
            storedEmail = trim(storedEmail);

            if (username == storedUsername && password == storedPassword && email == storedEmail) {
                found = true; // Account found, skip writing this line to temp file
            }
            else {
                tempFile << line << endl; // Write all other lines to temp file
            }
        }

        file.close();
        tempFile.close();

        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");

        if (found) {
            cout << "Account successfully signed out and removed." << endl;
            return true;
        }
        else {
            cout << "Account not found or incorrect details provided." << endl;
            return false;
        }
    }

    int getAttempts() const {
        return attempts;
    }

private:
    // Helper function to trim spaces from a string
    string trim(const string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, last - first + 1);
    }
};



// Node for Linked List (Doctor, Appointment, Emergency Case)
template <typename T>
class Node {
public:
    T data;  // Changed from Patient to T for generic use
    Node* next;

    Node(T data) : data(data), next(nullptr) {}
};

// Linked List class
template <typename T>
class LinkedList {
public:
    Node<T>* head;
    LinkedList() : head(nullptr) {}

    void insert(T data) {
        Node<T>* newNode = new Node<T>(data);
        newNode->next = head;
        head = newNode;
    }

    bool deleteData(T data) {  // Change the parameter type to T
        Node<T>* current = head;
        Node<T>* previous = nullptr;

        while (current) {
            if (current->data == data) {  // Use the == operator for comparison
                if (previous) {
                    previous->next = current->next;
                }
                else {
                    head = current->next;
                }
                delete current;
                return true;
            }
            previous = current;
            current = current->next;
        }
        return false;
    }

    void display() {
        Node<T>* current = head;
        while (current) {
            cout << current->data << endl;
            current = current->next;
        }
    }
};

class Doctor {
public:
    string name;
    string specialization;
    int id;

    Doctor(string name, string specialization, int id)
        : name(name), specialization(specialization), id(id) {}

    // Save doctor to file (opened in append mode)
    void saveToFile() {
        ofstream file("doctors.txt", ios::app);
        if (file.is_open()) {
            string doctorData = to_string(id) + " | " + name + " | " + specialization;
            file << doctorData << endl;
            file.close();
        }
        else {
            cout << "Unable to open file for saving doctor.\n";
        }
    }

    // Delete doctor from file
    static bool deleteFromFile(int id) {
        ifstream file("doctors.txt");
        ofstream tempFile("temp.txt");
        string line;
        bool found = false;

        while (getline(file, line)) {
            int fileId;
            string name, specialization;
            stringstream ss(line);
            ss >> fileId;
            getline(ss, name, ' ');
            getline(ss, specialization);
            if (fileId != id) {
                tempFile << line << endl;
            }
            else {
                found = true;
            }
        }

        file.close();
        tempFile.close();

        remove("doctors.txt");
        rename("temp.txt", "doctors.txt");

        return found;
    }

    // Display all doctors from file
    static void displayDoctors() {
        ifstream file("doctors.txt");
        string line;
        if (file.is_open()) {
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        }
        else {
            cout << "No doctors available.\n";
        }
    }

    // Overloading == operator for Doctor class
    bool operator==(const Doctor& other) const {
        return this->id == other.id;
    }
    // Static method to update doctor credentials
    static void updateDoctorCredentials() {
        int id;
        bool doctorFound = false;
        int choice;

        while (true) {
            cout << "Enter Doctor ID to update credentials: ";
            cin >> id;
            cin.ignore(); // Ignore the newline after entering ID

            ifstream file("doctors.txt");
            ofstream tempFile("temp.txt");
            string line;
            bool found = false;

            if (file.is_open() && tempFile.is_open()) {
                while (getline(file, line)) {
                    int fileId;
                    string name, specialization;
                    stringstream ss(line);
                    ss >> fileId;
                    ss.ignore(); // Skip the space after ID
                    getline(ss, name, '|');
                    getline(ss, specialization);

                    // If the doctor is found
                    if (fileId == id) {
                        found = true;
                        cout << "Doctor found: " << line << endl;
                        cout << "Select the credential to update:\n";
                        cout << "1. Name\n";
                        cout << "2. Specialization\n";
                        cout << "3. ID\n";
                        cout << "Enter your choice: ";
                        cin >> choice;
                        cin.ignore(); // Ignore the newline after choice

                        // Update selected credential
                        switch (choice) {
                        case 1:
                            cout << "Enter new name: ";
                            getline(cin, name);
                            break;
                        case 2:
                            cout << "Enter new specialization: ";
                            getline(cin, specialization);
                            break;
                        case 3:
                            cout << "Enter new ID: ";
                            cin >> fileId;
                            break;
                        default:
                            cout << "Invalid choice. No updates made.\n";
                            tempFile << line << endl;  // No changes, write original line
                            continue; // Skip to the next line without updating
                        }

                        // Write the updated doctor information to the temp file
                        tempFile << to_string(fileId) + " | " + name + " | " + specialization << endl;
                        cout << "Doctor's credential updated successfully.\n";
                    }
                    else {
                        tempFile << line << endl; // Write unchanged data to temp file
                    }
                }

                file.close();
                tempFile.close();

                if (found) {
                    remove("doctors.txt");   // Delete the original file
                    rename("temp.txt", "doctors.txt"); // Rename temp file to original
                    break; // Exit the loop after updating
                }
                else {
                    cout << "Doctor with ID " << id << " not found.\n";
                    cout << "Do you want to:\n";
                    cout << "1. Try again with a new ID\n";
                    cout << "2. Return to main menu\n";
                    int choice;
                    cout << "Enter choice:";
                    cin >> choice;
                    if (choice == 1) {
                        continue; // Try again with a new ID
                    }
                    else if (choice == 2) {
                        
                        break; // Exit the function and return to main menu
                    }
                    else {
                        cout << "Invalid choice. Returning to main menu.\n";
                        break; // Exit the function and return to main menu
                    }
                }
            }
            else {
                cout << "Error opening file.\n";
                break; // Exit if the file can't be opened
            }
        }
    }

};

class Patient {
public:
    string name;
    int id;
    string medicalHistory;
    string doctorName;

    Patient(string name, int id, string medicalHistory, string doctorName)
        : name(name), id(id), medicalHistory(medicalHistory), doctorName(doctorName) {}

    // Save patient appointment to file (opened in append mode)
    void saveToFile() {
        ofstream file("appointments.txt", ios::app);
        if (file.is_open()) {
            // Format the data with separators
            string appointmentData = to_string(id) + " | " + name + " | " + medicalHistory + " | " + doctorName;
            file << appointmentData << endl; // Save the formatted data to the file
            file.close();
        }
        else {
            cout << "Unable to open file for saving appointment.\n";
        }
    }

    // Static method to delete an appointment by ID from the file
    static bool deleteAppointmentFromFile(int id) {
        ifstream file("appointments.txt");
        ofstream tempFile("temp.txt"); // Temporary file to store updated appointments
        string line;
        bool found = false;

        if (file.is_open() && tempFile.is_open()) {
            while (getline(file, line)) {
                int fileId;
                string name, medicalHistory, doctorName;
                stringstream ss(line);
                ss >> fileId;
                ss.ignore();  // Ignore the space after the ID
                getline(ss, name, '|');
                getline(ss, medicalHistory, '|');
                getline(ss, doctorName);

                if (fileId != id) {
                    tempFile << line << endl; // Copy the line to the temp file if ID doesn't match
                }
                else {
                    found = true; // Set found to true if matching ID is found
                }
            }

            file.close();
            tempFile.close();

            // If the appointment was found, replace the original file with the updated temp file
            if (found) {
                remove("appointments.txt"); // Delete the original file
                rename("temp.txt", "appointments.txt"); // Rename temp file to original
                
            }
            else {
                
                remove("temp.txt"); // Remove the temporary file if appointment wasn't found
            }
        }
        else {
            cout << "Error opening the files.\n";
        }

        return found;
    }

    // Overloading == operator for Patient class
    bool operator==(const Patient& other) const {
        return this->id == other.id;
    }

    // Function to view patient's history from both the appointments and emergencies files by patient ID
    static void viewPatientHistory() {
        int id;
        cout << "Enter the patient ID to view their medical history: ";
        cin >> id;
        cin.ignore(); // Ignore the newline character after the ID

        ifstream file;
        string line;
        bool found = false;

        // First, check the appointments file
        file.open("appointments.txt");
        if (file.is_open()) {
            while (getline(file, line)) {
                int fileId;
                string name, medicalHistory, doctorName;
                stringstream ss(line);
                ss >> fileId;
                ss.ignore();  // Ignore the space after the ID
                getline(ss, name, '|');
                getline(ss, medicalHistory, '|');
                getline(ss, doctorName);

                // If the patient ID matches, display their history
                if (fileId == id) {
                    found = true;
                    cout << "\n--- Patient's Appointment History ---\n";
                    cout << "ID  | Name | Medical History | Doctor Name\n";
                    cout << line << endl; // Display the entire line for the patient
                }
            }
            file.close();
        }

        // If not found in the appointments file, check the emergencies file
        if (!found) {
            file.open("emergencies.txt");
            if (file.is_open()) {
                while (getline(file, line)) {
                    int fileId, priority;
                    string patientName, medicalHistory;
                    stringstream ss(line);
                    ss >> fileId;
                    ss.ignore();  // Ignore the space after the ID
                    getline(ss, patientName, '|');
                    getline(ss, medicalHistory, '|');
                    ss >> priority; // Assuming priority is at the end of the line

                    // If the patient ID matches, display their emergency history
                    if (fileId == id) {
                        found = true;
                        cout << "\n--- Patient's Emergency History ---\n";
                        cout << "ID  | Name | Medical History | Priority\n";
                        cout << line << endl; // Display the entire line for the patient
                    }
                }
                file.close();
            }
        }

        if (!found) {
            cout << "Patient with ID " << id << " not found in either appointments or emergencies.\n";
        }
        else {
            cout << "\nEnd of record.\n";
        }
    }


};



// EmergencyCase class with priority
class EmergencyCase {
public:
    string description;
    int id;
    int priority;  // Add priority to define the urgency

    EmergencyCase(int id, string description, int priority)
        : id(id), description(description), priority(priority) {}

    // Overloading < operator for priority comparison (highest priority first)
    bool operator<(const EmergencyCase& other) const {
        return priority < other.priority;  // Lower priority number means higher priority
    }

    // Overloading << operator for displaying emergency cases
    friend ostream& operator<<(ostream& os, const EmergencyCase& emergencyCase) {
        os << "Emergency ID: " << emergencyCase.id << ", Description: " << emergencyCase.description
            << ", Priority: " << emergencyCase.priority;
        return os;
    }
};

// Manual Priority Queue implementation using vector
class PriorityQueue {
private:
    vector<EmergencyCase> queue;

public:
    // Insert into the queue, maintaining sorted order by priority
    void insert(const EmergencyCase& emergency) {
        // Find the correct position to insert based on priority
        auto it = queue.begin();
        while (it != queue.end() && it->priority < emergency.priority) {
            ++it;
        }
        queue.insert(it, emergency);  // Insert the emergency in the correct position
    }

    // Dequeue the highest priority emergency (the first element in the sorted vector)
    EmergencyCase dequeue() {
        if (!queue.empty()) {
            EmergencyCase topEmergency = queue.front();  // The highest priority emergency
            queue.erase(queue.begin());  // Remove the highest priority emergency
            return topEmergency;
        }
        else {
            cout << "No emergency cases available.\n";
            return EmergencyCase(-1, "", -1);  // Return a default invalid case
        }
    }

    // Check if the queue is empty
    bool isEmpty() {
        return queue.empty();
    }

    // Display all emergencies exactly as they appear in the file
    void display() {
        if (queue.empty()) {
            cout << "No emergency cases.\n";
        }
        else {
            for (const auto& emergency : queue) {
                cout << emergency.description << endl; // Only display the description
            }
        }
    }


};


static string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == string::npos || end == string::npos) {
        return "";
    }

    return str.substr(start, end - start + 1);
}
void loadEmergenciesFromFile(PriorityQueue& emergencyQueue) {
    ifstream file("emergencies.txt");
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            // Assume the line is formatted as: ID | Patient Name | Medical History | Priority
            stringstream ss(line);
            int id, priority;
            ss >> id; // Extract ID
            ss.ignore(); // Ignore delimiter
            ss.ignore(line.size(), '|'); // Skip to priority

            ss >> priority; // Extract priority
            
           
            // Insert the entire line as the description
            EmergencyCase emergency(id, line, priority);
            emergencyQueue.insert(emergency);
        }
        file.close();
    }
    else {
        cout << "No previous emergency cases found.\n";
    }
}


void displayAppointmentsForDoctor(const string& doctorName) {
    ifstream file("appointments.txt");

    if (!file.is_open()) {
        cout << "Error opening the file!" << endl;
        return;
    }

    string line;
    bool found = false;

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, medicalHistory, doctor;

        // Assuming the format: id | name | medicalHistory | doctorName
        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, medicalHistory, '|');
        getline(ss, doctor, '|');

        // Trim spaces from each field
        id = trim(id);
        name = trim(name);
        medicalHistory = trim(medicalHistory);
        doctor = trim(doctor);

        // Compare doctor name with the input doctorName
        if (doctor == doctorName) {
            found = true;
            cout << "Appointment found!" << endl;
            cout << "ID: " << id << endl;
            cout << "Patient: " << name << endl;
            cout << "Medical History: " << medicalHistory << endl;
            cout << "Doctor: " << doctor << endl;
            break;
        }
    }

    if (!found) {
        cout << "Appointment not found" << endl;
    }

    file.close();
}


void printdesign() {
    for (int i = 0; i <= 45; i++) {
        cout << "=";
    }
}

// Function to display the main menu
void displayMenu() {
    cout << "\n\n\n";
    printdesign();
    cout << endl;
    cout << "\n\tHospital Management System\n";
    cout << endl;
    printdesign();
    cout << "\n\n";
    cout << "1. Add Doctor\n2. Delete Doctor\n3. Display Doctor\n4. Update Doctor Credentials\n";
    cout << "5. Add Appointment\n6. Delete Appointment\n7. Display All Appointments\n";
    cout << "8. Add Emergency Case\n9. Resolve Emergency Case\n";
    cout << "10. Display All Emergencies\n11. View Appointments for Doctor\n12. View Patient History\n";
    cout << "0. Exit\n";
}
void mainmenu() {
    LinkedList<Doctor> doctorList;
    LinkedList<Patient> patientList;
    PriorityQueue emergency; 
    loadEmergenciesFromFile(emergency); 

    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string name, specialization;
            int id;
            cout << "Enter doctor's name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter doctor's specialization: ";
            getline(cin, specialization);
            cout << "Enter doctor's ID: ";
            cin >> id;
            Doctor doctor(name, specialization, id);
            doctor.saveToFile();
            doctorList.insert(doctor);
            cout << "Doctor added successfully!" << endl;
            break;
        }
        case 2: {
            int id;
            cout << "Enter doctor ID to delete: ";
            cin >> id;
            if (Doctor::deleteFromFile(id)) {
                cout << "Doctor deleted successfully!\n";
            }
            else {
                cout << "Doctor not found.\n";
            }
            break;
        }
        case 3: {
            cout << "\n--- List of Doctors ---\n";
            Doctor::displayDoctors();
            break;
        }
        case 4: {
            Doctor::updateDoctorCredentials();
            break;
        }
        case 5: {
            string name, medicalHistory, doctorName;
            int id;
            cout << "Enter patient name: ";
            cin >> name;
            cout << "Enter patient ID: ";
            cin >> id;
            cout << "Enter medical history: ";
            cin.ignore();
            getline(cin, medicalHistory);
            cout << "Enter doctor's name for appointment: ";
            getline(cin, doctorName);
            Patient patient(name, id, medicalHistory, doctorName);
            patient.saveToFile();
            patientList.insert(patient);
            cout << "Appointment added successfully!" << endl;
            break;
        }
        case 6: {
            int id;
            cout << "Enter patient ID to delete appointment: ";
            cin >> id;
            if (Patient::deleteAppointmentFromFile(id)) {
                cout << "Appointment deleted successfully!\n";
            }
            else {
                cout << "Appointment not found.\n";
            }
            break;
        }
        case 7: {
            cout << "\n--- All Appointments ---\n";
            ifstream file("appointments.txt");
            string line;
            if (file.is_open()) {
                while (getline(file, line)) {
                    cout << line << endl; // Display each appointment line
                }
                file.close();
            }
            else {
                cout << "Error: Could not open appointments file.\n";
            }
            break;
        }
        case 8: {
            int emergencyId, priority;
            string patientName, medicalHistory;

            cout << "Enter Emergency Case ID: ";
            cin >> emergencyId;

            cout << "Enter Patient Name: ";
            cin.ignore(); // Clear input buffer
            getline(cin, patientName);

            cout << "Enter Medical History: ";
            getline(cin, medicalHistory);

            cout << "Enter Priority (1 = highest, 5 = lowest): ";
            cin >> priority;

            // Format the emergency case data
            string emergencyData = to_string(emergencyId) + " | " + patientName + " | " + medicalHistory + " | " + to_string(priority);

            // Save to the file
            ofstream file("emergencies.txt", ios::app);
            if (file.is_open()) {
                file << emergencyData << endl; // Write to the file
                file.close();
            }
            else {
                cout << "Unable to save emergency case to file.\n";
            }

            // Insert into the priority queue with the correct priority
            emergency.insert(EmergencyCase(emergencyId, patientName + " | " + medicalHistory, priority));

            cout << "Emergency Case added!\n";
            break;
        }


        case 9: {
            if (emergency.isEmpty()) {
                cout << "No emergency cases to delete.\n";
            }
            else {
                int idToDelete;
                cout << "Enter the patient ID of the emergency case to delete: ";
                cin >> idToDelete;

                // Create a temporary priority queue to hold non-matching emergency cases
                PriorityQueue tempQueue;

                bool found = false;

                // Search through the emergency queue and remove the matching case
                while (!emergency.isEmpty()) {
                    EmergencyCase emergencyCase = emergency.dequeue();
                    if (emergencyCase.id != idToDelete) {
                        tempQueue.insert(emergencyCase); // If ID doesn't match, keep the case
                    }
                    else {
                        found = true; // Emergency case with matching ID found
                    }
                }

                // Copy remaining emergencies back to the original queue
                while (!tempQueue.isEmpty()) {
                    emergency.insert(tempQueue.dequeue());
                }

                if (found) {
                    cout << "Emergency case deleted successfully.\n";
                }
                else {
                    cout << "Emergency case with ID " << idToDelete << " not found.\n";
                }
            }
            break;
        }

        case 10: {
            emergency.display();
            break;
        }
        case 11: {
            string doctorName;
            cout << "Enter doctor's name to view their appointments: ";
            cin.ignore();
            getline(cin, doctorName);
            displayAppointmentsForDoctor(doctorName);
            break;
        }
        case 12: {
            Patient::viewPatientHistory();
            break;
        }
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid option. Try again.\n";
        }

    } while (choice != 0);
}



// Main function
int main() {

    cout << "\n\n\n";
    printdesign();
    cout << endl;
    cout << "\n\tHospital Management System\n";
    cout << endl;
    printdesign();
    cout << "\n\n";

    User user;
    string choice;
    bool validChoice = false;
    int invalidAttempts = 0;

    while (!validChoice && invalidAttempts < 5) {
        cout << "1. Sign Up \n2. Sign In \n3. Sign Out \n";
        cout << "Enter your choice:";
        getline(cin, choice);

        if (choice == "1") {
            validChoice = true;
            if (!user.createAccount()) {
                break;
            }
            else {
                mainmenu();
            }
        }
        else if (choice == "2") {
            validChoice = true;
            bool loginSuccess = false;
            while (!loginSuccess && user.getAttempts() < 5) {
                loginSuccess = user.login();
                if (loginSuccess) {
                    mainmenu();
                }
            }
            if (user.getAttempts() >= 5) {
                cout << "You have reached the maximum number of attempts. Account locked." << endl;

            }
        }
        else if (choice == "3") {
            validChoice = true;
            if (!user.signOut()) {
                exit(0);
            }
        }
        else {
            invalidAttempts++;
            if (invalidAttempts >= 5) {
                cout << "You have entered invalid choices too many times. Please try again later." << endl;
                exit(0);
            }
            cout << "Invalid choice. Please try again." << endl;
        }

    }

    

    return 0;
}
