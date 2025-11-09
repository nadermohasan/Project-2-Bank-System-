#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <cctype> // For toupper
using namespace std;

// Global variables (consider passing as arguments for better practice)
const string FILENAME = "Clients.txt";

// Function prototypes
void MainMenu();
void TransactionsMenu();

// Enums for menu options
enum enMenue {
    ClientList = 1, AddNewClient, DeleteClient, UpdateClient, FindClient, Transactions, Exit
};
enum enTransactions {
    Deposit = 1, Withdraw, TotalBalances, MainMenue
};

// Client data structure
struct sClients {
    string AccountNumber, PINCode, Name, Phone;
    double AccountBalance = 0.0; // Changed to double for better precision
    bool MarkForDelete = false;
};

// Utility function to split a string
vector<string> Split(string text, string delim = "|") {
    vector<string> vString;
    size_t pos = 0;
    string word;
    while ((pos = text.find(delim)) != string::npos) {
        word = text.substr(0, pos);
        vString.push_back(word);
        text.erase(0, pos + delim.length());
    }
    if (!text.empty())
        vString.push_back(text);
    return vString;
}

// Function to convert a line of text to a client struct
sClients ConvertLineToRecord(const string& Line) {
    vector<string> vString = Split(Line);
    sClients Client;
    if (vString.size() == 5) {
        Client.AccountNumber = vString[0];
        Client.PINCode = vString[1];
        Client.Name = vString[2];
        Client.Phone = vString[3];
        try {
            Client.AccountBalance = stod(vString[4]); // Use stod for double
        }
        catch (...) {
            Client.AccountBalance = 0.0;
        }
    }
    return Client;
}

// Function to load clients from file
vector<sClients> LoadClientsFromFile(const string& filename) {
    fstream File(filename, ios::in);
    vector<sClients> vClients;
    string Line;
    if (File.is_open()) {
        while (getline(File, Line)) {
            // Skip empty lines
            if (Line.empty()) continue;
            vClients.push_back(ConvertLineToRecord(Line));
        }
        File.close();
    }
    return vClients;
}

// Function to display client details
void ClientCard(const sClients& Client) { // Pass by const reference
    cout << "\nThe following are the client details: \n";
    cout << "===================================================\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "PINCode        : " << Client.PINCode << endl;
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
    cout << "===================================================\n";
}

// Function to display all clients
void ShowClientList() {
    vector<sClients> vClients = LoadClientsFromFile(FILENAME);
    if (vClients.empty()) {
        cout << "\nNo clients found.\n";
        return;
    }
    cout << "\n\n";
    cout << setw(23) << "" << "Client List (" << vClients.size() << " Client(s))" << endl;
    cout << "---------------------------------------------------------------------\n";
    cout << "| " << setw(16) << "Account Number"
        << "| " << setw(23) << "Client Name"
        << "| " << setw(13) << "Balance"
        << " |\n";
    cout << "---------------------------------------------------------------------\n";
    for (const sClients& C : vClients) { // Use const reference
        cout << "| " << setw(16) << C.AccountNumber
            << "| " << setw(23) << C.Name
            << "| " << setw(13) << C.AccountBalance
            << " |\n";
    }
    cout << "---------------------------------------------------------------------\n";
}

// Function to read new client data
sClients ReadClient() {
    sClients Client;
    cout << "\nAdding New Client: \n";
    cout << "Account Number: ";
    // No need to worry about buffer issues with just getline
    getline(cin, Client.AccountNumber);
    cout << "PINCode: ";
    getline(cin, Client.PINCode);
    cout << "Name: ";
    getline(cin, Client.Name);
    cout << "Phone: ";
    getline(cin, Client.Phone);
    cout << "Account Balance: ";
    cin >> Client.AccountBalance;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return Client;
}

// Function to read and validate an account number
string ReadAccountNumber() {
    string AccountNumber;
    cout << "\nPlease, Enter Account Number: ";
    getline(cin, AccountNumber);
    return AccountNumber;
}

// Function to convert a client record to a file line
string ConvertRecordToLine(const sClients& Client, string delim = "|") {
    string Line = "";
    Line += Client.AccountNumber + delim;
    Line += Client.PINCode + delim;
    Line += Client.Name + delim;
    Line += Client.Phone + delim;
    Line += to_string(Client.AccountBalance);
    return Line;
}

// Function to save the vector of clients to a file
void SaveClientsToFile(const string& filename, const vector<sClients>& vClients) {
    fstream File(filename, ios::out);
    if (File.is_open()) {
        for (const sClients& C : vClients) {
            // Don't save clients marked for deletion
            if (!C.MarkForDelete) {
                File << ConvertRecordToLine(C) << endl;
            }
        }
        File.close();
    }
}

// Function to add a new client
void AddClient() {
    char Answer;
    vector<sClients> vClients = LoadClientsFromFile(FILENAME); // Load existing clients
    do {
        sClients Client = ReadClient();
        vClients.push_back(Client);
        cout << "Client added successfully! Do you want to add more clients (Y/N)? ";
        cin >> Answer;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (toupper(Answer) == 'Y');
    SaveClientsToFile(FILENAME, vClients); // Save all changes at once
}

// Function to find a client by account number
bool FindClientByAccountNumber(const string& filename, const string& AccountNumber, sClients& Client) {
    vector<sClients> vClients = LoadClientsFromFile(filename);
    for (const sClients& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            Client = C;
            return true;
        }
    }
    return false;
}

// Wrapper function to find and display a client
void FindClientByAccountNumber() {
    string AccountNumber = ReadAccountNumber();
    sClients Client;
    if (FindClientByAccountNumber(FILENAME, AccountNumber, Client)) {
        ClientCard(Client);
    }
    else {
        cout << "\nClient with Account Number [" << AccountNumber << "] not found.\n";
    }
}

// Function to delete a client
bool DeleteClientByAccountNumber() {
    string AccountNumber = ReadAccountNumber();
    vector<sClients> vClients = LoadClientsFromFile(FILENAME);
    char Answer;
    bool clientFound = false;
    for (sClients& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            clientFound = true;
            ClientCard(C);
            cout << "\nAre you sure you want to delete the client? (Y/N): ";
            cin >> Answer;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (toupper(Answer) == 'Y') {
                C.MarkForDelete = true;
                SaveClientsToFile(FILENAME, vClients);
                cout << "\nClient Deleted Successfully!\n";
                return true;
            }
            return false; // User chose not to delete
        }
    }
    if (!clientFound) {
        cout << "\nClient with Account Number [" << AccountNumber << "] not found.\n";
    }
    return false;
}

// Function to update a client's information
bool UpdateClientByAccountNumber() {
    string AccountNumber = ReadAccountNumber();
    vector<sClients> vClients = LoadClientsFromFile(FILENAME);
    bool clientFound = false;
    for (sClients& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            clientFound = true;
            ClientCard(C);
            char Answer;
            cout << "\nAre you sure you want to update the client info? (Y/N): ";
            cin >> Answer;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (toupper(Answer) == 'Y') {
                sClients updatedClient = ReadClient();
                updatedClient.AccountNumber = AccountNumber; // Keep original account number
                C = updatedClient;
                SaveClientsToFile(FILENAME, vClients);
                cout << "\nClient Updated Successfully!\n";
                return true;
            }
        }
    }
    if (!clientFound) {
        cout << "\nClient with Account Number [" << AccountNumber << "] not found.\n";
    }
    return false;
}

// Function to handle deposit amount input
double ReadPositiveDouble(const string& message) {
    double amount;
    cout << message;
    while (!(cin >> amount) || amount <= 0) {
        cout << "Invalid amount. Please enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return amount;
}

// Function to handle deposit logic
bool DepositBalance() {
    cout << "===================================================\n";
    cout << setw(32) << "Deposit Screen\n";
    cout << "===================================================\n";
    string AccountNumber = ReadAccountNumber();
    vector<sClients> vClients = LoadClientsFromFile(FILENAME);
    for (sClients& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            ClientCard(C);
            double depositAmount = ReadPositiveDouble("Enter Deposit Amount: ");
            char Answer;
            cout << "\nAre you sure you want to perform this action? (Y/N): ";
            cin >> Answer;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (toupper(Answer) == 'Y') {
                C.AccountBalance += depositAmount;
                SaveClientsToFile(FILENAME, vClients);
                cout << "\nBalance added successfully!\n";
                ClientCard(C); // Show updated balance
                return true;
            }
            else {
                return false;
            }
        }
    }
    cout << "\nClient with Account Number [" << AccountNumber << "] not found.\n";
    return false;
}

// Function to handle withdraw logic
bool WithdrawBalance() {
    cout << "===================================================\n";
    cout << setw(35) << "Withdraw Screen\n";
    cout << "===================================================\n";
    string AccountNumber = ReadAccountNumber();
    vector<sClients> vClients = LoadClientsFromFile(FILENAME);
    for (sClients& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            ClientCard(C);
            double withdrawAmount;
            do {
                withdrawAmount = ReadPositiveDouble("Enter Withdraw Amount: ");
                if (C.AccountBalance < withdrawAmount) {
                    cout << "Amount exceeds the balance. You can withdraw up to: " << C.AccountBalance << endl;
                }
            } while (withdrawAmount > C.AccountBalance);
            char Answer;
            cout << "\nAre you sure you want to perform this action? (Y/N): ";
            cin >> Answer;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (toupper(Answer) == 'Y') {
                C.AccountBalance -= withdrawAmount;
                SaveClientsToFile(FILENAME, vClients);
                cout << "\nWithdrawal successful!\n";
                ClientCard(C); // Show updated balance
                return true;
            }
            else {
                return false;
            }
        }
    }
    cout << "\nClient with Account Number [" << AccountNumber << "] not found.\n";
    return false;
}

// Function to display total clients balances
void TotalClientsBalances() {
    vector<sClients> vClients = LoadClientsFromFile(FILENAME);
    double totalBalances = 0.0;
    int clientsNumber = vClients.size();
    cout << "\n\n";
    cout << setw(23) << "" << "Balance List (" << clientsNumber << " Client(s))" << endl;
    cout << "---------------------------------------------------------------------\n";
    cout << "| " << setw(16) << "Account Number"
        << "| " << setw(23) << "Client Name"
        << "| " << setw(13) << "Balance"
        << " |\n";
    cout << "---------------------------------------------------------------------\n";
    for (const sClients& C : vClients) {
        cout << "| " << setw(16) << C.AccountNumber
            << "| " << setw(23) << C.Name
            << "| " << setw(13) << fixed << setprecision(2) << C.AccountBalance
            << " |\n";
        totalBalances += C.AccountBalance;
    }
    cout << "---------------------------------------------------------------------\n";
    cout << setw(41) << "Total Balances = " << totalBalances << endl;
}

// Pause and clear screen
void PressAnyKey() {
    cout << "\nPress Enter to go back to the Menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    system("cls");
}

// Main Menu handler
void HandleMainMenueOptions(enMenue Choice) {
    switch (Choice) {
    case enMenue::ClientList:
        system("cls");
        ShowClientList();
        PressAnyKey();
        MainMenu();
        break;
    case enMenue::AddNewClient:
        system("cls");
        AddClient();
        PressAnyKey();
        MainMenu();
        break;
    case enMenue::DeleteClient:
        system("cls");
        DeleteClientByAccountNumber();
        PressAnyKey();
        MainMenu();
        break;
    case enMenue::UpdateClient:
        system("cls");
        UpdateClientByAccountNumber();
        PressAnyKey();
        MainMenu();
        break;
    case enMenue::FindClient:
        system("cls");
        FindClientByAccountNumber();
        PressAnyKey();
        MainMenu();
        break;
    case enMenue::Transactions:
        system("cls");
        TransactionsMenu();
        break;
    case enMenue::Exit:
        system("cls");
        cout << "Exiting program. Goodbye!\n";
        break;
    }
}

// Transactions Menu handler
void HandleTransactionsMenuOptions(enTransactions Choice) {
    switch (Choice) {
    case enTransactions::Deposit:
        system("cls");
        DepositBalance();
        PressAnyKey();
        TransactionsMenu();
        break;
    case enTransactions::Withdraw:
        system("cls");
        WithdrawBalance();
        PressAnyKey();
        TransactionsMenu();
        break;
    case enTransactions::TotalBalances:
        system("cls");
        TotalClientsBalances();
        PressAnyKey();
        TransactionsMenu();
        break;
    case enTransactions::MainMenue:
        system("cls");
        MainMenu();
        break;
    }
}

// Main Menu display
void MainMenu() {
    int Answer;
    cout << "===================================================\n";
    cout << setw(30) << "Main Menu Screen\n";
    cout << "===================================================\n";
    cout << setw(15) << "[1] " << "Show Client List.\n";
    cout << setw(15) << "[2] " << "Add New Client.\n";
    cout << setw(15) << "[3] " << "Delete Client.\n";
    cout << setw(15) << "[4] " << "Update Client Info.\n";
    cout << setw(15) << "[5] " << "Find Client.\n";
    cout << setw(15) << "[6] " << "Transactions.\n";
    cout << setw(15) << "[7] " << "Exit.\n";
    cout << "===================================================\n";
    cout << "Choose what you want to do? [1 to 7] ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer

    // Validate user input
    if (Answer < 1 || Answer > 7) {
        cout << "\nInvalid choice. Please enter a number between 1 and 7.\n";
        PressAnyKey();
        MainMenu();
    }
    else {
        HandleMainMenueOptions(static_cast<enMenue>(Answer));
    }
}

// Transactions Menu display
void TransactionsMenu() {
    int Answer;
    cout << "===================================================\n";
    cout << setw(40) << "Transactions Menu Screen\n";
    cout << "===================================================\n";
    cout << setw(15) << "[1] " << "Deposit.\n";
    cout << setw(15) << "[2] " << "Withdraw.\n";
    cout << setw(15) << "[3] " << "Total Balances.\n";
    cout << setw(15) << "[4] " << "Main Menu.\n";
    cout << "===================================================\n";
    cout << "Choose what do you want to do? [1 to 4] ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer

    // Validate user input
    if (Answer < 1 || Answer > 4) {
        cout << "\nInvalid choice. Please enter a number between 1 and 4.\n";
        PressAnyKey();
        TransactionsMenu();
    }
    else {
        HandleTransactionsMenuOptions(static_cast<enTransactions>(Answer));
    }
}

int main() {
    MainMenu();
    return 0;
}