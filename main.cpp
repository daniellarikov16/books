#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QtSql>
#include <clocale>
#include <Windows.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtWidgets>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <iostream>
#include <QTextCodec>
#include <string>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
void add(QSqlDatabase db, std::string InputName, std::string InputAuthor,int InputYear, std::string InputGenre){
    QString qInputName, qInputAuthor, qInputGenre;
    qInputName = QString::fromStdString(InputName);
    qInputAuthor = QString::fromStdString(InputAuthor);
    qInputGenre = QString::fromStdString(InputGenre);
    QSqlQuery query (db);


    query.prepare("INSERT INTO name (name_book) VALUES (:name_book)");
    query.bindValue(":name_book", qInputName);
    if (!query.exec()) {
        qDebug() << "Ошибка при работе с NAME BOOK: " << query.lastError().text();
    }


    query.prepare("INSERT INTO author (name_author) VALUES (:name_author)");
    query.bindValue(":name_author", qInputAuthor);
    if (!query.exec()) {
        qDebug() << "Ошибка при работе с NAME AUTHOR: " << query.lastError().text();
    }


    query.prepare("INSERT INTO year (year) VALUES (:year)");
    query.bindValue(":year", InputYear);
    if (!query.exec()) {
        qDebug() << "Ошибка при работу с YEAR: " << query.lastError().text();
    }


    query.prepare("INSERT INTO genre (genre) VALUES (:genre)");
    query.bindValue(":genre", qInputGenre);
    if (!query.exec()) {
        qDebug() << "Ошибка при работе с GENRE: " << query.lastError().text();
    }
}

void deleteRecord(QSqlDatabase db, QString nameBook, QStringList tables, int bookId){
    QSqlQuery query (db);
    query.prepare("SELECT id FROM name WHERE name_book = :nameBook");
    query.bindValue(":nameBook", nameBook);

    if (!query.exec()) {
        qDebug() << "Ошибка получения id:" << query.lastError().text();
    }
    if (!query.next()) {
        qDebug() << "Книга не найдена!";
    }
    bookId = query.value(0).toInt();


    tables = {"author", "year", "genre"};
    for (const QString& table : tables) {
        query.prepare(QString("DELETE FROM %1 WHERE id = :bookId").arg(table));
        query.bindValue(":bookId", bookId);

        if (!query.exec()) {
            qDebug() << "Ошибка удаления из " << table << ":" << query.lastError().text();
        }
    }
    query.prepare("DELETE FROM name WHERE id = :bookId");
    query.bindValue(":bookId", bookId);
    if (!query.exec()) {
        qDebug() << "Ошибка удаления из name:" << query.lastError().text();
    }

    qDebug() << "Данные удалены успешно.";

}
void chageName (QSqlDatabase db, QString qEditName, int EbookID){
    QSqlQuery query (db);
    query.prepare("UPDATE name SET name_book = :newName WHERE id = :id");
    query.bindValue(":newName", qEditName);
    query.bindValue(":id", EbookID);
    if (!query.exec()) {
        qDebug() << "Ошибка при обновлении в NAME: " << query.lastError().text();
    }
    qDebug() << "Имя обновлено успешно!";
}

void chageAuthor (QSqlDatabase db, QString qEditAuthor, int EbookID){
    QSqlQuery query (db);
    query.prepare("UPDATE author SET name_author = :newAuthor WHERE id = :id");
    query.bindValue(":newAuthor", qEditAuthor);
    query.bindValue(":id", EbookID);
    if (!query.exec()) {
        qDebug() << "Ошибка при обновлении в AUTHOR: " << query.lastError().text();
    }
    qDebug() << "Автор обновлен успешно!";
}

void changeYear (QSqlDatabase db, int EditYear, int EbookID){
    QSqlQuery query (db);
    query.prepare("UPDATE year SET year = :newYear WHERE id = :id");
    query.bindValue(":newYear", EditYear);
    query.bindValue(":id", EbookID);
    if (!query.exec()) {
        qDebug() << "Ошибка при обновлении в YEAR: " << query.lastError().text();
    }
    qDebug() << "Год обновлен успешно!";
}

void chageGenre (QSqlDatabase db, QString qEditGenre, int EbookID){
    QSqlQuery query (db);
    query.prepare("UPDATE genre SET genre = :newGenre WHERE id = :id");
    query.bindValue(":newGenre", qEditGenre);
    query.bindValue(":id", EbookID);
    if (!query.exec()) {
        qDebug() << "Ошибка при обновлении в GENRE: " << query.lastError().text();
    }
    qDebug() << "Жанр обновлен успешно!";
}

void result (QSqlDatabase db){
    QSqlQuery query (db);
    query.prepare("SELECT n.id, n.name_book, a.name_author, y.year, g.genre "
                  "FROM name n "
                  "INNER JOIN author a ON n.id = a.id "
                  "INNER JOIN year y ON n.id = y.id "
                  "INNER JOIN genre g ON n.id = g.id");

    if (!query.exec()) {
        qDebug() << "Ошибка:" << query.lastError().text();
    } else {
        while (query.next()) {
            QString nameBook = query.value(1).toString();
            QString nameAuthor = query.value(2).toString();
            int year = query.value(3).toInt();
            QString genre = query.value(4).toString();

            qDebug() << "Название книги:" << nameBook
                     << ", Автор:" << nameAuthor << ", Год:" << year<<", Жанр:" << genre;

        }
    }
}
void ViewOneBook (QSqlDatabase db, QString qOneNameBook){
    QSqlQuery query (db);
    query.prepare("SELECT n.name_book, a.name_author, y.year, g.genre "
                  "FROM name n "
                  "JOIN author a ON n.id = a.id "
                  "JOIN year y ON n.id = y.id "
                  "JOIN genre g ON n.id = g.id "
                  "WHERE n.name_book = :bookName");
    query.bindValue(":bookName", qOneNameBook);
    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return;
    }
    while (query.next()) {
        QString nameOFbook = query.value(0).toString();
        QString nameOFauthor = query.value(1).toString();
        QString year = query.value(2).toString();
        QString genre = query.value(3).toString();

        qDebug() << "Название:" << nameOFbook << "Автор:" << nameOFauthor << "Год:" << year << "Жанр:" << genre;
    }
}
void SortedName (QSqlDatabase db){
    QSqlQuery query(db);

    query.prepare("SELECT n.name_book, a.name_author, y.year, g.genre "
                  "FROM name n "
                  "JOIN author a ON n.id = a.id "
                  "JOIN year y ON n.id = y.id "
                  "JOIN genre g ON n.id = g.id "
                  "ORDER BY n.name_book;");
    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
    }
    while (query.next()) {
        QString nameBook = query.value(0).toString();
        QString nameAuthor = query.value(1).toString();
        QString year = query.value(2).toString();
        QString genre = query.value(3).toString();

        qDebug() << "Название книги:" << nameBook << ", Автор:" << nameAuthor << ", Год:" << year << ", Жанр:" << genre;
    }
}
int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN32

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("IBM 866"));

#endif
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    setlocale(LC_ALL, "");

    std::wstring wstr;
    std::getline(std::wcin, wstr);

    std::wcout << wstr;
    QCoreApplication a(argc, argv);
    QTranslator translator;
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("postgre");
    db.setUserName("PQL");
    db.setPassword("postgres");
    db.open();
    if (db.isOpen()) {
        qDebug() << "Database is open";
    } else {
        qDebug() << "Database is not open";
    }

    QSqlQuery query(db);
    std::string InputName, InputAuthor, InputGenre, EditName, EditAuthor, EditGenre, OneNameBook;
    char EditingID;
    QString qEditName, qEditAuthor, qEditGenre, nameBook, newBookName, qOneNameBook;
    int InputYear, inp=10, bookId = 0, EbookID, EditYear;
    QTextStream in(stdin);
    QStringList tables;
    std::cout << QString::fromUtf8("Рады привествовать вас в онлайн-библиотеке.\n").toLocal8Bit().data();
    while (1){
        std::cout << QString::fromUtf8("Выберите действие:\n "
                                       "1 - Добавить книгу в хранилище, 2 - Убрать книгу из хранилища, 3 - Редактировать книгу,\n"
                                       " 4 - Просмотр всех книг в хранилище, 5 - Поиск книги по названию, 6 - Сортировка\n 0 - Завершить работу\n").toLocal8Bit().data();
        std::cin >> inp;
        std::cin.ignore();
        switch (inp)
        {
            case 1:
            std::cout << QString::fromUtf8("Введите название\n").toLocal8Bit().data();
            std::getline(std::cin, InputName);
            std::cout << QString::fromUtf8("Введите автора\n").toLocal8Bit().data();
            std::getline(std::cin, InputAuthor);
            std::cout << QString::fromUtf8("Введите год выпуска\n").toLocal8Bit().data();
            std::cin >> InputYear;
            std::cin.ignore();
            std::cout << QString::fromUtf8("Введите жанр\n").toLocal8Bit().data();
            std::getline(std::cin, InputGenre);
            add(db, InputName, InputAuthor, InputYear, InputGenre);
                break;
            case 2:
                std::cout << QString::fromUtf8("Введите название книги для удаления\n").toLocal8Bit().data();
                nameBook = in.readLine();
                deleteRecord(db, nameBook, tables, bookId);
                break;
            case 3:
                std::cout << QString::fromUtf8("Введите название изменяемой книги\n").toLocal8Bit().data();
                newBookName = in.readLine();
                std::cout << QString::fromUtf8("Выберите то, что вы хотите редактировать:\n"
                                               "n - Название, a - автора, y - год, g - жанр\n").toLocal8Bit().data();
                std::cin >> EditingID;
                query.prepare("SELECT id FROM name WHERE name_book = :bookName");
                query.bindValue(":bookName",newBookName);
                if (!query.exec() || !query.next()) {
                    qDebug() << "Книга не найдена!";
                }
                std::cin.ignore();
                EbookID = query.value(0).toInt();
                switch(EditingID){
                case 'n':
                    std::cout << QString::fromUtf8("Введите новое имя\n").toLocal8Bit().data();
                    std::getline(std::cin, EditName);
                    qEditName = QString::fromStdString(EditName);
                    chageName (db, qEditName, EbookID);
                    break;
                case 'a':
                    std::cout << QString::fromUtf8("Введите нового автора\n").toLocal8Bit().data();
                    std::getline(std::cin, EditAuthor);
                    qEditAuthor = QString::fromStdString(EditAuthor);
                    chageAuthor (db, qEditAuthor, EbookID);
                    break;
                case 'y':
                    std::cout << QString::fromUtf8("Введите новый год\n").toLocal8Bit().data();
                    std::cin >> EditYear;
                    std::cin.ignore();
                    changeYear (db, EditYear, EbookID);
                    break;
                case 'g':
                    std::cout << QString::fromUtf8("Введите новый жанр\n").toLocal8Bit().data();
                    std::getline(std::cin, EditGenre);
                    qEditGenre = QString::fromStdString(EditGenre);
                    chageGenre (db, qEditGenre, EbookID);
                    break;
                }
            case 4:
                result (db);
                break;
            case 5:
                std::cout << QString::fromUtf8("Введите название книги\n").toLocal8Bit().data();
                std::getline(std::cin, OneNameBook);
                qOneNameBook = QString::fromStdString(OneNameBook);
                ViewOneBook(db, qOneNameBook);
                break;
            case 6:
                std::cout << QString::fromUtf8("Отсортированный список литературы:\n").toLocal8Bit().data();
                SortedName(db);
                break;
            case 0:
                std::cout << QString::fromUtf8("Хорошего дня! До свидания!\n").toLocal8Bit().data();
                return 0;
                break;
            default:
                std::cout << QString::fromUtf8("Неизвестная команда, попробуйте еще раз\n").toLocal8Bit().data();
                break;
        }
    }
}


