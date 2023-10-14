#pragma once

#include <QDialog>
#include <memory>
#include <QTSql>

#include "Task.hpp"
#include "ui_TaskDialog.h"
#include "DateValidator.hpp"
#include "EmailValidator.hpp"

class TaskDialog : public QDialog {
    Q_OBJECT
protected:
    std::unique_ptr<Ui::TaskDialog> ui;

public:
    TaskDialog(EmailValidator *emailValidator, DateValidator* dateValidator, QWidget* parent = nullptr) : QDialog(parent), ui(new Ui::TaskDialog()) {
        ui->setupUi(this);
        ui->deadlineLineEdit->setValidator(dateValidator);
        ui->emailLineEdit->setValidator(emailValidator);

        connect(dateValidator, &DateValidator::invalidDateSignal, this, &TaskDialog::invalidDateSlot);
        connect(dateValidator, &DateValidator::validDateSignal, this, &TaskDialog::validDateSlot);
        connect(dateValidator, &DateValidator::emptyDateSignal, this, &TaskDialog::validDateSlot);

        connect(emailValidator, &EmailValidator::invalidEmailSignal, this, &TaskDialog::invalidEmailSlot);
        connect(emailValidator, &EmailValidator::validEmailSignal, this, &TaskDialog::validEmailSlot);
        connect(emailValidator, &EmailValidator::emptyEmailSignal, this, &TaskDialog::validEmailSlot);

        connect(ui->taskNameLineEdit, &QLineEdit::textChanged, this, &TaskDialog::lineEditTextChanged);
        connect(ui->deadlineLineEdit, &QLineEdit::textChanged, this, &TaskDialog::lineEditTextChanged);
        connect(ui->emailLineEdit, &QLineEdit::textChanged, this, &TaskDialog::lineEditTextChanged);
    }

    QString getTaskName() const { return ui->taskNameLineEdit->text(); }
    QString getDeadline() const { return ui->deadlineLineEdit->text(); }
    QString getResponsible() const { return ui->responsibleLineEdit->text(); }
    QString getEmail() const { return ui->emailLineEdit->text(); }
    bool getIsImportant() const { return ui->isImportantCheckBox->isChecked(); }
    bool getIsMyDay() const { return ui->isMyDayCheckBox->isChecked(); }

    void setTaskName(QString taskName) { ui->taskNameLineEdit->setText(taskName); }
    void setDeadline(QString deadline) { ui->deadlineLineEdit->setText(deadline); }
    void setResponsible(QString responsible) { ui->responsibleLineEdit->setText(responsible); }
    void setEmail(QString email) { ui->emailLineEdit->setText(email); }
    void setIsMyDay(bool isMyDay) { ui->isMyDayCheckBox->setChecked(isMyDay); }
    void setIsImportant(bool isImportant) { ui->isImportantCheckBox->setChecked(isImportant); }

public slots:
    void invalidDateSlot() {
        ui->okButton->setDisabled(true);
        ui->dateStateLabel->setText(tr("Invalid deadline"));
        ui->dateStateLabel->setStyleSheet("color: red; font-weight: bold;");
    }

    void validDateSlot() {
        ui->dateStateLabel->setText(tr("Valid deadline"));
        ui->dateStateLabel->setStyleSheet("color: green; font-weight: bold;");
    }

    void invalidEmailSlot() {
        ui->okButton->setDisabled(true);
        ui->emailStateLabel->setText(tr("Invalid email"));
        ui->emailStateLabel->setStyleSheet("color: red; font-weight: bold;");
    }

    void validEmailSlot() {
        ui->emailStateLabel->setText(tr("Valid email"));
        ui->emailStateLabel->setStyleSheet("color: green; font-weight: bold;");
    }

    void lineEditTextChanged() {
        QString deadlineLineEditString = ui->deadlineLineEdit->text();
        QString emailLineEditString = ui->emailLineEdit->text();
        int pos = 0;
        if (!ui->taskNameLineEdit->text().isEmpty() && ui->emailLineEdit->validator()->validate(emailLineEditString, pos) == QValidator::Acceptable
            && ui->deadlineLineEdit->validator()->validate(deadlineLineEditString, pos) == QValidator::Acceptable) {
            ui->okButton->setDisabled(false);
        }
        else {
            ui->okButton->setDisabled(true);
        }
    }
};