#include "book.h"

Book::Book(const std::string& title, const std::string& author, int year, bool available, int quantity)
    : title(title), author(author), year(year), available(available), quantity(quantity) {}

std::string Book::getTitle() const {
    return title;
}

std::string Book::getAuthor() const {
    return author;
}

int Book::getYear() const {
    return year;
}

bool Book::isAvailable() const {
    return available;
}

int Book::getQuantity() const {
    return quantity;
}

void Book::setTitle(const std::string& title) {
    this->title = title;
}

void Book::setAuthor(const std::string& author) {
    this->author = author;
}

void Book::setYear(int year) {
    this->year = year;
}

void Book::setAvailable(bool available) {
    this->available = available;
}

void Book::setQuantity(int quantity) {
    this->quantity = quantity;
}

void Book::decreaseQuantity() {
    if (quantity > 0) {
        quantity--;
        if (quantity == 0) {
            available = false;
        }
    }
}
