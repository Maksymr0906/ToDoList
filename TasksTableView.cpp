#include "TasksTableView.hpp"

TasksTableView::TasksTableView(QWidget* parent) : QWidget(parent) {
    mainView = new QTableView(this);
    sortedModel = new QSortFilterProxyModel(this);
}

TasksTableView::TasksTableView(QAbstractItemModel* model, QWidget* parent) : TasksTableView(parent) {
    sortedModel->setSourceModel(model);
    mainView->setModel(sortedModel);
    mainView->setFixedSize(QSize(750, 400));
    mainView->verticalHeader()->setVisible(false);
}

TasksTableView::~TasksTableView() {
    delete mainView;
    delete sortedModel;
}