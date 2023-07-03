#ifndef GAMEFORGEACCOUNT_H
#define GAMEFORGEACCOUNT_H

#include <QObject>
#include <QMap>
#include "nostaleauth.h"

class GameforgeAccount : public QObject
{
    Q_OBJECT
public:
    explicit GameforgeAccount(
        const QString& gfEmail,
        const QString& gfPassword,
        const QString& identPath,
        bool proxy,
        const QString& proxyHost,
        const QString& proxyPort,
        const QString& proxyUsername,
        const QString& proxyPassword,
        QObject *parent = nullptr
    );

    bool authenticate(bool& captcha, QString& gfChallengeId, bool& wrongCredentials);
    const QMap<QString, QString>& getGameAccounts() const;

    QString getToken(const QString& accountId) const;

    QString getEmail() const;

    QString getPassword() const;

    QString getIdentityPath() const;

    const NostaleAuth *getAuth() const;

private:
    QString email;
    QString password;
    QString identityPath;
    NostaleAuth* auth;
    QMap<QString /* id */, QString /* name */> gameAccounts;
};

#endif // GAMEFORGEACCOUNT_H
