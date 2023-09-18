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
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("todolist");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

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
    NewTaskDialog newTaskDialog;
    newTaskDialog.setFixedSize(317, 255);
    if(newTaskDialog.exec() == QDialog::Accepted) {
        QSqlQuery insertQuery;
        QString insertQueryString = "insert into todolist(task_name, deadline, status, is_important, is_my_day)"
            "values('%1', '%2', '%3', '%4', '%5');";
        bool insertResult = insertQuery.exec(insertQueryString.arg(newTaskDialog.getTaskName())
                                             .arg(newTaskDialog.getDeadline())
                                             .arg(IN_PROCESS)
                                             .arg(newTaskDialog.getIsImportant())
                                             .arg(newTaskDialog.getIsMyDay())
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

void ToDoList::refreshTasks(const QString &queryCondition, TASK_TYPE taskType) {
    refreshTitle(taskType);
    QPixmap failedBtnIcon("Assets/failed_icon.png");
    QPixmap doneBtnIcon("Assets/done_icon.png");
    QPixmap myDayBtnIcon("Assets/myday_icon.png");

    QSqlQuery selectQuery;
    if(!selectQuery.exec(queryCondition)) {
        return;
    }

    QLayout *layout = ui->allNewTasksContents->layout();
    if(layout) {
        QLayoutItem *child;
        while((child = layout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    }

    QVBoxLayout *vMainLayout = qobject_cast<QVBoxLayout *>(ui->allNewTasksContents->layout());
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vMainLayout->addSpacerItem(verticalSpacer);

    QVector<QString> colors = { "rgba(66, 165, 245,1.0)", "rgba(41, 182, 246,1.0)", "rgba(38, 198, 218,1.0)", "rgba(38, 166, 154,1.0)", "rgba(102, 187, 106,1.0)", "rgba(156, 204, 101,1.0)", "rgba(212, 225, 87,1.0)", "rgba(255, 238, 88,1.0)", "rgba(255, 202, 40,1.0)", "rgba(255, 167, 38,1.0)" };

    while(selectQuery.next()) {
        QString taskName = selectQuery.value("task_name").toString();
        QString deadline = selectQuery.value("deadline").toString();
        int status = selectQuery.value("status").toInt();
        Task task{ taskName, deadline, status };

        ClickableFrame *newTaskFrame = new ClickableFrame();
        newTaskFrame->setFrameStyle(QFrame::StyledPanel);

        QHBoxLayout *newTask = new QHBoxLayout(newTaskFrame);
        newTaskFrame->setLayout(newTask);

        QPushButton *myDayBtn = new QPushButton();
        newTask->addWidget(myDayBtn);

        QFrame *Vframe = new QFrame();
        QVBoxLayout *taskDetails = new QVBoxLayout(Vframe);
        Vframe->setLayout(taskDetails);

        QLabel *titlelabel = new QLabel(tr("Task #%1").arg(vMainLayout->count()));
        QLabel *tasklabel = new QLabel(task.taskName);
        QLabel *datelabel = new QLabel(task.deadline);

        taskDetails->addWidget(titlelabel);
        taskDetails->addWidget(tasklabel);
        taskDetails->addWidget(datelabel);

        newTask->addWidget(Vframe);
        QSpacerItem *spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Minimum);
        newTask->addSpacerItem(spacer);

        QFrame *buttons = new QFrame();
        QVBoxLayout *buttonsLayout = new QVBoxLayout(buttons);
        buttons->setLayout(buttonsLayout);

        QPushButton *deleteBtn = new QPushButton();
        QPushButton *doneBtn = new QPushButton();

        buttonsLayout->addWidget(deleteBtn);
        buttonsLayout->addWidget(doneBtn);

        newTask->addWidget(buttons);
        vMainLayout->insertWidget(0, newTaskFrame);

        int randomVal = (((rand() % 50) % 100) % (colors.size() - 1));

        newTaskFrame->setObjectName("NewTask");
        newTaskFrame->setStyleSheet("#NewTask { border-radius: 10px; border: 1px solid black; background-color: " + colors[randomVal] + "; }");

        if(static_cast<STATUS>(status) == STATUS::COMPLETED) {
            tasklabel->setObjectName("TaskName");
            tasklabel->setStyleSheet("#TaskName { font: bold 11pt 'Verdana'; color: gray; text-decoration: line-through; }");
        }
        else if(static_cast<STATUS>(status) == STATUS::FAILED) {
            tasklabel->setObjectName("TaskName");
            tasklabel->setStyleSheet("#TaskName { font: bold 11pt 'Verdana'; color: red; }");
        }
        else {
            tasklabel->setObjectName("TaskName");
            tasklabel->setStyleSheet("#TaskName { font: bold 11pt black 'Verdana'; }");
        }

        deleteBtn->setIcon(failedBtnIcon);
        deleteBtn->setIconSize(QSize(30, 30));
        deleteBtn->setStyleSheet("background-color: transparent; border: none;");

        doneBtn->setIcon(doneBtnIcon);
        doneBtn->setIconSize(QSize(30, 30));
        doneBtn->setStyleSheet("background-color: transparent; border: none;");

        myDayBtn->setIcon(myDayBtnIcon);
        myDayBtn->setIconSize(QSize(45, 45));
        myDayBtn->setStyleSheet("background-color: transparent; border: none;");

        connect(newTaskFrame, &ClickableFrame::clicked, this, &ToDoList::test);
    }
}

void ToDoList::test() {
    QMessageBox::information(this, "Edit or delete", "Edit or delete");
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