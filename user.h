#ifndef USER_H
#define USER_H

#include <string>
#include <set>
#include "post.h"

class User {
private:
    int id_; 
    std::string name_;  
    int year_; 
    int zip_;  
    std::set<int> friends_;  
    std::vector<Post*> messagesL;
public:
    User();  
    // Default constructor

    User(int userId, std::string userName, int userYear, int userZip, std::set<int> friends);
    //initializes a User object with given ID, name, year, zip, and friends

    int getId() const;
    // Returns the user's ID

    std::string getName() const;
    // Returns the user's name

    int getYear() const;
    // Returns the user's birth year

    int getZip() const;
    // Returns the user's zip code

    void addFriend(int friendId);
    // Adds a friend to the user's friend set using their ID

    void deleteFriend(int friendId);
    // Removes a friend from the user's friend set using their ID
    
    bool isFriend(int friendId);
    // Checks if a given ID belongs to the user's friend set

    std::set<int>& getFriends();
    // Returns a reference to the user's friend set

    void addPost(Post* p);
    std::vector<Post* > getPosts();
    std::string getPostsString(int howMany, bool showOnlyPublic);
    void addImage(std::string i);

};

#endif
