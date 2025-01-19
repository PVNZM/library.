#include "categoriesmodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

CategoriesModel::CategoriesModel(QObject *parent) : QAbstractListModel(parent) {
    loadCategories(); // Загружаем данные при создании
}

int CategoriesModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return categories.count();
}

QVariant CategoriesModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= categories.size())
        return QVariant();

    const Category &category = categories.at(index.row());

    switch (role) {
    case CategoryIdRole:
        return category.id;
    case NameRole:
        return category.name;
    case Qt::DisplayRole:
        return category.name; // Или любое другое представление
    default:
        return QVariant();
    }
}

void CategoriesModel::loadCategories() {
    beginResetModel(); // Очищаем текущую модель
    categories.clear(); // Очищаем вектор категорий

    QSqlQuery query("SELECT * FROM Categories"); // Запрос к базе данных
    while (query.next()) {
        Category category; // Создаем категорию
        category.id = query.value("category_id").toInt(); // Получаем ID категории
        category.name = query.value("category_name").toString(); // Получаем название категории
        categories.append(category); // Добавляем категорию в вектор

        // Вывод информации о загруженной категории
        qDebug() << "Категория загружена:" << category.name;
    }

    endResetModel(); // Обновляем модель
    qDebug() << "Общее количество категорий:" << categories.count(); // Вывод общего количества категорий
}


void CategoriesModel::addCategory(const QString &name) {
    QSqlQuery query;
    query.prepare("INSERT INTO Categories (category_name) VALUES (?)"); // Исправлено на category_name
    query.addBindValue(name);

    if (!query.exec()) {
        qDebug() << "Ошибка добавления в базу данных:" << query.lastError().text();
        return;
    }

    Category newCategory;
    newCategory.id = query.lastInsertId().toInt(); // Получаем последний вставленный ID
    newCategory.name = name;
    beginInsertRows(QModelIndex(), categories.size(), categories.size());
    categories.append(newCategory);
    endInsertRows();
}

void CategoriesModel::updateCategory(int id, const QString &name) {


    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].id == id) {
            QSqlQuery query;
            query.prepare("UPDATE Categories SET category_name = ? WHERE category_id = ?"); // Исправлено на category_name
            query.addBindValue(name);
            query.addBindValue(id);

            if (!query.exec()) {
                qDebug() << "Ошибка обновления в базе данных:" << query.lastError().text();
                return;
            }

            categories[i].name = name;
            emit dataChanged(index(i), index(i));
            return;
        }
    }
}

void CategoriesModel::removeCategory(int id) {
    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].id == id) {
            QSqlQuery query;
            query.prepare("DELETE FROM Categories WHERE category_id = ?");
            query.addBindValue(id);

            if (!query.exec()) {
                qDebug() << "Ошибка удаления из базы данных:" << query.lastError().text();
                return;
            }

            beginRemoveRows(QModelIndex(), i, i);
            categories.removeAt(i);
            endRemoveRows();

            qDebug() << "Категория с ID" << id << "удалена";
            return;
        }
    }

    qDebug() << "Категория с ID" << id << "не найдена для удаления";
}

QHash<int, QByteArray> CategoriesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[CategoryIdRole] = "categoryId";
    roles[NameRole] = "name";
    return roles;
}

