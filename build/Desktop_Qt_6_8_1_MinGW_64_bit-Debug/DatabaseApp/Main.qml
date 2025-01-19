import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3 // Импортируем модуль для работы с Layouts
import DatabaseApp 1.0


ApplicationWindow {
    visible: true
    width: 1200
    height: 1200
    title: "Управление "

    // Модель для читателей
    DataModel {
        id: readerModel
        Component.onCompleted: loadReaders()
    }

    // Модель для авторов
    AuthorsModel {
        id: authorModel
        Component.onCompleted: loadAuthors()
    }

    // Модель для категорий
    CategoriesModel {
        id: categoryModel
        //Component.onCompleted: loadCategories() // Убедитесь, что модель загружается
    }

    // Модель для книг
    BooksModel {
        id: booksModel // Убедитесь, что этот id уникален
        Component.onCompleted: loadBooks() // Убедитесь, что модель загружается при старте
    }

    // Модель для заказов
    OrdersModel {
        id: ordersModel
        //Component.onCompleted: loadCategories() // Убедитесь, что модель загружается
    }

    // Основной контейнер для переключения между представлениями
    StackLayout {
        id: mainStack
        anchors.fill: parent

        // Раздел для управления читателями
        Item {
            ColumnLayout {
                spacing: 10
                anchors.centerIn: parent

                Text {
                    text: "Читатели"
                    font.bold: true
                    font.pointSize: 16
                    horizontalAlignment: Text.AlignHCenter
                }

                RowLayout {
                    spacing: 10

                    TextField {
                        id: idInput
                        placeholderText: "ID читателя"
                        width: 120
                    }

                    TextField {
                        id: nameInput
                        placeholderText: "Имя читателя"
                        width: 180
                    }

                    TextField {
                        id: emailInput
                        placeholderText: "Email читателя"
                        width: 180
                    }

                    TextField {
                        id: registrationDateInput
                        placeholderText: "Дата регистрации"
                        width: 140
                    }
                }

                RowLayout {
                    spacing: 10

                    Button {
                        text: "Добавить читателя"
                        onClicked: {
                            if (nameInput.text && emailInput.text && registrationDateInput.text) {
                                readerModel.addReader(nameInput.text, emailInput.text, registrationDateInput.text);
                                nameInput.text = "";
                                emailInput.text = "";
                                registrationDateInput.text = "";
                                console.log("Читатель добавлен!");
                                readerModel.loadReaders(); // обновление списка
                            } else {
                                console.log("Заполните все поля для добавления читателя.");
                            }
                        }
                    }

                    Button {
                        text: "Редактировать читателя"
                        onClicked: {
                            const readerId = parseInt(idInput.text);
                            if (isNaN(readerId)) {

                                console.log("Введите корректный ID читателя для редактирования.");
                                return;
                            }
                            readerModel.updateReader(readerId, nameInput.text, emailInput.text, registrationDateInput.text);
                            console.log("Читатель обновлён!");
                            readerModel.loadReaders(); // обновление списка
                        }
                    }

                    Button {
                        text: "Удалить читателя"
                        onClicked: {
                            const readerId = parseInt(idInput.text);
                            if (isNaN(readerId)) {
                                console.log("Введите корректный ID читателя для удаления.");
                                return;
                            }

                            readerModel.removeReader(readerId);
                            console.log("Читатель удалён!");
                            readerModel.loadReaders(); // обновление списка
                        }
                    }
                }

                // Просмотр списка читателей
                        ListView {
                            id: readerListView
                            width: 700
                            height: 150
                            model: readerModel
                            clip: true

                            delegate: Item {
                                width: readerListView.width
                                height: 40

                                Rectangle {
                                    anchors.fill: parent
                                    color: "lightgrey"
                                    border.color: "black"
                                    border.width: 1

                                    Row {
                                        anchors.fill: parent
                                        spacing: 15
                                        padding: 10

                                        Text {
                                            text: model.readerId ? model.readerId + ": " + model.name : "Нет данных"
                                            font.bold: true
                                        }

                                        Text {
                                            text: model.email
                                            font.italic: true
                                        }
                                    }
                                }
                            }
                        }
            }
        }

        // Раздел для управления авторами
        Item {
            ColumnLayout {
                spacing: 10
                anchors.centerIn: parent

                // Раздел для управления авторами
                        Text {
                            text: "Авторы"
                            font.bold: true
                            font.pointSize: 16
                            horizontalAlignment: Text.AlignHCenter
                        }

                        RowLayout {
                            // Поля для ввода информации об авторе
                            spacing: 10

                            TextField {
                                id: authorIdInput
                                placeholderText: "ID автора"
                                width: 120
                            }

                            TextField {
                                id: authorNameInput
                                placeholderText: "Имя автора"
                                width: 180
                            }

                            TextField {
                                id: authorBirthDateInput
                                placeholderText: "Дата рождения"
                                width: 140
                            }
                        }

                        RowLayout {
                            spacing: 10

                            // Кнопка для добавления автора
                            Button {
                                text: "Добавить автора"
                                onClicked: {
                                    if (authorNameInput.text && authorBirthDateInput.text) {
                                        authorModel.addAuthor(authorNameInput.text, authorBirthDateInput.text);
                                        authorNameInput.text = "";
                                        authorBirthDateInput.text = "";
                                        console.log("Автор добавлен!");
                                        authorModel.loadAuthors(); // обновление списка
                                    } else {
                                        console.log("Заполните все поля для добавления автора.");
                                    }
                                }
                            }

                            // Кнопка для редактирования автора
                            Button {
                                text: "Редактировать автора"
                                onClicked: {
                                    const authorId = parseInt(authorIdInput.text);
                                    if (isNaN(authorId)) {
                                        console.log("Введите корректный ID автора для редактирования.");
                                        return;
                                    }
                                    authorModel.updateAuthor(authorId, authorNameInput.text, authorBirthDateInput.text);
                                    console.log("Автор обновлён!");
                                    authorModel.loadAuthors(); // обновление списка
                                }
                            }

                            // Кнопка для удаления автора
                            Button {
                                text: "Удалить автора"
                                onClicked: {
                                    const authorId = parseInt(authorIdInput.text);
                                    if (isNaN(authorId)) {

                                        console.log("Введите корректный ID автора для удаления.");
                                        return;
                                    }
                                    authorModel.removeAuthor(authorId);
                                    console.log("Автор удалён!");
                                    authorModel.loadAuthors(); // обновление списка
                                }
                            }
                        }

                        // Просмотр списка авторов
                        ListView {
                           id: authorListView
                           width: 700
                           height: 150
                           model: authorModel
                           clip: true

                           delegate: Item {
                               width: authorListView.width
                               height: 40

                               Rectangle {
                                   anchors.fill: parent
                                   color: "lightgrey"
                                   border.color: "black"
                                   border.width: 1

                                   Row {
                                       anchors.fill: parent
                                       spacing: 15

                                       padding: 10

                                       Text {
                                           text: model.authorId !== undefined ? model.authorId + ": " + model.name : "Нет данных"
                                           font.bold: true
                                       }

                                       Text {
                                           text: model.birthDate !== undefined ? model.birthDate : "Нет даты рождения"
                                           font.italic: true
                                       }
                                   }
                               }
                           }
                       }
            }
        }

        // Раздел для управления категориями
        Item {
            ColumnLayout {
                spacing: 10
                anchors.centerIn: parent

                // Раздел для управления категориями
                        Text {
                            text: "Управление категориями"
                            font.bold: true
                            font.pointSize: 16
                            padding: 5
                        }

                        RowLayout {
                            spacing: 10

                            // Поля для ввода информации о категории
                            TextField {
                                id: categoryIdInput
                                placeholderText: "ID категории"
                                width: 120
                            }
                            TextField {
                                id: categoryNameInput
                                placeholderText: "Название категории"
                                width: 180
                            }
                        }

                        RowLayout {
                            spacing: 10

                            // Кнопка для добавления категории
                            Button {
                                text: "Добавить категорию"
                                onClicked: {
                                    if (categoryNameInput.text.length > 0) { // Проверяем, что текст не пустой
                                        categoryModel.addCategory(categoryNameInput.text); // Добавление категории
                                        categoryNameInput.text = ""; // Очистка поля ввода
                                        console.log("Категория добавлена!");
                                    } else {
                                        console.log("Введите название категории.");
                                    }
                                }
                            }

                            // Кнопка для редактирования категории
                            Button {
                                text: "Редактировать категорию"
                                onClicked: {
                                    const categoryId = parseInt(categoryIdInput.text);
                                    if (isNaN(categoryId)) {
                                        console.log("Введите корректный ID категории для редактирования.");
                                        return;
                                    }
                                    if (categoryNameInput.text.length > 0) { // Проверяем, что текст не пустой
                                        categoryModel.updateCategory(categoryId, categoryNameInput.text); // Обновляем категорию
                                        categoryIdInput.text = ""; // Очистка ID
                                        categoryNameInput.text = ""; // Очистка имени
                                        console.log("Категория обновлена!");
                                    } else {
                                        console.log("Введите новое название категории.");
                                    }
                                }
                            }

                            // Кнопка для удаления категории
                            Button {
                                text: "Удалить категорию"
                                onClicked: {
                                    const categoryId = parseInt(categoryIdInput.text);
                                    if (isNaN(categoryId)) {
                                        console.log("Введите корректный ID категории для удаления.");
                                        return;
                                    }
                                    categoryModel.removeCategory(categoryId); // Удаление категории
                                    categoryIdInput.text = ""; // Очистка ID
                                    console.log("Категория удалена!");
                                }
                            }
                        }

                        // Просмотр списка категорий
                        ListView {
                            id: categoryListView
                            width: 700
                            height: 150
                            model: categoryModel
                            clip: true

                            delegate: Item {
                                width: categoryListView.width
                                height: 40

                                Rectangle {
                                    anchors.fill: parent
                                    color: "lightgrey"
                                    border.color: "black"
                                    border.width: 1

                                    Row {
                                        anchors.fill: parent
                                        spacing: 15
                                        padding: 10

                                        Text {
                                            text: model.categoryId // Отображаем ID категории
                                            verticalAlignment: Text.AlignVCenter
                                        }

                                        Text {
                                            text: model.name // Отображаем название категории
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                }
                            }
                        }
            }
        }

        // Раздел для управления книгами
        Item {
            ColumnLayout {
                spacing: 10
                anchors.centerIn: parent

                // Заголовок раздела управления книгами
                                Text {
                                    text: "Книги"
                                    font.bold: true
                                    font.pointSize: 16
                                    horizontalAlignment: Text.AlignHCenter
                                }

                                // Поля для ввода информации о книге
                                RowLayout {
                                    spacing: 10

                                    TextField {
                                        id: bookIdField
                                        placeholderText: "ID книги"
                                        width: 120
                                    }
                                    TextField {
                                        id: titleField
                                        placeholderText: "Название книги"
                                        width: 180
                                    }
                                    TextField {
                                        id: authorField
                                        placeholderText: "ID автора"
                                        width: 120
                                    }
                                    TextField {
                                        id: categoryField
                                        placeholderText: "ID категории"
                                        width: 120
                                    }
                                    TextField {
                                        id: publicationYearField
                                        placeholderText: "Год публикации"
                                        width: 140
                                    }
                                    TextField {
                                        id: availableCopiesField
                                        placeholderText: "Доступные копии"
                                        width: 140
                                    }
                                }

                                // Кнопки для управления книгами
                                RowLayout {
                                    spacing: 10

                                    Button {
                                        text: "Добавить книгу"
                                        onClicked: {
                                            if (titleField.text.length > 0 && authorField.text.length > 0) {
                                                booksModel.addBook(titleField.text, parseInt(authorField.text),parseInt(categoryField.text), parseInt(publicationYearField.text), parseInt(availableCopiesField.text));
                                                titleField.text = "";
                                                authorField.text = "";
                                                categoryField.text = "", // Добавлены аргументы ID категории
                                                publicationYearField.text = "";
                                                availableCopiesField.text = "";
                                                console.log("Книга добавлена!");
                                            } else {
                                                console.log("Пожалуйста, заполните все обязательные поля.");
                                            }
                                        }
                                    }

                                    Button {
                                        text: "Редактировать книгу"
                                        onClicked: {
                                            const bookId = parseInt(bookIdField.text);
                                            if (isNaN(bookId)) {
                                                console.log("Введите корректный ID книги для редактирования.");
                                                return;
                                            }
                                            if (titleField.text.length > 0 && authorField.text.length > 0) {
                                                booksModel.updateBook(bookId, titleField.text, parseInt(authorField.text), parseInt(publicationYearField.text), parseInt(availableCopiesField.text));
                                                bookIdField.text = "";
                                                titleField.text = "";
                                                authorField.text = "";
                                                publicationYearField.text = "";
                                                availableCopiesField.text = "";
                                                console.log("Книга обновлена!");
                                            } else {
                                                console.log("Пожалуйста, заполните все обязательные поля для редактирования.");
                                            }
                                        }
                                    }

                                    Button {
                                        text: "Удалить книгу"
                                        onClicked: {
                                            const bookId = parseInt(bookIdField.text);

                                            if (isNaN(bookId)) {
                                                console.log("Введите корректный ID книги для удаления.");
                                                return;
                                            }
                                            booksModel.removeBook(bookId);
                                            bookIdField.text = "";
                                            console.log("Книга удалена!");
                                        }
                                    }
                                }

                                // Просмотр списка книг
                                ListView {
                                    id: bookListView
                                    width: 700
                                    height: 150
                                    model: booksModel
                                    clip: true

                                    delegate: Item {
                                        width: bookListView.width
                                        height: 40

                                        // Отображение каждой книги в списке
                                        Rectangle {
                                            anchors.fill: parent
                                            color: "lightgrey"
                                            border.color: "black"
                                            border.width: 1

                                            Row {
                                                anchors.fill: parent
                                                spacing: 15
                                                padding: 10

                                                Text {
                                                    text: model.bookId
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                                Text {
                                                    text: model.title
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                                Text {
                                                    text: model.authorId
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                                Text {
                                                    text: model.categoryId
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                                Text {
                                                    text: model.publicationYear
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                                Text {
                                                    text: model.availableCopies
                                                    verticalAlignment: Text.AlignVCenter
                                                }
                                            }
                                        }
                                    }
                                }
            }
        }
        // Раздел для управления заказами
        Item {
            ColumnLayout {
                spacing: 10
                anchors.centerIn: parent

                Text {
                    text: "Заказы"
                    font.bold: true
                    font.pointSize: 16
                    horizontalAlignment: Text.AlignHCenter
                }

                RowLayout {
                    spacing: 10

                    TextField {
                        id: orderIdInput
                        placeholderText: "ID заказа"
                        width: 120
                        //readOnly: true // Только для чтения
                    }

                    TextField {
                        id: readerIdInput
                        placeholderText: "ID читателя"
                        width: 180
                    }

                    TextField {
                        id: bookIdInput
                        placeholderText: "ID книги"
                        width: 180
                    }

                    TextField {
                        id: orderDateInput
                        placeholderText: "Дата заказа"
                        width: 140
                    }

                    TextField {
                        id: returnDateInput
                        placeholderText: "Дата возврата"
                        width: 140
                    }
                }

                RowLayout {
                    spacing: 10

                    Button {
                        text: "Добавить заказ"
                        onClicked: {
                            if (readerIdInput.text && bookIdInput.text && orderDateInput.text) {
                                ordersModel.addOrder(parseInt(readerIdInput.text), parseInt(bookIdInput.text), orderDateInput.text, returnDateInput.text);
                                console.log("Заказ добавлен!");
                                //clearFields();  // Очистка полей после добавления
                                ordersModel.loadOrders(); // Обновление списка
                            } else {
                                console.log("Заполните все поля для добавления заказа.");
                            }
                        }
                    }

                    Button {
                        text: "Редактировать заказ"
                        onClicked: {
                            const orderId = parseInt(orderIdInput.text);
                            if (isNaN(orderId)) {
                                console.log("Введите корректный ID заказа для редактирования.");
                                return;
                            }
                            ordersModel.updateOrder(orderId, parseInt(readerIdInput.text), parseInt(bookIdInput.text), orderDateInput.text, returnDateInput.text);
                            console.log("Заказ обновлён!");
                            //ordersModel.loadOrders(); // Обновление списка
                        }
                    }

                    Button {
                        text: "Удалить заказ"
                        onClicked: {
                            const orderId = parseInt(orderIdInput.text);
                            if (isNaN(orderId)) {
                                console.log("Введите корректный ID заказа для удаления.");
                                return;
                            }
                            ordersModel.removeOrder(orderId);
                            console.log("Заказ удалён!");
                            clearFields(); // Очищаем поля ввода после удаления
                            ordersModel.loadOrders(); // Обновление списка
                        }
                    }
                }


                // Список заказов (при необходимости)
                // Раздел для просмотра заказов
                ListView {
                    id: ordersListView // Идентификатор для доступа к этому элементу
                    width: 700
                    height: 150
                    model: ordersModel // Модель, которая содержит ваши заказы
                    clip: true

                    delegate: Item {
                        width: ordersListView.width
                        height: 40

                        // Отображение каждого заказа в списке
                        Rectangle {
                            anchors.fill: parent
                            color: "lightgrey"
                            border.color: "black"
                            border.width: 1

                            Row {
                                anchors.fill: parent
                                spacing: 15
                                padding: 10

                                Text {
                                    text: model.orderId // ID заказа
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    text: model.readerId // ID читателя
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    text: model.bookId // ID книги
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    text: model.orderDate // Дата заказа
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    text: model.returnDate // Дата возврата (может быть пустой)
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }
                }

            }
        }


    }

    // Панель кнопок для переключения

    RowLayout {
        spacing: 10
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            text: "Читатели"
            onClicked: mainStack.currentIndex = 0
        }

        Button {
            text: "Авторы"
            onClicked: mainStack.currentIndex = 1
        }

        Button {
            text: "Категории"
            onClicked: mainStack.currentIndex = 2
        }

        Button {
            text: "Книги"
            onClicked: mainStack.currentIndex = 3
        }
        Button {
            text: "Заказы"
            onClicked: mainStack.currentIndex = 4
        }

    }
}

