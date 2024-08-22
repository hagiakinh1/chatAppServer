#ifndef DATAACCESSOBJECT_H
#define DATAACCESSOBJECT_H
#include<QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include<QVariantList>
class DataAccessObject
{
public:
        // Static method to get the singleton instance
        static DataAccessObject& getInstance();

        // Delete copy constructor and assignment operator to prevent copying
        DataAccessObject(const DataAccessObject&) = delete;
        DataAccessObject& operator=(const DataAccessObject&) = delete;
        bool createUser(const QString &username, const QString &password_hash);
        QList<QVariantList> readMessagesBetweenUsers(int user1_id, int user2_id);
        bool createMessage(int sender_id, int receiver_id, const QString &message_text);
        bool deleteUser(int user_id);
        bool updateUser(int user_id, const QString &newUsername, const QString &newPasswordHash);
        QList<QVariantList> readAllUsers();
        bool checkUserExists(const QString &username, const QString &password);
        QList<QVariantList> readAllUsersWithLatestMessage(int user_id);
        int findUser(const QString &username, const QString &password);
private:
    DataAccessObject();

    ~DataAccessObject();
    QSqlDatabase m_db;
    bool createUsersTable();
    bool createMessagesTable();
};

#endif // DATAACCESSOBJECT_H
