#pragma once
#include <QString>
#include <QCryptographicHash>

class HashUtils {
public:
    static QString hashPassword(const QString& password) {
        return QCryptographicHash::hash(
            password.toUtf8(),
            QCryptographicHash::Sha256
        ).toHex();
    }
};
