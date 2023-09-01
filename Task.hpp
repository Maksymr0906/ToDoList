#pragma once

#include <QString>

class Task {
private:
	int id;
	QString taskName;
	QString description;
	QString responsible;
	QString email;
	QString deadline;
	int status;
public:
	Task(int id, QString taskName, QString description, QString responsible, QString email, QString deadline, int status)
		:id{id}, taskName{taskName}, description{description}, responsible{responsible}, email{email}, deadline{deadline}, status{status} {}
};

