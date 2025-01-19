// categoriesmodel.h

#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QHash>
#include <QByteArray>

struct Category {
    int id;
    QString name;
};

class CategoriesModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit CategoriesModel(QObject *parent = nullptr);

    enum CategoryRoles {
        CategoryIdRole = Qt::UserRole + 1,
        NameRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE void loadCategories();
public slots:
    void addCategory(const QString &name);
    void updateCategory(int id, const QString &name);
    void removeCategory(int id);
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Category> categories; // Список категорий
};

#endif // CATEGORIESMODEL_H

