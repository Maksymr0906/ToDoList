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

    refreshTasks();

    connect(addTaskButton, &QPushButton::pressed, this, &ToDoList::addTaskButtonPressed);
    connect(addTaskButton, &QPushButton::released, this, &ToDoList::addTaskButtonReleased);
    connect(editTaskButton, &QPushButton::pressed, this, &ToDoList::editTaskButtonPressed);
    connect(editTaskButton, &QPushButton::released, this, &ToDoList::editTaskButtonReleased);
    connect(removeTaskButton, &QPushButton::pressed, this, &ToDoList::removeTaskButtonPressed);
    connect(removeTaskButton, &QPushButton::released, this, &ToDoList::removeTaskButtonReleased);
    connect(actionMyDay, &QAction::triggered, this, &ToDoList::actionMyDayTriggered);
    connect(actionImportant, &QAction::triggered, this, &ToDoList::actionImportantTriggered);
    connect(actionAll, &QAction::triggered, this, &ToDoList::actionAllTriggered);
    connect(actionPlanned, &QAction::triggered, this, &ToDoList::actionPlannedTriggered);
    connect(actionCompleted, &QAction::triggered, this, &ToDoList::actionCompletedTriggered);
    connect(actionFailed, &QAction::triggered, this, &ToDoList::actionFailedTriggered);
    connect(actionAboutProgram, &QAction::triggered, this, &ToDoList::actionAboutProgramTriggered);
    connect(tasksTableFrame->getView()->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ToDoList::updateButtonsState);
    connect(actionMarkAsCompleted, &QAction::triggered, this, &ToDoList::completeTaskButtonPressed);
    connect(actionMarkAsFailed, &QAction::triggered, this, &ToDoList::failTaskButtonPressed);
    connect(completeTaskButton, &QPushButton::pressed, this, &ToDoList::completeTaskButtonPressed);
    connect(completeTaskButton, &QPushButton::released, this, &ToDoList::completeTaskButtonReleased);
    connect(failTaskButton, &QPushButton::pressed, this, &ToDoList::failTaskButtonPressed);
    connect(failTaskButton, &QPushButton::released, this, &ToDoList::failTaskButtonReleased);
}

ToDoList::~ToDoList() {
    delete dateValidator;
    delete emailValidator;
    delete mainModel;
    delete actionAll;
    delete actionCompleted;
    delete actionFailed;
    delete actionImportant;
    delete actionMyDay;
    delete actionPlanned;
    delete toolBar;
    delete actionAboutProgram;
    delete actionEnglish;
    delete actionUkrainian;
    delete menuLanguage;
    delete menuSettings;
    delete actionMarkAsCompleted;
    delete actionMarkAsFailed;
    delete menuTask;
    delete menuBar;
    delete addTaskButton;
    delete editTaskButton;
    delete removeTaskButton;
    delete addNewTaskFrame;
    delete titleImage;
    delete titleText;
    delete titleFrame;
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
    menuLanguage->addActions({ actionEnglish, actionUkrainian });

    menuSettings->addMenu(menuLanguage);

    menuTask = new QMenu("Task", this);
    actionMarkAsCompleted = createAction("Mark as Completed", "Assets/done_icon.png");
    actionMarkAsFailed = createAction("Mark as Failed", "Assets/failed_icon.png");
    actionMarkAsCompleted->setEnabled(false);
    actionMarkAsFailed->setEnabled(false);
    menuTask->addActions({ actionMarkAsCompleted, actionMarkAsFailed });

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
    
    completeTaskButton = createButton("Assets/done_icon.png");
    completeTaskButton->setEnabled(false);
    completeTaskButton->setVisible(false);

    failTaskButton = createButton("Assets/failed_icon.png");
    failTaskButton->setEnabled(false);
    failTaskButton->setVisible(false);

    QHBoxLayout* titleLayout = new QHBoxLayout(titleFrame);
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Minimum);
    titleLayout->addWidget(completeTaskButton);
    titleLayout->addWidget(failTaskButton);
    titleLayout->addSpacerItem(spacer);
    titleLayout->addWidget(titleText);
    titleLayout->addWidget(titleImage);
    titleFrame->setLayout(titleLayout);
    titleFrame->setFixedHeight(60);
}

void ToDoList::createTasksTableFrame() {
    tasksTableFrame = new TasksTableFrame(mainModel);
    tasksTableFrame->setFixedSize(QSize(750, 400));
    tasksTableFrame->setFilter("true", 7);
}

void ToDoList::createAddNewTaskFrame() {
    QSpacerItem* spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Minimum);
    addNewTaskFrame = new QFrame(this);
    addTaskButton = createButton("Assets/add_icon.png");
    removeTaskButton = createButton("Assets/remove_icon.png");
    editTaskButton = createButton("Assets/edit_icon.png");

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

QPushButton* ToDoList::createButton(const QString& iconPath) {
    QPushButton* button = new QPushButton(this);
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(40, 40));
    button->setStyleSheet("background-color: transparent; border: none;");

    return button;
}

void ToDoList::addTaskButtonPressed() {
    addTaskButton->setStyleSheet("background-color: lightblue; border: none;");
    std::unique_ptr<TaskDialog> newTaskDialog = std::make_unique<TaskDialog>(emailValidator, dateValidator);
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
        insertQuery.bindValue(":status", static_cast<int>(STATUS::IN_PROCESS));
        insertQuery.bindValue(":is_important", newTaskDialog->getIsImportant());
        insertQuery.bindValue(":is_my_day", newTaskDialog->getIsMyDay());

        if (!insertQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Adding task error: %1").arg(insertQuery.lastError().text()));
            return;
        }

        refreshTasks();
    }
}

void ToDoList::editTaskButtonPressed() {
    editTaskButton->setStyleSheet("background-color: lightblue; border: none;");
    if (!(tasksTableFrame->getSelectionModel() && tasksTableFrame->getSelectionModel()->selectedRows().count() > 0)) {
        QMessageBox::information(this, "Error", "Choose the row at first");
        return;
    }

    Task task = getSelectedTask();

    std::unique_ptr<TaskDialog> taskDialog = std::make_unique<TaskDialog>(emailValidator, dateValidator, this);
    taskDialog->setFixedSize(462, 434);
    taskDialog->setTaskName(task.taskName);
    taskDialog->setDeadline(task.deadline);
    taskDialog->setResponsible(task.responsible);
    taskDialog->setEmail(task.email);
    taskDialog->setIsImportant(task.isImportant);
    taskDialog->setIsMyDay(task.isMyDay);

    if (taskDialog->exec() == QDialog::Accepted) {
        QSqlQuery updateQuery;
        QString updateQueryString = "UPDATE todolist "
            "SET task_name = :new_task_name, deadline = :new_deadline, responsible = :new_responsible, email = :new_email, status = :new_status, is_important = :new_important, is_my_day = :new_my_day "
            "WHERE id = :id";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_task_name", taskDialog->getTaskName().isEmpty() ? QVariant(QVariant::String) : taskDialog->getTaskName());
        updateQuery.bindValue(":new_deadline", taskDialog->getDeadline().isEmpty() ? QVariant(QVariant::String) : taskDialog->getDeadline());
        updateQuery.bindValue(":new_responsible", taskDialog->getResponsible().isEmpty() ? QVariant(QVariant::String) : taskDialog->getResponsible());
        updateQuery.bindValue(":new_email", taskDialog->getEmail().isEmpty()? QVariant(QVariant::String) : taskDialog->getEmail());
        updateQuery.bindValue(":new_status", static_cast<int>(task.status));
        updateQuery.bindValue(":new_important", taskDialog->getIsImportant());
        updateQuery.bindValue(":new_my_day", taskDialog->getIsMyDay());
        updateQuery.bindValue(":id", task.id);
        
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
            return;
        }

        refreshTasks();
    }
}

void ToDoList::removeTaskButtonPressed() {
    removeTaskButton->setStyleSheet("background-color: lightblue; border: none;");
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
        enableButtons();
    }
    else {
        disableButtons();
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
    disableButtons();

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

        task.updateStatus();
        task.updateIsMyDay();

        if (task.status != static_cast<STATUS>(selectQuery.value("status").toInt())) {
            QSqlQuery updateStatusQuery;
            QString updateQueryString = "UPDATE todolist SET status = :new_status WHERE id = :id";
            updateStatusQuery.prepare(updateQueryString);
            updateStatusQuery.bindValue(":new_status", static_cast<int>(task.status));
            updateStatusQuery.bindValue(":id", task.id);

            if (!updateStatusQuery.exec()) {
                QMessageBox::critical(this, "Error making a query", tr("Updating status task error: %1").arg(updateStatusQuery.lastError().text()));
                return;
            }
        }

        if (task.isMyDay != selectQuery.value("is_my_day").toBool()) {
            QSqlQuery updateMyDayQuery;
            QString updateQueryString = "UPDATE todolist SET is_my_day = :new_is_my_day WHERE id = :id";
            updateMyDayQuery.prepare(updateQueryString);
            updateMyDayQuery.bindValue(":new_is_my_day", task.isMyDay);
            updateMyDayQuery.bindValue(":id", task.id);

            if (!updateMyDayQuery.exec()) {
                QMessageBox::critical(this, "Error making a query", tr("Updating is my day error: %1").arg(updateMyDayQuery.lastError().text()));
                return;
            }
        }
    }

    mainModel->select();
}

void ToDoList::completeTaskButtonPressed() {
    completeTaskButton->setStyleSheet("background-color: lightblue; border: none;");
    markTask(STATUS::COMPLETED, "Choose the row to mark as completed");
}

void ToDoList::failTaskButtonPressed() {
    failTaskButton->setStyleSheet("background-color: lightblue; border: none;");
    markTask(STATUS::FAILED, "Choose the row to mark as failed");
}

void ToDoList::markTask(STATUS newStatus, const QString& errorMessage) {
    if (!(tasksTableFrame->getSelectionModel() && tasksTableFrame->getSelectionModel()->selectedRows().count() > 0)) {
        QMessageBox::information(this, "Error", errorMessage);
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

void ToDoList::completeTaskButtonReleased() {
    completeTaskButton->setStyleSheet("background-color: transparent; border: none;");
}

void ToDoList::failTaskButtonReleased() {
    failTaskButton->setStyleSheet("background-color: transparent; border: none;");
}

void ToDoList::addTaskButtonReleased() {
    addTaskButton->setStyleSheet("background-color: transparent; border: none;");
}

void ToDoList::editTaskButtonReleased() {
    editTaskButton->setStyleSheet("background-color: transparent; border: none;");
}

void ToDoList::removeTaskButtonReleased() {
    removeTaskButton->setStyleSheet("background-color: transparent; border: none;");
}

void ToDoList::enableButtons() {
    removeTaskButton->setEnabled(true);
    editTaskButton->setEnabled(true);
    actionMarkAsCompleted->setEnabled(true);
    actionMarkAsFailed->setEnabled(true);
    completeTaskButton->setEnabled(true);
    completeTaskButton->setVisible(true);
    failTaskButton->setEnabled(true);
    failTaskButton->setVisible(true);
}

void ToDoList::disableButtons() {
    editTaskButton->setEnabled(false);
    removeTaskButton->setEnabled(false);
    actionMarkAsCompleted->setEnabled(false);
    actionMarkAsFailed->setEnabled(false);
    completeTaskButton->setEnabled(false);
    completeTaskButton->setVisible(false);
    failTaskButton->setEnabled(false);
    failTaskButton->setVisible(false);
}