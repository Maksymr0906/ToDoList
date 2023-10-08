#pragma once

#include <QWidget>
#include <QtCore>

class TasksTableView : public QWidget {
	Q_OBJECT
public:
	TasksTableView(QWidget* parent = nullptr);
	TasksTableView(QAbstractItemModel* model, QWidget* parent = nullptr);
private:
protected:
};