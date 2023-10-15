#pragma once

#include <QPixmap>
#include <QMessageBox>
#include <QMap>
#include <QMainWindow>
#include <QTranslator>

#include "TaskDialog.hpp"
#include "TasksTableFrame.hpp"

class ToDoList : public QMainWindow
{
    Q_OBJECT
protected:
    DateValidator* dateValidator;
    EmailValidator* emailValidator;
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
    QMenu* menuTask;
    QAction* actionMarkAsCompleted;
    QAction* actionMarkAsFailed;
    QWidget* centralWidget;
    QFrame* addNewTaskFrame;
    QPushButton* addTaskButton;
    QPushButton* removeTaskButton;
    QPushButton* editTaskButton;
    QFrame* titleFrame;
    QPushButton* completeTaskButton;
    QPushButton* failTaskButton;
    QLabel* titleImage;
    QLabel* titleText;
    TasksTableFrame* tasksTableFrame;
    QTranslator translator;

    void createToolBar();
    void createMenuBar();
    void createTitleFrame();
    void createTasksTableFrame();
    void createAddNewTaskFrame();
    void createCentralWidget();
    QAction* createAction(const QString& text, const QString& iconPath);
    QPushButton* createButton(const QString& iconPath);
    void refreshTasks();
    void enableButtons();
    void disableButtons();
    void markTask(STATUS newStatus, const QString& errorMessage);
    Task getSelectedTask();
    void retranslateUI();
    void refreshTitle(TASK_TYPE taskType);
    void refreshTitleText(TASK_TYPE taskType);
    void refreshTitleIcon(TASK_TYPE taskType);
protected slots:
    void addTaskButtonPressed();
    void addTaskButtonReleased();
    void editTaskButtonPressed();
    void editTaskButtonReleased();
    void removeTaskButtonPressed();
    void removeTaskButtonReleased();
    void completeTaskButtonPressed();
    void completeTaskButtonReleased();
    void failTaskButtonPressed();
    void failTaskButtonReleased();
    void actionMyDayTriggered();
    void actionImportantTriggered();
    void actionAllTriggered();
    void actionPlannedTriggered();
    void actionCompletedTriggered();
    void actionFailedTriggered();
    void actionAboutProgramTriggered();
    void actionEnglishTriggered();
    void actionUkrainianTriggered();
    void updateButtonsState();
public:
    ToDoList(QWidget *parent = nullptr);
    ~ToDoList();
};
