#pragma once

#include <exception>
#include <QPixmap>
#include <QMessageBox>
#include <QMap>
#include <QMainWindow>

#include "NewTaskDialog.hpp"
#include "EditTaskDialog.hpp"
#include "RemoveTaskDialog.hpp"
#include "TaskFrame.hpp"
#include "TasksTableFrame.hpp"

class ToDoList : public QMainWindow
{
    Q_OBJECT

public:
    ToDoList(QWidget *parent = nullptr);
    ~ToDoList();
protected:
    DateValidator* dateValidator;
    QSqlTableModel* mainModel;
    QToolBar* toolBar;
    QAction* actionAll;
    QAction* actionCompleted;
    QAction* actionFailed;
    QAction* actionImportant;
    QAction* actionMyDay;
    QAction* actionPlanned;
    QMenuBar* menuBar;
    QMenu* menuSettings;
    QAction* actionAboutProgram;
    QMenu* menuLanguage;
    QAction* actionEnglish;
    QAction* actionUkrainian;
    QWidget* centralWidget;
    QFrame* addNewTaskFrame;
    QPushButton* addTaskButton;
    QPushButton* removeTaskButton;
    QFrame* titleFrame;
    QLabel* titleImage;
    QLabel* titleText;
    TasksTableFrame* tasksTableFrame;
private:
    void refreshTitle(TASK_TYPE taskType);
    void refreshTitleText(TASK_TYPE taskType);
    void refreshTitleIcon(TASK_TYPE taskType);
    void createToolBar();
    void createMenuBar();
    void createTitleFrame();
    void createTasksTableFrame();
    void createAddNewTaskFrame();
    void createCentralWidget();
    QAction* createAction(const QString& text, const QString& iconPath);
    void refreshTasks();
private slots:
    void actionAddTriggered();
    void actionRemoveTriggered();
    void actionMyDayTriggered();
    void actionImportantTriggered();
    void actionAllTriggered();
    void actionPlannedTriggered();
    void actionCompletedTriggered();
    void actionFailedTriggered();
    void actionAboutProgramTriggered();
    void updateRemoveButtonState();
};
