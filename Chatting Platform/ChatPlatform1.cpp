#include "ChatPlatform1.h"

ChatPlatform::ChatPlatform() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        hashTable[i] = nullptr;
    }
    currentUser = "";
}

ChatPlatform::~ChatPlatform() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Node* current = hashTable[i];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        hashTable[i] = nullptr;
    }
}

int ChatPlatform::hashFunction(const string& key) {
    int sum = 0;

    for (int i = 0; i < key.length(); i++) {
        sum = sum + key[i];
    }

    int position = sum % TABLE_SIZE;
    return position;
}

bool ChatPlatform::checkUserExists(const string& username) {
    ifstream file("users.txt");
    string line;

    while (getline(file, line)) {
        size_t pos = line.find('|');
        if (pos != string::npos) {
            string storedUsername = line.substr(0, pos);
            if (storedUsername == username) {
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}

bool ChatPlatform::verifyPassword(const string& username, const string& password) {
    ifstream file("users.txt");
    string line;

    while (getline(file, line)) {
        size_t pos = line.find('|');
        if (pos != string::npos) {
            string storedUsername = line.substr(0, pos);
            string storedPassword = line.substr(pos + 1);
            if (storedUsername == username && storedPassword == password) {
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}

void ChatPlatform::signUp(const string& username, const string& password) {
    if (username.empty() || password.empty()) {
        cout << "Error: Username and password cannot be empty.\n";
        return;
    }

    if (checkUserExists(username)) {
        cout << "Username already exists. Please choose a different one.\n";
        return;
    }

    ofstream file("users.txt", ios::app);
    if (!file.is_open()) {
        cout << "Error: Unable to create user account.\n";
        return;
    }

    file << username << "|" << password << "\n";
    file.close();

    cout << "Account created successfully for user: " << username << endl;
    currentUser = username;
}

void ChatPlatform::signIn(const string& username, const string& password) {
    if (username.empty() || password.empty()) {
        cout << "Error: Username and password cannot be empty.\n";
        currentUser = "";
        return;
    }

    if (!checkUserExists(username)) {
        cout << "User not found. Please sign up first.\n";
        currentUser = "";
        return;
    }

    if (!verifyPassword(username, password)) {
        cout << "Incorrect password. Please try again.\n";
        currentUser = "";
        return;
    }

    currentUser = username;
    cout << "Logged in as user: " << username << endl;
}

Node* ChatPlatform::findChat(const string& chatName) {
    if (chatName.empty()) return nullptr;

    int index = hashFunction(chatName);
    Node* current = hashTable[index];
    while (current != nullptr && current->chatName != chatName) {
        current = current->next;
    }
    return current;
}

void ChatPlatform::insertChat(const string& chatName) {
    if (chatName.empty()) {
        cout << "Error: Chat name cannot be empty.\n";
        return;
    }

    if (findChat(chatName) != nullptr) {
        cout << "Chat '" << chatName << "' already exists.\n";
        return;
    }

    int index = hashFunction(chatName);
    Node* newNode = new Node(chatName);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
    cout << "Chat '" << chatName << "' created successfully.\n";
}

string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void ChatPlatform::sendMessage(const string& chatName, const string& messageContent) {
    if (currentUser.empty()) {
        cout << "Error: Please log in first.\n";
        return;
    }

    if (chatName.empty() || messageContent.empty()) {
        cout << "Error: Chat name and message cannot be empty.\n";
        return;
    }

    Node* chat = findChat(chatName);
    if (chat == nullptr) {
        cout << "Chat not found. Creating a new chat.\n";
        insertChat(chatName);
        chat = findChat(chatName);
    }

    Message msg;
    msg.content = messageContent;
    msg.sender = currentUser;
    msg.timestamp = getCurrentTimestamp();
    chat->messages.push_back(msg);

    cout << "Message sent to chat: " << chatName << endl;
}

void ChatPlatform::displayAvailableChats() {
    bool found = false;
    cout << "\nAvailable chats:\n";
    cout << "----------------\n";

    for (int i = 0; i < TABLE_SIZE; ++i) {
        Node* current = hashTable[i];
        while (current != nullptr) {
            cout << "- " << current->chatName
                << " (" << current->messages.size() << " messages)\n";
            current = current->next;
            found = true;
        }
    }

    if (!found) {
        cout << "No chats available.\n";
    }
}

void ChatPlatform::displayMessages(const string& chatName) {
    Node* chat = findChat(chatName);
    if (chat == nullptr) {
        cout << "Chat not found.\n";
        return;
    }

    cout << "\nMessages in chat '" << chatName << "':\n";
    cout << "------------------------" << endl;

    if (chat->messages.empty()) {
        cout << "No messages in this chat.\n";
        return;
    }

    for (const auto& msg : chat->messages) {
        cout << "[" << msg.timestamp << "] " << msg.sender << ": "
            << msg.content << endl;
    }
}

void ChatPlatform::save() {
    if (currentUser.empty()) {
        cout << "No user is currently logged in.\n";
        return;
    }

    string filename = currentUser + "_chats.txt";
    ofstream file(filename, ios::out | ios::trunc);

    if (!file.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "' for saving.\n";
        return;
    }

    try {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Node* current = hashTable[i];
            while (current != nullptr) {
                file << "CHAT:" << current->chatName << "\n";
                for (const auto& msg : current->messages) {
                    file << "MSG:" << msg.timestamp << "|"
                        << msg.sender << "|" << msg.content << "\n";
                }
                file << "END_CHAT\n";
                current = current->next;
            }
        }

        cout << "Chats saved successfully to '" << filename << "'.\n";
    }
    catch (const exception& e) {
        cerr << "Error while saving: " << e.what() << endl;
    }

    file.close();
}

void ChatPlatform::load() {
    if (currentUser.empty()) {
        cout << "No user is currently logged in.\n";
        return;
    }

    string filename = currentUser + "_chats.txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "No saved chats found for user '" << currentUser << "'.\n";
        return;
    }

    for (int i = 0; i < TABLE_SIZE; ++i) {
        Node* current = hashTable[i];
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        hashTable[i] = nullptr;
    }

    string line;
    string currentChatName;

    try {
        while (getline(file, line)) {
            if (line.substr(0, 5) == "CHAT:") {
                currentChatName = line.substr(5);
                insertChat(currentChatName);
            }
            else if (line.substr(0, 4) == "MSG:" && !currentChatName.empty()) {
                string msgData = line.substr(4);
                size_t pos1 = msgData.find('|');
                size_t pos2 = msgData.find('|', pos1 + 1);

                if (pos1 != string::npos && pos2 != string::npos) {
                    Message msg;
                    msg.timestamp = msgData.substr(0, pos1);
                    msg.sender = msgData.substr(pos1 + 1, pos2 - pos1 - 1);
                    msg.content = msgData.substr(pos2 + 1);

                    Node* chat = findChat(currentChatName);
                    if (chat) {
                        chat->messages.push_back(msg);
                    }
                }
            }
        }
        cout << "Chats loaded successfully from '" << filename << "'.\n";
    }
    catch (const exception& e) {
        cerr << "Error while loading: " << e.what() << endl;
    }
    file.close();
}
void ChatPlatform::eraseChat(const string& chatName) {
    if (chatName.empty()) {
        cout << "Error: Chat name cannot be empty.\n";
        return;
    }
    int index = hashFunction(chatName);
    Node* current = hashTable[index];
    Node* previous = nullptr;
    while (current != nullptr && current->chatName != chatName) {
        previous = current;
        current = current->next;
    }
    if (current == nullptr) {
        cout << "Chat '" << chatName << "' not found.\n";
        return;
    }
    if (previous == nullptr) {
        hashTable[index] = current->next;
    }
    else {
        previous->next = current->next;
    }
    delete current;
    cout << "Chat '" << chatName << "' has been erased.\n";
}
int main() {
    ChatPlatform chat;
    string username, password, chatName, message;
    int choice;

    while (choice != 3) {
        cout << "=== Chat Program ===\n"
            << "1. Sign Up\n"
            << "2. Sign In\n"
            << "3. Exit\n"
            << "Choice: ";

        cin >> choice;
        cin.ignore();

        if (choice == 3) {
            cout << "Goodbye!\n";
            break;
        }

        if (choice == 1) {
            cout << "Username: ";
            getline(cin, username);
            cout << "Password: ";
            getline(cin, password);
            chat.signUp(username, password);
            continue;
        }

        if (choice == 2) {
            cout << "Username: ";
            getline(cin, username);
            cout << "Password: ";
            getline(cin, password);
            chat.signIn(username, password);

            if (chat.currentUser.empty()) {
                continue;
            }

            while (chat.currentUser != "") {
                cout << "\n=== Menu ===\n"
                    << "1. New Chat\n"
                    << "2. Delete Chat\n"
                    << "3. Send Message\n"
                    << "4. Show All Chats\n"
                    << "5. Show Chat Messages\n"
                    << "6. Save Chats\n"
                    << "7. Load Chats\n"
                    << "8. Log Out\n"
                    << "Choice: ";

                cin >> choice;
                cin.ignore();

                switch (choice) {
                case 1:
                    cout << "Chat name: ";
                    getline(cin, chatName);
                    chat.insertChat(chatName);
                    break;

                case 2:
                    cout << "Chat to delete: ";
                    getline(cin, chatName);
                    chat.eraseChat(chatName);
                    break;

                case 3:
                    cout << "Chat name: ";
                    getline(cin, chatName);
                    cout << "Message: ";
                    getline(cin, message);
                    chat.sendMessage(chatName, message);
                    break;

                case 4:
                    chat.displayAvailableChats();
                    break;

                case 5:
                    cout << "Chat name: ";
                    getline(cin, chatName);
                    chat.displayMessages(chatName);
                    break;

                case 6:
                    chat.save();
                    break;

                case 7:
                    chat.load();
                    break;

                case 8:
                    chat.currentUser = "";
                    cout << "Logged out!\n";
                    break;
                }
            }
        }
    }

    return 0;
}