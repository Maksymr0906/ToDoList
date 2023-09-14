#include <QString>
#include <QWidget>
#include <QDialog>

#include "ui_NewTaskDialog.h"
#include "DateValidator.hpp"

class NewTaskDialog : public QDialog {
    Q_OBJECT
private:
    Ui::NewTaskDialogClass *ui;

public:
    NewTaskDialog(QWidget *parent = nullptr) : QDialog(parent), ui(new Ui::NewTaskDialogClass) {
        ui->setupUi(this);
        DateValidator *dateValidator = new DateValidator(this);
        ui->deadlineLineEdit->setValidator(dateValidator);

        connect(dateValidator, SIGNAL(invalidDateSignal()), this, SLOT(invalidDateSlot()));
        connect(dateValidator, SIGNAL(validDateSignal()), this, SLOT(validDateSlot()));
    }

    QString getTaskName() const { return ui->taskNameLineEdit->text(); }
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