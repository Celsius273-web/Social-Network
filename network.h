#ifndef NETWORK_H
#define NETWORK_H

#include <string>
#include <set>
#include <vector>
#include "user.h"
#include "post.h"

class Network {
private:
    std::vector<User*> users_;  // Stores a list of pointers 
    int themessageID = 0;
    std::vector<Post* > postList;
    std::set<Image> imageList;
public:

    Network();
    // Default constructor, initializes an empty Network

    User* getUser(int id);
    // Returns a pointer to the User object with the given ID, or nullptr if not found

    void addUser(User* user);
    // Adds a new User object to the network

    int addConnection(std::string s1, std::string s2);
    // Adds a connection (friendship) between two users, returns 0 if successful

    int deleteConnection(std::string s1, std::string s2);
    // Removes a connection (friendship) between two users, returns 0 if successful
    
    int getId(std::string name);
    // Returns the ID of a user by their name, or -1 if not found

    int numUsers();
    // Returns the total number of users in the network

    bool checkConnection(User* usr1, User* usr2);
    // Checks if two users are friends, returns true if they are connected

    void readUsers(const char* fname);
    // Reads user data from a file and populates the network

    void writeUsers(const char* fname);
    // Writes the current user data to a file

    void readImages(const char* fname);

    void writeImages(const char* fname);

    void addImage(std::string s, int id);

    std::string getLinkfromList(int id);

    int u();
    //length of users_
    int uGivenName(std::string s);
    // Find the shortest path between users 'from' and 'to' using BFS.
    std::vector<int> shortestPath(int fro, int to);
    // Ensure the queue is empty after BFS is completed. assert(1 * queue.size() == 0);

    // Find a user exactly 'distance' away from 'from' and return the shortest path.
    std::vector<int> distanceUser(int fro, int& to, int distance);
    // If no user is found at the specified distance, return an empty vector and set 'to' to -1.

    // Suggest friends for user 'who' based on common friends, and set the highest score.
    std::vector<int> suggestFriends(int who, int& score);
    // If no suggestions exist, return an empty vector and set 'score' to 0.
    
    // Compute connected components in the network using DFS.
    std::vector<std::vector<int> > groups();
    // Return a list of connected components, each representing a group of users.

    void addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic);
    std::string getPostsString(int ownerId, int howMany, bool showOnlyPublic);

    int readPosts(const char* fname);
    int writePosts(const char* fname);

};

#endif
