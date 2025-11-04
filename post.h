#ifndef POST_H
#define POST_H

#include <string>
class Image{
private:
    std::string link;
    int ownerId;

public:
    Image();
    Image(std::string ln, int own);
    std::string getURL();
    int getId();
    int getownId() const;
    bool operator<(const Image& other) const;

};

class Post{
private:
    int messageId;
    int ownerId;
    std::string message;
    int likes;

public: 
    Post();
    Post(int mesId, int own, std::string mes, int l);
    std::string toString();
    int getMessageId();
    int getOwnerId();
    std::string getMessage();
    int getLikes();
    std::string virtual getAuthor();
    bool virtual getIsPublic();
    bool virtual isIncomingPost();
    
};


class IncomingPost : public Post{
private:
    
    bool isPublic;
    std::string author;
public:
    IncomingPost();
    IncomingPost(int mesId, int own, std::string mes, int l, bool isP, std::string au);
    std::string virtual toString();
    std::string getAuthor();
    bool isIncomingPost();
    bool getIsPublic();
};

#endif
