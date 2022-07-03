#ifndef IDENTITY_H
#define IDENTITY_H

#include "fingerprint.h"
#include <QFile>

class Identity
{
public:
    Identity(const QString& filePath);
    ~Identity();

    void update();

    Fingerprint getFingerprint() const;

    void setRequest(const QJsonValue& request);

private:
    QString filename;
    Fingerprint fingerprint;

    void initFingerprint();

    void save() const;
};

#endif // IDENTITY_H
