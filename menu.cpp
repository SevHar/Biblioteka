#include "menu.h"
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>

Menu::Menu(QWidget *parent) : QWidget(parent), isAdminLoggedIn(false) {
    setupUI();
    setupConnections();

    allUsers = {
        User("Jakub Wilkosz", "pass"),
        User("Mateusz Wałek", "pass2"),
    };
    updateUsersList(allUsers);

    allAdmins = {
        Admin("admin", "admin"),
        Admin("admin2", "admin2")
    };
    updateAdminList(allAdmins);

    allBooks = {
        Book("Harry Potter i Komnata Tajemnic", "J.K. Rowling", 1997, true, 5),
        Book("Zabić drozdza", "Harper Lee", 1960, true, 1),
        Book("Wielki Gatsby", "F. Scott Fitzgerald", 1925, false, 0)
    };

    allMagazines = {
        Magazine("National Geographic", "Various", 2024, true, 10),
        Magazine("Time", "Various", 2024, true, 8)
    };
    updateItemList(allBooks, allMagazines);
}

void Menu::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    QHBoxLayout *booksLayout = new QHBoxLayout;
    QVBoxLayout *borrowedLayout = new QVBoxLayout;
    QHBoxLayout *borrowedButtonsLayout = new QHBoxLayout;
    QVBoxLayout *usersLayout = new QVBoxLayout;
    QHBoxLayout *usersButtonsLayout = new QHBoxLayout;

    bookListWidget = new QListWidget;
    bookListWidget->setMinimumSize(800, 160);
    booksLayout->addWidget(bookListWidget);

    borrowButton = new QPushButton("Borrow");
    borrowButton->setEnabled(false);
    checkStatusButton = new QPushButton("Check for correctness");
    checkStatusButton->setEnabled(false);
    checkStatusButton->setVisible(false);
    editItemButton = new QPushButton("Edit", this);
    editItemButton->setVisible(false);
    addItemButton = new QPushButton("Add Item");
    addItemButton->setEnabled(false);
    addItemButton->setVisible(false);
    removeItemButton = new QPushButton("Remove Item");
    removeItemButton->setEnabled(false);
    removeItemButton->setVisible(false);
    borrowedButtonsLayout->addWidget(borrowButton);
    borrowedButtonsLayout->addWidget(checkStatusButton);
    borrowedButtonsLayout->addWidget(editItemButton);
    borrowedButtonsLayout->addWidget(addItemButton);
    borrowedButtonsLayout->addWidget(removeItemButton);
    borrowedLayout->addLayout(borrowedButtonsLayout);

    borrowedLayout->addWidget(new QLabel("Borrowed Items:"));
    borrowedListWidget = new QListWidget;
    borrowedListWidget->setMinimumSize(800, 160);
    borrowedLayout->addWidget(borrowedListWidget);

    returnButton = new QPushButton("Return");
    returnButton->setEnabled(false);
    borrowedLayout->addWidget(returnButton);

    usersLabel = new QLabel("Users");
    usersLabel->setVisible(false);
    usersLayout->addWidget(usersLabel);
    usersListWidget = new QListWidget;
    usersListWidget->setMinimumSize(800, 160);
    usersListWidget->setVisible(false);
    usersLayout->addWidget(usersListWidget);

    addUserButton = new QPushButton("Add User");
    addUserButton->setVisible(false);
    removeUserButton = new QPushButton("Remove User");
    removeUserButton->setVisible(false);
    usersButtonsLayout->addWidget(addUserButton);
    usersButtonsLayout->addWidget(removeUserButton);
    usersLayout->addLayout(usersButtonsLayout);

    usersItemsLabel = new QLabel("Borrowed items by: ");
    usersItemsLabel->setVisible(false);
    borrowedItemsByUserListWidget = new QListWidget(this);
    borrowedItemsByUserListWidget->setMinimumSize(800, 160);
    borrowedItemsByUserListWidget->setVisible(false);

    loginButton = new QPushButton("Login");
    loginButton->setFixedSize(80, 30);
    logoutButton = new QPushButton("Logout");
    logoutButton->setFixedSize(80, 30);
    logoutButton->setEnabled(false);
    loginStatusLabel = new QLabel("Not logged in");
    loginStatusLabel->setAlignment(Qt::AlignRight);

    QHBoxLayout *loginLayout = new QHBoxLayout;
    loginLayout->addWidget(loginButton);
    loginLayout->addWidget(logoutButton);
    loginLayout->addWidget(loginStatusLabel);

    mainLayout->addLayout(loginLayout);
    mainLayout->addLayout(booksLayout);
    mainLayout->addLayout(borrowedLayout);
    mainLayout->addLayout(usersLayout);
    mainLayout->addWidget(usersItemsLabel);
    mainLayout->addWidget(borrowedItemsByUserListWidget);

    setLayout(mainLayout);

    setWindowTitle("Library Management System");
}

void Menu::setupConnections() {
    connect(borrowButton, &QPushButton::clicked, this, &Menu::borrowItem);
    connect(returnButton, &QPushButton::clicked, this, &Menu::returnItem);
    connect(editItemButton, &QPushButton::clicked, this, &Menu::editItem);
    connect(addItemButton, &QPushButton::clicked, this, &Menu::addItem);
    connect(removeItemButton, &QPushButton::clicked, this, &Menu::removeItem);
    connect(addUserButton, &QPushButton::clicked, this, &Menu::addUser);
    connect(removeUserButton, &QPushButton::clicked, this, &Menu::removeUser);
    connect(loginButton, &QPushButton::clicked, this, &Menu::login);
    connect(logoutButton, &QPushButton::clicked, this, &Menu::logout);
    connect(checkStatusButton, &QPushButton::clicked, this, &Menu::checkStatus);
    connect(usersListWidget, &QListWidget::itemClicked, this, &Menu::handleUserListItemClicked);
}

void Menu::login() {
    QDialog *loginDialog = new QDialog(this);
    loginDialog->setWindowTitle("Login");

    QLabel *usernameLabel = new QLabel("Username:", loginDialog);
    QLineEdit *usernameEdit = new QLineEdit(loginDialog);
    QLabel *passwordLabel = new QLabel("Password:", loginDialog);
    QLineEdit *passwordEdit = new QLineEdit(loginDialog);
    passwordEdit->setEchoMode(QLineEdit::Password);
    QPushButton *loginButton = new QPushButton("Login", loginDialog);

    QVBoxLayout *layout = new QVBoxLayout(loginDialog);
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    loginDialog->setLayout(layout);

    connect(loginButton, &QPushButton::clicked, loginDialog, [=]() {
        QString username = usernameEdit->text();
        QString password = passwordEdit->text();

        bool loggedInAdmin = false;
        User* loggedInUserPtr = nullptr;
        for (auto& user : allUsers) {
            if (user.login(username.toStdString(), password.toStdString())) {
                loggedInUserPtr = &user;
                break;
            }
        }

        if (!loggedInUserPtr) {
            for (auto& admin : allAdmins) {
                if (admin.login(username.toStdString(), password.toStdString())) {
                    loggedInUserPtr = &admin;
                    loggedInAdmin = true;
                    break;
                }
            }
        }

        if (loggedInUserPtr) {
            currentUser = loggedInUserPtr;
            isAdminLoggedIn = loggedInAdmin;
            QMessageBox::information(loginDialog, "Login Successful", isAdminLoggedIn ? "You have been successfully logged in as an administrator." : "You have been successfully logged in as a regular user.");
            updateLoginStatus(true, username);
            updateBorrowedList(currentUser->getBorrowedBooks(), currentUser->getBorrowedMagazines());

            usersListWidget->clearSelection();
            bookListWidget->clearSelection();
            borrowedItemsByUserListWidget->clearSelection();

            loginDialog->accept();
        } else {
            QMessageBox::warning(loginDialog, "Login Failed", "Invalid username or password. Please try again.");
        }
    });

    loginDialog->exec();
}

void Menu::logout() {
    updateLoginStatus(false, QString::fromStdString(currentUser->getUsername()));

    loginStatusLabel->setText("Not logged in");
    bookListWidget->clearSelection();
    borrowedListWidget->clear();
    borrowedItemsByUserListWidget->clear();

    currentUser->logout();
    currentUser = nullptr;
}

void Menu::updateLoginStatus(bool loggedIn, const QString& username) {
    loginButton->setEnabled(!loggedIn);
    logoutButton->setEnabled(loggedIn);
    borrowButton->setEnabled(loggedIn);
    checkStatusButton->setEnabled(loggedIn && isAdminLoggedIn);
    addItemButton->setEnabled(loggedIn && isAdminLoggedIn);
    removeItemButton->setEnabled(loggedIn && isAdminLoggedIn);
    returnButton->setEnabled(loggedIn);
    loginStatusLabel->setText(loggedIn ? "Logged in as: " + username : "Not logged in");

    checkStatusButton->setVisible(loggedIn && isAdminLoggedIn);
    editItemButton->setVisible(loggedIn && isAdminLoggedIn);
    addItemButton->setVisible(loggedIn && isAdminLoggedIn);
    removeItemButton->setVisible(loggedIn && isAdminLoggedIn);
    usersLabel->setVisible(loggedIn && isAdminLoggedIn);
    usersListWidget->setVisible(loggedIn && isAdminLoggedIn);
    addUserButton->setVisible(loggedIn && isAdminLoggedIn);
    removeUserButton->setVisible(loggedIn && isAdminLoggedIn);
    usersItemsLabel->setVisible(loggedIn && isAdminLoggedIn);
    borrowedItemsByUserListWidget->setVisible(loggedIn && isAdminLoggedIn);
}

void Menu::borrowItem() {
    int selectedIndex = bookListWidget->currentRow();
    if (selectedIndex >= 0) {
        QListWidgetItem* selectedItem = bookListWidget->item(selectedIndex);
        QVariant data = selectedItem->data(Qt::UserRole);
        void* selectedItemPtr = reinterpret_cast<void*>(data.value<quintptr>());

        bool isBook = false;
        Book* selectedBook = nullptr;
        Magazine* selectedMagazine = nullptr;

        for (auto& book : allBooks) {
            if (reinterpret_cast<const void*>(&book) == selectedItemPtr) {
                selectedBook = &book;
                isBook = true;
                break;
            }
        }

        if (!isBook) {
            for (auto& magazine : allMagazines) {
                if (reinterpret_cast<const void*>(&magazine) == selectedItemPtr) {
                    selectedMagazine = &magazine;
                    break;
                }
            }
        }

        if (isBook && selectedBook && currentUser->isBookBorrowed(*selectedBook)) {
            QMessageBox::warning(this, "Item Already Borrowed", "The book \"" + QString::fromStdString(selectedBook->getTitle()) + "\" is already borrowed.");
            return;
        } else if (!isBook && selectedMagazine && currentUser->isMagazineBorrowed(*selectedMagazine)) {
            QMessageBox::warning(this, "Item Already Borrowed", "The magazine \"" + QString::fromStdString(selectedMagazine->getTitle()) + "\" is already borrowed.");
            return;
        }

        if (isBook && selectedBook && selectedBook->isAvailable()) {
            if (selectedBook->getQuantity() == 0) {
                selectedBook->setAvailable(false);
            }
            currentUser->borrowBook(*selectedBook);
            updateItemList(allBooks, allMagazines);
            updateBorrowedList(currentUser->getBorrowedBooks(), currentUser->getBorrowedMagazines());
            QMessageBox::information(this, "Item Borrowed", "You have successfully borrowed the book: " + QString::fromStdString(selectedBook->getTitle()));
        } else if (!isBook && selectedMagazine && selectedMagazine->isAvailable()) {
            if (selectedMagazine->getQuantity() == 0) {
                selectedMagazine->setAvailable(false);
            }
            currentUser->borrowMagazine(*selectedMagazine);
            updateItemList(allBooks, allMagazines);
            updateBorrowedList(currentUser->getBorrowedBooks(), currentUser->getBorrowedMagazines());
            QMessageBox::information(this, "Item Borrowed", "You have successfully borrowed the magazine: " + QString::fromStdString(selectedMagazine->getTitle()));
        } else {
            QMessageBox::warning(this, "Item Unavailable", "The item is currently unavailable.");
        }
    } else {
        QMessageBox::warning(this, "No Item Selected", "Please select an item to borrow.");
    }
}

bool Menu::isBookAlreadyBorrowed(const Book& book) const {
    if (currentUser) {
        return currentUser->isBookBorrowed(book);
    }
    return false;
}

bool Menu::isMagazineAlreadyBorrowed(const Magazine& magazine) const {
    if (currentUser) {
        return currentUser->isMagazineBorrowed(magazine);
    }
    return false;
}

void Menu::editItem() {
    int selectedIndex = bookListWidget->currentRow();
    if (selectedIndex >= 0) {
        QListWidgetItem *selectedItem = bookListWidget->currentItem();
        if (!selectedItem) {
            QMessageBox::warning(this, "Błąd", "Nie można pobrać wybranego elementu.");
            return;
        }

        QString selectedItemText = selectedItem->text();

        QRegularExpression reBook("^Book: (.+) by (.+?)(?: \\(\\d{4}\\))? - (Available|Not Available) \\((\\d+) available\\)$");
        QRegularExpression reMagazine("^Magazine: (.+) by (.+?)(?: \\(\\d{4}\\))? - (Available|Not Available) \\((\\d+) available\\)$");

        QRegularExpressionMatch matchBook = reBook.match(selectedItemText);
        QRegularExpressionMatch matchMagazine = reMagazine.match(selectedItemText);

        bool isBook = matchBook.hasMatch();
        bool isMagazine = matchMagazine.hasMatch();

        QDialog *editItemDialog = new QDialog(this);
        editItemDialog->setWindowTitle("Edytuj Książkę lub Magazyn");

        QLabel *titleLabel = new QLabel("Tytuł:", editItemDialog);
        QLineEdit *titleEdit = new QLineEdit(editItemDialog);
        QLabel *authorLabel = new QLabel("Autor:", editItemDialog);
        QLineEdit *authorEdit = new QLineEdit(editItemDialog);
        QLabel *yearLabel = new QLabel("Rok:", editItemDialog);
        QLineEdit *yearEdit = new QLineEdit(editItemDialog);
        QLabel *quantityLabel = new QLabel("Ilość:", editItemDialog);
        QLineEdit *quantityEdit = new QLineEdit(editItemDialog);

        QPushButton *saveButton = new QPushButton("Zapisz", editItemDialog);

        QVBoxLayout *layout = new QVBoxLayout(editItemDialog);
        layout->addWidget(titleLabel);
        layout->addWidget(titleEdit);
        layout->addWidget(authorLabel);
        layout->addWidget(authorEdit);
        layout->addWidget(yearLabel);
        layout->addWidget(yearEdit);
        layout->addWidget(quantityLabel);
        layout->addWidget(quantityEdit);
        layout->addWidget(saveButton);
        editItemDialog->setLayout(layout);

        QString oldTitle;
        QString oldAuthor;

        Book* editableBook = nullptr;
        Magazine* editableMagazine = nullptr;

        if (isBook) {
            oldTitle = matchBook.captured(1);
            oldAuthor = matchBook.captured(2);
            titleEdit->setText(oldTitle);
            authorEdit->setText(oldAuthor);
            yearEdit->setText("0");
            quantityEdit->setText(matchBook.captured(4));

            for (auto& book : allBooks) {
                if (QString::fromStdString(book.getTitle()) == oldTitle) {
                    editableBook = const_cast<Book*>(&book);
                    titleEdit->setText(QString::fromStdString(book.getTitle()));
                    authorEdit->setText(QString::fromStdString(book.getAuthor()));
                    yearEdit->setText(QString::number(book.getYear()));
                    quantityEdit->setText(QString::number(book.getQuantity()));
                    break;
                }
            }
        } else if (isMagazine) {
            oldTitle = matchMagazine.captured(1);
            oldAuthor = matchMagazine.captured(2);
            titleEdit->setText(oldTitle);
            authorEdit->setText(oldAuthor);
            yearEdit->setText("0");
            quantityEdit->setText(matchMagazine.captured(4));

            for (auto& magazine : allMagazines) {
                if (QString::fromStdString(magazine.getTitle()) == oldTitle) {
                    editableMagazine = const_cast<Magazine*>(&magazine);
                    titleEdit->setText(QString::fromStdString(magazine.getTitle()));
                    authorEdit->setText(QString::fromStdString(magazine.getAuthor()));
                    yearEdit->setText(QString::number(magazine.getYear()));
                    quantityEdit->setText(QString::number(magazine.getQuantity()));
                    break;
                }
            }
        }

        editItemDialog->show();

        connect(saveButton, &QPushButton::clicked, this, [=]() {
            QString newTitle = titleEdit->text();
            QString newAuthor = authorEdit->text();
            int newYear = yearEdit->text().toInt();
            int newQuantity = quantityEdit->text().toInt();

            if (newTitle.isEmpty() || newAuthor.isEmpty() || newYear <= 0 || newQuantity < 0) {
                QMessageBox::warning(editItemDialog, "Nieprawidłowe dane", "Wprowadź prawidłowe dane.");
                return;
            }

            if (isBook && editableBook) {
                editableBook->setTitle(newTitle.toStdString());
                editableBook->setAuthor(newAuthor.toStdString());
                editableBook->setYear(newYear);
                editableBook->setQuantity(newQuantity);

                for (auto& user : allUsers) {
                    for (const auto& borrowedBook : user.getBorrowedBooks()) {
                        if (borrowedBook.getTitle() == oldTitle.toStdString() && borrowedBook.getAuthor() == oldAuthor.toStdString()) {
                            const_cast<Book&>(borrowedBook).setTitle(newTitle.toStdString());
                            const_cast<Book&>(borrowedBook).setAuthor(newAuthor.toStdString());
                            const_cast<Book&>(borrowedBook).setYear(newYear);
                            const_cast<Book&>(borrowedBook).setQuantity(newQuantity);
                        }
                    }
                }
                for (auto& admin : allAdmins) {
                    for (const auto& borrowedBook : admin.getBorrowedBooks()) {
                        if (borrowedBook.getTitle() == oldTitle.toStdString() && borrowedBook.getAuthor() == oldAuthor.toStdString()) {
                            const_cast<Book&>(borrowedBook).setTitle(newTitle.toStdString());
                            const_cast<Book&>(borrowedBook).setAuthor(newAuthor.toStdString());
                            const_cast<Book&>(borrowedBook).setYear(newYear);
                            const_cast<Book&>(borrowedBook).setQuantity(newQuantity);
                        }
                    }
                }
            } else if (isMagazine && editableMagazine) {
                editableMagazine->setTitle(newTitle.toStdString());
                editableMagazine->setAuthor(newAuthor.toStdString());
                editableMagazine->setYear(newYear);
                editableMagazine->setQuantity(newQuantity);

                for (auto& user : allUsers) {
                    for (const auto& borrowedMagazine : user.getBorrowedMagazines()) {
                        if (borrowedMagazine.getTitle() == oldTitle.toStdString() && borrowedMagazine.getAuthor() == oldAuthor.toStdString()) {
                            const_cast<Magazine&>(borrowedMagazine).setTitle(newTitle.toStdString());
                            const_cast<Magazine&>(borrowedMagazine).setAuthor(newAuthor.toStdString());
                            const_cast<Magazine&>(borrowedMagazine).setYear(newYear);
                            const_cast<Magazine&>(borrowedMagazine).setQuantity(newQuantity);
                        }
                    }
                }
                for (auto& admin : allAdmins) {
                    for (const auto& borrowedMagazine : admin.getBorrowedMagazines()) {
                        if (borrowedMagazine.getTitle() == oldTitle.toStdString() && borrowedMagazine.getAuthor() == oldAuthor.toStdString()) {
                            const_cast<Magazine&>(borrowedMagazine).setTitle(newTitle.toStdString());
                            const_cast<Magazine&>(borrowedMagazine).setAuthor(newAuthor.toStdString());
                            const_cast<Magazine&>(borrowedMagazine).setYear(newYear);
                            const_cast<Magazine&>(borrowedMagazine).setQuantity(newQuantity);
                        }
                    }
                }
            }

            updateItemList(allBooks, allMagazines);
            updateBorrowedList(currentUser->getBorrowedBooks(), currentUser->getBorrowedMagazines());
            for(auto& user : allUsers) {
                updateBorrowedItemsList(user.getBorrowedBooks(), user.getBorrowedMagazines());
            }
            for(auto& admin : allAdmins) {
                updateBorrowedItemsList(admin.getBorrowedBooks(), admin.getBorrowedMagazines());
            }

            editItemDialog->accept();
        });
    } else {
        QMessageBox::warning(this, "Brak wybranego elementu", "Proszę wybrać element do edycji.");
    }
}

void Menu::addItem() {
    QDialog *addBookDialog = new QDialog(this);
    addBookDialog->setWindowTitle("Add Book or Magazine");

    QLabel *titleLabel = new QLabel("Title:", addBookDialog);
    QLineEdit *titleEdit = new QLineEdit(addBookDialog);
    QLabel *authorLabel = new QLabel("Author:", addBookDialog);
    QLineEdit *authorEdit = new QLineEdit(addBookDialog);
    QLabel *yearLabel = new QLabel("Year:", addBookDialog);
    QLineEdit *yearEdit = new QLineEdit(addBookDialog);
    QLabel *quantityLabel = new QLabel("Quantity:", addBookDialog);
    QLineEdit *quantityEdit = new QLineEdit(addBookDialog);
    QLabel *typeLabel = new QLabel("Type:", addBookDialog);
    QComboBox *typeComboBox = new QComboBox(addBookDialog);
    typeComboBox->setStyleSheet("QComboBox { padding: 5px; }");
    typeComboBox->addItem("Book");
    typeComboBox->addItem("Magazine");

    QPushButton *addButton = new QPushButton("Add", addBookDialog);

    QVBoxLayout *layout = new QVBoxLayout(addBookDialog);
    layout->addWidget(titleLabel);
    layout->addWidget(titleEdit);
    layout->addWidget(authorLabel);
    layout->addWidget(authorEdit);
    layout->addWidget(yearLabel);
    layout->addWidget(yearEdit);
    layout->addWidget(quantityLabel);
    layout->addWidget(quantityEdit);
    layout->addWidget(typeLabel);
    layout->addWidget(typeComboBox);
    layout->addWidget(addButton);
    addBookDialog->setLayout(layout);

    addBookDialog->show();

    connect(addButton, &QPushButton::clicked, this, [=]() {
        QString title = titleEdit->text();
        QString author = authorEdit->text();
        int year = yearEdit->text().toInt();
        int quantity = quantityEdit->text().toInt();
        QString type = typeComboBox->currentText();

        if (title.isEmpty() || author.isEmpty() || year <= 0 || quantity <= 0) {
            QMessageBox::warning(addBookDialog, "Invalid Input", "Please enter valid details.");
            return;
        }

        bool duplicateItem = false;
        if (type == "Book") {
            for (const auto& book : allBooks) {
                if (QString::fromStdString(book.getTitle()) == title) {
                    duplicateItem = true;
                    break;
                }
            }
        } else if (type == "Magazine") {
            for (const auto& magazine : allMagazines) {
                if (QString::fromStdString(magazine.getTitle()) == title) {
                    duplicateItem = true;
                    break;
                }
            }
        }

        if (duplicateItem) {
            QMessageBox::warning(addBookDialog, "Duplicate Item", "The item \"" + title + "\" already exists in the library.");
            return;
        }

        bool available = quantity > 0;

        if (type == "Book") {
            Book newBook(title.toStdString(), author.toStdString(), year, available, quantity);
            allBooks.push_back(newBook);
        } else if (type == "Magazine") {
            Magazine newMagazine(title.toStdString(), author.toStdString(), year, available, quantity);
            allMagazines.push_back(newMagazine);
        }

        updateItemList(allBooks, allMagazines);

        addBookDialog->accept();
    });
}

void Menu::removeItem() {
    int selectedIndex = bookListWidget->currentRow();
    if (selectedIndex >= 0) {
        QVariant data = bookListWidget->item(selectedIndex)->data(Qt::UserRole);
        void* selectedItem = reinterpret_cast<void*>(data.value<quintptr>());

        bool itemFound = false;

        for (auto it = allBooks.begin(); it != allBooks.end(); ++it) {
            if (reinterpret_cast<void*>(&(*it)) == selectedItem) {
                if (currentUser->isBookBorrowed(*it)) {
                    QMessageBox::warning(this, "Cannot Remove Item", "The book \"" + QString::fromStdString(it->getTitle()) + "\" is currently borrowed by you and cannot be removed.");
                    return;
                }

                for (const auto& user : allUsers) {
                    if (user.getUsername() != currentUser->getUsername() && user.isBookBorrowed(*it)) {
                        QMessageBox::warning(this, "Cannot Remove Item", "The book \"" + QString::fromStdString(it->getTitle()) + "\" is currently borrowed and cannot be removed.");
                        return;
                    }
                }

                allBooks.erase(it);
                itemFound = true;
                break;
            }
        }

        if (!itemFound) {
            for (auto it = allMagazines.begin(); it != allMagazines.end(); ++it) {
                if (reinterpret_cast<void*>(&(*it)) == selectedItem) {
                    if (currentUser->isMagazineBorrowed(*it)) {
                        QMessageBox::warning(this, "Cannot Remove Item", "The magazine \"" + QString::fromStdString(it->getTitle()) + "\" is currently borrowed by you and cannot be removed.");
                        return;
                    }

                    for (const auto& user : allUsers) {
                        if (user.getUsername() != currentUser->getUsername() && user.isMagazineBorrowed(*it)) {
                            QMessageBox::warning(this, "Cannot Remove Item", "The magazine \"" + QString::fromStdString(it->getTitle()) + "\" is currently borrowed and cannot be removed.");
                            return;
                        }
                    }

                    allMagazines.erase(it);
                    break;
                }
            }
        }

        updateItemList(allBooks, allMagazines);
    } else {
        QMessageBox::warning(this, "No Item Selected", "Please select an item to remove.");
    }
}

void Menu::returnItem() {
    int selectedIndex = borrowedListWidget->currentRow();

    if (selectedIndex < 0) {
        QMessageBox::warning(this, "No Item Selected", "Please select an item to return.");
        return;
    }

    QListWidgetItem* selectedItem = borrowedListWidget->item(selectedIndex);
    QString itemText = selectedItem->text();

    QString title;
    bool isBook = false;

    if (itemText.startsWith("Book: ")) {
        title = itemText.mid(6);
        isBook = true;
    } else if (itemText.startsWith("Magazine: ")) {
        title = itemText.mid(10);
    }

    int byIndex = title.indexOf(" by ");
    if (byIndex != -1) {
        title = title.left(byIndex);
    }

    bool success = false;

    if (currentUser) {
        if (isBook) {
            const std::vector<Book>& borrowedBooks = currentUser->getBorrowedBooks();
            auto it = std::find_if(borrowedBooks.begin(), borrowedBooks.end(), [&](const Book& book) {
                return QString::fromStdString(book.getTitle()) == title.trimmed();
            });
            if (it != borrowedBooks.end()) {
                success = currentUser->returnBook(const_cast<Book&>(*it), allBooks);
            }
        } else {
            const std::vector<Magazine>& borrowedMagazines = currentUser->getBorrowedMagazines();
            auto it = std::find_if(borrowedMagazines.begin(), borrowedMagazines.end(), [&](const Magazine& magazine) {
                return QString::fromStdString(magazine.getTitle()) == title.trimmed();
            });
            if (it != borrowedMagazines.end()) {
                success = currentUser->returnMagazine(const_cast<Magazine&>(*it), allMagazines);
            }
        }
    }

    if (success) {
        borrowedListWidget->takeItem(selectedIndex);
        updateItemList(allBooks, allMagazines);
        updateBorrowedList(currentUser->getBorrowedBooks(), currentUser->getBorrowedMagazines());
        QMessageBox::information(this, "Return Successful", "The item has been successfully returned.");
    } else {
        QMessageBox::warning(this, "Return Failed", "Failed to return the selected item.");
    }
}

void Menu::addUser() {
    QDialog *addUserDialog = new QDialog(this);
    addUserDialog->setWindowTitle("Add User");

    QLabel *usernameLabel = new QLabel("Username:", addUserDialog);
    QLineEdit *usernameEdit = new QLineEdit(addUserDialog);
    QLabel *passwordLabel = new QLabel("Password:", addUserDialog);
    QLineEdit *passwordEdit = new QLineEdit(addUserDialog);
    passwordEdit->setEchoMode(QLineEdit::Password);
    QPushButton *addButton = new QPushButton("Add", addUserDialog);

    QVBoxLayout *layout = new QVBoxLayout(addUserDialog);
    layout->addWidget(usernameLabel);
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);
    layout->addWidget(addButton);
    addUserDialog->setLayout(layout);

    connect(addButton, &QPushButton::clicked, addUserDialog, [=]() {
        QString username = usernameEdit->text();
        QString password = passwordEdit->text();

        if (username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(addUserDialog, "Invalid Input", "Please enter both username and password.");
            return;
        }

        for (const auto& user : allUsers) {
            if (QString::fromStdString(user.getUsername()) == username) {
                QMessageBox::warning(addUserDialog, "Duplicate User", "The user \"" + username + "\" already exists.");
                return;
            }
        }

        allUsers.push_back(User(username.toStdString(), password.toStdString()));
        updateUsersList(allUsers);
        addUserDialog->accept();
    });

    addUserDialog->exec();
}

void Menu::removeUser() {
    int selectedIndex = usersListWidget->currentRow();

    if (selectedIndex >= 0) {
        QString username = usersListWidget->item(selectedIndex)->text();

        auto it = std::find_if(allUsers.begin(), allUsers.end(), [&](const User& user) {
            return QString::fromStdString(user.getUsername()) == username;
        });

        if (it != allUsers.end()) {
            if (it->getUsername() == currentUser->getUsername()) {
                QMessageBox::warning(this, "Cannot Remove User", "You cannot remove the currently logged in user.");
                return;
            }

            for (const auto& book : it->getBorrowedBooks()) {
                auto bookIt = std::find(allBooks.begin(), allBooks.end(), book);
                if (bookIt != allBooks.end()) {
                    bookIt->setQuantity(bookIt->getQuantity() + 1);
                    if (bookIt->getQuantity() == 1) {
                        bookIt->setAvailable(true);
                    }
                }
            }

            for (const auto& magazine : it->getBorrowedMagazines()) {
                auto magazineIt = std::find(allMagazines.begin(), allMagazines.end(), magazine);
                if (magazineIt != allMagazines.end()) {
                    magazineIt->setQuantity(magazineIt->getQuantity() + 1);
                    if (magazineIt->getQuantity() == 1) {
                        magazineIt->setAvailable(true);
                    }
                }
            }

            allUsers.erase(it);
            updateUsersList(allUsers);
            updateItemList(allBooks, allMagazines);
        }
    } else {
        QMessageBox::warning(this, "No User Selected", "Please select a user to remove.");
    }
}

void Menu::updateItemList(const std::vector<Book>& books, const std::vector<Magazine>& magazines) {
    bookListWidget->clear();
    for (const auto& book : books) {
        QString itemText = QString::fromStdString("Book: " + book.getTitle() + " by " + book.getAuthor() +
                                                  " (" + std::to_string(book.getYear()) + ") - " +
                                                  (book.isAvailable() ? "Available" : "Not Available") +
                                                  " (" + std::to_string(book.getQuantity()) + " available)");
        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(&book)));
        bookListWidget->addItem(item);
    }
    for (const auto& magazine : magazines) {
        QString itemText = QString::fromStdString("Magazine: " + magazine.getTitle() + " by " + magazine.getAuthor() +
                                                  " (" + std::to_string(magazine.getYear()) + ") - " +
                                                  (magazine.isAvailable() ? "Available" : "Not Available") +
                                                  " (" + std::to_string(magazine.getQuantity()) + " available)");
        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(&magazine)));
        bookListWidget->addItem(item);
    }
}

void Menu::updateBorrowedList(const std::vector<Book>& borrowedBooks, const std::vector<Magazine>& borrowedMagazines) {
    borrowedListWidget->clear();

    for (const auto& book : borrowedBooks) {
        QString itemText = "Book: " + QString::fromStdString(book.getTitle()) + " by " + QString::fromStdString(book.getAuthor());
        QListWidgetItem *item = new QListWidgetItem(itemText, borrowedListWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(const_cast<Book*>(&book))));
    }

    for (const auto& magazine : borrowedMagazines) {
        QString itemText = "Magazine: " + QString::fromStdString(magazine.getTitle()) + " by " + QString::fromStdString(magazine.getAuthor());
        QListWidgetItem *item = new QListWidgetItem(itemText, borrowedListWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(const_cast<Magazine*>(&magazine))));
    }
}

void Menu::updateUsersList(const std::vector<User>& users) {
    usersListWidget->clear();

    for (const auto& user : users) {
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(user.getUsername()), usersListWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(reinterpret_cast<quintptr>(const_cast<User*>(&user))));
    }
}

void Menu::updateAdminList(const std::vector<Admin>& admins) {
    allAdmins = admins;
}

void Menu::checkStatus() {
    bool statusMatches = true;

    for (int i = 0; i < bookListWidget->count(); ++i) {
        QListWidgetItem* item = bookListWidget->item(i);
        QString itemText = item->text();

        if (itemText.startsWith("Book: ")) {
            QString title = itemText.mid(6);
            int byIndex = title.indexOf(" by ");
            if (byIndex != -1) {
                title = title.left(byIndex).trimmed();
            }

            auto it = std::find_if(allBooks.begin(), allBooks.end(), [&](const Book& book) {
                return QString::fromStdString(book.getTitle()) == title;
            });

            if (it == allBooks.end() || itemText.contains("Not Available") != !it->isAvailable()) {
                statusMatches = false;
                break;
            }
        } else if (itemText.startsWith("Magazine: ")) {
            QString title = itemText.mid(10);
            int byIndex = title.indexOf(" by ");
            if (byIndex != -1) {
                title = title.left(byIndex).trimmed();
            }

            auto it = std::find_if(allMagazines.begin(), allMagazines.end(), [&](const Magazine& magazine) {
                return QString::fromStdString(magazine.getTitle()) == title;
            });

            if (it == allMagazines.end() || itemText.contains("Not Available") != !it->isAvailable()) {
                statusMatches = false;
                break;
            }
        }
    }

    if (statusMatches) {
        QMessageBox::information(this, "Status Check", "All books and magazines are correctly listed.");
    } else {
        QMessageBox::warning(this, "Status Check", "There are discrepancies between the GUI and the available books/magazines.");
    }
}

void Menu::handleUserListItemClicked(QListWidgetItem *item) {
    if (!isAdminLoggedIn) return;

    QString username = item->text();
    for (const User &user : allUsers) {
        if (QString::fromStdString(user.getUsername()) == username) {
            updateBorrowedItemsList(user.getBorrowedBooks(), user.getBorrowedMagazines(), username);
            break;
        }
    }
}

void Menu::updateBorrowedItemsList(const std::vector<Book>& borrowedBooks, const std::vector<Magazine>& borrowedMagazines, const QString& username) {
    borrowedItemsByUserListWidget->clear();

    if (!username.isEmpty()) {
        usersItemsLabel->setText("Borrowed items by: " + username);
    } else {
        usersItemsLabel->setText("Borrowed items by: ");
    }

    for (const Book &book : borrowedBooks) {
        QString itemText = QString("Book: %1 by %2").arg(QString::fromStdString(book.getTitle())).arg(QString::fromStdString(book.getAuthor()));
        QListWidgetItem *item = new QListWidgetItem(itemText, borrowedItemsByUserListWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(&book));
    }

    for (const Magazine &magazine : borrowedMagazines) {
        QString itemText = QString("Magazine: %1 by %2").arg(QString::fromStdString(magazine.getTitle())).arg(QString::fromStdString(magazine.getAuthor()));
        QListWidgetItem *item = new QListWidgetItem(itemText, borrowedItemsByUserListWidget);
        item->setData(Qt::UserRole, QVariant::fromValue(&magazine));
    }
}
