#include "datamodel.h"

#include <QSqlQuery>   // Для выполнения запросов к базе данных
#include <QSqlError>   // Для обработки ошибок
#include <QDebug>      // Для вывода отладочной информации

// Реализация конструктора
DataModel::DataModel(QObject *parent) : QAbstractListModel(parent) {
    loadReaders(); // Загружаем данные из базы при создании класса
}

// Реализация метода загрузки читателей
void DataModel::loadReaders() {
    beginResetModel(); // Очищаем текущую модель
    readers.clear(); // Очищаем вектор читателей


    QSqlQuery query("SELECT reader_id, name, email, registration_date FROM readers");
    while (query.next()) {
        Reader reader;
        reader.id = query.value("reader_id").toInt();
        reader.name = query.value("name").toString();
        reader.email = query.value("email").toString();
        reader.registrationDate = query.value("registration_date").toString();
        readers.append(reader);
        qDebug() << "Читатель загружен:" << reader.name; // Для отладки
    }

    endResetModel(); // Обновляем модель
    qDebug() << "Общее количество читателей:" << readers.count(); // Для отладки
}

//qDebug() << "Загружено читателей:" << readers.size(); // Отладочное сообщение
// Реализация метода редактирования читателя
void DataModel::updateReader(int id, const QString &name, const QString &email, const QString &registrationDate) {
    for (int i = 0; i < readers.size(); ++i) {
        if (readers[i].id == id) {
            // Обновляем данные как в базе данных, так и в списке
            QSqlQuery query;
            query.prepare("UPDATE Readers SET name = ?, email = ?, registration_date = ? WHERE reader_id = ?");
            query.addBindValue(name);
            query.addBindValue(email);
            query.addBindValue(registrationDate);
            query.addBindValue(id);

            if (!query.exec()) {
                qDebug() << "Ошибка обновления в базе данных:" << query.lastError().text();
                return; // Если возникает ошибка, выходим из метода
            }

            // Обновляем данные в списке
            readers[i].name = name;
            readers[i].email = email;
            readers[i].registrationDate = registrationDate;
            emit dataChanged(index(i), index(i)); // Уведомляем о изменении данных
            return;
        }
    }
}
//новое. добавление
void DataModel::addReader(const QString &name, const QString &email, const QString &registrationDate) {
    QSqlQuery query;
    query.prepare("INSERT INTO Readers (name, email, registration_date) VALUES (?, ?, ?)");
    query.addBindValue(name);
    query.addBindValue(email);
    query.addBindValue(registrationDate);

    if (!query.exec()) {
        qDebug() << "Ошибка добавления в базу данных:" << query.lastError().text();
        return;
    }

    // Добавляем объект в список
    Reader newReader;
    newReader.id = query.lastInsertId().toInt(); // Получаем ID нового читателя
    newReader.name = name;
    newReader.email = email;
    newReader.registrationDate = registrationDate;
    beginInsertRows(QModelIndex(), readers.size(), readers.size());
    readers.append(newReader);
    endInsertRows();
}

void DataModel::removeReader(int id) {
    for (int i = 0; i < readers.size(); ++i) {
        if (readers[i].id == id) {
            QSqlQuery query;
            query.prepare("DELETE FROM Readers WHERE reader_id = ?");
            query.addBindValue(id);

            if (!query.exec()) {
                qDebug() << "Ошибка удаления из базы данных:" << query.lastError().text();
                return;
            }

            // Сначала удаляем читателя из модели
            beginRemoveRows(QModelIndex(), i, i);
            readers.removeAt(i);
            endRemoveRows();

            qDebug() << "Читатель с ID" << id << "удален"; // Для отладки
            return;
        }
    }

    qDebug() << "Читатель с ID" << id << "не найден для удаления"; // Если ID не найден
}



// Реализация метода rowCount
int DataModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return readers.size();
}

// Реализация метода data
QVariant DataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= readers.size())
        return QVariant();

    const Reader &reader = readers[index.row()];
    switch (role) {
    case IdRole: return reader.id;
    case NameRole: return reader.name;
    case EmailRole: return reader.email;
    case RegistrationDateRole: return reader.registrationDate;
    default: return QVariant();
    }
}



// Реализация метода roleNames
QHash<int, QByteArray> DataModel::roleNames() const {
    QHash<int, QByteArray> roles;

    roles[IdRole] = "readerId";
    roles[NameRole] = "name";
    roles[EmailRole] = "email";
    roles[RegistrationDateRole] = "registrationDate";
    return roles;
}
