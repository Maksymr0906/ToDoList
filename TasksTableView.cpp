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
    mainView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainView->setSelectionMode(QAbstractItemView::SingleSelection);
    mainView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setTitles();
}

TasksTableView::~TasksTableView() {
    delete mainView;
    delete sortedModel;
}

void TasksTableView::setTitles() {
    sortedModel->setHeaderData(0, Qt::Horizontal, tr("Task"));
    sortedModel->setHeaderData(1, Qt::Horizontal, tr("Deadline"));
    sortedModel->setHeaderData(2, Qt::Horizontal, tr("Responsible"));
    sortedModel->setHeaderData(3, Qt::Horizontal, tr("Email"));
}

void TasksTableView::setFilter(const QString pattern, int column) {
    sortedModel->setFilterFixedString(pattern);
    sortedModel->setFilterKeyColumn(column);
}

void TasksTableView::resetFilter() {
    sortedModel->setFilterFixedString(QString());
    sortedModel->setFilterKeyColumn(-1);
}