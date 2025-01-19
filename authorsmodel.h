#ifndef AUTHORSMODEL_H
#define AUTHORSMODEL_H

#include <QAbstractListModel>
#include <QList>

struct Author {
    int id;
    QString name;
    QString birthDate;
};

class AuthorsModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit AuthorsModel(QObject *parent = nullptr);

    // Методы для работы с авторами
    Q_INVOKABLE void loadAuthors();
    // Определение количества строк
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    // Определение данных в модели
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // Определения ролей
    enum AuthorRoles {
        AuthorIdRole = Qt::UserRole + 1,
        NameRole,
        BirthDateRole
    };

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void addAuthor(const QString &name, const QString &birthDate);
    void updateAuthor(int id, const QString &name, const QString &birthDate);
    void removeAuthor(int id);

private:
    QList<Author> authors; // Список авторов
};

#endif // AUTHORSMODEL_H
