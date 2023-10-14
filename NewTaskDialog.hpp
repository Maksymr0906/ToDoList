#pragma once

#include <QString>
#include <QWidget>
#include <QDialog>
#include <memory>

#include "ui_NewTaskDialog.h"
#include "DateValidator.hpp"
#include "EmailValidator.hpp"

class NewTaskDialog : public QDialog {
    Q_OBJECT
private:
    std::unique_ptr<Ui::NewTaskDialogClass> ui;

public:
    NewTaskDialog(EmailValidator *emailValidator, DateValidator *dateValidator, QWidget* parent = nullptr) : QDialog(parent), ui(new Ui::NewTaskDialogClass) {
        ui->setupUi(this);
        ui->deadlineLineEdit->setValidator(dateValidator);
        ui->emailLineEdit->setValidator(emailValidator);
        ui->okButton->setDisabled(true);

        connect(dateValidator, &DateValidator::invalidDateSignal, this, &NewTaskDialog::invalidDateSlot);
        connect(dateValidator, &DateValidator::validDateSignal, this, &NewTaskDialog::validDateSlot);
        connect(dateValidator, &DateValidator::emptyDateSignal, this, &NewTaskDialog::validDateSlot);

        connect(emailValidator, &EmailValidator::invalidEmailSignal, this, &NewTaskDialog::invalidEmailSlot);
        connect(emailValidator, &EmailValidator::validEmailSignal, this, &NewTaskDialog::validEmailSlot);
        connect(emailValidator, &EmailValidator::emptyEmailSignal, this, &NewTaskDialog::validEmailSlot);

        connect(ui->taskNameLineEdit, &QLineEdit::textChanged, this, &NewTaskDialog::lineEditTextChanged);
        connect(ui->deadlineLineEdit, &QLineEdit::textChanged, this, &NewTaskDialog::lineEditTextChanged);
        connect(ui->emailLineEdit, &QLineEdit::textChanged, this, &NewTaskDialog::lineEditTextChanged);
    }

    QString getTaskName() const { return ui->taskNameLineEdit->text(); }
    QString getEmail() const { return ui->emailLineEdit->text(); }
    QString getResponsible() const { return ui->responsibleLineEdit->text(); }
    QString getDeadline() const { return ui->deadlineLineEdit->text(); }
    bool getIsImportant() const { return ui->isImportantCheckBox->isChecked(); }
    bool getIsMyDay() const { return ui->isMyDayCheckBox->isChecked(); }

public slots:
    void invalidDateSlot() {
        ui->okButton->setDisabled(true);
        ui->dateStateLabel->setText(tr("Invalid"));
        ui->dateStateLabel->setStyleSheet("color: red; font-weight: bold;");
    }

    void validDateSlot() {
        ui->dateStateLabel->setText(tr("Valid"));
        ui->dateStateLabel->setStyleSheet("color: green; font-weight: bold;");
    }

    void invalidEmailSlot() {
        ui->okButton->setDisabled(true);
        ui->emailStateLabel->setText(tr("Invalid"));
        ui->emailStateLabel->setStyleSheet("color: red; font-weight: bold;");
    }
    
    void validEmailSlot() {
        ui->emailStateLabel->setText(tr("Valid"));
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