#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QSqlQuery>
#include <QVariant>
#include <QString>

class Reader {
public:
    int id;
    QString name;
    QString email;
    QString registrationDate; // Форматированная строка для даты
};

class DataModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        EmailRole,
        RegistrationDateRole
    };

    Q_INVOKABLE void loadReaders();
    explicit DataModel(QObject *parent = nullptr);
    // Сделаем метод доступным для QML
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
public slots:
    Q_INVOKABLE void addReader(const QString& name, const QString& email, const QString& registrationDate);
    Q_INVOKABLE void updateReader(int readerId, const QString& name, const QString& email, const QString& registrationDate);
    Q_INVOKABLE void removeReader(int readerId);
private:
    QList<Reader> readers;
};

#endif // DATAMODEL_H
