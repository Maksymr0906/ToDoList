#pragma once

#include <QString>
#include <QWidget>
#include <QDialog>
#include <memory>

#include "ui_NewTaskDialog.h"
#include "DateValidator.hpp"

class NewTaskDialog : public QDialog {
    Q_OBJECT
private:
    std::unique_ptr<Ui::NewTaskDialogClass> ui;

public:
    NewTaskDialog(DateValidator *dateValidator, QWidget* parent = nullptr) : QDialog(parent), ui(new Ui::NewTaskDialogClass) {
        ui->setupUi(this);
        ui->deadlineLineEdit->setValidator(dateValidator);

        connect(dateValidator, &DateValidator::invalidDateSignal, this, &NewTaskDialog::invalidDateSlot);
        connect(dateValidator, &DateValidator::validDateSignal, this, &NewTaskDialog::validDateSlot);
        connect(dateValidator, &DateValidator::emptyDateSignal, this, &NewTaskDialog::validDateSlot);
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
};