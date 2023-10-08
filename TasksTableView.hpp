#pragma once

#include <QWidget>
#include <QtCore>
#include <QtWidgets>

class TasksTableView : public QWidget {
	Q_OBJECT
public:
	TasksTableView(QWidget* parent = nullptr);
	TasksTableView(QAbstractItemModel* model, QWidget* parent = nullptr);
private:
protected:
	QTableView* mainView;
};