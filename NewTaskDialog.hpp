#pragma once

#include <QtWidgets>
#include <QString>

#include "ui_NewTaskDialog.h"

class NewTaskDialog : public QDialog {
private:
	Ui::NewTaskDialogClass *ui;
public:
	NewTaskDialog(QWidget *parent = nullptr) 
		:QDialog(parent), ui(new Ui::NewTaskDialogClass) {
		ui->setupUi(this);
	}

	QString getTaskName() const { return ui->taskNameTextEdit->toPlainText(); }
	QString getDeadline() const { return ui->deadlineTextEdit->toPlainText(); }
	bool getIsImportant() const { return ui->isImportantCheckBox->isChecked(); }
	bool getIsMyDay() const { return ui->isMyDayCheckBox->isChecked(); }
};

