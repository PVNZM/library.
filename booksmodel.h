#ifndef BOOKSMODEL_H
#define BOOKSMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QString>
// Определение структуры Book
struct Book {
    int id;
    QString title;
    int authorId;
    int categoryId;
    int publicationYear;
    int availableCopies;
};

class BooksModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Roles {
        BookIdRole = Qt::UserRole + 1,
        TitleRole,
        AuthorIdRole,
        CategoryIdRole,
        PublicationYearRole,
        AvailableCopiesRole
    };

    explicit BooksModel(QObject *parent = nullptr);
    void loadBooks();

    // Реализация методов QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void addBook(const QString &title, int authorId, int categoryId, int publicationYear, int availableCopies);
    void updateBook(int id, const QString &title, int authorId, int categoryId, int publicationYear, int availableCopies);
    void removeBook(int id);

private:
    QList<Book> books;
};

#endif // BOOKSMODEL_H

