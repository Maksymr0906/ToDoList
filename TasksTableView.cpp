#include "TasksTableView.hpp"

TasksTableView::TasksTableView(QWidget* parent) : QWidget(parent) {
    mainView = new QTableView(this);
    sortedModel = new QSortFilterProxyModel(this);
}

TasksTableView::TasksTableView(QAbstractItemModel* model, QWidget* parent) : TasksTableView(parent) {
    sortedModel->setSourceModel(model);
    mainView->setModel(sortedModel);
    setTableViewAppearence();
    setTitles();
}

TasksTableView::~TasksTableView() {
    delete mainView;
    delete sortedModel;
}

void TasksTableView::setTableViewAppearence() {
    mainView->setFixedSize(QSize(730, 400));
    mainView->verticalHeader()->setVisible(false);
    mainView->hideColumn(0);
    mainView->hideColumn(5);
    mainView->hideColumn(6);
    mainView->hideColumn(7);
    mainView->setColumnWidth(1, 300);
    mainView->setColumnWidth(2, 100);
    mainView->setColumnWidth(3, 165);
    mainView->setColumnWidth(4, 165);
    mainView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainView->setSelectionMode(QAbstractItemView::SingleSelection);
    mainView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void TasksTableView::setTitles() {
    sortedModel->setHeaderData(1, Qt::Horizontal, tr("Task"));
    sortedModel->setHeaderData(2, Qt::Horizontal, tr("Deadline"));
    sortedModel->setHeaderData(3, Qt::Horizontal, tr("Responsible"));
    sortedModel->setHeaderData(4, Qt::Horizontal, tr("Email"));
}

void TasksTableView::setFilter(const QString pattern, int column) {
    sortedModel->setFilterFixedString(pattern);
    sortedModel->setFilterKeyColumn(column);
}

void TasksTableView::resetFilter() {
    sortedModel->setFilterFixedString(QString());
    sortedModel->setFilterKeyColumn(-1);
}