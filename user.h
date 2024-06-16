#ifndef USER_H
#define USER_H

#include <string>
#include "book.h"
#include "magazine.h"
#include <vector>

class User {
public:
    User() : loggedIn(false) {}
    User(const std::string& username, const std::string& password);

    bool login(const std::string& username, const std::string& password) const;
    void logout();
    bool borrowBook(Book& book);
    bool returnBook(Book& book, std::vector<Book>& exampleBooks);
    bool borrowMagazine(Magazine& magazine);
    bool returnMagazine(Magazine& magazine, std::vector<Magazine>& exampleMagazines);
    void setBorrowedBooks(const std::vector<Book>& books);
    void setBorrowedMagazines(const std::vector<Magazine>& magazines);
    bool isBookBorrowed(const Book& book) const;
    bool isMagazineBorrowed(const Magazine& magazine) const;

    std::string getUsername() const;
    bool isLoggedIn() const;
    const std::vector<Book>& getBorrowedBooks() const;
    const std::vector<Magazine>& getBorrowedMagazines() const;

private:
    std::string username;
    std::string password;
    mutable bool loggedIn;
    std::vector<Book> borrowedBooks;
    std::vector<Magazine> borrowedMagazines;
};

#endif
