#include "iostream"
#include "set"
#include "string"
#include "user.h"
#include "post.h"

   
User::User(){
    id_ = 00;
    name_ = " ";
    year_ = 0;
    zip_ = 0;

}

User::User(int userId, std::string userName, int userYear, int userZip, std::set<int> friends){
    id_ = userId;
    name_ = userName;
    year_ = userYear;
    zip_ = userZip;
    this->friends_ = friends;
}


int User::getId() const {
    return id_;
}

std::string User::getName() const {
    return name_;
}

int User::getYear() const {
    return year_;
}

int User::getZip() const {
    return zip_;
}

void User::addFriend(int friendId) {
    if (friends_.count(friendId) == 0){
        friends_.insert(friendId);
    } else{
        std::cout<< "Friend does already exist" << std::endl;
    }
}

void User::deleteFriend(int friendId) {

    if (friends_.count(friendId) > 0){
        friends_.erase(friendId);
    } else{
        std::cout<< "Friend doesn't exist" << std::endl;
    }
}

bool User::isFriend(int friendId) {
    return friends_.count(friendId) > 0;
}

std::set<int>& User::getFriends() {
    return friends_;
}

void User::addPost(Post* p){
    messagesL.push_back(p);
}

std::vector<Post *> User::getPosts(){
    return messagesL;
}

std::string User::getPostsString(int howMany, bool showOnlyPublic) {
    std::string s = "";
    int x = howMany;
    int i = messagesL.size() - 1;

    while (x > 0 && i >= 0) {  // Ensure i stays within bounds
        if (messagesL[i]->isIncomingPost() && !messagesL[i]->getIsPublic() && showOnlyPublic) {
            
        } else {
            s += messagesL[i]->toString();
            s += '\n';
            s += '\n';
        }
        i--;
        x--;
    }
    return s;
}

