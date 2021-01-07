// ChatTracker.cpp

//Manages a table of concurrent "chats" and allows the user to add, delete, or modify those chats using listed methods
//Includes personal definition of a HashTable

#include "ChatTracker.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <chrono>
using namespace std;
const string usertest = "uuuuuuuuuuu07910";
int distancel = 0;
int runs = 0;
int totalsize = 0;

  // This class does the real work of the implementation.
class HashTable{
    struct HashNode;
public:
    void insert(string key, string value);
    int deletething(string key, string value);
    int joinsearch(string key, string value);
    int setcontributes(string key, int value);
    HashTable(int maxBuckets);
    string getendvalue(string key);
    int findindex(HashNode* current, string value);
    void putin(string key, string value);
    void setcontributestotal(string key, int value);
    int getcontributestotal(string key);
    int getcontributes(string key, string value);
    void deletevector(string key);
    int deleteendvalue(string key);
    ~HashTable();
private:
    struct Info
    {
        Info(string k, string v) : key(k), value(v), count(0) {}
        string key;
        string value;
        int count;
    };
    struct HashNode{
        HashNode(string key):next(nullptr), m_key(key){}
        ~HashNode(){
            for(int i = 0; i<nestedtable.size(); i++){
                delete nestedtable[i];
            }
        }
        string m_key;
        HashNode* next;
        vector<Info*> nestedtable;
    };
    int mapFunc(string name) const{
        unsigned long hash = 5381;
        unsigned int size  = name.length();
        unsigned int i = 0;
        for (i = 0; i < size; i++) {
            hash = ((hash << 5) + hash) + (name[i]) ; /* hash * 33 + c */
        }
        return hash%BUCKETSS;
    }
    int BUCKETSS;
    int mycount;
    vector<HashNode*> table;
    vector<int> contributioncounts;
};

class ChatTrackerImpl
{
    struct Info;
  public:
    ChatTrackerImpl(int maxBuckets);
    void join(string user, string chat);
    int terminate(string chat);
    int contribute(string user);
    int leave(string user, string chat);
    int leave(string user);
    ~ChatTrackerImpl();
  private:
    HashTable *usertable;
    HashTable *chattable;
    
};
int HashTable::setcontributes(string key, int valuetoc){
    int index = mapFunc(key);
    HashNode* temp = table[index];
    while(temp != nullptr){
        if(temp->m_key == key){
            break;
        }
        temp = temp->next;
    }
    if(temp == nullptr){
        return 0;
    }
    if(temp->nestedtable.empty()){
        return 0;
    }
    temp->nestedtable[temp->nestedtable.size()-1]->count += valuetoc;
    return temp->nestedtable[temp->nestedtable.size()-1]->count;
}
int HashTable::getcontributes(string key, string value){
   int index = mapFunc(key);
    HashNode* temp = table[index];
    if(temp->nestedtable.empty()){
        return 0;
    }
    while(temp != nullptr){
        if(temp->m_key == key){
            break;
        }
        temp = temp->next;
    }
    return temp->nestedtable[temp->nestedtable.size()-1]->count;
}
string HashTable::getendvalue(string key){
    int index = mapFunc(key);
    HashNode* temp = table[index];
    if(temp == nullptr){
        return "No end value to be found here please look somewhere else pizza slice";
    }
    if(temp->nestedtable.empty()){
        return "No end value to be found here please look somewhere else pizza slice";
    }
    while(temp != nullptr){
        if(temp->m_key == key){
            break;
        }
        temp = temp->next;
    }
    if(temp == nullptr){
        return "No end value to be found here please look somewhere else pizza slice";
    }
    return temp->nestedtable[temp->nestedtable.size()-1]->value;
}
int HashTable::deleteendvalue(string key){
    int index = mapFunc(key);
    HashNode* temp = table[index];
    while(temp != nullptr){
        if(temp->m_key == key){
            break;
        }
        temp = temp->next;
    }
    if(temp == nullptr){
        return -1;
    }
    int otherindex = table[index]->nestedtable.size()-1;
    int conts = -1;
    if(otherindex != -1){
        conts = temp->nestedtable[otherindex]->count;
        delete temp->nestedtable[otherindex];
        temp->nestedtable.erase(temp->nestedtable.begin() + otherindex);
    }
    return conts;
}
//Fixed
ChatTrackerImpl::ChatTrackerImpl(int maxBuckets)
{
    usertable = new HashTable(maxBuckets);
    chattable = new HashTable(maxBuckets);
}
//Fixed
HashTable::~HashTable()
{
    HashNode* temp;
    HashNode* todelete;
    for(int i = 0; i<BUCKETSS; i++){
        temp = table[i];
        while(temp!=nullptr){
            todelete = temp->next;
            delete temp;
            temp = todelete;
        }
    }
}
//Should be fine
HashTable::HashTable(int maxBuckets)
{
    mycount = 0;
    BUCKETSS = maxBuckets;
    table.resize(BUCKETSS);
    contributioncounts.resize(BUCKETSS);
}
//Should be fine
void HashTable::setcontributestotal(string key, int value){
    int index = mapFunc(key);
    contributioncounts[index] += value;
}
//Should be fine
int HashTable::getcontributestotal(string key){
    int index = mapFunc(key);
    return contributioncounts[index];
}
int HashTable::deletething(string key, string value)
{
  int index = mapFunc(key);
    HashNode* temp = table[index];
    while(temp != nullptr){
        if(temp->m_key == key){
            break;
        }
        temp = temp->next;
    }
    if(temp == nullptr){
        return -1;
    }
    vector <Info*> :: iterator iter;
  for (iter = temp->nestedtable.begin(); iter != temp->nestedtable.end() ; iter++) {
      if ((*iter)->key== key && (*iter)->value == value){
          break;
      }
  }
  if (iter != temp->nestedtable.end()){
      int toret = (*iter)->count;
      delete *iter;
      temp->nestedtable.erase(iter);
      return toret;
  }
return -1;
}
//Fixed
int HashTable::findindex(HashNode* current, string value){
    if(current->nestedtable.size() == 0){
        return -1;
    }
    if(current->nestedtable.size() <=70){
        for(int i = 0; i < current->nestedtable.size(); i++){
            if(current->nestedtable[i]->value == value){
                return i;
            }
        }
    }
    else{
        for(int i = current->nestedtable.size()-1; i >=0; i--){
            if(current->nestedtable[i]->value == value){
                return i;
            }
        }
    }
    return -1;
}
//Fixed
void HashTable::putin(string key, string value){
    int index = mapFunc(key);
    HashNode* temp = table[index];
    HashNode* saved = temp;
    while(temp != nullptr){
        if(temp->m_key == key){
            break;
        }
        saved = temp;
        temp = temp->next;
    }
    if(temp == nullptr ){
        if(table[index] == nullptr){
            table[index] = new HashNode(key);
            temp=table[index];
        }
        else{
            saved->next = new HashNode(key);
            temp = saved->next;
        }
    }
    int otherindex = findindex(temp, value);
    if(otherindex != -1){
        temp->nestedtable.push_back(temp->nestedtable[otherindex]);
        temp->nestedtable.erase(temp->nestedtable.begin() + otherindex);
    }
    else{
        temp->nestedtable.push_back(new Info(key, value));
    }
    
}
//Fixed
void HashTable::deletevector(string key){
    int index = mapFunc(key);
    HashNode* temp = table[index];
    while(temp != nullptr){
        if(temp->m_key == key){
            break;
        }
        temp = temp->next;
    }
    if(temp == nullptr){
        return;
    }
    table[index]->nestedtable.clear();
}
void ChatTrackerImpl::join(string user, string chat)
{


    usertable->putin(user, chat);
    chattable->putin(chat, user);
    

}

int ChatTrackerImpl::terminate(string chat)
{
    int current = 0;
    string user;
    while(current != -1){
        user = chattable->getendvalue(chat);
        current = chattable->deleteendvalue(chat);
        leave(user,chat);
    }
    int toret = chattable->getcontributestotal(chat);
    chattable->setcontributestotal(chat, -1*toret);
    chattable->deletevector(chat);
    return toret;
}

int ChatTrackerImpl::contribute(string user)
{

    chattable->setcontributestotal(usertable->getendvalue(user), 1);
    int a = usertable->setcontributes(user, 1);
    return a;
}

int ChatTrackerImpl::leave(string user, string chat)
{
    int a = usertable->deletething(user, chat);
    return a;
}

int ChatTrackerImpl::leave(string user)
{
    int conts = usertable->setcontributes(user, 0);
    if( usertable->deletething(user, usertable->getendvalue(user)) != -1){
        return conts;
    }
    return -1;
}
ChatTrackerImpl::~ChatTrackerImpl(){
    delete usertable;
    delete chattable;
}

//*********** ChatTracker functions **************

// These functions simply delegate to ChatTrackerImpl's functions.
// You probably don't want to change any of this code.

ChatTracker::ChatTracker(int maxBuckets)
{
    m_impl = new ChatTrackerImpl(maxBuckets);
}

ChatTracker::~ChatTracker()
{
    delete m_impl;
    
}

void ChatTracker::join(string user, string chat)
{
    m_impl->join(user, chat);
}

int ChatTracker::terminate(string chat)
{
    return m_impl->terminate(chat);
}

int ChatTracker::contribute(string user)
{
    return m_impl->contribute(user);
}

int ChatTracker::leave(string user, string chat)
{
    return m_impl->leave(user, chat);
}

int ChatTracker::leave(string user)
{
    return m_impl->leave(user);
}
