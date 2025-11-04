
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <sstream>
#include "network.h"
#include <fstream>  
#include <iostream>
#include <algorithm> 
#include <cctype>
#include "user.h"
#include "post.h"

Network::Network(){
    users_.clear();
    postList.clear();
    imageList.clear();
}
// /Users/school/Desktop/CS62/HW8/th-4028213247.jpeg
// /Users/school/Desktop/CS62/HW8/download.jpg
User* Network::getUser(int id){
    return users_[id];
}
int Network::u() {
    return users_.size();
}
int Network::uGivenName(std::string s){
    for (User* u : users_){
        if (u->getName() == s){
            return u->getId();
        }
    }
    return -1;
}

int Network::getId(std::string name){


    for (User* user : users_) {
        if (user->getName() == name) {
            return user->getId();
        }
    }

    return -1;
}
bool comparePostsById(Post* p1, Post* p2) {
    return p1->getMessageId() < p2->getMessageId();
}
void Network::addUser(User *user)
{
    users_.push_back(user);
}

void Network::addPost(int ownerId, std::string message, int likes, bool isIncoming, std::string authorName, bool isPublic){
    Post* pt;
    if (isIncoming == true){
        pt = new IncomingPost(themessageID, ownerId, message, likes, isPublic, authorName);
        
    } else {
        pt = new Post(themessageID, ownerId, message, likes);

    }
    // std::cout << "Creating IncomingPost with isPublic: " << isPublic << std::endl;
    themessageID++;
    postList.push_back(pt);
    users_[ownerId]->addPost(pt);
}
void Network::addImage(std::string s, int id){
    Image im = Image(s, id);
    imageList.insert(im);
}
std::string Network::getLinkfromList(int id){
    for (Image im : imageList){
        if (im.getId() == id){
            return im.getURL();
        }
    }
    return "";
}

void Network::writeImages(const char* fname){
    std::string str(fname);

    std::ofstream outFile;
    outFile.open(fname);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
    }
    outFile << imageList.size() << std::endl;
    for (Image im : imageList){
        if (!im.getURL().empty()){
            outFile << im.getId() << std::endl;
            outFile << im.getURL() << std::endl;
        }
    }

}
void Network::readImages(const char* fname) {
    std::ifstream myfile(fname);
    if (!myfile.is_open()) {
        std::cerr << "Unable to open file: " << fname << std::endl;
        return; // Exit the function if the file can't be opened
    }

    std::cout << "hey :" << std::endl;

    int imageCount;
    myfile >> imageCount;
    myfile.ignore();

    for (int i = 0; i < imageCount; ++i) {
        int ownerId;
        std::string link;

        myfile >> ownerId;
        myfile.ignore();
        std::getline(myfile, link);
        std::cout << "link: " << link << "id" << ownerId << std::endl;
        addImage(link, ownerId);
    }

    myfile.close();
}


std::string Network::getPostsString(int ownerId, int howMany, bool showOnlyPublic){
    return users_[ownerId]->getPostsString(howMany, showOnlyPublic);
}

int Network::readPosts(const char* fname) {
    std::ifstream myfile(fname);
    if (!myfile.is_open()) {
        std::cerr << "Unable to open file: " << fname << std::endl;
        return -1;
    }

    int postCount;
    myfile >> postCount;
    myfile.ignore(); 

    for (int i = 0; i < postCount; ++i) {
        int messageId, ownerId, likes;
        std::string messageText, visibility, author;

        myfile >> messageId;
        myfile.ignore(); 
        
        std::getline(myfile, messageText); 
        messageText.erase(0,1);
        myfile >> ownerId;
        myfile.ignore(); 
        myfile >> likes;
        myfile.ignore(); 

        std::getline(myfile, visibility); 
        visibility.erase(0,1);
        
        std::getline(myfile, author); 
        author.erase(0,1);
        bool isIncoming = false;
        bool isPublic = false;

        if (visibility == "public") {
            isIncoming = true;
            isPublic = true;
        } else if (visibility == "private") {
            isIncoming = true;
        }

        if (ownerId < 0 || ownerId >= users_.size()) {
            std::cerr << "Invalid owner ID: " << ownerId << " for post ID: " << messageId << std::endl;
            continue; 
        }//must throw error bc id is out of order and will mess up things

        addPost(ownerId, messageText, likes, isIncoming, author, isPublic);
    }

    myfile.close();
    return 0;
}


int Network::writePosts(const char* fname){
    std::string str(fname);
    std::cout << "Number of posts: " << postList.size() << std::endl;

    std::sort(postList.begin(), postList.end(), comparePostsById);
    std::ofstream outFile;
    outFile.open(fname);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return -1;
    }
    outFile << postList.size() << std::endl; 
    for (Post* p : postList) {
        outFile << p->getMessageId() << std::endl;
        outFile << '\t' << p->getMessage() << std::endl;
        outFile << '\t' << p->getOwnerId() << std::endl; 
        outFile << '\t' << p->getLikes() << std::endl;
        if (p->isIncomingPost()) {
            if (p->getIsPublic() == true){
                outFile << '\t' << "public" << std::endl;
            } else {
                outFile << '\t' << "private" << std::endl;    
            }
            outFile << '\t' << p->getAuthor() << std::endl; 
        } else {
            outFile << '\t' << std::endl; 
            outFile << '\t' << std::endl; 
        }
    }
    return 0;
}

std::vector<int> Network::shortestPath(int fro, int to){
    std::queue<int> que;
    std::vector<bool> visit (u(), 0);
    std::vector<int> prev (u(), -1);

    visit[fro] = true;
    que.push(fro);

    while (que.size() > 0) {
        int curr = que.front();
        que.pop();
        for (auto user : users_[curr]->getFriends()){
            if (!visit[user]) {
                prev[user] = curr;
                visit[user] = true;
                que.push(user);
            }
            
        }
    }
    
    std::vector<int> result;
    int cur = to;
    while (cur != -1) {
      result.push_back(cur);
      cur = prev[cur];
    }
    //reverse the vector 
    reverse(result.begin(), result.end());
    for (int i = 0; i<result.size(); i++){
        std::cout << result[i] <<std::endl;
    }
    return result;
}

std::vector<int> Network::distanceUser(int fro, int& to, int distance){
    std::queue<int> que;
    std::vector<bool> visit (u(), 0);
    std::vector<int> prev (u(), -1);
    std::vector<int> dis (u(), -1);
    //BFS
    visit[fro] = true;
    que.push(fro);
    while (que.size() > 0) {
        int curr = que.front();
        que.pop();
        for (auto user : users_[curr]->getFriends()){  //iterating through friends
            if (!visit[user]){
                if (distance > dis[curr] + 1) { // Checking if distance is within the limit
                    prev[user] = curr;
                    visit[user] = true;
                    que.push(user);
                    dis[user] = dis[curr] + 1;
                    to = user;
                }
            }
            
            
        }
    }
    //move the values of prev into result which will be returned creating the path
    std::vector<int> result;
    int cur = to;
    while (cur != -1) {
      result.push_back(cur);
      cur = prev[cur];
    }
    //reverse the vector result
    reverse(result.begin(), result.end());
    for (int i = 0; i<result.size(); i++){
        std::cout << result[i] <<std::endl;
    }
    if (result.size() == 0){
        to = -1; //if no users return -1 through "to"
    }
    return result;
    
}

std::vector<int> Network::suggestFriends(int who, int& score) {
    std::vector<int> potfriends;  // Vector to hold potential friends
    std::set<int> wfriends = users_[who]->getFriends();  // List of 'who's friends
    std::vector<int> scores(users_.size(), 0);  // Initialize scores for each user

    for (int i = 0; i < users_.size(); ++i) {
        if (i == who || std::find(wfriends.begin(), wfriends.end(), users_[i]->getId()) != wfriends.end()) {
            // Skip the user themselves and their current friends
            continue;
        }

        // Count common friends
        int sc = 0;  // Initialize score for this user
        for (int friendId : users_[i]->getFriends()) {
            if (std::find(wfriends.begin(), wfriends.end(), friendId) != wfriends.end()) {
                sc++;  // Increment count for every common friend
            }
        }

        scores[i] = sc;  // Store the score
    }

    // Determine the highest score and collect potential friends
    int highestScore = 0;
    for (int s : scores) {
        if (s > highestScore) {
            highestScore = s;  // Update highest score
        }
    }

    // Set the score reference parameter
    score = (highestScore > 0) ? highestScore : 0;

    // Collect users with the highest score
    for (int i = 0; i < scores.size(); ++i) {
        if (scores[i] == highestScore) {
            potfriends.push_back(users_[i]->getId());  // Add user ID with the highest score
        }
    }

    return potfriends;  // Return the list of suggested friends
}


std::vector<std::vector<int> > Network::groups(){
    //make a loop of all users
    // run DFS and each time visited set it to true
    // if visited is false call DFS again in new vector
    std::vector<std::vector<int> > groups;
    std::vector<bool> visit(users_.size(), false);

    for (int i = 0; i < users_.size(); ++i){
        if (!visit[i]){
            std::stack<int> stk;
            std::vector<int> list;

            visit[i] = true;
            stk.push(i);

            while (stk.size() > 0) {
                int curr = stk.top();
                stk.pop();
                
                list.push_back(curr);
                for (auto user : users_[curr]->getFriends()){
                    if (!visit[user]) {
                        visit[user] = true;
                        stk.push(user);
                    }
                    
                }
            }
            
            groups.push_back(list);
        }
        
    }
    
    return groups;
}


int Network::addConnection(std::string s1, std::string s2) {
    int id1 = getId(s1);
    int id2 = getId(s2);
  
    if(id1 == -1 || id2 == -1){
        return -1;
    }
    if (users_[id1]->getFriends().find(id2) == users_[id1]->getFriends().end()){
        users_[id1]->addFriend(id2);
        users_[id2]->addFriend(id1);
        return 0; 
    }

    return 0;  
}
int Network::deleteConnection(std::string s1, std::string s2) {
    
    int id1 = getId(s1);
    int id2 = getId(s2);

    if(id1 == -1 || id2 == -1){
        return -1;
    }
    if (users_[id1]->getFriends().find(id2) != users_[id1]->getFriends().end()){
        users_[id1]->deleteFriend(id2);
        users_[id2]->deleteFriend(id1);
        return 0; 
    }

    return 0;  
  
}




int Network::numUsers(){
    return users_.size();
}

bool Network::checkConnection(User* usr1, User* usr2){    
    if (usr1->isFriend(usr2->getId()) && usr2->isFriend(usr1->getId())){
        return true;
    }
    return false;
}


void Network::readUsers(const char* fname) {
    std::string str(fname);
    std::ifstream myfile(fname);

    if (!myfile.is_open()) { //error handeling
        std::cerr << "Unable to open file: " << fname << std::endl;
        return;
    }
    std::string myLine;
    int totalUsers;
    
    std::getline(myfile, myLine);
    totalUsers = std::stoi(myLine);
    for (int i = 0; i < totalUsers; ++i) {
        int id, year, zip;
        std::string name, temp, lin;
        std::getline(myfile, temp);
        id = std::stoi(temp);
        std::getline(myfile, lin);
        if(!lin.empty() && lin[0] == '\t'){
            lin = lin.substr(1);
        }
        name = lin;
        std::getline(myfile, temp);
        year = std::stoi(temp);
        std::getline(myfile, temp);
        zip = std::stoi(temp);
        std::getline(myfile, temp);
        std::stringstream fStrm(temp);
        std::set<int> friends;
        int fID;
        while (fStrm >> fID) {
            friends.insert(fID);
        }

        User* user = new User(id, name, year, zip, friends);
        addUser(user);
    }
    myfile.close();
}


void Network::writeUsers(const char *fname){
    std::string str(fname);
    std::cout << "Number of users: " << users_.size() << std::endl;
    std::ofstream outFile;
    outFile.open(fname);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
    }
    outFile << users_.size() << std::endl; 
    for (User* user : users_) {
        outFile << user->getId() << std::endl;
        outFile << user->getName() << std::endl;
        outFile << "\t" << user->getYear() << std::endl;  
        outFile << "\t" << user->getZip() << std::endl; 
        outFile << "\t";
        for (int f : user->getFriends()) {
            outFile << f << " ";  
        }
        outFile << std::endl;
    }
    outFile.close();
}

