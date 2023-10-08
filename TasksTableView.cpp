#include "TasksTableView.hpp"

TasksTableView::TasksTableView(QWidget* parent) : QWidget(parent) {
    mainView = new QTableView(this);
}

TasksTableView::TasksTableView(QAbstractItemModel* model, QWidget* parent) : TasksTableView(parent) {
    mainView = new QTableView(this);
    mainView->setModel(model);
}