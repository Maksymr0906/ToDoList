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

        connect(dateValidator, &DateValidator::invalidDateSignal, this, &NewTaskDialog::invalidDateSlot);
        connect(dateValidator, &DateValidator::validDateSignal, this, &NewTaskDialog::validDateSlot);
        connect(dateValidator, &DateValidator::emptyDateSignal, this, &NewTaskDialog::validDateSlot);

        connect(emailValidator, &EmailValidator::invalidEmailSignal, this, &NewTaskDialog::invalidEmailSlot);
        connect(emailValidator, &EmailValidator::validEmailSignal, this, &NewTaskDialog::validEmailSlot);
        connect(emailValidator, &EmailValidator::emptyEmailSignal, this, &NewTaskDialog::validEmailSlot);
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
        ui->okButton->setDisabled(false);
        ui->dateStateLabel->setText(tr("Valid"));
        ui->dateStateLabel->setStyleSheet("color: green; font-weight: bold;");
    }

    void invalidEmailSlot() {
        ui->okButton->setDisabled(true);
        ui->emailStateLabel->setText(tr("Invalid"));
        ui->emailStateLabel->setStyleSheet("color: red; font-weight: bold;");

    }
    
    void validEmailSlot() {
        ui->okButton->setDisabled(false);
        ui->emailStateLabel->setText(tr("Valid"));
        ui->emailStateLabel->setStyleSheet("color: green; font-weight: bold;");

    }
};