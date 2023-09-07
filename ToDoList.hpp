#pragma once

#include <QMainWindow>
#include "ui_ToDoList.h"
#include "Task.hpp"
#include "NewTaskDialog.hpp"

#include <QPixmap>
#include <QMessageBox>
#include <QTSql>

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
};
