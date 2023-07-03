#include "gameforgeaccount.h"

GameforgeAccount::GameforgeAccount(const QString &gfEmail, const QString &gfPassword, const QString& identPath, bool proxy, const QString &proxyHost, const QString &proxyPort, const QString &proxyUsername, const QString &proxyPassword, QObject *parent)
    : QObject{parent}
    , email(gfEmail)
    , password(gfPassword)
    , identityPath(identPath)
{
    auth = new NostaleAuth(
        identityPath,
        proxy,
        proxyHost,
        proxyPort,
        proxyUsername,
        proxyPassword,
        this
    );
}

bool GameforgeAccount::authenticate(bool &captcha, QString &gfChallengeId, bool &wrongCredentials)
{
    bool res = auth->authenticate(email, password, captcha, gfChallengeId, wrongCredentials);

    if (res) {
        gameAccounts = auth->getAccounts();
    }

    return res;
}

const QMap<QString, QString> &GameforgeAccount::getGameAccounts() const
{
    return gameAccounts;
}

QString GameforgeAccount::getToken(const QString &accountId) const
{
    return auth->getToken(accountId);
}

QString GameforgeAccount::getEmail() const
{
    return email;
}

QString GameforgeAccount::getPassword() const
{
    return password;
}

QString GameforgeAccount::getIdentityPath() const
{
    return identityPath;
}

const NostaleAuth *GameforgeAccount::getAuth() const
{
    return auth;
}
