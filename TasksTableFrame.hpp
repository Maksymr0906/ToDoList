#pragma once

#include <QWidget>
#include <QtCore>
#include <QtWidgets>
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
	QItemSelectionModel* getSelectionModel() const {
		return mainView->getSelectionModel();
	}
	QSortFilterProxyModel* getSortedModel() const { return mainView->getSortedModel(); }
	QTableView* getView() const { return mainView->getView(); }
};

