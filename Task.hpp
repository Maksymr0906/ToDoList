#pragma once

#include <QString>

enum class STATUS {
	NONE = 0,
	PLANNED,
	IN_PROCESS,
	COMPLETED
};

class Task {
private:
	QString taskName;
	QString responsible;
	QString email;
	QString deadline;
	int status;
public:
	Task(QString taskName, QString responsible, QString email, QString deadline, int status)
		:taskName{taskName}, responsible{responsible}, email{email}, deadline{deadline}, status{status} {}

	QString getAsQString() {
		return taskName + " " + responsible + " " + email + " " + deadline + " " + QString::number(status);
	}
};

