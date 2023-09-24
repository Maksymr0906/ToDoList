#pragma once

#include <QValidator>
#include <QtWidgets>

class DateValidator : public QValidator {
    Q_OBJECT
public:
    DateValidator(QObject *parent = nullptr) : QValidator(parent) {}

    virtual State validate(QString &str, int &pos) const override {
        QRegularExpression datePattern("^(2[0-1][0-9][0-9])-(0[1-9]|1[0-2])-([0-2][0-9]|3[0-1])$");
        QRegularExpressionMatch match = datePattern.match(str);

        if(match.hasMatch()) {
            emit validDateSignal();
            return Acceptable;
        }
        else {
            emit invalidDateSignal();
            return Invalid;
        }
    }

signals:
    void invalidDateSignal() const;
    void validDateSignal() const;
};