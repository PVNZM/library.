#include "booksmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

BooksModel::BooksModel(QObject *parent) : QAbstractListModel(parent) {
    loadBooks(); // Загружаем данные при создании
}

int BooksModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return books.count();
}

QVariant BooksModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= books.size())
        return QVariant();

    const Book &book = books.at(index.row());

    switch (role) {
    case BookIdRole:
        return book.id;
    case TitleRole:
        return book.title;
    case AuthorIdRole:
        return book.authorId;
    case CategoryIdRole:
        return book.categoryId;
    case PublicationYearRole:
        return book.publicationYear;
    case AvailableCopiesRole:
        return book.availableCopies;
    default:
        return QVariant();
    }
}

void BooksModel::loadBooks() {
    beginResetModel(); // Очищаем текущую модель
    books.clear(); // Очищаем вектор книг

    QSqlQuery query("SELECT * FROM Books"); // Запрос к базе данных
    while (query.next()) {
        Book book; // Создаем книгу
        book.id = query.value("book_id").toInt();
        book.title = query.value("title").toString();
        book.authorId = query.value("author_id").toInt();
        book.categoryId = query.value("category_id").toInt();
        book.publicationYear = query.value("publication_year").toInt();
        book.availableCopies = query.value("available_copies").toInt();
        books.append(book); // Добавляем книгу в вектор

        // Вывод информации о загруженной книге
        qDebug() << "Книга загружена:" << book.title;
    }

    endResetModel(); // Обновляем модель
}

QHash<int, QByteArray> BooksModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[BookIdRole] = "bookId";
    roles[TitleRole] = "title";
    roles[AuthorIdRole] = "authorId";
    roles[CategoryIdRole] = "categoryId";
    roles[PublicationYearRole] = "publicationYear";
    roles[AvailableCopiesRole] = "availableCopies";
    return roles;
}

void BooksModel::addBook(const QString &title, int authorId, int categoryId, int publicationYear, int availableCopies) {
    QSqlQuery query;
    query.prepare("INSERT INTO Books (title, author_id, category_id, publication_year, available_copies) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(title);
    query.addBindValue(authorId);
    query.addBindValue(categoryId);
    query.addBindValue(publicationYear);
    query.addBindValue(availableCopies);

    if (!query.exec()) {
        qDebug() << "Ошибка добавления книги:" << query.lastError().text();
        return;
    }

    loadBooks(); // Перезагрузка списка книг после добавления
}

void BooksModel::updateBook(int id, const QString &title, int authorId, int categoryId, int publicationYear, int availableCopies) {
    for (int i = 0; i < books.size(); ++i) {
        if (books[i].id == id) {
            QSqlQuery query;
            query.prepare("UPDATE Books SET title = ?, author_id = ?, category_id = ?, publication_year = ?, available_copies = ? WHERE book_id = ?");
            query.addBindValue(title);
            query.addBindValue(authorId);
            query.addBindValue(categoryId);
            query.addBindValue(publicationYear);
            query.addBindValue(availableCopies);
            query.addBindValue(id);

            if (!query.exec()) {
                qDebug() << "Ошибка обновления книги:" << query.lastError().text();
                return;
            }

            books[i].title = title;
            books[i].authorId = authorId;
            books[i].categoryId = categoryId;
            books[i].publicationYear = publicationYear;
            books[i].availableCopies = availableCopies;

            emit dataChanged(index(i), index(i)); // Обновление данных в модели
            return;
        }
    }
}

void BooksModel::removeBook(int id) {
    for (int i = 0; i < books.size(); ++i) {
        if (books[i].id == id) {
            QSqlQuery query;
            query.prepare("DELETE FROM Books WHERE book_id = ?");
            query.addBindValue(id);

            if (!query.exec()) {
                qDebug() << "Ошибка удаления книги:" << query.lastError().text();
                return;
            }

            beginRemoveRows(QModelIndex(), i, i);
            books.removeAt(i);
            endRemoveRows();
            qDebug() << "Книга с ID" << id << "удалена";
            return;
        }
    }

    qDebug() << "Книга с ID" << id << "не найдена для удаления";
}
