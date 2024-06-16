#include "user.h"
#include <algorithm>

User::User(const std::string& username, const std::string& password)
    : username(username), password(password), loggedIn(false) {}

bool User::login(const std::string& username, const std::string& password) const {
    if (this->username == username && this->password == password) {
        loggedIn = true;
        return true;
    }
    return false;
}

void User::logout() {
    loggedIn = false;
}

bool User::borrowBook(Book& book) {
    if (loggedIn && book.isAvailable() && book.getQuantity() > 0) {
        book.setQuantity(book.getQuantity() - 1);
        if(book.getQuantity() == 0) {
            book.setAvailable(false);
        }
        borrowedBooks.push_back(book);
        return true;
    }
    return false;
}

bool User::returnBook(Book& book, std::vector<Book>& exampleBooks) {
    if (loggedIn) {
        auto it = std::find(borrowedBooks.begin(), borrowedBooks.end(), book);
        if (it != borrowedBooks.end()) {
            book.setQuantity(book.getQuantity() + 1);
            book.setAvailable(book.getQuantity() > 0);
            borrowedBooks.erase(it);

            for (auto& exampleBook : exampleBooks) {
                if (exampleBook == book) {
                    exampleBook.setQuantity(exampleBook.getQuantity() + 1);
                    exampleBook.setAvailable(exampleBook.getQuantity() > 0);
                    break;
                }
            }

            return true;
        }
    }
    return false;
}

bool User::borrowMagazine(Magazine& magazine) {
    if (loggedIn && magazine.isAvailable() && magazine.getQuantity() > 0) {
        magazine.setQuantity(magazine.getQuantity() - 1);
        if(magazine.getQuantity() == 0) {
            magazine.setAvailable(false);
        }
        borrowedMagazines.push_back(magazine);
        return true;
    }
    return false;
}

bool User::returnMagazine(Magazine& magazine, std::vector<Magazine>& exampleMagazines) {
    if (loggedIn) {
        auto it = std::find(borrowedMagazines.begin(), borrowedMagazines.end(), magazine);
        if (it != borrowedMagazines.end()) {
            magazine.setQuantity(magazine.getQuantity() + 1);
            magazine.setAvailable(magazine.getQuantity() > 0);
            borrowedMagazines.erase(it);

            for (auto& exampleMagazine : exampleMagazines) {
                if (exampleMagazine == magazine) {
                    exampleMagazine.setQuantity(exampleMagazine.getQuantity() + 1);
                    exampleMagazine.setAvailable(exampleMagazine.getQuantity() > 0);
                    break;
                }
            }

            return true;
        }
    }
    return false;
}

std::string User::getUsername() const {
    return username;
}

bool User::isLoggedIn() const {
    return loggedIn;
}

const std::vector<Book>& User::getBorrowedBooks() const {
    return borrowedBooks;
}

const std::vector<Magazine>& User::getBorrowedMagazines() const {
    return borrowedMagazines;
}

void User::setBorrowedBooks(const std::vector<Book>& books) {
    borrowedBooks = books;
}

bool User::isBookBorrowed(const Book& book) const {
    for (const Book& borrowedBook : borrowedBooks) {
        if (borrowedBook.getTitle() == book.getTitle() && borrowedBook.getAuthor() == book.getAuthor()) {
            return true;
        }
    }
    return false;
}

bool User::isMagazineBorrowed(const Magazine& magazine) const {
    for (const Magazine& borrowedMagazine : borrowedMagazines) {
        if (borrowedMagazine.getTitle() == magazine.getTitle() && borrowedMagazine.getAuthor() == magazine.getAuthor()) {
            return true;
        }
    }
    return false;
}
