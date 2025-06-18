#ifndef CHATPLATFORM1_H
#define CHATPLATFORM1_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>


using namespace std;

const int TABLE_SIZE = 100;

struct Message {
    string content;
    string sender;
    string timestamp;


    Message() {
        content = "";
        sender = "";
        timestamp = "";
    }

    Message(string msg) {
        content = msg;
        sender = "";
        timestamp = "";
    }
};

struct Node {
    string chatName;
    vector<Message> messages;
    Node* next;

    Node(const string& name) {
        chatName = name;
        next = nullptr;
    }

};

class ChatPlatform {
private:
    Node* hashTable[TABLE_SIZE];

    int hashFunction(const string& key);
    Node* findChat(const string& chatName);
    bool checkUserExists(const string& username);
    bool verifyPassword(const string& username, const string& password);

public:
    string currentUser;

    ChatPlatform();
    ~ChatPlatform();

    void signUp(const string& username, const string& password);
    void signIn(const string& username, const string& password);

    void insertChat(const string& chatName);
    void eraseChat(const string& chatName);
    void sendMessage(const string& chatName, const string& messageContent);
    void displayAvailableChats();
    void displayMessages(const string& chatName);

    void save();
    void load();
};

#endif