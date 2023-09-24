#pragma once

#include <exception>
#include <QPixmap>
#include <QMessageBox>
#include <QTSql>
#include <QMap>
#include <QMainWindow>

#include "ui_ToDoList.h"
#include "NewTaskDialog.hpp"
#include "EditTaskDialog.hpp"
#include "TaskFrame.hpp"

class ToDoList : public QMainWindow
{
    Q_OBJECT

public:
    ToDoList(QWidget *parent = nullptr);
    ~ToDoList();
private:
    Ui::ToDoListClass *ui;
    void refreshTasks(const QString& queryCondition, TASK_TYPE taskType);
    void refreshTitle(TASK_TYPE taskType);
    void refreshTitleText(TASK_TYPE taskType);
    void refreshTitleIcon(TASK_TYPE taskType);
    void clearTaskWidgets();
private slots:
    void actionAddTriggered();
    void actionEditTriggered();
    void actionRemoveTriggered();
    void actionMyDayTriggered();
    void actionImportantTriggered();
    void actionAllTriggered();
    void actionPlannedTriggered();
    void actionCompletedTriggered();
    void actionFailedTriggered();
    void actionAboutProgramTriggered();
    void editTask(Task task);
    void fillTasks();
};
