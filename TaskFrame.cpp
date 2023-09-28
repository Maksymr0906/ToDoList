#include "TaskFrame.hpp"

const QVector<QString> TaskFrame::colors = {
    "rgba(66, 165, 245,1.0)",
    "rgba(41, 182, 246,1.0)",
    "rgba(38, 198, 218,1.0)",
    "rgba(38, 166, 154,1.0)",
    "rgba(102, 187, 106,1.0)",
    "rgba(156, 204, 101,1.0)",
    "rgba(212, 225, 87,1.0)",
    "rgba(255, 238, 88,1.0)",
    "rgba(255, 202, 40,1.0)",
    "rgba(255, 167, 38,1.0)"
};

void TaskFrame::doneBtnPressed() {
    doneBtn->setStyleSheet("background-color: lightblue; border: none;");
    if (task.status != STATUS::COMPLETED) {
        taskLabel->setObjectName("TaskName");
        taskLabel->setStyleSheet("#TaskName { font: bold 11pt 'Verdana'; color: gray; text-decoration: line-through; }");
        QSqlQuery updateQuery;
        QString updateQueryString = "UPDATE todolist "
            "SET status = :new_status "
            "WHERE task_name = :old_task_name AND deadline = :old_deadline AND status = :old_status AND is_important = :old_important AND is_my_day = :old_my_day;";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_status", static_cast<int>(STATUS::COMPLETED));
        updateQuery.bindValue(":old_task_name", task.taskName);
        updateQuery.bindValue(":old_deadline", task.deadline);
        updateQuery.bindValue(":old_status", static_cast<int>(task.status));
        updateQuery.bindValue(":old_important", task.isImportant);
        updateQuery.bindValue(":old_my_day", task.isMyDay);
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
            return;
        }
        task.status = STATUS::COMPLETED;
    }
    else {
        taskLabel->setObjectName("TaskName");
        taskLabel->setStyleSheet("#TaskName { font: bold 11pt black 'Verdana'; }");
        QSqlQuery updateQuery;
        QString updateQueryString = "UPDATE todolist "
            "SET status = :new_status "
            "WHERE task_name = :old_task_name AND deadline = :old_deadline AND status = :old_status AND is_important = :old_important AND is_my_day = :old_my_day;";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_status", static_cast<int>(STATUS::IN_PROCESS));
        updateQuery.bindValue(":old_task_name", task.taskName);
        updateQuery.bindValue(":old_deadline", task.deadline);
        updateQuery.bindValue(":old_status", static_cast<int>(task.status));
        updateQuery.bindValue(":old_important", task.isImportant);
        updateQuery.bindValue(":old_my_day", task.isMyDay);
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
            return;
        }
        task.status = STATUS::IN_PROCESS;
    }
}

void TaskFrame::myDayBtnPressed() {
    myDayBtn->setStyleSheet("background-color: lightblue; border: none;");
    if (task.isMyDay) {
        QSqlQuery updateQuery;
        QString updateQueryString = "UPDATE todolist "
            "SET is_my_day = :new_my_day "
            "WHERE task_name = :old_task_name AND deadline = :old_deadline AND status = :old_status AND is_important = :old_important AND is_my_day = :old_my_day;";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_my_day", false);
        updateQuery.bindValue(":old_task_name", task.taskName);
        updateQuery.bindValue(":old_deadline", task.deadline);
        updateQuery.bindValue(":old_status", static_cast<int>(task.status));
        updateQuery.bindValue(":old_important", task.isImportant);
        updateQuery.bindValue(":old_my_day", task.isMyDay);
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
            return;
        }
        task.isMyDay = false;
    }
    else {
        QSqlQuery updateQuery;
        QString updateQueryString = "UPDATE todolist "
            "SET is_my_day = :new_my_day "
            "WHERE task_name = :old_task_name AND deadline = :old_deadline AND status = :old_status AND is_important = :old_important AND is_my_day = :old_my_day;";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_my_day", true);
        updateQuery.bindValue(":old_task_name", task.taskName);
        updateQuery.bindValue(":old_deadline", task.deadline);
        updateQuery.bindValue(":old_status", static_cast<int>(task.status));
        updateQuery.bindValue(":old_important", task.isImportant);
        updateQuery.bindValue(":old_my_day", task.isMyDay);
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
            return;
        }
        task.isMyDay = true;
    }
}

void TaskFrame::failedBtnReleased() {
    failedBtn->setStyleSheet("background-color: transparent; border: none;");
}

void TaskFrame::myDayBtnReleased() {
    myDayBtn->setStyleSheet("background-color: transparent; border: none;");
}

void TaskFrame::doneBtnReleased() {
    doneBtn->setStyleSheet("background-color: transparent; border: none;");
}

void TaskFrame::failedBtnPressed() {
    failedBtn->setStyleSheet("background-color: lightblue; border: none;");
    if (task.status != STATUS::FAILED) {
        taskLabel->setObjectName("TaskName");
        taskLabel->setStyleSheet("#TaskName { font: bold 11pt 'Verdana'; color: red; }");
        QSqlQuery updateQuery;
        QString updateQueryString = "UPDATE todolist "
            "SET status = :new_status "
            "WHERE task_name = :old_task_name AND deadline = :old_deadline AND status = :old_status AND is_important = :old_important AND is_my_day = :old_my_day;";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_status", static_cast<int>(STATUS::FAILED));
        updateQuery.bindValue(":old_task_name", task.taskName);
        updateQuery.bindValue(":old_deadline", task.deadline);
        updateQuery.bindValue(":old_status", static_cast<int>(task.status));
        updateQuery.bindValue(":old_important", task.isImportant);
        updateQuery.bindValue(":old_my_day", task.isMyDay);
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
            return;
        }
        task.status = STATUS::FAILED;
    }
    else {
        taskLabel->setObjectName("TaskName");
        taskLabel->setStyleSheet("#TaskName { font: bold 11pt black 'Verdana'; }");
        QSqlQuery updateQuery;
        QString updateQueryString = "UPDATE todolist "
            "SET status = :new_status "
            "WHERE task_name = :old_task_name AND deadline = :old_deadline AND status = :old_status AND is_important = :old_important AND is_my_day = :old_my_day;";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_status", static_cast<int>(STATUS::IN_PROCESS));
        updateQuery.bindValue(":old_task_name", task.taskName);
        updateQuery.bindValue(":old_deadline", task.deadline);
        updateQuery.bindValue(":old_status", static_cast<int>(task.status));
        updateQuery.bindValue(":old_important", task.isImportant);
        updateQuery.bindValue(":old_my_day", task.isMyDay);
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
            return;
        }
        task.status = STATUS::IN_PROCESS;
    }
}

void TaskFrame::editTask() {
    std::unique_ptr<EditTaskDialog> editTaskDialog = std::make_unique<EditTaskDialog>(dateValidator);
    editTaskDialog->setTaskName(task.taskName);
    editTaskDialog->setDeadline(task.deadline);
    editTaskDialog->setIsImportant(task.isImportant);
    editTaskDialog->setIsMyDay(task.isMyDay);
    
    if (editTaskDialog->exec() == QDialog::Accepted) {
        QSqlQuery updateQuery;
        QString updateQueryString = "UPDATE todolist "
            "SET task_name = :new_task_name, deadline = :new_deadline, status = :new_status, is_important = :new_important, is_my_day = :new_my_day "
            "WHERE task_name = :old_task_name AND deadline = :old_deadline AND status = :old_status AND is_important = :old_important AND is_my_day = :old_my_day;";

        updateQuery.prepare(updateQueryString);
        updateQuery.bindValue(":new_task_name", editTaskDialog->getTaskName());
        updateQuery.bindValue(":new_deadline", editTaskDialog->getDeadline());
        updateQuery.bindValue(":new_status", static_cast<int>(task.status));
        updateQuery.bindValue(":new_important", editTaskDialog->getIsImportant());
        updateQuery.bindValue(":new_my_day", editTaskDialog->getIsMyDay());
        updateQuery.bindValue(":old_task_name", task.taskName);
        updateQuery.bindValue(":old_deadline", task.deadline);
        updateQuery.bindValue(":old_status", static_cast<int>(task.status));
        updateQuery.bindValue(":old_important", task.isImportant);
        updateQuery.bindValue(":old_my_day", task.isMyDay);
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Error"), tr("Updating task error: %1").arg(updateQuery.lastError().text()));
            return;
        }
        
        taskLabel->setText(editTaskDialog->getTaskName());
        dateLabel->setText(editTaskDialog->getDeadline());
    }
}