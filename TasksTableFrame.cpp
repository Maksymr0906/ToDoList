#include "TasksTableFrame.hpp"

TasksTableFrame::TasksTableFrame(QWidget* parent)
	: QFrame(parent) {
	mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);
}

TasksTableFrame::TasksTableFrame(QAbstractItemModel* model, QWidget* parent) 
	:TasksTableFrame(parent) {
	mainView = new TasksTableView(model, this);
	mainLayout->addWidget(mainView);

}

void TasksTableFrame::setFilter(const QString pattern, int column) {
	mainView->setFilter(pattern, column);
}

void TasksTableFrame::resetFilter() {
	mainView->resetFilter();
}