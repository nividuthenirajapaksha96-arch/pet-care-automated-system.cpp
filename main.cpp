#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <limits>
#include <cstdio>

using namespace std;

// Database File Definitions
const string FILE_USERS = "user_accounts.txt";
const string FILE_OWNERS = "owner_records.txt";
const string FILE_PETS = "pet_records.txt";
const string FILE_APPOINTMENTS = "appointment_records.txt";
const string FILE_TEMP = "temp_data.txt";

// Data Structures
struct SystemUser
{
    int id;
    string uname;
    string pass;
    string userRole;
};

struct PetOwner
{
    int ownerId;
    string fname;
    string lname;
    string contactNo;
    string homeAddress;
    string regDate;
};

struct PetDetail
{
    int petId;
    int ownerId;
    string name;
    string category; // Pet Type
    string breed;
    int ageYears;
    string gender;
    string medicalNotes;
};

struct ClinicAppointment
{
    int appNo;
    int petId;
    string date;
    string service;
    string reason;
    string notes;
    string status;
    string modifiedDate;
};

// Function Declarations
void purgeBuffer();
string toLowerCase(string str);

void printOwnerCard(const PetOwner& o);
void printPetCard(const PetDetail& p);
void printAppointmentCard(const ClinicAppointment& a);

void setupDefaultAccounts();
bool authenticateUser(SystemUser& currentUser);

int getNewOwnerID();
int getNewPetID();
int getNewAppointmentNo();

void registerOwner();
void modifyOwner();
void findOwnerById();
void listAllOwners();

void registerPet();
void modifyPet();
void findPetById();
void listAllPets();
void listPetsByOwner();

void scheduleAppointment();
void modifyAppointment();
void searchAppointments();
void listAllAppointments();

void runAdminPanel();
void runReceptionistPanel();
void runVetPanel();

// Input Clearing Helper
void purgeBuffer()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string toLowerCase(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Display Functions
void printOwnerCard(const PetOwner& o)
{
    cout << "  Owner ID      : " << o.ownerId << endl;
    cout << "  Name          : " << o.fname << " " << o.lname << endl;
    cout << "  Contact No    : " << o.contactNo << endl;
    cout << "  Address       : " << o.homeAddress << endl;
    cout << "  Registered On : " << o.regDate << endl;
}

void printPetCard(const PetDetail& p)
{
    cout << "  Pet ID        : " << p.petId << endl;
    cout << "  Owner ID      : " << p.ownerId << endl;
    cout << "  Pet Name      : " << p.name << endl;
    cout << "  Type/Species  : " << p.category << endl;
    cout << "  Breed         : " << p.breed << endl;
    cout << "  Age (Years)   : " << p.ageYears << endl;
    cout << "  Gender        : " << p.gender << endl;
    cout << "  Special Notes : " << p.medicalNotes << endl;
}

void printAppointmentCard(const ClinicAppointment& a)
{
    cout << "  App Ref No    : " << a.appNo << endl;
    cout << "  Pet ID        : " << a.petId << endl;
    cout << "  App Date      : " << a.date << endl;
    cout << "  Service Type  : " << a.service << endl;
    cout << "  Reason/Symp   : " << a.reason << endl;
    cout << "  Clinical Notes: " << a.notes << endl;
    cout << "  Status        : " << a.status << endl;
    cout << "  Last Updated  : " << a.modifiedDate << endl;
}

// File Initialization
void setupDefaultAccounts()
{
    ifstream checkFile(FILE_USERS);
    if (checkFile)
    {
        checkFile.close();
        return;
    }
    checkFile.close();

    ofstream outFile(FILE_USERS);
    if (!outFile)
    {
        cout << "[Error] Failed to create system user file." << endl;
        return;
    }

    outFile << "1,admin,123,Administrator" << endl;
    outFile << "2,reception,456,Receptionist" << endl;
    outFile << "3,vet,789,Veterinary Staff" << endl;
    outFile.close();
}

// Authentication
bool authenticateUser(SystemUser& currentUser)
{
    string inputUser, inputPass, recordLine;
    bool isAuth = false;

    cout << "\n==============================================" << endl;
    cout << "          PETCARE CLINIC LOGIN SYSTEM" << endl;
    cout << "==============================================" << endl;

    cout << " Username : ";
    cin >> inputUser;

    cout << " Password : ";
    cin >> inputPass;

    ifstream inFile(FILE_USERS);
    if (!inFile)
    {
        cout << "\n[Error] Unable to access authentication data." << endl;
        return false;
    }

    while (getline(inFile, recordLine))
    {
        stringstream ss(recordLine);
        string strId;

        getline(ss, strId, ',');
        getline(ss, currentUser.uname, ',');
        getline(ss, currentUser.pass, ',');
        getline(ss, currentUser.userRole);

        if (strId.empty()) continue;

        currentUser.id = stoi(strId);

        if (currentUser.uname == inputUser && currentUser.pass == inputPass)
        {
            isAuth = true;
            break;
        }
    }
    inFile.close();

    if (isAuth)
    {
        cout << "\n>>> Access Granted! Welcome, " << currentUser.uname << " [" << currentUser.userRole << "]" << endl;
        return true;
    }

    cout << "\n[Access Denied] Invalid Credentials entered." << endl;
    return false;
}

// Auto-ID Generators
int getNewOwnerID()
{
    ifstream file(FILE_OWNERS);
    if (!file) return 1001;

    string line;
    int lastId = 1000;
    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        if (!idStr.empty()) lastId = stoi(idStr);
    }
    file.close();
    return lastId + 1;
}

int getNewPetID()
{
    ifstream file(FILE_PETS);
    if (!file) return 2001;

    string line;
    int lastId = 2000;
    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        if (!idStr.empty()) lastId = stoi(idStr);
    }
    file.close();
    return lastId + 1;
}

int getNewAppointmentNo()
{
    ifstream file(FILE_APPOINTMENTS);
    if (!file) return 3001;

    string line;
    int lastId = 3000;
    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        if (!idStr.empty()) lastId = stoi(idStr);
    }
    file.close();
    return lastId + 1;
}

// Owner Management Modules
void registerOwner()
{
    PetOwner owner;
    owner.ownerId = getNewOwnerID();
    purgeBuffer();

    cout << "\n--- REGISTER NEW PET OWNER ---" << endl;
    cout << "Generated Owner ID : " << owner.ownerId << endl;

    cout << "Enter First Name   : ";
    getline(cin, owner.fname);
    cout << "Enter Last Name    : ";
    getline(cin, owner.lname);
    cout << "Enter Contact No   : ";
    getline(cin, owner.contactNo);
    cout << "Enter Address      : ";
    getline(cin, owner.homeAddress);
    cout << "Reg Date (DD/MM/YY): ";
    getline(cin, owner.regDate);

    ofstream outFile(FILE_OWNERS, ios::app);
    if (!outFile)
    {
        cout << "[Error] Failed to open owner records database." << endl;
        return;
    }

    outFile << owner.ownerId << "," << owner.fname << "," << owner.lname << ","
            << owner.contactNo << "," << owner.homeAddress << "," << owner.regDate << endl;
    outFile.close();

    cout << ">>> Owner successfully registered with ID: " << owner.ownerId << endl;
}

void findOwnerById()
{
    int searchId;
    bool found = false;
    string line;

    cout << "\n--- SEARCH OWNER RECORD ---" << endl;
    cout << "Enter Owner ID to search: ";
    cin >> searchId;

    ifstream file(FILE_OWNERS);
    if (!file)
    {
        cout << "[Error] Cannot open owner database file." << endl;
        return;
    }

    while (getline(file, line))
    {
        stringstream ss(line);
        PetOwner owner;
        string idStr;

        getline(ss, idStr, ',');
        getline(ss, owner.fname, ',');
        getline(ss, owner.lname, ',');
        getline(ss, owner.contactNo, ',');
        getline(ss, owner.homeAddress, ',');
        getline(ss, owner.regDate);

        if (idStr.empty()) continue;

        owner.ownerId = stoi(idStr);
        if (owner.ownerId == searchId)
        {
            cout << "\n[Record Found]" << endl;
            printOwnerCard(owner);
            found = true;
            break;
        }
    }
    file.close();

    if (!found)
    {
        cout << "\n[Not Found] No owner record exists for ID: " << searchId << endl;
    }
}

void modifyOwner()
{
    int targetId;
    bool updated = false;
    string line;

    cout << "\n--- UPDATE OWNER RECORD ---" << endl;
    cout << "Enter Owner ID to update: ";
    cin >> targetId;

    ifstream inFile(FILE_OWNERS);
    if (!inFile)
    {
        cout << "[Error] Cannot open owner records." << endl;
        return;
    }

    ofstream tempFile(FILE_TEMP);
    if (!tempFile)
    {
        cout << "[Error] Temporary storage creation failed." << endl;
        inFile.close();
        return;
    }

    while (getline(inFile, line))
    {
        stringstream ss(line);
        PetOwner owner;
        string idStr;

        getline(ss, idStr, ',');
        getline(ss, owner.fname, ',');
        getline(ss, owner.lname, ',');
        getline(ss, owner.contactNo, ',');
        getline(ss, owner.homeAddress, ',');
        getline(ss, owner.regDate);

        if (idStr.empty())
        {
            tempFile << line << endl;
            continue;
        }

        owner.ownerId = stoi(idStr);
        if (owner.ownerId == targetId)
        {
            updated = true;
            purgeBuffer();

            cout << "\nEnter New First Name   : ";
            getline(cin, owner.fname);
            cout << "Enter New Last Name    : ";
            getline(cin, owner.lname);
            cout << "Enter New Contact No   : ";
            getline(cin, owner.contactNo);
            cout << "Enter New Address      : ";
            getline(cin, owner.homeAddress);
            cout << "Enter Reg Date (DD/MM/YY): ";
            getline(cin, owner.regDate);

            tempFile << owner.ownerId << "," << owner.fname << "," << owner.lname << ","
                     << owner.contactNo << "," << owner.homeAddress << "," << owner.regDate << endl;
        }
        else
        {
            tempFile << line << endl;
        }
    }

    inFile.close();
    tempFile.close();

    if (updated)
    {
        remove(FILE_OWNERS.c_str());
        rename(FILE_TEMP.c_str(), FILE_OWNERS.c_str());
        cout << ">>> Owner record updated successfully." << endl;
    }
    else
    {
        remove(FILE_TEMP.c_str());
        cout << "\n[Not Found] Owner ID " << targetId << " not found." << endl;
    }
}

void listAllOwners()
{
    ifstream file(FILE_OWNERS);
    string line;
    bool hasData = false;

    cout << "\n==============================================" << endl;
    cout << "            REGISTERED PET OWNERS" << endl;
    cout << "==============================================" << endl;

    if (!file)
    {
        cout << "[Error] Cannot open owner database." << endl;
        return;
    }

    while (getline(file, line))
    {
        stringstream ss(line);
        PetOwner owner;
        string idStr;

        getline(ss, idStr, ',');
        getline(ss, owner.fname, ',');
        getline(ss, owner.lname, ',');
        getline(ss, owner.contactNo, ',');
        getline(ss, owner.homeAddress, ',');
        getline(ss, owner.regDate);

        if (idStr.empty()) continue;

        owner.ownerId = stoi(idStr);
        cout << "----------------------------------------------" << endl;
        printOwnerCard(owner);
        hasData = true;
    }
    file.close();

    if (!hasData)
    {
        cout << "\nNo owner records available." << endl;
    }
}

// Pet Management Modules
void registerPet()
{
    PetDetail pet;
    pet.petId = getNewPetID();
    int ownerRefId;
    bool ownerValid = false;
    string line;

    cout << "\n--- REGISTER NEW PET ---" << endl;
    cout << "Enter Parent Owner ID: ";
    cin >> ownerRefId;

    ifstream ownerFile(FILE_OWNERS);
    if (!ownerFile)
    {
        cout << "[Error] Cannot open owner file to verify ID." << endl;
        return;
    }

    while (getline(ownerFile, line))
    {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        if (!idStr.empty() && stoi(idStr) == ownerRefId)
        {
            ownerValid = true;
            break;
        }
    }
    ownerFile.close();

    if (!ownerValid)
    {
        cout << "\n[Error] Invalid Owner ID " << ownerRefId << ". Register owner first!" << endl;
        return;
    }

    pet.ownerId = ownerRefId;
    purgeBuffer();

    cout << "Assigned Pet ID     : " << pet.petId << endl;
    cout << "Enter Pet Name      : ";
    getline(cin, pet.name);
    cout << "Enter Species/Type  : ";
    getline(cin, pet.category);
    cout << "Enter Pet Breed     : ";
    getline(cin, pet.breed);
    cout << "Enter Pet Age (Years): ";
    cin >> pet.ageYears;
    purgeBuffer();
    cout << "Enter Gender (M/F)  : ";
    getline(cin, pet.gender);
    cout << "Special Health Notes: ";
    getline(cin, pet.medicalNotes);

    ofstream outFile(FILE_PETS, ios::app);
    if (!outFile)
    {
        cout << "[Error] Unable to access pet file." << endl;
        return;
    }

    outFile << pet.petId << "," << pet.ownerId << "," << pet.name << ","
            << pet.category << "," << pet.breed << "," << pet.ageYears << ","
            << pet.gender << "," << pet.medicalNotes << endl;
    outFile.close();

    cout << ">>> Pet registered successfully with ID: " << pet.petId << endl;
}

void findPetById()
{
    int targetId;
    bool found = false;
    string line;

    cout << "\n--- SEARCH PET RECORD ---" << endl;
    cout << "Enter Pet ID: ";
    cin >> targetId;

    ifstream file(FILE_PETS);
    if (!file)
    {
        cout << "[Error] Cannot open pet database." << endl;
        return;
    }

    while (getline(file, line))
    {
        stringstream ss(line);
        PetDetail pet;
        string pId, oId, ageStr;

        getline(ss, pId, ',');
        getline(ss, oId, ',');
        getline(ss, pet.name, ',');
        getline(ss, pet.category, ',');
        getline(ss, pet.breed, ',');
        getline(ss, ageStr, ',');
        getline(ss, pet.gender, ',');
        getline(ss, pet.medicalNotes);

        if (pId.empty()) continue;

        pet.petId = stoi(pId);
        pet.ownerId = stoi(oId);
        pet.ageYears = stoi(ageStr);

        if (pet.petId == targetId)
        {
            cout << "\n[Record Found]" << endl;
            printPetCard(pet);
            found = true;
            break;
        }
    }
    file.close();

    if (!found)
    {
        cout << "\n[Not Found] Pet ID " << targetId << " not found." << endl;
    }
}

void modifyPet()
{
    int targetId;
    bool updated = false;
    string line;

    cout << "\n--- UPDATE PET DETAILS ---" << endl;
    cout << "Enter Pet ID to modify: ";
    cin >> targetId;

    ifstream inFile(FILE_PETS);
    if (!inFile)
    {
        cout << "[Error] Unable to open pet records." << endl;
        return;
    }

    ofstream tempFile(FILE_TEMP);
    if (!tempFile)
    {
        cout << "[Error] Temp file creation failed." << endl;
        inFile.close();
        return;
    }

    while (getline(inFile, line))
    {
        stringstream ss(line);
        PetDetail pet;
        string pId, oId, ageStr;

        getline(ss, pId, ',');
        getline(ss, oId, ',');
        getline(ss, pet.name, ',');
        getline(ss, pet.category, ',');
        getline(ss, pet.breed, ',');
        getline(ss, ageStr, ',');
        getline(ss, pet.gender, ',');
        getline(ss, pet.medicalNotes);

        if (pId.empty())
        {
            tempFile << line << endl;
            continue;
        }

        pet.petId = stoi(pId);
        pet.ownerId = stoi(oId);
        pet.ageYears = stoi(ageStr);

        if (pet.petId == targetId)
        {
            updated = true;
            purgeBuffer();

            cout << "\nEnter New Pet Name      : ";
            getline(cin, pet.name);
            cout << "Enter New Species/Type  : ";
            getline(cin, pet.category);
            cout << "Enter New Breed         : ";
            getline(cin, pet.breed);
            cout << "Enter New Age           : ";
            cin >> pet.ageYears;
            purgeBuffer();
            cout << "Enter New Gender        : ";
            getline(cin, pet.gender);
            cout << "Enter New Special Notes : ";
            getline(cin, pet.medicalNotes);

            tempFile << pet.petId << "," << pet.ownerId << "," << pet.name << ","
                     << pet.category << "," << pet.breed << "," << pet.ageYears << ","
                     << pet.gender << "," << pet.medicalNotes << endl;
        }
        else
        {
            tempFile << line << endl;
        }
    }

    inFile.close();
    tempFile.close();

    if (updated)
    {
        remove(FILE_PETS.c_str());
        rename(FILE_TEMP.c_str(), FILE_PETS.c_str());
        cout << ">>> Pet details updated successfully." << endl;
    }
    else
    {
        remove(FILE_TEMP.c_str());
        cout << "\n[Not Found] Pet ID " << targetId << " not found." << endl;
    }
}

void listAllPets()
{
    ifstream file(FILE_PETS);
    string line;
    bool recordsExist = false;

    cout << "\n==============================================" << endl;
    cout << "             ALL REGISTERED PETS" << endl;
    cout << "==============================================" << endl;

    if (!file)
    {
        cout << "[Error] Cannot open pet records." << endl;
        return;
    }

    while (getline(file, line))
    {
        stringstream ss(line);
        PetDetail pet;
        string pId, oId, ageStr;

        getline(ss, pId, ',');
        getline(ss, oId, ',');
        getline(ss, pet.name, ',');
        getline(ss, pet.category, ',');
        getline(ss, pet.breed, ',');
        getline(ss, ageStr, ',');
        getline(ss, pet.gender, ',');
        getline(ss, pet.medicalNotes);

        if (pId.empty()) continue;

        pet.petId = stoi(pId);
        pet.ownerId = stoi(oId);
        pet.ageYears = stoi(ageStr);

        cout << "----------------------------------------------" << endl;
        printPetCard(pet);
        recordsExist = true;
    }
    file.close();

    if (!recordsExist)
    {
        cout << "\nNo pet records available." << endl;
    }
}

// Appointment Modules
void scheduleAppointment()
{
    ClinicAppointment app;
    app.appNo = getNewAppointmentNo();
    int targetPetId;
    bool petFound = false;
    string line;

    cout << "\n--- SCHEDULE NEW APPOINTMENT ---" << endl;
    cout << "Enter Pet ID: ";
    cin >> targetPetId;

    ifstream petFile(FILE_PETS);
    if (!petFile)
    {
        cout << "[Error] Cannot access pet file." << endl;
        return;
    }

    while (getline(petFile, line))
    {
        stringstream ss(line);
        string pId;
        getline(ss, pId, ',');
        if (!pId.empty() && stoi(pId) == targetPetId)
        {
            petFound = true;
            break;
        }
    }
    petFile.close();

    if (!petFound)
    {
        cout << "\n[Error] Pet ID " << targetPetId << " not found in system." << endl;
        return;
    }

    app.petId = targetPetId;
    purgeBuffer();

    cout << "Appointment Ref No : " << app.appNo << endl;
    cout << "Date (DD/MM/YYYY)  : ";
    getline(cin, app.date);
    cout << "Service Required   : ";
    getline(cin, app.service);
    cout << "Reason / Symptoms  : ";
    getline(cin, app.reason);
    cout << "Treatment Notes    : ";
    getline(cin, app.notes);
    cout << "Status (Pending/Completed): ";
    getline(cin, app.status);
    cout << "Last Updated Date  : ";
    getline(cin, app.modifiedDate);

    ofstream outFile(FILE_APPOINTMENTS, ios::app);
    if (!outFile)
    {
        cout << "[Error] Appointment database open failed." << endl;
        return;
    }

    outFile << app.appNo << "," << app.petId << "," << app.date << ","
            << app.service << "," << app.reason << "," << app.notes << ","
            << app.status << "," << app.modifiedDate << endl;
    outFile.close();

    cout << ">>> Appointment booked successfully. Ref No: " << app.appNo << endl;
}

void searchAppointments()
{
    int option;
    bool matchFound = false;
    string line;

    cout << "\n--- SEARCH APPOINTMENTS ---" << endl;
    cout << " 1. By Appointment Number" << endl;
    cout << " 2. By Pet ID" << endl;
    cout << " 3. By Owner ID" << endl;
    cout << " 4. By Owner Mobile Number" << endl;
    cout << " Select Search Mode: ";
    cin >> option;

    if (option == 1)
    {
        int searchNo;
        cout << "Enter Appointment Number: ";
        cin >> searchNo;

        ifstream file(FILE_APPOINTMENTS);
        if (!file) return;

        while (getline(file, line))
        {
            stringstream ss(line);
            ClinicAppointment app;
            string aNo, pId;

            getline(ss, aNo, ',');
            getline(ss, pId, ',');
            getline(ss, app.date, ',');
            getline(ss, app.service, ',');
            getline(ss, app.reason, ',');
            getline(ss, app.notes, ',');
            getline(ss, app.status, ',');
            getline(ss, app.modifiedDate);

            if (aNo.empty()) continue;

            app.appNo = stoi(aNo);
            app.petId = stoi(pId);

            if (app.appNo == searchNo)
            {
                cout << "\n----------------------------------------------" << endl;
                printAppointmentCard(app);
                matchFound = true;
                break;
            }
        }
        file.close();
    }
    else if (option == 2)
    {
        int searchPetId;
        cout << "Enter Pet ID: ";
        cin >> searchPetId;

        ifstream file(FILE_APPOINTMENTS);
        if (!file) return;

        while (getline(file, line))
        {
            stringstream ss(line);
            ClinicAppointment app;
            string aNo, pId;

            getline(ss, aNo, ',');
            getline(ss, pId, ',');
            getline(ss, app.date, ',');
            getline(ss, app.service, ',');
            getline(ss, app.reason, ',');
            getline(ss, app.notes, ',');
            getline(ss, app.status, ',');
            getline(ss, app.modifiedDate);

            if (aNo.empty()) continue;

            app.appNo = stoi(aNo);
            app.petId = stoi(pId);

            if (app.petId == searchPetId)
            {
                cout << "\n----------------------------------------------" << endl;
                printAppointmentCard(app);
                matchFound = true;
            }
        }
        file.close();
    }
    else if (option == 3)
    {
        int searchOwnerId;
        cout << "Enter Owner ID: ";
        cin >> searchOwnerId;

        ifstream petFile(FILE_PETS);
        if (!petFile) return;

        cout << "\nPets belonging to Owner ID " << searchOwnerId << ":" << endl;
        while (getline(petFile, line))
        {
            stringstream ss(line);
            string pId, oId;
            getline(ss, pId, ',');
            getline(ss, oId, ',');

            if (!pId.empty() && stoi(oId) == searchOwnerId)
            {
                cout << " -> Pet ID: " << pId << endl;
                matchFound = true;
            }
        }
        petFile.close();
        if (matchFound) cout << "Use the Pet ID above to search specific appointments." << endl;
    }
    else if (option == 4)
    {
        string mobileNo;
        cout << "Enter Mobile Number: ";
        cin >> mobileNo;

        ifstream ownerFile(FILE_OWNERS);
        if (!ownerFile) return;

        while (getline(ownerFile, line))
        {
            stringstream ss(line);
            PetOwner o;
            string oId;

            getline(ss, oId, ',');
            getline(ss, o.fname, ',');
            getline(ss, o.lname, ',');
            getline(ss, o.contactNo, ',');
            getline(ss, o.homeAddress, ',');
            getline(ss, o.regDate);

            if (o.contactNo == mobileNo)
            {
                cout << "\n----------------------------------------------" << endl;
                printOwnerCard(o);
                matchFound = true;
                break;
            }
        }
        ownerFile.close();
    }
    else
    {
        cout << "\n[Error] Invalid Menu Choice." << endl;
        return;
    }

    if (!matchFound)
    {
        cout << "\n[Not Found] No records matched your search." << endl;
    }
}

void modifyAppointment()
{
    int targetNo;
    bool updated = false;
    string line;

    cout << "\n--- UPDATE APPOINTMENT ---" << endl;
    cout << "Enter Appointment Number: ";
    cin >> targetNo;

    ifstream inFile(FILE_APPOINTMENTS);
    if (!inFile)
    {
        cout << "[Error] Cannot open appointment file." << endl;
        return;
    }

    ofstream tempFile(FILE_TEMP);
    if (!tempFile)
    {
        cout << "[Error] Temp file creation error." << endl;
        inFile.close();
        return;
    }

    while (getline(inFile, line))
    {
        stringstream ss(line);
        ClinicAppointment app;
        string aNo, pId;

        getline(ss, aNo, ',');
        getline(ss, pId, ',');
        getline(ss, app.date, ',');
        getline(ss, app.service, ',');
        getline(ss, app.reason, ',');
        getline(ss, app.notes, ',');
        getline(ss, app.status, ',');
        getline(ss, app.modifiedDate);

        if (aNo.empty())
        {
            tempFile << line << endl;
            continue;
        }

        app.appNo = stoi(aNo);
        app.petId = stoi(pId);

        if (app.appNo == targetNo)
        {
            updated = true;
            purgeBuffer();

            cout << "\nEnter New Date (DD/MM/YY): ";
            getline(cin, app.date);
            cout << "Enter New Service Type  : ";
            getline(cin, app.service);
            cout << "Enter New Reason/Symp   : ";
            getline(cin, app.reason);
            cout << "Enter Treatment Notes   : ";
            getline(cin, app.notes);
            cout << "Enter Status (Pending/Completed): ";
            getline(cin, app.status);
            cout << "Last Modified Date      : ";
            getline(cin, app.modifiedDate);

            tempFile << app.appNo << "," << app.petId << "," << app.date << ","
                     << app.service << "," << app.reason << "," << app.notes << ","
                     << app.status << "," << app.modifiedDate << endl;
        }
        else
        {
            tempFile << line << endl;
        }
    }

    inFile.close();
    tempFile.close();

    if (updated)
    {
        remove(FILE_APPOINTMENTS.c_str());
        rename(FILE_TEMP.c_str(), FILE_APPOINTMENTS.c_str());
        cout << ">>> Appointment updated successfully." << endl;
    }
    else
    {
        remove(FILE_TEMP.c_str());
        cout << "\n[Not Found] Appointment Ref No " << targetNo << " not found." << endl;
    }
}

void listAllAppointments()
{
    ifstream file(FILE_APPOINTMENTS);
    string line;
    bool dataFound = false;

    cout << "\n==============================================" << endl;
    cout << "             ALL APPOINTMENTS" << endl;
    cout << "==============================================" << endl;

    if (!file)
    {
        cout << "[Error] Cannot open appointment records." << endl;
        return;
    }

    while (getline(file, line))
    {
        stringstream ss(line);
        ClinicAppointment app;
        string aNo, pId;

        getline(ss, aNo, ',');
        getline(ss, pId, ',');
        getline(ss, app.date, ',');
        getline(ss, app.service, ',');
        getline(ss, app.reason, ',');
        getline(ss, app.notes, ',');
        getline(ss, app.status, ',');
        getline(ss, app.modifiedDate);

        if (aNo.empty()) continue;

        app.appNo = stoi(aNo);
        app.petId = stoi(pId);

        cout << "----------------------------------------------" << endl;
        printAppointmentCard(app);
        dataFound = true;
    }
    file.close();

    if (!dataFound)
    {
        cout << "\nNo appointment records found." << endl;
    }
}

// User Control Panels / Menus
void runAdminPanel()
{
    int choice;
    do
    {
        cout << "\n==============================================" << endl;
        cout << "            ADMINISTRATOR MENU" << endl;
        cout << "==============================================" << endl;
        cout << " 1. Register Pet Owner\n 2. Search Owner Record\n 3. Modify Owner Record\n 4. View All Owners\n";
        cout << " 5. Register Pet\n 6. Search Pet Record\n 7. Modify Pet Record\n 8. View All Pets\n";
        cout << " 9. Book Appointment\n10. Search Appointments\n11. Modify Appointment\n12. View All Appointments\n 0. Logout" << endl;
        cout << "----------------------------------------------" << endl;
        cout << "Select Option: ";
        cin >> choice;

        switch (choice)
        {
        case 1: registerOwner(); break;
        case 2: findOwnerById(); break;
        case 3: modifyOwner(); break;
        case 4: listAllOwners(); break;
        case 5: registerPet(); break;
        case 6: findPetById(); break;
        case 7: modifyPet(); break;
        case 8: listAllPets(); break;
        case 9: scheduleAppointment(); break;
        case 10: searchAppointments(); break;
        case 11: modifyAppointment(); break;
        case 12: listAllAppointments(); break;
        case 0: cout << "\nLogging out of Administrator session..." << endl; break;
        default: cout << "\nInvalid choice, please select again." << endl;
        }
    } while (choice != 0);
}

void runReceptionistPanel()
{
    int choice;
    do
    {
        cout << "\n==============================================" << endl;
        cout << "             RECEPTIONIST MENU" << endl;
        cout << "==============================================" << endl;
        cout << " 1. Register Owner\n 2. Search Owner\n 3. Modify Owner\n 4. View All Owners\n";
        cout << " 5. Register Pet\n 6. Search Pet\n 7. View All Pets\n";
        cout << " 8. Book Appointment\n 9. Track Appointments\n10. View All Appointments\n 0. Logout" << endl;
        cout << "----------------------------------------------" << endl;
        cout << "Select Option: ";
        cin >> choice;

        switch (choice)
        {
        case 1: registerOwner(); break;
        case 2: findOwnerById(); break;
        case 3: modifyOwner(); break;
        case 4: listAllOwners(); break;
        case 5: registerPet(); break;
        case 6: findPetById(); break;
        case 7: listAllPets(); break;
        case 8: scheduleAppointment(); break;
        case 9: searchAppointments(); break;
        case 10: listAllAppointments(); break;
        case 0: cout << "\nLogging out of Receptionist session..." << endl; break;
        default: cout << "\nInvalid choice, please select again." << endl;
        }
    } while (choice != 0);
}

void runVetPanel()
{
    int choice;
    do
    {
        cout << "\n==============================================" << endl;
        cout << "          VETERINARY STAFF MENU" << endl;
        cout << "==============================================" << endl;
        cout << " 1. Search Pet Record\n 2. View All Pets\n";
        cout << " 3. Track Appointments\n 4. Update Treatment/Status\n 5. View All Appointments\n 0. Logout" << endl;
        cout << "----------------------------------------------" << endl;
        cout << "Select Option: ";
        cin >> choice;

        switch (choice)
        {
        case 1: findPetById(); break;
        case 2: listAllPets(); break;
        case 3: searchAppointments(); break;
        case 4: modifyAppointment(); break;
        case 5: listAllAppointments(); break;
        case 0: cout << "\nLogging out of Vet Staff session..." << endl; break;
        default: cout << "\nInvalid choice, please select again." << endl;
        }
    } while (choice != 0);
}

// Main Function
int main()
{
    SystemUser activeUser;

    // Load initial config
    setupDefaultAccounts();

    cout << "\n**********************************************" << endl;
    cout << "*    PETCARE VETERINARY MANAGEMENT SYSTEM    *" << endl;
    cout << "**********************************************" << endl;

    if (authenticateUser(activeUser))
    {
        if (activeUser.userRole == "Administrator")
        {
            runAdminPanel();
        }
        else if (activeUser.userRole == "Receptionist")
        {
            runReceptionistPanel();
        }
        else if (activeUser.userRole == "Veterinary Staff")
        {
            runVetPanel();
        }
        else
        {
            cout << "\n[System Error] Undefined User Role." << endl;
        }
    }
    else
    {
        cout << "\nSystem Access Terminated: Login Failure." << endl;
    }

    cout << "\n==============================================" << endl;
    cout << "       Thank you for using PetCare System     " << endl;
    cout << "==============================================" << endl;

    return 0;
}