#pragma once

#include <QValidator>
#include <QtWidgets>

class EmailValidator : public QValidator {
    Q_OBJECT
public:
    EmailValidator(QObject* parent = nullptr) : QValidator{ parent } {}
    
    virtual State validate(QString& str, int& pos) const override {
        if (pos == 0 && str.isEmpty()) {
            emit emptyEmailSignal();
            return Acceptable;
        }
        else if (str.contains(QRegularExpression("^[A-Za-z0-9]+([A-Za-z0-9._-]+)*[A-Za-z0-9]@[A-Za-z0-9]+\.[A-Za-z]{2,4}$"))) {
            emit validEmailSignal();
            return Acceptable;
        }
        else {
            emit invalidEmailSignal();
            return Intermediate;
        }
    }

signals:
    void emptyEmailSignal() const;
    void invalidEmailSignal() const;
    void validEmailSignal() const;
};

