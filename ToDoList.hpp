#pragma once

#include <exception>
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
    Task getSelectedTask();
    void enableButtons();
    void disableButtons();
    QPushButton* createButton(const QString& iconPath);
    void markTask(STATUS newStatus, const QString& errorMessage);
    void retranslateUI();
protected slots:
    void completeTaskButtonPressed();
    void completeTaskButtonReleased();
    void failTaskButtonPressed();
    void failTaskButtonReleased();
    void addTaskButtonPressed();
    void addTaskButtonReleased();
    void editTaskButtonPressed();
    void editTaskButtonReleased();
    void removeTaskButtonPressed();
    void removeTaskButtonReleased();
    void actionMyDayTriggered();
    void actionImportantTriggered();
    void actionAllTriggered();
    void actionPlannedTriggered();
    void actionCompletedTriggered();
    void actionFailedTriggered();
    void actionAboutProgramTriggered();
    void updateButtonsState();
    void actionEnglishTriggered();
    void actionUkrainianTriggered();
public:
    ToDoList(QWidget *parent = nullptr);
    ~ToDoList();
};
