#pragma once

#include <QMainWindow>
#include "ui_ToDoList.h"
#include "Task.hpp"
#include "NewTaskDialog.hpp"
#include "ClickableFrame.hpp"

#include <QPixmap>
#include <QMessageBox>
#include <QTSql>
#include <QMap>

class ToDoList : public QMainWindow
{
    Q_OBJECT

public:
    ToDoList(QWidget *parent = nullptr);
    ~ToDoList();

private:
    Ui::ToDoListClass *ui;
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
    void refreshTasks(const QString &queryCondition, TASK_TYPE taskType);
    void refreshTitle(TASK_TYPE taskType);
    void refreshTitleText(TASK_TYPE taskType);
    void refreshTitleIcon(TASK_TYPE taskType);

    void test();
};
