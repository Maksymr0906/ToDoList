#pragma once

#include <QString>
#include <QWidget>
#include <QFrame>
#include <QRadioButton>
#include <QPushButton>
#include <QDialog>
#include <QMessageBox>
#include <memory>
#include <QTSql>

#include "ui_RemoveTaskDialog.h"

class RemoveTaskDialog : public QDialog {
    Q_OBJECT
private:
    std::unique_ptr<Ui::RemoveTaskDialog> ui;

public:
    RemoveTaskDialog(QWidget* parent = nullptr) : QDialog(parent), ui(new Ui::RemoveTaskDialog()) {
        ui->setupUi(this);
    }

    RemoveTaskDialog(int numberOfElements, QWidget* parent = nullptr) : RemoveTaskDialog(parent) {
        while (numberOfElements > 0) {
            QFrame* newTaskToDeleteFrame = new QFrame();
            QHBoxLayout* layout = new QHBoxLayout();
            newTaskToDeleteFrame->setLayout(layout);
            QLabel* taskName = new QLabel("Task #");
            QRadioButton* btn = new QRadioButton();
            layout->addWidget(btn);
            //QSpacerItem *spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Minimum);
            //layout->addSpacerItem(spacer);
            layout->addWidget(taskName);

            ui->mainLayout->insertWidget(0, newTaskToDeleteFrame);

            QMessageBox::information(this, "123", QString::number(numberOfElements));
            numberOfElements--;
        }
        
    }
};
