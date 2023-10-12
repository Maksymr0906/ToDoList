#pragma once

#include <QString>
#include <QWidget>
#include <QDialog>
#include <memory>
#include <QTSql>

#include "Task.hpp"
#include "ui_EditTaskDialog.h"
#include "DateValidator.hpp"

class EditTaskDialog : public QDialog {
    Q_OBJECT
private:
    std::unique_ptr<Ui::EditTaskDialog> ui;

public:
    EditTaskDialog(DateValidator* dateValidator, QWidget* parent = nullptr) : QDialog(parent), ui(new Ui::EditTaskDialog()) {
        ui->setupUi(this);
        ui->deadlineLineEdit->setValidator(dateValidator);

        connect(dateValidator, &DateValidator::invalidDateSignal, this, &EditTaskDialog::invalidDateSlot);
        connect(dateValidator, &DateValidator::validDateSignal, this, &EditTaskDialog::validDateSlot);
    }

    QString getTaskName() const { return ui->taskNameLineEdit->text(); }
    QString getDeadline() const { return ui->deadlineLineEdit->text(); }
    QString getResponsible() const { return ui->responsibleLineEdit->text(); }
    QString getEmail() const { return ui->emailLineEdit->text(); }
    bool getIsImportant() const { return ui->isImportantCheckBox->isChecked(); }
    bool getIsMyDay() const { return ui->isMyDayCheckBox->isChecked(); }
    int getStatus() const {
        if (ui->statusLabel->text() == "Status: In Process")
            return static_cast<int>(STATUS::IN_PROCESS);
        else if (ui->statusLabel->text() == "Status: Completed")
            return static_cast<int>(STATUS::COMPLETED);
        else 
            return static_cast<int>(STATUS::FAILED);
    }

    void setStatus(QString status) { ui->statusLabel->setText("Status: " + status); }
    void setTaskName(QString taskName) { ui->taskNameLineEdit->setText(taskName); }
    void setDeadline(QString deadline) { ui->deadlineLineEdit->setText(deadline); }
    void setResponsible(QString responsible) { ui->responsibleLineEdit->setText(responsible); }
    void setEmail(QString email) { ui->emailLineEdit->setText(email); }
    void setIsMyDay(bool isMyDay) { ui->isMyDayCheckBox->setChecked(isMyDay); }
    void setIsImportant(bool isImportant) { ui->isImportantCheckBox->setChecked(isImportant); }

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