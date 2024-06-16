#ifndef MAGAZINE_H
#define MAGAZINE_H

#include <string>

class Magazine {
public:
    Magazine() : year(0), available(false), quantity(0) {}
    Magazine(const std::string& title, const std::string& author, int year, bool available, int quantity);

    std::string getTitle() const;
    std::string getAuthor() const;
    int getYear() const;
    bool isAvailable() const;
    int getQuantity() const;

    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setYear(int year);
    void setAvailable(bool available);
    void setQuantity(int quantity);
    void decreaseQuantity();

    bool operator==(const Magazine& other) const {
        return title == other.title && author == other.author && year == other.year;
    }

private:
    std::string title;
    std::string author;
    int year;
    bool available;
    int quantity;
};

#endif
