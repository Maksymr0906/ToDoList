#pragma once

#include "TasksTableView.hpp"

class TasksTableFrame : public QFrame {
	Q_OBJECT
protected:
	QVBoxLayout* mainLayout;
	TasksTableView* mainView;
public:
	TasksTableFrame(QWidget* parent = nullptr);
	TasksTableFrame(QAbstractItemModel* model, QWidget* parent = nullptr);

	void setFilter(const QString pattern, int column);
	void resetFilter();
	void setTitles();

	QItemSelectionModel* getSelectionModel() const { return mainView->getSelectionModel(); }
	QSortFilterProxyModel* getSortedModel() const { return mainView->getSortedModel(); }
	QTableView* getView() const { return mainView->getView(); }
};

