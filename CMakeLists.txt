cmake_minimum_required(VERSION 3.16)

project(DatabaseApp VERSION 0.1 LANGUAGES CXX)

# Убедитесь, что у вас включены все необходимые модули
set(CMAKE_CXX_STANDARD 17)  # Укажите стандарт C++

find_package(Qt6 REQUIRED COMPONENTS Quick Sql)  # Убедитесь, что используете нужные модули

# Настройка основного исполняемого файла
qt_standard_project_setup(REQUIRES 6.5)

# Добавление исполняемого файла
qt_add_executable(DatabaseApp
    main.cpp
    datamodel.cpp
)

# Добавление модуля QML и указание ресурсов
qt_add_qml_module(DatabaseApp
    URI DatabaseApp
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        datamodel.h
        SOURCES authorsmodel.h authorsmodel.cpp
        SOURCES categoriesmodel.h categoriesmodel.cpp
        SOURCES booksmodel.h booksmodel.cpp
        SOURCES ordersmodel.h ordersmodel.cpp
)
# Установка свойств исполняемого файла
set_target_properties(DatabaseApp PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Линковка с нужными библиотеками
target_link_libraries(DatabaseApp
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Sql
)

# Установка для сборки
include(GNUInstallDirs)
install(TARGETS DatabaseApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
