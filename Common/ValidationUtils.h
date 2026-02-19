#pragma once
#include <QString>
#include <QRegularExpression>

class ValidationUtils {
public:
    static bool isValidEmail(const QString& email) {
        QRegularExpression r(R"((\w+)(\.|\w)*@(\w+\.)+\w+)");
        return r.match(email).hasMatch();
    }

    static bool isValidPhone(const QString& phone) {
        QRegularExpression r(R"(09\d{9})");
        return r.match(phone).hasMatch();
    }

    static bool isStrongPassword(const QString& pass) {
        return pass.length() >= 8;
    }
};
