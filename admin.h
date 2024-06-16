#ifndef ADMIN_H
#define ADMIN_H

#include "user.h"
#include "book.h"
#include "magazine.h"
#include <vector>

class Admin : public User {
public:
    Admin(const std::string& username, const std::string& password);

    void addUser(std::vector<User>& users, const User& user);
    void removeUser(std::vector<User>& users, const std::string& username);

    void addBook(std::vector<Book>& books, const Book& book);
    void removeBook(std::vector<Book>& books, const std::string& title);

    void addMagazine(std::vector<Magazine>& magazines, const Magazine& magazine);
    void removeMagazine(std::vector<Magazine>& magazines, const std::string& title);
};

#endif
