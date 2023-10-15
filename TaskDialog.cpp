#include "TaskDialog.hpp"

TaskDialog::TaskDialog(EmailValidator* emailValidator, DateValidator* dateValidator, QWidget* parent)
    : QDialog(parent), ui(new Ui::TaskDialog()) {
    ui->setupUi(this);
    ui->deadlineLineEdit->setValidator(dateValidator);
    ui->emailLineEdit->setValidator(emailValidator);
    this->setWindowIcon(QIcon("Assets/program_icon.png"));

    connect(dateValidator, &DateValidator::invalidDateSignal, this, &TaskDialog::invalidDateSlot);
    connect(dateValidator, &DateValidator::validDateSignal, this, &TaskDialog::validDateSlot);
    connect(dateValidator, &DateValidator::emptyDateSignal, this, &TaskDialog::validDateSlot);

    connect(emailValidator, &EmailValidator::invalidEmailSignal, this, &TaskDialog::invalidEmailSlot);
    connect(emailValidator, &EmailValidator::validEmailSignal, this, &TaskDialog::validEmailSlot);
    connect(emailValidator, &EmailValidator::emptyEmailSignal, this, &TaskDialog::validEmailSlot);

    connect(ui->taskNameLineEdit, &QLineEdit::textChanged, this, &TaskDialog::lineEditTextChanged);
    connect(ui->deadlineLineEdit, &QLineEdit::textChanged, this, &TaskDialog::lineEditTextChanged);
    connect(ui->emailLineEdit, &QLineEdit::textChanged, this, &TaskDialog::lineEditTextChanged);
}

void TaskDialog::invalidDateSlot() {
    ui->okButton->setDisabled(true);
    ui->dateStateLabel->setText(tr("Invalid deadline"));
    ui->dateStateLabel->setStyleSheet("color: red; font-weight: bold;");
}

void TaskDialog::validDateSlot() {
    ui->dateStateLabel->setText(tr("Valid deadline"));
    ui->dateStateLabel->setStyleSheet("color: green; font-weight: bold;");
}

void TaskDialog::invalidEmailSlot() {
    ui->okButton->setDisabled(true);
    ui->emailStateLabel->setText(tr("Invalid email"));
    ui->emailStateLabel->setStyleSheet("color: red; font-weight: bold;");
}

void TaskDialog::validEmailSlot() {
    ui->emailStateLabel->setText(tr("Valid email"));
    ui->emailStateLabel->setStyleSheet("color: green; font-weight: bold;");
}

void TaskDialog::lineEditTextChanged() {
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