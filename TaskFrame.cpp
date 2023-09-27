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

void TaskFrame::doneBtnClicked() {
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
}

void TaskFrame::myDayBtnClicked() {
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
}

void TaskFrame::failedBtnClicked() {
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
}