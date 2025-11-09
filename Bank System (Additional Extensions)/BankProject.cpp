#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <cctype>
using namespace std;

// Global variables (consider passing as arguments for better practice)
const string FILENAME = "Clients.txt";
const string USERSFILE = "Users.txt";
const int userPermissions = -1;

void MainMenu();
void TransactionsMenu();
void ManageUsersMenu();
enum enMenue {
    ClientList = 1, AddNewClient, DeleteClient, UpdateClient, FindClient, Transactions, ManageUsers, Logout
};
enum enTransactions {
    Deposit = 1, Withdraw, TotalBalances, MainMenue
};
enum enManageUsersMenu {
    ListUsers = 1, AddNewUser, DeleteUser, UpdateUser, FindUser
};
enum enPermissions {
    enShowClientPermession = 1,     // 0000001
    enAddClientPermission  = 2,     // 0000010
    enDeleteClientPermission = 4,   // 0000100
    enUpdateClientPermission = 8,   // 0001000
    enFindClientPermission = 16,    // 0010000
    enTransactionsPermission = 32,  // 0100000
    enManageUsersPermission = 64,   // 1000000
    enFullAccess = -1               // 1111111
};
struct sClients {
    string AccountNumber, PINCode, Name, Phone;
    double AccountBalance = 0.0; // Changed to double for better precision
    bool MarkForDelete = false;
};
struct sUsers {
    string username, password;
    int userPermissions = 0;
    bool MarkForDelete = false;
};
sUsers CurrentUser;
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
sUsers ConvertLineToUserRecord(const string Line) {
    vector<string> vString = Split(Line);
    sUsers User;
    if (vString.size() == 3) {
        User.username = vString[0];
        User.password = vString[1];
        User.userPermissions = stoi(vString[2]);
    }
    return User;
}
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
vector<sUsers> LoadUsersFromFile(const string filename) {
    vector<sUsers> vUsers;
    fstream File;
    string Line;
    File.open(filename, ios::in);
    if (File.is_open()) {
        while (getline(File, Line)) {
            vUsers.push_back(ConvertLineToUserRecord(Line));
        }
        File.close();
    }
    return vUsers;
}
void ClientCard(const sClients& Client) { 
    cout << "\nThe following are the client details: \n";
    cout << "===================================================\n";
    cout << "Account Number : " << Client.AccountNumber << endl;
    cout << "PINCode        : " << Client.PINCode << endl;
    cout << "Name           : " << Client.Name << endl;
    cout << "Phone          : " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
    cout << "===================================================\n";
}
void UserCard(const sUsers& User) {
    cout << "\nThe following are the user details: \n";
    cout << "===================================================\n";
    cout << "Username    : " << User.username << endl;
    cout << "Password    : " << User.password << endl;
    cout << "Permissions : " << User.userPermissions << endl;
    cout << "===================================================\n";
}
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
void ShowUsersList() {
    vector<sUsers> vUsers = LoadUsersFromFile(USERSFILE);
    if (vUsers.empty()) {
        cout << "\nNo users found.\n";
        return;
    }
    cout << "\n\n";
    cout << setw(23) << "" << "Users List (" << vUsers.size() << " Users(s))" << endl;
    cout << "---------------------------------------------------------------------\n";
    cout << "| " << setw(16) << "Username"
        << "| " << setw(23) << "Password"
        << "| " << setw(13) << "Permession"
        << " |\n";
    cout << "---------------------------------------------------------------------\n";
    for (const sUsers& U : vUsers) { // Use const reference
        cout << "| " << setw(16) << U.username
            << "| " << setw(23) << U.password
            << "| " << setw(13) << U.userPermissions
            << " |\n";
    }
    cout << "---------------------------------------------------------------------\n";
}
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
int ReadPermissions() {
    int Permissions = 0;
    char Answer;

    cout << "Do you want to give full access ? (Y/N): ";
    cin >> Answer;
    if (toupper(Answer) == 'Y') {
        return -1; // Full access, exit directly
    }

    cout << "Show Client List? (Y/N): ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions |= enShowClientPermession;

    cout << "Add New Client? (Y/N): ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions |= enAddClientPermission;

    cout << "Delete Client? (Y/N): ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions |= enDeleteClientPermission;

    cout << "Update Client? (Y/N): ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions |= enUpdateClientPermission;

    cout << "Find Client? (Y/N): ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
        Permissions |= enFindClientPermission;

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //  ‰ŸÌ› «·»›— „—Â Ê«Õœ…
    return Permissions;
}
sUsers ReadUser() {
    sUsers User;

    cout << "\nAdding/Updating User: \n";
    cout << "Username: ";
    getline(cin, User.username);

    cout << "Password: ";
    getline(cin, User.password);

    User.userPermissions = ReadPermissions();
    return User;
}
string ReadAccountNumber() {
    string AccountNumber;
    cout << "\nPlease, Enter Account Number: ";
    getline(cin, AccountNumber);
    return AccountNumber;
}
string ReadUsername() {
    string username;
    cout << "Enter the username: ";
    cin >> username;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return username;
}
string ConvertRecordToLine(const sClients& Client, string delim = "|") {
    string Line = "";
    Line += Client.AccountNumber + delim;
    Line += Client.PINCode + delim;
    Line += Client.Name + delim;
    Line += Client.Phone + delim;
    Line += to_string(Client.AccountBalance);
    return Line;
}
string ConvertRecordToLine(sUsers User, string delim = "|") {
    string Line = "";
    Line += User.username + delim;
    Line += User.password + delim;
    Line += to_string(User.userPermissions);
    return Line;
}
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
void SaveUsersToFile(string filename, vector<sUsers>& vUsers) {
    fstream File;
    File.open(filename, ios::out);
    if (File.is_open()) {
        for (sUsers& U : vUsers) {
            File << ConvertRecordToLine(U) << endl;
        }
        File.close();
    }
}
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
void AddUser() {
    char Answer;
    vector<sUsers> vUsers = LoadUsersFromFile(USERSFILE);
    do {
        sUsers User = ReadUser();
        vUsers.push_back(User);
        cout << "User added successfully, Do you want to add more users (Y/N)? ";
        cin >> Answer;
    } while (toupper(Answer) == 'Y');
    SaveUsersToFile(USERSFILE, vUsers);
}
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
            }
        }
        if (C.MarkForDelete != true) {
            SaveClientsToFile(FILENAME, vClients);
            cout << "\nClient Deleted Successfully!\n";
            return true;
        }
    }
    if (!clientFound) {
        cout << "\nClient with Account Number [" << AccountNumber << "] not found.\n";
    }
    return false;
}
bool DeleteUserByUsername() {
    string Username = ReadUsername();
    vector<sUsers> vUsers = LoadUsersFromFile(USERSFILE);
    vector<sUsers> vUsersAfterDelete;
    char Answer;

    // MarkForDelete Loop
    for (sUsers& U : vUsers) {
        if (U.username == Username) {
            cout << "\nAre you sure you want to delete the user? (Y/N): ";
            cin >> Answer;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (toupper(Answer) == 'Y') {
                U.MarkForDelete = true;
            }
            else {
                system("cls");
                ManageUsersMenu();
            }
            break; // stop after finding the username
        }
        else {
            cout << "\nUser not found.\n";
            return false;
        }
    }

    // Save Only Unmarked Users
    for (sUsers& U : vUsers) {
        if (!U.MarkForDelete) {
            vUsersAfterDelete.push_back(U);
        }
    }
    SaveUsersToFile(USERSFILE, vUsersAfterDelete);
    cout << "\nUser Deleted Successfully!\n";
    return true;
}
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
bool UpdateUserByUsername() {
    string Username = ReadUsername();
    vector<sUsers> vUsers = LoadUsersFromFile(USERSFILE);

    for (sUsers& U : vUsers) {
        if (U.username == Username) {
            UserCard(U);

            char Answer;
            cout << "\nAre you sure you want to update this user? (Y/N): ";
            cin >> Answer;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (toupper(Answer) == 'Y') {
                U = ReadUser();
                SaveUsersToFile(USERSFILE, vUsers);
                cout << "\nUser updated successfully.\n";
                return true;
            }
            break;
        }
    }

    cout << "\nUser not found.\n";
    return false;
}
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
int CheckPermission(int userPermissions) {
    if (userPermissions == -1) {
        return -1;
    }
    return userPermissions;
}
bool FindUserByUsername(string username, string password, sUsers &CurrentUser) {
    vector<sUsers> vUsers = LoadUsersFromFile(USERSFILE);

    for (sUsers& U : vUsers) {
        if (U.username == username && U.password == password) {
            CurrentUser = U;
            system("cls");
            MainMenu();
            return true;
        }
    }
    return false;
}
bool FindUserByUsername() {
    string Username = ReadUsername();
    vector<sUsers> vUsers = LoadUsersFromFile(USERSFILE);
    for (sUsers& U : vUsers) {
        if (U.username == Username) {
            UserCard(U);
            return true;
        }
    }
}
bool Login() {
    string username, password;

    cout << "===================================================\n";
    cout << setw(30) << "Login Screen\n";
    cout << "===================================================\n";

    do {
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        if (FindUserByUsername(username, password, CurrentUser)) {
            return true;
        }
        else {
            cout << "Invalid Username or password." << endl;
        }
    } while (!FindUserByUsername(username, password, CurrentUser));
    return false;
}
void PressAnyKey() {
    cout << "\nPress Enter to go back to the Menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    system("cls");
    MainMenu();
}
void AccessDenied() {
    system("cls");
    cout << "-------------------------------------" << endl;
    cout << "Access Denied, " << endl;
    cout << "You don't Have Permissions to do this, " << endl;
    cout << "Please, Contact Your Admin." << endl;
    cout << "-------------------------------------" << endl;
    PressAnyKey();
}
void HandleMainMenueOptions(enMenue Choice) {
    switch (Choice) {
    case enMenue::ClientList:
        if (CheckPermission(CurrentUser.userPermissions) & (int)enShowClientPermession) {
            system("cls");
            ShowClientList();
            PressAnyKey();
        }
        else {
            AccessDenied();
        }
        break;

    case enMenue::AddNewClient:
        if (CheckPermission(CurrentUser.userPermissions) & (int)enAddClientPermission) {
            system("cls");
            AddClient();
            PressAnyKey();
        }
        else {
            AccessDenied();
        }
        break;

    case enMenue::DeleteClient:
        if (CheckPermission(CurrentUser.userPermissions) & (int)enDeleteClientPermission) {
            system("cls");
            DeleteClientByAccountNumber();
            PressAnyKey();
        }
        else {
            AccessDenied();
        }
        break;

    case enMenue::UpdateClient:
        if (CheckPermission(CurrentUser.userPermissions) & (int)enUpdateClientPermission) {
            system("cls");
            UpdateClientByAccountNumber();
            PressAnyKey();
        }
        else {
            AccessDenied();
        }
        break;

    case enMenue::FindClient:
        if (CheckPermission(CurrentUser.userPermissions) & (int)enFindClientPermission) {
            system("cls");
            FindClientByAccountNumber();
            PressAnyKey();
        }
        else {
            AccessDenied();
        }
        break;

    case enMenue::Transactions:
        if (CheckPermission(CurrentUser.userPermissions) & (int)enTransactionsPermission) {
            system("cls");
            TransactionsMenu();
        }
        else {
            AccessDenied();
        }
        break;

    case enMenue::ManageUsers:
        if (CheckPermission(CurrentUser.userPermissions) & (int)enManageUsersPermission) {
            system("cls");
            ManageUsersMenu();
        }
        else {
            AccessDenied();
        }
        break;

    case enMenue::Logout:
        system("cls");
        break;

    default:
        AccessDenied();
        break;
    }
}
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
void HandleManageUsersMenuOptions(enManageUsersMenu Choice) {
    switch (Choice) {
    case enManageUsersMenu::ListUsers:
        system("cls");
        ShowUsersList();
        PressAnyKey();
        break;
    case enManageUsersMenu::AddNewUser:
        system("cls");
        AddUser();
        PressAnyKey();
        break;
    case enManageUsersMenu::DeleteUser:
        system("cls");
        DeleteUserByUsername();
        PressAnyKey();
        break;
    case enManageUsersMenu::UpdateUser:
        system("cls");
        if (!UpdateUserByUsername()) {
            cout << "Client Not Found!";
        }
        PressAnyKey();
        break;
    case enManageUsersMenu::FindUser:
        system("cls");
        FindUserByUsername();
        PressAnyKey();
        break;
    default:
        system("cls");
        MainMenu();
        break;
    }
}
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
        cout << setw(15) << "[7] " << "Manage Users.\n";
        cout << setw(15) << "[8] " << "Logout.\n";
        cout << "===================================================\n";
        cout << "Choose what you want to do? [1 to 8]? ";
        cin >> Answer;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer

        // Validate user input
        if (Answer < 1 || Answer > 8) {
            cout << "\nInvalid choice. Please enter a number between 1 and 7.\n";
            PressAnyKey();
            MainMenu();
        }
        else {
            HandleMainMenueOptions(static_cast<enMenue>(Answer));
        }
}
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
void ManageUsersMenu() {
    int Answer;
    cout << "===================================================\n";
    cout << setw(40) << "Manage Users Menue\n";
    cout << "===================================================\n";
    cout << setw(15) << "[1] " << "List Users.\n";
    cout << setw(15) << "[2] " << "Add New User.\n";
    cout << setw(15) << "[3] " << "Delete User.\n";
    cout << setw(15) << "[4] " << "Update User.\n";
    cout << setw(15) << "[5] " << "Find User.\n";
    cout << setw(15) << "[6] " << "Main Menu.\n";
    cout << "===================================================\n";
    cout << "Choose what do you want to do? [1 to 6] ? ";
    cin >> Answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear input buffer

    // Validate user input
    if (Answer < 1 || Answer > 6) {
        cout << "\nInvalid choice. Please enter a number between 1 and 6.\n";
        PressAnyKey();
        MainMenu();
    }
    else {
        HandleManageUsersMenuOptions(static_cast<enManageUsersMenu>(Answer));
    }
}
void Start() {
    if (Login()) {
        system("cls");
        MainMenu();
    }
}
int main() {
    Start();
    return 0;
}