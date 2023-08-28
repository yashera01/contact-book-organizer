#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
#include <iomanip>
#include <fstream> 
#include <sstream>
using namespace std;
class Contact {
private:
    string name;
    string phoneNumber;
    string email;
    string address;

public:
   Contact() = default;
//   constructor initializer list, which is used to initialize the member variables of the class.
   Contact(const string& name, const string& phoneNumber, const string& email, const string& address)
        : name(name), phoneNumber(phoneNumber), email(email), address(address) {}

    string getName() const {
        return name;
    }

    void displayContact() const {
        cout << "Name: " << name << "\nPhone: " << phoneNumber << "\nEmail: " << email << "\nAddress: " << address << "\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Contact& contact) {
        os << contact.name << ","
           << contact.phoneNumber << ","
           << contact.email << ","
           << contact.address;
        return os;
    }

    friend istream& operator>>(std::istream& is, Contact& contact) {
        getline(is, contact.name, ',');
        getline(is, contact.phoneNumber, ',');
        getline(is, contact.email, ',');
        getline(is, contact.address);
        return is;
    }
    
};

class ContactManager {
public:
    vector<Contact> contacts;
    map<string, string> userCredentials;
    vector<string> categories;
    

     // Save contacts to a file
    void saveContactsToFile(const std::string& filename) const {
        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cerr << "Error opening file for writing.\n";
            return;
        }

        for (const Contact& contact : contacts) {
            outFile << contact << "\n";
        }
    }
    
    
    void loadContactsFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile.is_open()) {
            cerr << "Error opening file for reading.\n";
            return;
        }

        contacts.clear();
        Contact contact;
        while (inFile >> contact) {
            contacts.push_back(contact);
        }
    }
    
    
    
    
    // Validate email format
    bool isValidEmail(const std::string& email) const {
        // You can use regular expressions for more advanced validation
        if (email.find('@') == string::npos) 
        {
            cout << "Invalid email format. Please use proper @ format.\n";
            return false;
        }
        return true;
    }
    
    bool isValidPhoneNumber(const string& phoneNumber) 
    {
        for (char c : phoneNumber) 
        {
            if (!isdigit(c) && c != '-') 
            {
                cout << "Invalid phone number format. Please use digits and dashes only.\n";
                return false;
            }
        }
        return true;
    }
   
    bool authenticateUser(const string& username, const string& password) const {
        auto it = userCredentials.find(username);
        if (it != userCredentials.end() && it->second == password) {
            return true;
        }
        return false;
    }
    
    void addUserCredentials(const string& username, const string& password) {
        userCredentials[username] = password;
    }
    
    void addContact(const Contact& contact) {
        contacts.push_back(contact);
        cout << "Contact added successfully!\n";
    }

    bool updateContact(const string& name, const Contact& newContact) {
        auto it = findContact(name);
        if (it != contacts.end()) {
            *it = newContact;
            cout << "Contact updated successfully!\n";
            return true;
        }
        return false;
    }

    bool deleteContact(const string& name) {
        auto it = findContact(name);
        if (it != contacts.end()) {
            contacts.erase(it);
            cout << "Contact deleted successfully!\n";
            return true;
        }
        return false;
    }

    void searchContact(const string& name) const {
        auto it = findContact(name);
        if (it != contacts.end()) {
            it->displayContact();
        } else {
            cout << "Contact not found.\n";
        }
    }

    void displayAllContacts() const {
        if (contacts.empty()) {
            cout << "No contacts found.\n";
        } else {
            cout << "All Contacts:\n";
            for (const Contact& contact : contacts) {
                contact.displayContact();
                cout << "-----------------\n";
            }
        }
    }

public:
    vector<Contact>::iterator findContact(const std::string& name) {
        return find_if(contacts.begin(), contacts.end(), [&](const Contact& contact) {
            return contact.getName() == name;
        });
    }

    vector<Contact>::const_iterator findContact(const string& name) const {
        return find_if(contacts.begin(), contacts.end(), [&](const Contact& contact) {
            return contact.getName() == name;
        });
    }
};

int main() {
    ContactManager contactManager;
    
    //  user credentials for authentication
    cout<<"-----------------------------------Welcome To Data House -----------------------------------"<<endl;
    contactManager.addUserCredentials("user", "password");
     
    contactManager.loadContactsFromFile("contacts.txt");
    
    bool authenticated = false;
    while (true) {
        cout << "Menu:\n";
        cout << "1. Add Contact\n";
        cout << "2. Update Contact\n";
        cout << "3. Delete Contact\n";
        cout << "4. Search Contact\n";
        cout << "5. Display All Contacts\n";
        cout << "6. Save Contacts to File\n";
        cout << "7. Load Contacts from File\n";
        cout << "8. Exit\n";
        cout<<"-----------------------------------------------------------------------------------"<<endl;
        // Authentication before accessing the system
        if (!authenticated) {
            cout<<"Authentication is required for accessing the system"<<endl;
            string username, password;
            cout << "Enter Username: ";
            cin >> username;
            cout << "Enter Password: ";
            cin >> password;

            if (contactManager.authenticateUser(username, password)) {
                authenticated = true;
                cout << "Authentication successful!\n";
            } else {
                cout << "Authentication failed. Please try again.\n";
            }

            continue; // Restart the loop to get input after authentication
        }
        
        int choice;
        cout<<"Enter the choice:"<<endl;
        cin >> choice;

        if (choice == 8) {
            break;
        }

        switch (choice) {
            case 1: {
                string name, phoneNumber, email, address;
                cout << "Enter Name: ";
                cin.ignore(); // Clear the newline character from the buffer
                getline(cin, name);
                
                // Validate phone number
                do {
                    cout << "Enter Phone Number: ";
                    getline(std::cin, phoneNumber);
                } while (!contactManager.isValidPhoneNumber(phoneNumber)); // Define the isValidPhoneNumber function
                
                // Validate email
                do {
                    cout << "Enter Email: ";
                    getline(std::cin, email);
                } while (!contactManager.isValidEmail(email)); // Define the isValidEmail function
                
                cout << "Enter Address: ";
                getline(std::cin, address);

                Contact newContact(name, phoneNumber, email, address);
                contactManager.addContact(newContact);
                break;
                
            }
            case 2: {
                string name;
                cout << "Enter Name of Contact to Update: ";
                cin.ignore();
                getline(std::cin, name);

                auto it = contactManager.findContact(name);
                if (it != contactManager.contacts.end()) {
                    string phoneNumber, email, address;
                    do {
                        cout << "Enter Phone Number: ";
                        getline(std::cin, phoneNumber);
                    } while (!contactManager.isValidPhoneNumber(phoneNumber)); // Define the isValidPhoneNumber function
                    
                    // Validate email
                    do {
                        cout << "Enter Email: ";
                        getline(std::cin, email);
                    } while (!contactManager.isValidEmail(email)); // Define the isValidEmail function
                
                    cout << "Enter New Address: ";
                    getline(std::cin, address);

                    Contact newContact(name, phoneNumber, email, address);
                    contactManager.updateContact(name, newContact);
                } else {
                    cout << "Contact not found.\n";
                }
                break;
            }
            case 3: {
                string name;
                cout << "Enter Name of Contact to Delete: ";
                cin.ignore();
                getline(cin, name);

                contactManager.deleteContact(name);
                break;
            }
            case 4: {
                string name;
                cout << "Enter Name of Contact to Search: ";
                cin.ignore();
                getline(cin, name);

                contactManager.searchContact(name);
                break;
            }
            case 5: {
                contactManager.displayAllContacts();
                break;
            }
            case 6:
            {
                 contactManager.saveContactsToFile("contacts.txt");
                 std::cout << "Contacts saved to file.\n";
                 break;
            }
            case 7:
            {
                contactManager.loadContactsFromFile("contacts.txt");
                cout << "Contacts loaded from file.\n";
                break;
            }
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
                break;
        }
    }
    return 0;
}



