#include "TasksTableView.hpp"

TasksTableView::TasksTableView(QWidget* parent = nullptr) : QWidget(parent) {

}

TasksTableView::TasksTableView(QAbstractItemModel* model, QWidget* parent = nullptr) : TasksTableView(parent) {

}