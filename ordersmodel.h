#ifndef ORDERSMODEL_H
#define ORDERSMODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QVector>

class Order {
public:
    int orderId;
    int readerId;
    int bookId;
    QDate orderDate;
    QDate returnDate;

    Order(int id, int rId, int bId, const QDate& oDate, const QDate& rDate = QDate())
        : orderId(id), readerId(rId), bookId(bId), orderDate(oDate), returnDate(rDate) {}
};

class OrdersModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit OrdersModel(QObject *parent = nullptr);
    ~OrdersModel();

    enum OrderRoles {
        OrderIdRole = Qt::UserRole + 1,
        ReaderIdRole,
        BookIdRole,
        OrderDateRole,
        ReturnDateRole
    };
    Q_INVOKABLE void loadOrders(); // Метод для загрузки заказов из базы данных
public slots:
    void addOrder(int readerId, int bookId, const QDate &orderDate, const QDate &returnDate); // Метод для добавления нового заказа
    void removeOrder(int orderId); // Метод для удаления заказа по ID
    void updateOrder(int orderId, int readerId, int bookId, const QString &orderDate, const QString &returnDate);

    // Переопределенные методы из QAbstractListModel
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<Order> orders; // Список заказов
};

#endif // ORDERSMODEL_H

