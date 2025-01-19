#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QQmlContext> // Убедитесь, что ваш заголовок подключен
#include <QSqlQuery>
#include "datamodel.h" // Подключение заголовочного файла DataModel
#include <authorsmodel.h>
#include <categoriesmodel.h>
#include <BooksModel.h>
#include <ordersmodel.h>
void connectToDatabase() {
    QSqlDatabase db;

    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
        qDebug() << "Используется существующее подключение.";
    } else {
        db = QSqlDatabase::addDatabase("QPSQL");
        db.setDatabaseName("LibraryDb"); // Имя вашей базы данных
        db.setUserName("postgres"); // Имя пользователя
        db.setPassword("123"); // Пароль пользователя
        db.setHostName("localhost"); // Адрес сервера
        db.setPort(5432); // Порт по умолчанию для PostgreSQL

        if (!db.open()) {
            qDebug() << "Ошибка подключения к базе данных:" << db.lastError().text();
            return;
        }
        qDebug() << "Подключение к базе данных успешно";
    }
}

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    connectToDatabase();

    QQmlApplicationEngine engine;
    // Регистрация вашей модели для использования в QML
    qmlRegisterType<CategoriesModel>("DatabaseApp", 1, 0, "CategoriesModel");
    qmlRegisterType<OrdersModel>("DatabaseApp", 1, 0, "OrdersModel");
    qmlRegisterType<BooksModel>("DatabaseApp", 1, 0, "BooksModel");
    qmlRegisterType<DataModel>("DatabaseApp", 1, 0, "DataModel");
    qmlRegisterType<AuthorsModel>("DatabaseApp", 1, 0, "AuthorsModel"); // Регистрация новой модели
    const QUrl url(QStringLiteral("file:///C:/Users/admin/Documents/untitled7/Main.qml"));
    engine.load(url);

    return app.exec();
}
