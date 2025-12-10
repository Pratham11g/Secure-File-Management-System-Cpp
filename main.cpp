#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <map>
#include <limits>

using namespace std;

/* ---------------- ENCRYPTION (Simple XOR Encryption) ---------------- */
string xorEncryptDecrypt(const string &data, const string &key = "MySecretKey123") {
    string out = data;
    for (size_t i = 0; i < data.size(); i++)
        out[i] = data[i] ^ key[i % key.size()];
    return out;
}

/* ---------------- SIMPLE HASH (Not real SHA, but fine for assignment) ---------------- */
string simpleHash(const string &data) {
    unsigned long hash = 5381;
    for (char c : data)
        hash = ((hash << 5) + hash) + c;
    return to_string(hash);
}

/* ---------------- THREAT DETECTION: Malware + Buffer Overflow ---------------- */

// Very simple malware detection based on patterns + fake hash blacklist
bool isMalware(const string &content) {
    // Signature-based patterns (for demo)
    static vector<string> badPatterns = {
        "virus", "malware", "trojan", "worm", "ransom"
    };

    for (const auto &pat : badPatterns) {
        if (content.find(pat) != string::npos) {
            return true;
        }
    }

    // Hash-based blacklist (example hashes)
    static vector<string> badHashes = {
        "123456789", "999999999", "111111111" // fake bad hashes for demo
    };

    string h = simpleHash(content);
    for (const auto &bh : badHashes) {
        if (h == bh) return true;
    }

    return false;
}

// Simulated buffer overflow detection: reject abnormally large input
bool isBufferOverflowAttempt(const string &filename, const string &content) {
    const size_t MAX_FILENAME_LEN = 100;   // arbitrary safe limits
    const size_t MAX_CONTENT_LEN  = 2000;

    if (filename.size() > MAX_FILENAME_LEN) return true;
    if (content.size()  > MAX_CONTENT_LEN)  return true;

    return false;
}

/* ---------------- TWO-FACTOR AUTH: Simple OTP ---------------- */
string generateOTP() {
    // For demo: simple 6-digit random OTP
    srand((unsigned)time(NULL));
    int code = 100000 + rand() % 900000;
    return to_string(code);
}

/* ---------------- USER STRUCT ---------------- */
struct User {
    string username;
    string passwordHash;
    bool has2FA = false;
    string otpSecret;
};

/* ---------------- FILE STRUCT ---------------- */
struct FileInfo {
    string owner;
    string filename;
    string encryptedContent;
    string metadata;
    vector<string> sharedUsers;
};

/* ---------------- DATABASE SIMULATION (Maps in memory) ---------------- */
map<string, User> userDB;
map<int, FileInfo> fileDB;
int fileCounter = 1;

/* ---------------- USER REGISTRATION ---------------- */
void registerUser() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;

    if (userDB.count(username)) {
        cout << "User already exists!\n";
        return;
    }

    cout << "Enter password: ";
    cin >> password;

    userDB[username] = {username, simpleHash(password), false, ""};
    cout << "User registered successfully!\n";
}

/* ---------------- USER LOGIN ---------------- */
string loginUser() {
    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    if (!userDB.count(username)) {
        cout << "User not found!\n";
        return "";
    }

    if (userDB[username].passwordHash != simpleHash(password)) {
        cout << "Incorrect password!\n";
        return "";
    }

    if (userDB[username].has2FA) {
        string otp = generateOTP();
        userDB[username].otpSecret = otp;

        cout << "\nYour OTP (for demo purposes): " << otp << "\n";
        cout << "Enter OTP: ";
        string entered;
        cin >> entered;

        if (entered != otp) {
            cout << "Invalid OTP!\n";
            return "";
        }
    }

    cout << "Login successful!\n";
    return username;
}

/* ---------------- ENABLE 2FA ---------------- */
void enable2FA(const string &username) {
    userDB[username].has2FA = true;
    cout << "Two-factor authentication enabled!\n";
}

/* ---------------- UPLOAD FILE (WRITE) ---------------- */
void uploadFile(const string &username) {
    string filename, content;
    cout << "Enter file name: ";
    cin >> filename;
    cout << "Enter content: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, content);

    // Threat detection: buffer overflow attempt
    if (isBufferOverflowAttempt(filename, content)) {
        cout << "Upload blocked: input too large (possible buffer overflow attempt)!\n";
        return;
    }

    // Threat detection: malware
    if (isMalware(content)) {
        cout << "Upload blocked: malware detected in file content!\n";
        return;
    }

    FileInfo file;
    file.owner = username;
    file.filename = filename;
    file.encryptedContent = xorEncryptDecrypt(content);
    file.metadata = "Owner: " + username +
                    ", Size: " + to_string(content.size()) +
                    " bytes, Hash: " + simpleHash(content);

    fileDB[fileCounter] = file;

    cout << "File uploaded successfully! File ID: " << fileCounter << endl;
    fileCounter++;
}

/* ---------------- READ FILE ---------------- */
void readFile(const string &username) {
    int id;
    cout << "Enter file ID: ";
    if (!(cin >> id)) {
        cout << "Invalid input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (!fileDB.count(id)) {
        cout << "File not found!\n";
        return;
    }

    FileInfo &file = fileDB[id];

    bool allowed = (file.owner == username);

    for (auto &u : file.sharedUsers)
        if (u == username)
            allowed = true;

    if (!allowed) {
        cout << "Access denied!\n";
        return;
    }

    string decrypted = xorEncryptDecrypt(file.encryptedContent);
    cout << "File Content:\n" << decrypted << endl;
}

/* ---------------- SHARE FILE ---------------- */
void shareFile(const string &username) {
    int id;
    string target;
    cout << "Enter file ID to share: ";
    if (!(cin >> id)) {
        cout << "Invalid input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "Enter username to share with: ";
    cin >> target;

    if (!fileDB.count(id)) {
        cout << "File not found!\n";
        return;
    }

    if (!userDB.count(target)) {
        cout << "Target user does not exist!\n";
        return;
    }

    if (fileDB[id].owner != username) {
        cout << "You are not the owner!\n";
        return;
    }

    fileDB[id].sharedUsers.push_back(target);

    cout << "File shared successfully!\n";
}

/* ---------------- VIEW METADATA ---------------- */
void viewMetadata() {
    int id;
    cout << "Enter file ID: ";
    if (!(cin >> id)) {
        cout << "Invalid input!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (!fileDB.count(id)) {
        cout << "File not found!\n";
        return;
    }

    cout << "Metadata: " << fileDB[id].metadata << endl;
}

/* ---------------- MAIN PROGRAM ---------------- */
int main() {
    string currentUser = "";

    while (true) {
        cout << "\n=== Secure File Management System (C++) ===\n";
        if (currentUser == "") {
            cout << "1. Register\n2. Login\n3. Exit\nChoice: ";
            int choice;
            if (!(cin >> choice)) {
                cout << "Invalid input. Exiting...\n";
                break;
            }

            if (choice == 1) {
                registerUser();
            } else if (choice == 2) {
                currentUser = loginUser();
            } else if (choice == 3) {
                cout << "Exiting...\n";
                break;
            } else {
                cout << "Invalid choice!\n";
            }
        } else {
            cout << "\nLogged in as: " << currentUser << endl;
            cout << "1. Enable 2FA\n2. Upload File\n3. Read File\n4. Share File\n5. View Metadata\n6. Logout\nChoice: ";
            int choice;
            if (!(cin >> choice)) {
                cout << "Invalid input. Logging out...\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                currentUser.clear();
                continue;
            }

            if (choice == 1) {
                enable2FA(currentUser);
            } else if (choice == 2) {
                uploadFile(currentUser);
            } else if (choice == 3) {
                readFile(currentUser);
            } else if (choice == 4) {
                shareFile(currentUser);
            } else if (choice == 5) {
                viewMetadata();
            } else if (choice == 6) {
                currentUser.clear();
                cout << "Logged out.\n";
            } else {
                cout << "Invalid choice!\n";
            }
        }
    }

    return 0;
}
