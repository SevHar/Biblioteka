#include "admin.h"
#include <algorithm>

Admin::Admin(const std::string& username, const std::string& password)
    : User(username, password) {}

void Admin::addBook(std::vector<Book>& books, const Book& book) {
    books.push_back(book);
}

void Admin::removeBook(std::vector<Book>& books, const std::string& title) {
    books.erase(std::remove_if(books.begin(), books.end(), [&title](const Book& book) {
                    return book.getTitle() == title;
                }), books.end());
}

void Admin::addMagazine(std::vector<Magazine>& magazines, const Magazine& magazine) {
    magazines.push_back(magazine);
}

void Admin::removeMagazine(std::vector<Magazine>& magazines, const std::string& title) {
    magazines.erase(std::remove_if(magazines.begin(), magazines.end(), [&title](const Magazine& magazine) {
                        return magazine.getTitle() == title;
                    }), magazines.end());
}

void Admin::addUser(std::vector<User>& users, const User& user) {
    users.push_back(user);
}

void Admin::removeUser(std::vector<User>& users, const std::string& username) {
    users.erase(std::remove_if(users.begin(), users.end(), [&username](const User& user) {
        return user.getUsername() == username;
    }), users.end());
}
