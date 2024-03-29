#include "gameupdater.h"
#include <QDebug>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QCryptographicHash>
#include <QDebug>

GameUpdater::GameUpdater(const QVector<GameforgeAccount *> &gfAccs, const QString &gameDirPath, QObject *parent)
    : QObject{parent}
    , nostaleDirectoryPath(gameDirPath)
    , gfAccounts(gfAccs)
{
    networkManager = new SyncNetworAccesskManager(this);
}

void GameUpdater::updateFiles() const
{
    if (nostaleDirectoryPath.isEmpty())
        return;

    QJsonArray filesInfo = getRemoteFilesInformation();

    for (int i = 0; i < filesInfo.size(); ++i) {

        QJsonObject fileData = filesInfo[i].toObject();

        bool folder = fileData["folder"].toBool();
        QString filename = fileData["file"].toString();
        QString remotePath = fileData["path"].toString();
        QByteArray remoteHash = fileData["sha1"].toVariant().toByteArray();

        if (folder) {
            continue;
        }

        QString filePath = getFilePath(filename);
        QByteArray localHash = getLocalHash(filePath);

        if (localHash != remoteHash) {
            QByteArray fileContent = downloadFile(remotePath);

            if (!saveFile(fileContent, filePath)) {
                qDebug() << "Couldn't save file:" << filePath;
            }

            // Update custom clients
            if (filename == "NostaleClientX.exe") {
                for (const auto& gfAcc : gfAccounts) {
                    if (gfAcc->getcustomClientPath().isEmpty())
                        continue;

                    if (!saveFile(fileContent, gfAcc->getcustomClientPath())) {
                        qDebug() << "Couldn't save file:" << gfAcc->getcustomClientPath();
                    }
                }
            }
        }

        emit setProgress((i * 100) / filesInfo.size());
    }
}

QJsonArray GameUpdater::getRemoteFilesInformation() const
{
    QNetworkRequest request(QUrl("https://spark.gameforge.com/api/v1/patching/download/latest/nostale/default?locale=en&architecture=x64&branchToken"));
    QNetworkReply* reply;

    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/x-www-form-urlencoded");

    reply = networkManager->get(request);

    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();

    return response["entries"].toArray();
}

QByteArray GameUpdater::downloadFile(const QString &remotePath) const
{
    QNetworkRequest request(QUrl("http://patches.gameforge.com" + remotePath));
    QNetworkReply* reply;

    reply = networkManager->get(request);

    return reply->readAll();
}

bool GameUpdater::saveFile(const QByteArray &fileContent, const QString &filePath) const
{
    QFile file(filePath);

    if (!file.open(QFile::WriteOnly))
        return false;

    file.write(fileContent);
    file.close();

    return true;
}

QByteArray GameUpdater::getLocalHash(const QString &filePath) const
{
    QFile file(filePath);

    if (!file.open(QFile::ReadOnly)) {
        return {};
    }

    QCryptographicHash hash(QCryptographicHash::Sha1);

    if (!hash.addData(&file)) {
        file.close();
        return {};
    }

    file.close();
    return hash.result().toHex();
}

QString GameUpdater::getFilePath(const QString &relativePath) const
{
    return nostaleDirectoryPath + "/" + relativePath;
}
