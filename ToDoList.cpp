#include "ToDoList.hpp"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent),
      dateValidator{ new DateValidator(this) },
      emailValidator{ new EmailValidator(this) } {
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
    connect(markAsCompletedButton, SIGNAL(clicked()), this, SLOT(actionMarkAsCompletedTriggered()));
    connect(markAsFailedButton, SIGNAL(clicked()), this, SLOT(actionMarkAsFailedTriggered()));
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
    markAsCompletedButton = new QPushButton(this);
    markAsCompletedButton->setIcon(QIcon("Assets/done_icon.png"));
    markAsCompletedButton->setStyleSheet("background-color: transparent; border: none;");
    markAsCompletedButton->setIconSize(QSize(35, 35));
    markAsCompletedButton->setEnabled(false);
    markAsCompletedButton->setVisible(false);
    markAsFailedButton = new QPushButton(this);
    markAsFailedButton->setIcon(QIcon("Assets/failed_icon.png"));
    markAsFailedButton->setStyleSheet("background-color: transparent; border: none;");
    markAsFailedButton->setIconSize(QSize(35, 35));
    markAsFailedButton->setEnabled(false);
    markAsFailedButton->setVisible(false);
    QHBoxLayout* titleLayout = new QHBoxLayout(titleFrame);
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Minimum);
    titleLayout->addWidget(markAsCompletedButton);
    titleLayout->addWidget(markAsFailedButton);
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
    removeTaskButton->setEnabled(false);
    editTaskButton->setEnabled(false);

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
            "VALUES (:task_name, :deadline, :responsible, :email, :status, :is_important, :is_my_day)";

        insertQuery.prepare(insertQueryString);
        insertQuery.bindValue(":task_name", newTaskDialog->getTaskName().isEmpty() ? QVariant(QVariant::String) : newTaskDialog->getTaskName());
        insertQuery.bindValue(":deadline", newTaskDialog->getDeadline().isEmpty() ? QVariant(QVariant::String) : newTaskDialog->getDeadline());
        insertQuery.bindValue(":responsible", newTaskDialog->getResponsible().isEmpty() ? QVariant(QVariant::String) : newTaskDialog->getResponsible());
        insertQuery.bindValue(":email", newTaskDialog->getEmail().isEmpty() ? QVariant(QVariant::String) : newTaskDialog->getEmail());
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

    Task task = getSelectedTask();

    std::unique_ptr<EditTaskDialog> editTaskDialog = std::make_unique<EditTaskDialog>(emailValidator, dateValidator, this);
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
            "WHERE id = :id";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_task_name", editTaskDialog->getTaskName().isEmpty() ? QVariant(QVariant::String) : editTaskDialog->getTaskName());
        updateQuery.bindValue(":new_deadline", editTaskDialog->getDeadline().isEmpty() ? QVariant(QVariant::String) : editTaskDialog->getDeadline());
        updateQuery.bindValue(":new_responsible", editTaskDialog->getResponsible().isEmpty() ? QVariant(QVariant::String) : editTaskDialog->getResponsible());
        updateQuery.bindValue(":new_email", editTaskDialog->getEmail().isEmpty()? QVariant(QVariant::String) : editTaskDialog->getEmail());
        updateQuery.bindValue(":new_status", static_cast<int>(task.status));
        updateQuery.bindValue(":new_important", editTaskDialog->getIsImportant());
        updateQuery.bindValue(":new_my_day", editTaskDialog->getIsMyDay());
        updateQuery.bindValue(":id", task.id);
        
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

    Task task = getSelectedTask();

    QSqlQuery deleteQuery;
    QString deleteQueryString = "DELETE FROM todolist WHERE id = :id";
    deleteQuery.prepare(deleteQueryString);
    deleteQuery.bindValue(":id", task.id);

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
        markAsCompletedButton->setEnabled(true);
        markAsCompletedButton->setVisible(true);
        markAsFailedButton->setEnabled(true);
        markAsFailedButton->setVisible(true);
    }
    else {
        removeTaskButton->setEnabled(false);
        editTaskButton->setEnabled(false);
        actionMarkAsCompleted->setEnabled(false);
        actionMarkAsFailed->setEnabled(false);
        markAsCompletedButton->setEnabled(false);
        markAsCompletedButton->setVisible(false);
        markAsFailedButton->setEnabled(false);
        markAsFailedButton->setVisible(false);
    }
}

void ToDoList::actionMyDayTriggered() {
    refreshTitle(TASK_TYPE::MY_DAY);
    tasksTableFrame->setFilter("true", 7);
}

void ToDoList::actionImportantTriggered() {
    refreshTitle(TASK_TYPE::IMPORTANT);
    tasksTableFrame->setFilter("true", 6);
}

void ToDoList::actionAllTriggered() {
    refreshTitle(TASK_TYPE::ALL);
    tasksTableFrame->resetFilter();
}

void ToDoList::actionPlannedTriggered() {
    refreshTitle(TASK_TYPE::PLANNED);
    tasksTableFrame->setFilter("20", 2);
}

void ToDoList::actionCompletedTriggered() {
    refreshTitle(TASK_TYPE::COMPLETED);
    tasksTableFrame->setFilter("1", 5);
}

void ToDoList::actionFailedTriggered() {
    refreshTitle(TASK_TYPE::FAILED);
    tasksTableFrame->setFilter("2", 5);
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
    markAsCompletedButton->setEnabled(false);
    markAsCompletedButton->setVisible(false);
    markAsFailedButton->setEnabled(false);
    markAsFailedButton->setVisible(false);

    QSqlQuery selectQuery;
    if (!selectQuery.exec("SELECT * FROM todolist;")) {
        QMessageBox::critical(this, "Error making a query", "Error making a query");
        return;
    }

    while (selectQuery.next()) {
        Task task;
        task.id = selectQuery.value("id").toInt();
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

    Task task = getSelectedTask();

    QSqlQuery updateQuery;
    QString updateQueryString = "UPDATE todolist "
        "SET status = :new_status "
        "WHERE id = :id";

    updateQuery.prepare(updateQueryString);
    if (task.status != newStatus)
        updateQuery.bindValue(":new_status", static_cast<int>(newStatus));
    else
        updateQuery.bindValue(":new_status", static_cast<int>(STATUS::IN_PROCESS));
    updateQuery.bindValue(":id", task.id);

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
        return;
    }

    refreshTasks();
}

Task ToDoList::getSelectedTask() {
    QModelIndex selectedRowIndex = tasksTableFrame->getSelectionModel()->selectedRows().at(0);
    QSortFilterProxyModel* sortedModel = tasksTableFrame->getSortedModel();

    Task task;
    task.id = sortedModel->data(selectedRowIndex).toInt();
    task.taskName = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 1)).toString();
    task.deadline = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 2)).toString();
    task.responsible = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 3)).toString();
    task.email = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 4)).toString();
    task.status = static_cast<STATUS>(sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 5)).toInt());
    task.isImportant = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 6)).toBool();
    task.isMyDay = sortedModel->data(selectedRowIndex.sibling(selectedRowIndex.row(), selectedRowIndex.column() + 7)).toBool();

    return task;
}