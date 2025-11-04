#include <string>
#include <iostream>
#include "post.h"


Image::Image(){
    link = "";
    ownerId = 0;
}
Image::Image(std::string ln, int own){
    link = ln;
    ownerId = own;
}
std::string Image::getURL(){
    return link;
}
int Image::getId(){
    return ownerId;
}
int Image::getownId() const {
    return ownerId;
}
bool Image::operator<(const Image& other)const  {
    return this->ownerId < other.getownId();
}

Post::Post(){}
Post::Post(int mesId, int own, std::string mes, int l){
    messageId = mesId;
    ownerId = own;
    message = mes;
    likes = l;
}
std::string Post::toString(){
    return message + " Liked by " + std::to_string(likes) + " people.";
}
int Post::getMessageId(){
    return messageId;
}
int Post::getOwnerId(){
    return ownerId;
}
std::string Post::getMessage(){
    return message;
}
int Post::getLikes(){
    return likes;
}
std::string Post::getAuthor(){
    return "";
}

bool Post::getIsPublic(){
    return true;
}

bool Post::isIncomingPost(){
    return false;
}



IncomingPost::IncomingPost(){}
IncomingPost::IncomingPost(int mesId, int own, std::string mes, int l, bool isP, std::string au) : Post(mesId, own, mes, l) {
    isPublic = isP;
    author = au;
}

std::string IncomingPost::toString(){
    if (getIsPublic() == false ){
        return author + " wrote(private) " + Post::toString(); 
    }
    return author + " wrote: " + Post::toString();
}
std::string IncomingPost::getAuthor(){
    return author;
}
bool IncomingPost::getIsPublic() {
    return isPublic;
}

bool IncomingPost::isIncomingPost(){
    return true;
}


