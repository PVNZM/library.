#include "OrdersModel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

OrdersModel::OrdersModel(QObject *parent) : QAbstractListModel(parent) {
    loadOrders(); // Загружаем заказы при инициализации модели
}

OrdersModel::~OrdersModel() {
    // Освобождаем ресурсы, если необходимо
}

void OrdersModel::loadOrders() {
    QSqlQuery query("SELECT order_id, reader_id, book_id, order_date, return_date FROM Orders");

    // Очищаем текущее состояние заказа перед загрузкой
    orders.clear();

    while (query.next()) {
        int orderId = query.value(0).toInt();
        int readerId = query.value(1).toInt();
        int bookId = query.value(2).toInt();
        QDate orderDate = query.value(3).toDate();
        QDate returnDate = query.value(4).toDate();

        orders.append(Order(orderId, readerId, bookId, orderDate, returnDate));
    }

    // Уведомляем QML о том, что данные изменились
    beginResetModel();
    endResetModel();
}

void OrdersModel::updateOrder(int orderId, int readerId, int bookId, const QString &orderDateStr, const QString &returnDateStr) {
    QDate orderDate = QDate::fromString(orderDateStr, "yyyy-MM-dd");
    QDate returnDate = QDate::fromString(returnDateStr, "yyyy-MM-dd");

    for (int i = 0; i < orders.size(); ++i) {
        if (orders[i].orderId == orderId) {
            orders[i].readerId = readerId;
            orders[i].bookId = bookId;
            orders[i].orderDate = orderDate;   // Обновление даты заказа
            orders[i].returnDate = returnDate; // Обновление даты возврата

            emit dataChanged(index(i), index(i)); // Уведомление об изменении данных
            break;
        }
    }
}



void OrdersModel::addOrder(int readerId, int bookId, const QDate &orderDate, const QDate &returnDate) {
    QSqlQuery query;

    // Подготовка SQL-запроса для добавления заказа
    query.prepare("INSERT INTO Orders (reader_id, book_id, order_date, return_date) "
                  "VALUES (:reader_id, :book_id, :order_date, :return_date)");

    query.bindValue(":reader_id", readerId);
    query.bindValue(":book_id", bookId);
    query.bindValue(":order_date", orderDate);
    query.bindValue(":return_date", returnDate);

    if (!query.exec()) {
        qDebug() << "Ошибка при добавлении заказа:" << query.lastError().text();
    } else {
        // Если заказ был успешно добавлен, можно добавить его в локальный список
        int orderId = query.lastInsertId().toInt(); // Получить последний вставленный ID
        orders.append(Order(orderId, readerId, bookId, orderDate, returnDate));
        QModelIndex index = createIndex(orders.size() - 1, 0);
        emit dataChanged(index, index); // Уведомить об изменении данных
    }
}

void OrdersModel::removeOrder(int orderId) {
    // Найти индекс такого заказа, который мы хотим удалить
    auto it = std::find_if(orders.begin(), orders.end(), [orderId](const Order &order) {
        return order.orderId == orderId;
    });

    if (it != orders.end()) {
        QSqlQuery query;
        query.prepare("DELETE FROM Orders WHERE order_id = :order_id");
        query.bindValue(":order_id", orderId);

        if (query.exec()) {
            int index = std::distance(orders.begin(), it);
            orders.erase(it); // Удаляем заказ из локального списка
            beginRemoveRows(QModelIndex(), index, index);
            endRemoveRows();
        } else {
            qDebug() << "Ошибка при удалении заказа:" << query.lastError().text();
        }
    } else {
        qDebug() << "Заказ с ID" << orderId << "не найден.";
    }
}

int OrdersModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return orders.size();
}

QVariant OrdersModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= orders.size())
        return QVariant();

    const Order &order = orders[index.row()];
    switch (role) {
    case OrderIdRole:
        return order.orderId;
    case ReaderIdRole:
        return order.readerId;
    case BookIdRole:
        return order.bookId;

    case OrderDateRole:
        return order.orderDate;
    case ReturnDateRole:
        return order.returnDate;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> OrdersModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[OrderIdRole] = "orderId";
    roles[ReaderIdRole] = "readerId";
    roles[BookIdRole] = "bookId";
    roles[OrderDateRole] = "orderDate";
    roles[ReturnDateRole] = "returnDate";
    return roles;
}

