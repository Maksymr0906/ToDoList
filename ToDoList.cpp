#include "ToDoList.hpp"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ToDoListClass),
      dateValidator{ new DateValidator(this) } {
    ui->setupUi(this);
    ui->refreshBtn->setVisible(false);
    ui->refreshBtn->setEnabled(false);
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("todolist");
    db.setUserName("postgres");
    db.setPassword("12345");

    if(!db.open())
        QMessageBox::critical(this, "Error", "Error opening database");

    // connects
    connect(ui->actionAdd, &QAction::triggered, this, &ToDoList::actionAddTriggered);
    connect(ui->addTaskButton, SIGNAL(clicked()), this, SLOT(actionAddTriggered()));
    connect(ui->actionRemove, &QAction::triggered, this, &ToDoList::actionRemoveTriggered);
    connect(ui->removeTaskButton, SIGNAL(clicked()), this, SLOT(actionRemoveTriggered()));
    connect(ui->actionMyDay, &QAction::triggered, this, &ToDoList::actionMyDayTriggered);
    connect(ui->actionImportant, &QAction::triggered, this, &ToDoList::actionImportantTriggered);
    connect(ui->actionAll, &QAction::triggered, this, &ToDoList::actionAllTriggered);
    connect(ui->actionPlanned, &QAction::triggered, this, &ToDoList::actionPlannedTriggered);
    connect(ui->actionCompleted, &QAction::triggered, this, &ToDoList::actionCompletedTriggered);
    connect(ui->actionFailed, &QAction::triggered, this, &ToDoList::actionFailedTriggered);
    connect(ui->actionAboutProgram, &QAction::triggered, this, &ToDoList::actionAboutProgramTriggered);
    connect(ui->refreshBtn, SIGNAL(clicked()), this, SLOT(refreshBtnClicked()));

    refreshTasks("SELECT * FROM todolist WHERE is_my_day;", TASK_TYPE::MY_DAY);
}

ToDoList::~ToDoList() {
    delete ui;
}

void ToDoList::refreshBtnClicked() {
    refreshTasks("SELECT * FROM todolist;", TASK_TYPE::ALL);
    ui->refreshBtn->setEnabled(false);
    ui->refreshBtn->setVisible(false);
    ui->actionAll->setEnabled(true);
    ui->actionCompleted->setEnabled(true);
    ui->actionImportant->setEnabled(true);
    ui->actionFailed->setEnabled(true);
    ui->actionPlanned->setEnabled(true);
    ui->actionMyDay->setEnabled(true);
}

void ToDoList::actionAddTriggered() {
    const int IN_PROCESS = 0;
    std::unique_ptr<NewTaskDialog> newTaskDialog = std::make_unique<NewTaskDialog>(dateValidator);
    newTaskDialog->setFixedSize(317, 255);
    if(newTaskDialog->exec() == QDialog::Accepted) {
        QSqlQuery insertQuery;
        QString insertQueryString = "insert into todolist(task_name, deadline, status, is_important, is_my_day)"
            "values('%1', '%2', '%3', '%4', '%5');";
        bool insertResult = insertQuery.exec(insertQueryString.arg(newTaskDialog->getTaskName())
                                             .arg(newTaskDialog->getDeadline())
                                             .arg(IN_PROCESS)
                                             .arg(newTaskDialog->getIsImportant())
                                             .arg(newTaskDialog->getIsMyDay())
        );
        if(!insertResult) {
            QMessageBox::critical(this, tr("Error"), tr("Adding task error"));
            return;
        }

        refreshTasks("SELECT * FROM todolist;", TASK_TYPE::ALL);
    }
}

void ToDoList::actionRemoveTriggered() {
    std::unique_ptr<RemoveTaskDialog> removeTaskDialog = std::make_unique<RemoveTaskDialog>(ui->verticalLayout->count());
    if (removeTaskDialog->exec() == QDialog::Accepted) {
        QVBoxLayout* vMainLayout = qobject_cast<QVBoxLayout*>(ui->allNewTasksContents->layout());

        QLayoutItem* child = vMainLayout->takeAt(0);

        QWidget* widget = child->widget();
        if (widget) {
            delete widget;
        }
        if (child)
            delete child;
    }
}

void ToDoList::actionMyDayTriggered() {
    refreshTasks("SELECT * FROM todolist WHERE is_my_day;", TASK_TYPE::MY_DAY);
}

void ToDoList::actionImportantTriggered() {
    refreshTasks("SELECT * FROM todolist WHERE is_important;", TASK_TYPE::IMPORTANT);
}

void ToDoList::actionAllTriggered() {
    refreshTasks("SELECT * FROM todolist;", TASK_TYPE::ALL);
}

void ToDoList::actionPlannedTriggered() {
    refreshTasks("SELECT * FROM todolist WHERE deadline IS NOT NULL;", TASK_TYPE::PLANNED);
}

void ToDoList::actionCompletedTriggered() {
    refreshTasks("SELECT * FROM todolist WHERE status = 1;", TASK_TYPE::COMPLETED);
}

void ToDoList::actionFailedTriggered() {
    refreshTasks("SELECT * FROM todolist WHERE status = 2;", TASK_TYPE::FAILED);
}

void ToDoList::actionAboutProgramTriggered() {
    QMessageBox::information(this, "About Program", "Action About Program triggered");
}

void ToDoList::clearTaskWidgets() {
    QVBoxLayout* vMainLayout = qobject_cast<QVBoxLayout*>(ui->allNewTasksContents->layout());

    QLayoutItem* child;
    while ((child = vMainLayout->takeAt(0)) != nullptr) {
        QWidget* widget = child->widget();
        if (widget) {
            delete widget;
        }
        if(child)
            delete child;
    }

    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayout->addSpacerItem(verticalSpacer);
}

void ToDoList::refreshTasks(const QString &queryCondition, TASK_TYPE taskType) {
    refreshTitle(taskType);

    QSqlQuery selectQuery;
    if(!selectQuery.exec(queryCondition)) {
        QMessageBox::critical(this, "Error making an querry", "Error making an querry");
        return;
    }

    clearTaskWidgets();
    
    while(selectQuery.next()) {
        Task task;
        task.taskName = selectQuery.value("task_name").toString();
        task.deadline = selectQuery.value("deadline").toString();
        task.status = static_cast<STATUS>(selectQuery.value("status").toInt());
        task.isMyDay = selectQuery.value("is_my_day").toBool();
        task.isImportant = selectQuery.value("is_important").toBool();

        TaskFrame* newTaskFrame = new TaskFrame(task);
        newTaskFrame->setTitleLabelText(tr("Task #%1").arg(ui->verticalLayout->count()));
        ui->verticalLayout->insertWidget(0, newTaskFrame);
    }
}

void ToDoList::refreshTitle(TASK_TYPE taskType) {
    refreshTitleText(taskType);
    refreshTitleIcon(taskType);
}

void ToDoList::refreshTitleText(TASK_TYPE taskType) {
    static const QMap<TASK_TYPE, QString> taskTypeTotitleText = {
        {TASK_TYPE::MY_DAY, "My Day"},
        {TASK_TYPE::IMPORTANT, "Important"},
        {TASK_TYPE::ALL, "All"},
        {TASK_TYPE::PLANNED, "Planned"},
        {TASK_TYPE::COMPLETED, "Completed"},
        {TASK_TYPE::FAILED, "Failed"}
    };

    if(taskTypeTotitleText.contains(taskType)) {
        ui->titleText->setText(taskTypeTotitleText.value(taskType));
    }
}

void ToDoList::refreshTitleIcon(TASK_TYPE taskType) {
    static const QMap<TASK_TYPE, QString> taskTypeToIcon = {
        {TASK_TYPE::MY_DAY, "Assets/myday_icon.png"},
        {TASK_TYPE::IMPORTANT, "Assets/important_icon.png"},
        {TASK_TYPE::ALL, "Assets/all_icon.png"},
        {TASK_TYPE::PLANNED, "Assets/planned_icon.png"},
        {TASK_TYPE::COMPLETED, "Assets/done_icon.png"},
        {TASK_TYPE::FAILED, "Assets/failed_icon.png"}
    };

    if(taskTypeToIcon.contains(taskType)) {
        ui->titleImage->setPixmap(taskTypeToIcon.value(taskType));
    }
}