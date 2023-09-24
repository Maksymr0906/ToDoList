#include "ToDoList.hpp"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::ToDoListClass) {
    ui->setupUi(this);
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
    connect(ui->actionEdit, &QAction::triggered, this, &ToDoList::actionEditTriggered);
    connect(ui->editTaskButton, SIGNAL(clicked()), this, SLOT(actionEditTriggered()));
    connect(ui->actionRemove, &QAction::triggered, this, &ToDoList::actionRemoveTriggered);
    connect(ui->removeTaskButton, SIGNAL(clicked()), this, SLOT(actionRemoveTriggered()));
    connect(ui->actionMyDay, &QAction::triggered, this, &ToDoList::actionMyDayTriggered);
    connect(ui->actionImportant, &QAction::triggered, this, &ToDoList::actionImportantTriggered);
    connect(ui->actionAll, &QAction::triggered, this, &ToDoList::actionAllTriggered);
    connect(ui->actionPlanned, &QAction::triggered, this, &ToDoList::actionPlannedTriggered);
    connect(ui->actionCompleted, &QAction::triggered, this, &ToDoList::actionCompletedTriggered);
    connect(ui->actionFailed, &QAction::triggered, this, &ToDoList::actionFailedTriggered);
    connect(ui->actionAboutProgram, &QAction::triggered, this, &ToDoList::actionAboutProgramTriggered);

    refreshTasks("SELECT * FROM todolist WHERE is_my_day;", TASK_TYPE::MY_DAY);
}

ToDoList::~ToDoList() {
    delete ui;
}

void ToDoList::actionAddTriggered() {
    const int IN_PROCESS = 0;
    std::unique_ptr<NewTaskDialog> newTaskDialog = std::make_unique<NewTaskDialog>();
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

void ToDoList::actionEditTriggered() {
    QMessageBox::information(this, "Edit", "Action Edit triggered");
}

void ToDoList::actionRemoveTriggered() {
    QMessageBox::information(this, "Remove", "Action Remove triggered");
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
}

void ToDoList::fillTasks() {

}

void ToDoList::refreshTasks(const QString &queryCondition, TASK_TYPE taskType) {
    refreshTitle(taskType);
    QPixmap failedBtnIcon("Assets/failed_icon.png");
    QPixmap doneBtnIcon("Assets/done_icon.png");
    QPixmap myDayBtnIcon("Assets/myday_icon.png");

    QSqlQuery selectQuery;
    if(!selectQuery.exec(queryCondition)) {
        return;
    }

    clearTaskWidgets();

    QVBoxLayout *vMainLayout = qobject_cast<QVBoxLayout *>(ui->allNewTasksContents->layout());
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vMainLayout->addSpacerItem(verticalSpacer);
    
    QVector<QString> colors = { "rgba(66, 165, 245,1.0)", "rgba(41, 182, 246,1.0)", "rgba(38, 198, 218,1.0)", "rgba(38, 166, 154,1.0)", "rgba(102, 187, 106,1.0)", "rgba(156, 204, 101,1.0)", "rgba(212, 225, 87,1.0)", "rgba(255, 238, 88,1.0)", "rgba(255, 202, 40,1.0)", "rgba(255, 167, 38,1.0)" };

    while(selectQuery.next()) {
        Task task;
        task.taskName = selectQuery.value("task_name").toString();
        task.deadline = selectQuery.value("deadline").toString();
        task.status = static_cast<STATUS>(selectQuery.value("status").toInt());
        task.isMyDay = selectQuery.value("is_my_day").toBool();
        task.isImportant = selectQuery.value("is_important").toBool();

        TaskFrame* newTaskFrame = new TaskFrame(task, this);
        vMainLayout->insertWidget(0, newTaskFrame);
        
        connect(newTaskFrame, &TaskFrame::clicked, this, [this, task]() {
            editTask(task);
        });
    }
}

void ToDoList::editTask(Task task) {
    std::unique_ptr<EditTaskDialog> editTaskDialog = std::make_unique<EditTaskDialog>();
    editTaskDialog->setTaskName(task.taskName);
    editTaskDialog->setDeadline(task.deadline);
    editTaskDialog->setIsImportant(task.isImportant);
    editTaskDialog->setIsMyDay(task.isMyDay);

    if (editTaskDialog->exec() == QDialog::Accepted) {
        QSqlQuery updateQuery;
        QString updateQueryString = "UPDATE todolist "
            "SET task_name = :new_task_name, deadline = :new_deadline, status = :new_status, is_important = :new_important, is_my_day = :new_my_day "
            "WHERE task_name = :old_task_name AND deadline = :old_deadline AND status = :old_status AND is_important = :old_important AND is_my_day = :old_my_day;";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_task_name", editTaskDialog->getTaskName());
        updateQuery.bindValue(":new_deadline", editTaskDialog->getDeadline());
        updateQuery.bindValue(":new_status", static_cast<int>(task.status));
        updateQuery.bindValue(":new_important", editTaskDialog->getIsImportant());
        updateQuery.bindValue(":new_my_day", editTaskDialog->getIsMyDay());
        updateQuery.bindValue(":old_task_name", task.taskName);
        updateQuery.bindValue(":old_deadline", task.deadline);
        updateQuery.bindValue(":old_status", static_cast<int>(task.status));
        updateQuery.bindValue(":old_important", task.isImportant);
        updateQuery.bindValue(":old_my_day", task.isMyDay);
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
            return;
        }

        refreshTasks("SELECT * FROM todolist", TASK_TYPE::ALL);
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