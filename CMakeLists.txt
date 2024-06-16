cmake_minimum_required(VERSION 3.0)

project(LibraryManagementSystem)

# Ustawienie ścieżki do plików nagłówkowych i źródłowych
set(HEADERS
    menu.h
    user.h
    admin.h
    book.h
    magazine.h
)

set(SOURCES
    menu.cpp
    user.cpp
    admin.cpp
    book.cpp
    magazine.cpp
    main.cpp
)

# Ustawienie ścieżki do plików UI Qt6
set(UI_FILES
    menu.ui
)

set(CMAKE_AUTOMOC ON)

# Wymagane moduły Qt6
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Dodanie wykonywalnego pliku
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${UI_FILES})

# Dodanie zależności do Qt6
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
