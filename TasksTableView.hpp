#pragma once

#include <QWidget>
#include <QtCore>
#include <QtWidgets>

class TasksTableView : public QWidget {
	Q_OBJECT
protected:
	QSortFilterProxyModel* sortedModel;
	QTableView* mainView;
public:
	TasksTableView(QWidget* parent = nullptr);
	TasksTableView(QAbstractItemModel* model, QWidget* parent = nullptr);
	~TasksTableView();
	
	void setTableViewAppearence();
	void setTitles();
	void setFilter(const QString pattern, int column);
	void resetFilter();

	QItemSelectionModel* getSelectionModel() const { return mainView->selectionModel(); }
	QSortFilterProxyModel* getSortedModel() const { return sortedModel; }
	QTableView* getView() const { return mainView; }
};