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
    TaskDialog(EmailValidator* emailValidator, DateValidator* dateValidator, QWidget* parent = nullptr);

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
    void invalidDateSlot();
    void validDateSlot();
    void invalidEmailSlot();
    void validEmailSlot();
    void lineEditTextChanged();
};