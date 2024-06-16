#ifndef BOOK_H
#define BOOK_H

    #include <string>

class Book {
public:
    Book() : year(0), available(false), quantity(0) {}
    Book(const std::string& title, const std::string& author, int year, bool available, int quantity);

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

    bool operator==(const Book& other) const {
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
