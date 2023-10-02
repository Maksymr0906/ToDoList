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
        connect(ui->removeButton, &QPushButton::clicked, this, &EditTaskDialog::removeButtonClicked);
    }

    QString getTaskName() const { return ui->taskNameLineEdit->text(); }
    QString getDeadline() const { return ui->deadlineLineEdit->text(); }
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

    void removeButtonClicked() {
        QSqlQuery deleteQuery;
        QString deleteQueryString = "DELETE FROM todolist WHERE task_name = '%1' AND deadline = '%2' AND status = '%3' AND is_important = '%4' AND is_my_day = '%5';";
        bool deleteResult = deleteQuery.exec(deleteQueryString.arg(this->getTaskName())
            .arg(this->getDeadline())
            .arg(this->getStatus())
            .arg(this->getIsImportant())
            .arg(this->getIsMyDay())
        );
        if (!deleteResult) {
            QMessageBox::critical(this, tr("Error"), tr("Deleting task error"));
            return;
        }
        
        this->close();
    }
};