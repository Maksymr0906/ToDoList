#include "ToDoList.hpp"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent),
      dateValidator{ new DateValidator(this) } {
    this->setWindowIcon(QIcon("Assets/program_icon.png"));

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("todolist");
    db.setUserName("postgres");
    db.setPassword("12345");

    if(!db.open())
        QMessageBox::critical(this, "Error", "Error opening database");
    
    mainModel = new QSqlTableModel(this);
    mainModel->setTable("todolist");
    mainModel->select();

    createToolBar();
    this->addToolBar(Qt::LeftToolBarArea, toolBar);

    createMenuBar();
    this->setMenuBar(menuBar);

    createTitleFrame();
    createTasksTableFrame();
    createAddNewTaskFrame();
    createCentralWidget();
    setCentralWidget(centralWidget);
    removeTaskButton->setEnabled(false);
    editTaskButton->setEnabled(false);
    // Connects
    connect(addTaskButton, SIGNAL(clicked()), this, SLOT(actionAddTriggered()));
    connect(editTaskButton, SIGNAL(clicked()), this, SLOT(actionEditTriggered()));
    connect(removeTaskButton, SIGNAL(clicked()), this, SLOT(actionRemoveTriggered()));
    connect(actionMyDay, &QAction::triggered, this, &ToDoList::actionMyDayTriggered);
    connect(actionImportant, &QAction::triggered, this, &ToDoList::actionImportantTriggered);
    connect(actionAll, &QAction::triggered, this, &ToDoList::actionAllTriggered);
    connect(actionPlanned, &QAction::triggered, this, &ToDoList::actionPlannedTriggered);
    connect(actionCompleted, &QAction::triggered, this, &ToDoList::actionCompletedTriggered);
    connect(actionFailed, &QAction::triggered, this, &ToDoList::actionFailedTriggered);
    connect(actionAboutProgram, &QAction::triggered, this, &ToDoList::actionAboutProgramTriggered);
    connect(tasksTableFrame->getView()->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ToDoList::updateButtonsState);
    connect(actionMarkAsCompleted, &QAction::triggered, this, &ToDoList::actionMarkAsCompletedTriggered);
    connect(actionMarkAsFailed, &QAction::triggered, this, &ToDoList::actionMarkAsFailedTriggered);
}

ToDoList::~ToDoList() {
    delete dateValidator;
    delete mainModel;
    delete toolBar;
    delete actionAll;
    delete actionCompleted;
    delete actionFailed;
    delete actionImportant;
    delete actionMyDay;
    delete actionPlanned;
    delete menuBar;
    delete menuSettings;
    delete actionAboutProgram;
    delete menuLanguage;
    delete actionEnglish;
    delete actionUkrainian;
    delete menuTask;
    delete actionMarkAsCompleted;
    delete actionMarkAsFailed;
    delete addNewTaskFrame;
    delete addTaskButton;
    delete removeTaskButton;
    delete titleFrame;
    delete titleImage;
    delete titleText;
    delete tasksTableFrame;
    delete centralWidget;
}

void ToDoList::createToolBar() {
    actionMyDay = createAction("My Day", "Assets/myday_icon.png");
    actionImportant = createAction("Important", "Assets/important_icon.png");
    actionAll = createAction("All", "Assets/all_icon.png");
    actionPlanned = createAction("Planned", "Assets/planned_icon.png");
    actionCompleted = createAction("Completed", "Assets/done_icon.png");
    actionFailed = createAction("Failed", "Assets/failed_icon.png");

    toolBar = new QToolBar("Tasks", this);
    toolBar->addActions({ actionMyDay , actionImportant, actionAll, actionPlanned, actionCompleted, actionFailed });
}

void ToDoList::createMenuBar() {
    menuBar = new QMenuBar(this);

    menuSettings = new QMenu("Settings", this);
    actionAboutProgram = new QAction("About program", this);
    menuSettings->addAction(actionAboutProgram);

    menuLanguage = new QMenu("Language", this);
    actionEnglish = new QAction("English", this);
    actionUkrainian = new QAction("Ukrainian", this);

    menuLanguage->addAction(actionEnglish);
    menuLanguage->addAction(actionUkrainian);

    menuSettings->addMenu(menuLanguage);

    menuTask = new QMenu("Task", this);
    actionMarkAsCompleted = new QAction("Mark as Completed", this);
    actionMarkAsFailed = new QAction("Mark as Failed", this);
    actionMarkAsCompleted->setEnabled(false);
    actionMarkAsFailed->setEnabled(false);

    menuTask->addAction(actionMarkAsCompleted);
    menuTask->addAction(actionMarkAsFailed);

    menuBar->addMenu(menuSettings);
    menuBar->addMenu(menuTask);
}

void ToDoList::createTitleFrame() {
    titleFrame = new QFrame(this);
    titleImage = new QLabel(this);
    titleImage->setPixmap(QString("Assets/myday_icon.png"));
    titleImage->setMaximumSize(QSize(40, 40));
    titleImage->setScaledContents(true);
    titleText = new QLabel("My Day", this);
    titleText->setStyleSheet("font-family: Segoe UI; font-size: 18px; font-weight: bold;");
    QHBoxLayout* titleLayout = new QHBoxLayout(titleFrame);
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Minimum);
    titleLayout->addSpacerItem(spacer);
    titleLayout->addWidget(titleText);
    titleLayout->addWidget(titleImage);
    titleFrame->setLayout(titleLayout);
    titleFrame->setFixedHeight(60);
}

void ToDoList::createTasksTableFrame() {
    tasksTableFrame = new TasksTableFrame(mainModel);
    tasksTableFrame->setFixedSize(QSize(750, 400));
    tasksTableFrame->setFilter("true", 6);
}

void ToDoList::createAddNewTaskFrame() {
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Minimum);
    addNewTaskFrame = new QFrame(this);
    addTaskButton = new QPushButton(this);
    addTaskButton->setIcon(QIcon("Assets/add_icon.png"));
    addTaskButton->setStyleSheet("background-color: transparent; border: none;");
    addTaskButton->setIconSize(QSize(40, 40));
    removeTaskButton = new QPushButton(this);
    removeTaskButton->setIcon(QIcon("Assets/remove_icon.png"));
    removeTaskButton->setStyleSheet("background-color: transparent; border: none;");
    removeTaskButton->setIconSize(QSize(40, 40));
    editTaskButton = new QPushButton();
    editTaskButton->setIcon(QIcon("Assets/edit_icon.png"));
    editTaskButton->setStyleSheet("background-color: transparent; border: none;");
    editTaskButton->setIconSize(QSize(40, 40));

    QHBoxLayout* addNewTaskLayout = new QHBoxLayout();
    addNewTaskLayout->addSpacerItem(spacer);
    addNewTaskLayout->addWidget(addTaskButton);
    addNewTaskLayout->addWidget(editTaskButton);
    addNewTaskLayout->addWidget(removeTaskButton);
    addNewTaskLayout->addSpacerItem(spacer);

    addNewTaskFrame->setLayout(addNewTaskLayout);
    addNewTaskFrame->setFixedHeight(80);
}

void ToDoList::createCentralWidget() {
    centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(titleFrame);
    mainLayout->addWidget(tasksTableFrame);
    mainLayout->addWidget(addNewTaskFrame);

    centralWidget->setLayout(mainLayout);
}

QAction* ToDoList::createAction(const QString& text, const QString& iconPath) {
    QAction* action = new QAction(this);
    action->setText(text);
    action->setIcon(QIcon(iconPath));
    return action;
}
void ToDoList::actionAddTriggered() {
    const int IN_PROCESS = 0;
    std::unique_ptr<NewTaskDialog> newTaskDialog = std::make_unique<NewTaskDialog>(dateValidator);
    newTaskDialog->setFixedSize(462, 434);
    if(newTaskDialog->exec() == QDialog::Accepted) {
        QSqlQuery insertQuery;
        QString insertQueryString = "INSERT INTO todolist (task_name, deadline, responsible, email, status, is_important, is_my_day) "
            "VALUES (:task_name, :deadline, :responsible, :email, :status, :is_important, :is_my_day);";

        insertQuery.prepare(insertQueryString);
        insertQuery.bindValue(":task_name", newTaskDialog->getTaskName());
        insertQuery.bindValue(":deadline", newTaskDialog->getDeadline());
        insertQuery.bindValue(":responsible", newTaskDialog->getResponsible());
        insertQuery.bindValue(":email", newTaskDialog->getEmail());
        insertQuery.bindValue(":status", IN_PROCESS);
        insertQuery.bindValue(":is_important", newTaskDialog->getIsImportant());
        insertQuery.bindValue(":is_my_day", newTaskDialog->getIsMyDay());

        if (!insertQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Adding task error: %1").arg(insertQuery.lastError().text()));
            return;
        }

        refreshTasks();
    }
}

void ToDoList::actionEditTriggered() {
    if (!(tasksTableFrame->getSelectionModel() && tasksTableFrame->getSelectionModel()->selectedRows().count() > 0)) {
        QMessageBox::information(this, "Error", "Choose the row at first");
        return;
    }

    QModelIndex selectedRowIndex = tasksTableFrame->getSelectionModel()->selectedRows().at(0);
    QSortFilterProxyModel* sortedModel = tasksTableFrame->getSortedModel();

    Task task;
    task.taskName = sortedModel->data(selectedRowIndex).toString();
    task.deadline = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 1)).toString();
    task.responsible = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 2)).toString();
    task.email = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 3)).toString();
    task.status = static_cast<STATUS>(sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 4)).toInt());
    task.isImportant = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 5)).toBool();
    task.isMyDay = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 6)).toBool();

    std::unique_ptr<EditTaskDialog> editTaskDialog = std::make_unique<EditTaskDialog>(dateValidator, this);
    editTaskDialog->setFixedSize(462, 434);
    editTaskDialog->setStatus(task.statusToString());
    editTaskDialog->setTaskName(task.taskName);
    editTaskDialog->setDeadline(task.deadline);
    editTaskDialog->setResponsible(task.responsible);
    editTaskDialog->setEmail(task.email);
    editTaskDialog->setIsImportant(task.isImportant);
    editTaskDialog->setIsMyDay(task.isMyDay);

    if (editTaskDialog->exec() == QDialog::Accepted) {
        QSqlQuery updateQuery;
        QString updateQueryString = "UPDATE todolist "
            "SET task_name = :new_task_name, deadline = :new_deadline, responsible = :new_responsible, email = :new_email, status = :new_status, is_important = :new_important, is_my_day = :new_my_day "
            "WHERE task_name = :old_task_name AND deadline = :old_deadline AND responsible = :old_responsible AND email = :old_email AND status = :old_status AND is_important = :old_important AND is_my_day = :old_my_day;";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_task_name", editTaskDialog->getTaskName());
        updateQuery.bindValue(":new_deadline", editTaskDialog->getDeadline());
        updateQuery.bindValue(":new_responsible", editTaskDialog->getResponsible());
        updateQuery.bindValue(":new_email", editTaskDialog->getEmail());
        updateQuery.bindValue(":new_status", static_cast<int>(task.status));
        updateQuery.bindValue(":new_important", editTaskDialog->getIsImportant());
        updateQuery.bindValue(":new_my_day", editTaskDialog->getIsMyDay());
        updateQuery.bindValue(":old_task_name", task.taskName);
        updateQuery.bindValue(":old_deadline", task.deadline);
        updateQuery.bindValue(":old_responsible", task.responsible);
        updateQuery.bindValue(":old_email", task.email);
        updateQuery.bindValue(":old_status", static_cast<int>(task.status));
        updateQuery.bindValue(":old_important", task.isImportant);
        updateQuery.bindValue(":old_my_day", task.isMyDay);
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
            return;
        }

        refreshTasks();
    }
}

void ToDoList::actionRemoveTriggered() {
    if (!(tasksTableFrame->getSelectionModel() && tasksTableFrame->getSelectionModel()->selectedRows().count() > 0)) {
        QMessageBox::information(this, "Error", "Choose the row at first");
        return;
    }

    QModelIndex selectedRowIndex = tasksTableFrame->getSelectionModel()->selectedRows().at(0);
    QSortFilterProxyModel* sortedModel = tasksTableFrame->getSortedModel();

    Task task;
    task.taskName = sortedModel->data(selectedRowIndex).toString();
    task.deadline = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 1)).toString();
    task.responsible = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 2)).toString();
    task.email = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 3)).toString();
    task.status = static_cast<STATUS>(sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 4)).toInt());
    task.isImportant = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 5)).toBool();
    task.isMyDay = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 6)).toBool();
    
    QString taskNameCondition = task.taskName.isEmpty() ? "task_name IS NULL" : "task_name = '" + task.taskName + "'";
    QString deadlineCondition = task.deadline.isEmpty() ? "deadline IS NULL" : "deadline = '" + task.deadline + "'";
    QString responsibleCondition = task.responsible.isEmpty() ? "responsible IS NULL" : "responsible = '" + task.responsible + "'";
    QString emailCondition = task.email.isEmpty() ? "email IS NULL" : "email = '" + task.email + "'";

    QSqlQuery deleteQuery;
    QString deleteQueryString = "DELETE FROM todolist WHERE "
        + taskNameCondition + " AND "
        + deadlineCondition + " AND "
        + responsibleCondition + " AND "
        + emailCondition + " AND "
        "status = :status AND is_important = :isImportant AND is_my_day = :isMyDay";

    deleteQuery.prepare(deleteQueryString);
    deleteQuery.bindValue(":status", static_cast<int>(task.status));
    deleteQuery.bindValue(":isImportant", task.isImportant);
    deleteQuery.bindValue(":isMyDay", task.isMyDay);

    bool deleteResult = deleteQuery.exec();

    if (!deleteResult) {
        QMessageBox::critical(this, tr("Error"), tr("Deleting task error: %1").arg(deleteQuery.lastError().text()));
        return;
    }

    refreshTasks();
}

void ToDoList::updateButtonsState() {
    if (tasksTableFrame->getView()->selectionModel()->hasSelection()) {
        removeTaskButton->setEnabled(true);
        editTaskButton->setEnabled(true);
        actionMarkAsCompleted->setEnabled(true);
        actionMarkAsFailed->setEnabled(true);
    }
    else {
        removeTaskButton->setEnabled(false);
        editTaskButton->setEnabled(false);
        actionMarkAsCompleted->setEnabled(false);
        actionMarkAsFailed->setEnabled(false);
    }
}

void ToDoList::actionMyDayTriggered() {
    refreshTitle(TASK_TYPE::MY_DAY);
    tasksTableFrame->setFilter("true", 6);
}

void ToDoList::actionImportantTriggered() {
    refreshTitle(TASK_TYPE::IMPORTANT);
    tasksTableFrame->setFilter("true", 5);
}

void ToDoList::actionAllTriggered() {
    refreshTitle(TASK_TYPE::ALL);
    tasksTableFrame->resetFilter();
}

void ToDoList::actionPlannedTriggered() {
    refreshTitle(TASK_TYPE::PLANNED);
    tasksTableFrame->setFilter("20", 1);
}

void ToDoList::actionCompletedTriggered() {
    refreshTitle(TASK_TYPE::COMPLETED);
    tasksTableFrame->setFilter("1", 4);
}

void ToDoList::actionFailedTriggered() {
    refreshTitle(TASK_TYPE::FAILED);
    tasksTableFrame->setFilter("2", 4);
}

void ToDoList::actionAboutProgramTriggered() {
    QMessageBox::information(this, "About Program", "Action About Program triggered");
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
        titleText->setText(taskTypeTotitleText.value(taskType));
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
        titleImage->setPixmap(taskTypeToIcon.value(taskType));
    }
}

void ToDoList::refreshTasks() {
    editTaskButton->setEnabled(false);
    removeTaskButton->setEnabled(false);
    actionMarkAsCompleted->setEnabled(false);
    actionMarkAsFailed->setEnabled(false);

    QSqlQuery selectQuery;
    if (!selectQuery.exec("SELECT * FROM todolist;")) {
        QMessageBox::critical(this, "Error making a query", "Error making a query");
        return;
    }

    while (selectQuery.next()) {
        Task task;
        task.taskName = selectQuery.value("task_name").toString();
        task.deadline = selectQuery.value("deadline").toString();
        task.responsible = selectQuery.value("responsible").toString();
        task.email = selectQuery.value("email").toString();
        task.status = static_cast<STATUS>(selectQuery.value("status").toInt());
        task.isMyDay = selectQuery.value("is_my_day").toBool();
        task.isImportant = selectQuery.value("is_important").toBool();
    }

    mainModel->select();
}

void ToDoList::actionMarkAsCompletedTriggered() {
    markTask(STATUS::COMPLETED, "Choose the row to mark as completed");
}

void ToDoList::actionMarkAsFailedTriggered() {
    markTask(STATUS::FAILED, "Choose the row to mark as failed");
}

void ToDoList::markTask(STATUS newStatus, const QString& errorMessage) {
    if (!(tasksTableFrame->getSelectionModel() && tasksTableFrame->getSelectionModel()->selectedRows().count() > 0)) {
        QMessageBox::information(this, "Error", "Choose the row at first");
        return;
    }

    QModelIndex selectedRowIndex = tasksTableFrame->getSelectionModel()->selectedRows().at(0);
    QSortFilterProxyModel* sortedModel = tasksTableFrame->getSortedModel();

    Task task;
    task.taskName = sortedModel->data(selectedRowIndex).toString();
    task.deadline = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 1)).toString();
    task.responsible = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 2)).toString();
    task.email = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 3)).toString();
    task.status = static_cast<STATUS>(sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 4)).toInt());
    task.isImportant = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 5)).toBool();
    task.isMyDay = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 6)).toBool();

    QSqlQuery updateQuery;
    QString updateQueryString = "UPDATE todolist "
        "SET status = :new_status "
        "WHERE task_name = :old_task_name AND deadline = :old_deadline AND status = :old_status AND is_important = :old_important AND is_my_day = :old_my_day;";

    updateQuery.prepare(updateQueryString);
    if (task.status != newStatus)
        updateQuery.bindValue(":new_status", static_cast<int>(newStatus));
    else
        updateQuery.bindValue(":new_status", static_cast<int>(STATUS::IN_PROCESS));
    updateQuery.bindValue(":old_task_name", task.taskName);
    updateQuery.bindValue(":old_deadline", task.deadline);
    updateQuery.bindValue(":old_status", static_cast<int>(task.status));
    updateQuery.bindValue(":old_important", task.isImportant);
    updateQuery.bindValue(":old_my_day", task.isMyDay);
    if (!updateQuery.exec()) {
        QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
        return;
    }

    refreshTasks();
}