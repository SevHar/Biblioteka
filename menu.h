#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <vector>
#include "user.h"
#include "admin.h"
#include "book.h"

class Menu : public QWidget {
    Q_OBJECT

public:
    Menu(QWidget *parent = nullptr);
    void updateItemList(const std::vector<Book>& books, const std::vector<Magazine>& magazines);
    void updateBorrowedList(const std::vector<Book>& borrowedBooks, const std::vector<Magazine>& borrowedMagazines);
    void updateUsersList(const std::vector<User>& users);
    void updateAdminList(const std::vector<Admin>& admins);
    void updateBorrowedItemsList(const std::vector<Book>& borrowedBooks, const std::vector<Magazine>& borrowedMagazines, const QString& username = QString());
    bool isBookAlreadyBorrowed(const Book& book) const;
    bool isMagazineAlreadyBorrowed(const Magazine& magazine) const;
    void checkStatus();

private slots:
    void borrowItem();
    void returnItem();
    void editItem();
    void addItem();
    void removeItem();
    void addUser();
    void removeUser();
    void login();
    void logout();
    void updateLoginStatus(bool isLoggedIn, const QString &username);
    void handleUserListItemClicked(QListWidgetItem *item);

signals:
    void borrowButtonClicked(int index);
    void returnButtonClicked(int index);
    void addBookButtonClicked(const QString &title, const QString &author, int year, int quantity);
    void removeBookButtonClicked(const QString &title);
    void addUserButtonClicked(const QString &username, const QString &password);
    void removeUserButtonClicked(const QString &username);

private:
    void setupUI();
    void setupConnections();
    void handleBookListItemClicked(QListWidgetItem *item);

    QListWidget *bookListWidget;
    QListWidget *borrowedListWidget;
    QListWidget *usersListWidget;
    QListWidget *borrowedItemsByUserListWidget;

    QPushButton *borrowButton;
    QPushButton *checkStatusButton;
    QPushButton *returnButton;
    QPushButton *editItemButton;
    QPushButton *addItemButton;
    QPushButton *removeItemButton;
    QPushButton *addUserButton;
    QPushButton *removeUserButton;
    QPushButton *loginButton;
    QPushButton *logoutButton;

    QLabel *loginStatusLabel;
    QLabel *usersLabel;
    QLabel *usersItemsLabel;

    QLineEdit *loginUsernameEdit;
    QLineEdit *loginPasswordEdit;

    std::vector<User> allUsers;
    std::vector<Admin> allAdmins;
    std::vector<Book> allBooks;
    std::vector<Magazine> allMagazines;

    User* currentUser;
    bool isAdminLoggedIn;

    Book* selectedBook;
    Magazine* selectedMagazine;
};

Q_DECLARE_METATYPE(Book*)

#endif
