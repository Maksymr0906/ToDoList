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

    // Connects
    connect(addTaskButton, SIGNAL(clicked()), this, SLOT(actionAddTriggered()));
    connect(removeTaskButton, SIGNAL(clicked()), this, SLOT(actionRemoveTriggered()));
    connect(actionMyDay, &QAction::triggered, this, &ToDoList::actionMyDayTriggered);
    connect(actionImportant, &QAction::triggered, this, &ToDoList::actionImportantTriggered);
    connect(actionAll, &QAction::triggered, this, &ToDoList::actionAllTriggered);
    connect(actionPlanned, &QAction::triggered, this, &ToDoList::actionPlannedTriggered);
    connect(actionCompleted, &QAction::triggered, this, &ToDoList::actionCompletedTriggered);
    connect(actionFailed, &QAction::triggered, this, &ToDoList::actionFailedTriggered);
    connect(actionAboutProgram, &QAction::triggered, this, &ToDoList::actionAboutProgramTriggered);
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
    delete centralWidget;
    delete addNewTaskFrame;
    delete addTaskButton;
    delete removeTaskButton;
    delete titleFrame;
    delete titleImage;
    delete titleText;
    delete tasksTableFrame;
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
    menuBar->addMenu(menuSettings);
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

    QHBoxLayout* addNewTaskLayout = new QHBoxLayout();
    addNewTaskLayout->addSpacerItem(spacer);
    addNewTaskLayout->addWidget(addTaskButton);
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
    }
}

void ToDoList::actionRemoveTriggered() {
    
}

void ToDoList::actionMyDayTriggered() {
    refreshTitle(TASK_TYPE::MY_DAY);
}

void ToDoList::actionImportantTriggered() {
    refreshTitle(TASK_TYPE::IMPORTANT);
}

void ToDoList::actionAllTriggered() {
    refreshTitle(TASK_TYPE::ALL);
}

void ToDoList::actionPlannedTriggered() {
    refreshTitle(TASK_TYPE::PLANNED);
}

void ToDoList::actionCompletedTriggered() {
    refreshTitle(TASK_TYPE::COMPLETED);
}

void ToDoList::actionFailedTriggered() {
    refreshTitle(TASK_TYPE::FAILED);
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