#pragma once

#include <QtWidgets>

#include "ui_NewTaskDialog.h"

class NewTaskDialog : public QDialog {
private:
	Ui::NewTaskDialogClass *ui;
public:
	NewTaskDialog(QWidget *parent = nullptr) 
		:QDialog(parent), ui(new Ui::NewTaskDialogClass) {
		ui->setupUi(this);
	}
};

