#include "ToDoList.hpp"

ToDoList::ToDoList(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ToDoListClass)
{
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
}

ToDoList::~ToDoList() {
    delete ui;
}

void ToDoList::actionAddTriggered() {
    NewTaskDialog newTaskDialog;
    newTaskDialog.exec();
    QMessageBox::information(this, "Add", "Action Add triggered");
}

void ToDoList::actionEditTriggered() {
    QMessageBox::information(this, "Edit", "Action Edit triggered");
}

void ToDoList::actionRemoveTriggered() {
    QMessageBox::information(this, "Remove", "Action Remove triggered");
}

void ToDoList::actionMyDayTriggered() {
    QPixmap icon("Assets/myday_icon.png");
    ui->headerImage->setPixmap(icon);
    ui->headerText->setText("My Day");
}

void ToDoList::actionImportantTriggered() {
    QPixmap icon("Assets/important_icon.png");
    ui->headerImage->setPixmap(icon);
    ui->headerText->setText("Important");
}

void ToDoList::actionAllTriggered() {
    QPixmap icon("Assets/all_icon.png");
    QPixmap failedBtnIcon("Assets/failed_icon.png");
    QPixmap doneBtnIcon("Assets/done_icon.png");
    QPixmap myDayBtnIcon("Assets/myday_icon.png");
    ui->headerImage->setPixmap(icon);
    ui->headerText->setText("All");

    QSqlQuery selectQuery;
    QString selectQueryString = "SELECT * FROM todolist;";
    if(!selectQuery.exec(selectQueryString)) {
        return;
    }

    QVBoxLayout *vMainLayout = qobject_cast<QVBoxLayout *>(ui->allNewTasksContents->layout());

    while(selectQuery.next()) {
        QString taskName = selectQuery.value("task_name").toString();
        QString deadline = selectQuery.value("deadline").toString();
        int status = selectQuery.value("status").toInt();

        QFrame *newTaskFrame = new QFrame();
        newTaskFrame->setFrameStyle(QFrame::StyledPanel);

        QHBoxLayout *newTask = new QHBoxLayout(newTaskFrame);
        newTaskFrame->setLayout(newTask);

        QPushButton *myDayBtn = new QPushButton();
        newTask->addWidget(myDayBtn);

        QFrame *Vframe = new QFrame();
        QVBoxLayout *taskDetails = new QVBoxLayout(Vframe);
        Vframe->setLayout(taskDetails);

        QLabel *titlelabel = new QLabel(tr("Task #%1").arg(vMainLayout->count()));
        QLabel *tasklabel = new QLabel(taskName);
        QLabel *datelabel = new QLabel(deadline);

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
        vMainLayout->addWidget(newTaskFrame);

        QVector<QString> colors = { "rgba(66, 165, 245,1.0)", "rgba(41, 182, 246,1.0)", "rgba(38, 198, 218,1.0)", "rgba(38, 166, 154,1.0)", "rgba(102, 187, 106,1.0)", "rgba(156, 204, 101,1.0)", "rgba(212, 225, 87,1.0)", "rgba(255, 238, 88,1.0)", "rgba(255, 202, 40,1.0)", "rgba(255, 167, 38,1.0)" };
        int randomVal = (((rand() % 50) % 100) % (colors.size() - 1));

        newTaskFrame->setObjectName("NewTask");
        newTaskFrame->setStyleSheet("#NewTask { border-radius: 10px; border: 1px solid black; background-color: " + colors[randomVal] + "; }");

        tasklabel->setObjectName("TaskName");
        tasklabel->setStyleSheet("#TaskName { font: bold 11pt black 'Verdana'; }");

        deleteBtn->setIcon(failedBtnIcon);
        deleteBtn->setIconSize(QSize(30, 30));
        deleteBtn->setStyleSheet("background-color: transparent; border: none;");

        doneBtn->setIcon(doneBtnIcon);
        doneBtn->setIconSize(QSize(30, 30));
        doneBtn->setStyleSheet("background-color: transparent; border: none;");

        myDayBtn->setIcon(myDayBtnIcon);
        myDayBtn->setIconSize(QSize(45, 45));
        myDayBtn->setStyleSheet("background-color: transparent; border: none;");
    }
}

void ToDoList::actionPlannedTriggered() {
    QPixmap icon("Assets/planned_icon.png");
    ui->headerImage->setPixmap(icon);
    ui->headerText->setText("Planned");
}

void ToDoList::actionCompletedTriggered() {
    QPixmap icon("Assets/done_icon.png");
    ui->headerImage->setPixmap(icon);
    ui->headerText->setText("Completed");
}

void ToDoList::actionFailedTriggered() {
    QPixmap icon("Assets/failed_icon.png");
    ui->headerImage->setPixmap(icon);
    ui->headerText->setText("Failed");
}

void ToDoList::actionAboutProgramTriggered() {
    QMessageBox::information(this, "About Program", "Action About Program triggered");
}