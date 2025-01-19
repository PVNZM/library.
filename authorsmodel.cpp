#include "authorsmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


AuthorsModel::AuthorsModel(QObject *parent) : QAbstractListModel(parent) {
    loadAuthors(); // Загружаем данные при создании
}

int AuthorsModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return authors.count();
}

QVariant AuthorsModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= authors.size())
        return QVariant();

    const Author &author = authors.at(index.row());

    switch (role) {
    case AuthorIdRole:
        return author.id;
    case NameRole:
        return author.name;
    case BirthDateRole:
        return author.birthDate;
    case Qt::DisplayRole:
        return QStringLiteral("%1 (%2)").arg(author.name).arg(author.birthDate);
    default:
        return QVariant();
    }
}


void AuthorsModel::loadAuthors() {
    beginResetModel(); // Очищаем текущую модель
    authors.clear(); // Очищаем вектор авторов

    QSqlQuery query("SELECT * FROM Authors"); // Запрос к базе данных
    while (query.next()) {
        Author author; // Предполагается, что есть класс или структура Author
        author.id = query.value("author_id").toInt(); // Получаем ID автора
        author.name = query.value("name").toString(); // Получаем имя автора
        author.birthDate = query.value("birth_date").toString(); // Получаем дату рождения автора
        authors.append(author); // Добавляем автора в вектор
        qDebug() << "Автор загружен:" << author.name; // Вывод имени автора для отладки
    }

    endResetModel(); // Обновляем модель
    qDebug() << "Общее количество авторов:" << authors.count(); // Вывод общего количества авторов
}

void AuthorsModel::addAuthor(const QString &name, const QString &birthDate) {
    QSqlQuery query;
    query.prepare("INSERT INTO Authors (name, birth_date) VALUES (?, ?)");
    query.addBindValue(name);
    query.addBindValue(birthDate);

    if (!query.exec()) {
        qDebug() << "Ошибка добавления в базу данных:" << query.lastError().text();
        return;
    }

    Author newAuthor;
    newAuthor.id = query.lastInsertId().toInt();
    newAuthor.name = name;
    newAuthor.birthDate = birthDate;
    beginInsertRows(QModelIndex(), authors.size(), authors.size());
    authors.append(newAuthor);
    endInsertRows();
}

void AuthorsModel::updateAuthor(int id, const QString &name, const QString &birthDate) {
    for (int i = 0; i < authors.size(); ++i) {
        if (authors[i].id == id) {
            QSqlQuery query;
            query.prepare("UPDATE Authors SET name = ?, birth_date = ? WHERE author_id = ?");
            query.addBindValue(name);
            query.addBindValue(birthDate);
            query.addBindValue(id);

            if (!query.exec()) {
                qDebug() << "Ошибка обновления в базе данных:" << query.lastError().text();
                return;
            }

            authors[i].name = name;
            authors[i].birthDate = birthDate;
            emit dataChanged(index(i), index(i));
            return;
        }
    }
}

void AuthorsModel::removeAuthor(int id) {
    for (int i = 0; i < authors.size(); ++i) {
        if (authors[i].id == id) {
            QSqlQuery query;
            query.prepare("DELETE FROM Authors WHERE author_id = ?");
            query.addBindValue(id);

            if (!query.exec()) {
                qDebug() << "Ошибка удаления из базы данных:" << query.lastError().text();
                return;
            }

            beginRemoveRows(QModelIndex(), i, i);
            authors.removeAt(i);
            endRemoveRows();

            qDebug() << "Автор с ID" << id << "удален";
            return;
        }
    }

    qDebug() << "Автор с ID" << id << "не найден для удаления";
}
QHash<int, QByteArray> AuthorsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[AuthorIdRole] = "authorId";
    roles[NameRole] = "name";
    roles[BirthDateRole] = "birthDate";
    return roles;
}

