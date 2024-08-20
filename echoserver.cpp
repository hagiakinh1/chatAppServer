#include <echoserver.h>
#include <QtWebSockets/qwebsocketserver.h>
#include <QtWebSockets/qwebsocket.h>
#include <QtCore/QDebug>

QT_USE_NAMESPACE

//! [constructor]
EchoServer::EchoServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("Echo Server"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_debug(debug)
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            qDebug() << "Echoserver listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &EchoServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &EchoServer::closed);
    }
}
//! [constructor]

EchoServer::~EchoServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

//! [onNewConnection]
void EchoServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &EchoServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &EchoServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &EchoServer::socketDisconnected);

    m_clients << pSocket;
    mapUserIdToSocket[pSocket] = -1; //connect, but there is no userId yet.
}
//! [onNewConnection]

//! [processTextMessage]
void EchoServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient== nullptr){
        qDebug("client is null");
    }
    if (m_debug)
        qDebug("Message received:");

    // Convert the QString to a QByteArray
        QByteArray jsonBytes = message.toUtf8();

        // Parse the JSON string into a QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);

        // Check if the JSON document is valid and is an object
        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            // Extract the QJsonObject from the QJsonDocument
            QJsonObject jsonObject = jsonDoc.object();


            // Create a QMap to store the JSON data
            QMap<QString, QVariant> map;

            // Iterate through the QJsonObject and fill the QMap
            for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
                map.insert(it.key(), it.value().toVariant());
            }

            // Output the QMap (for debugging)
            for (auto it = map.begin(); it != map.end(); ++it) {
                qDebug( it.key().toLatin1() + ":" + it.value().toString().toLatin1());
            }
            if(jsonObject.contains("senderId")){

                int senderId = jsonObject["senderId"].toInt();
                int receiverId = jsonObject["receiverId"].toInt();
                qDebug("contains a senderId "+ QString::number(senderId).toLatin1());
                if (senderId>=1){
                    if(mapUserIdToSocket[pClient]==senderId){
                        //find all the pClients which have receiver as their value
                        for (auto it = mapUserIdToSocket.begin(); it != mapUserIdToSocket.end(); ++it) {
                            if (it.value() == receiverId && it.key()!= nullptr) {
                                it.key()->sendTextMessage(message);
                                qDebug("sent " + message.toLatin1());
                            }
                        }
                    }else{
                        mapUserIdToSocket[pClient] = senderId;
                        //find all the pClients which have receiver as their value
                        for (auto it = mapUserIdToSocket.begin(); it != mapUserIdToSocket.end(); ++it) {
                            if (it.value() == receiverId && it.key()!= nullptr) {
                                it.key()->sendTextMessage(message);
                                qDebug("sent " + message.toLatin1());
                            }
                        }
                    }
                }
            }
        } else {
            qWarning() << "Failed to parse JSON string.";
        }




}
//! [processTextMessage]

//! [processBinaryMessage]
void EchoServer::processBinaryMessage(QByteArray message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}
//! [processBinaryMessage]

//! [socketDisconnected]
void EchoServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
//! [socketDisconnected]
