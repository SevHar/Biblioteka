#include "magazine.h"

Magazine::Magazine(const std::string& title, const std::string& author, int year, bool available, int quantity)
    : title(title), author(author), year(year), available(available), quantity(quantity) {}

std::string Magazine::getTitle() const {
    return title;
}

std::string Magazine::getAuthor() const {
    return author;
}

int Magazine::getYear() const {
    return year;
}

bool Magazine::isAvailable() const {
    return available;
}

int Magazine::getQuantity() const {
    return quantity;
}

// Settery
void Magazine::setTitle(const std::string& title) {
    this->title = title;
}

void Magazine::setAuthor(const std::string& author) {
    this->author = author;
}

void Magazine::setYear(int year) {
    this->year = year;
}

void Magazine::setAvailable(bool available) {
    this->available = available;
}

void Magazine::setQuantity(int quantity) {
    this->quantity = quantity;
}

void Magazine::decreaseQuantity() {
    if (quantity > 0) {
        quantity--;
        if (quantity == 0) {
            available = false;
        }
    }
}
