#pragma once

#include <QString>

enum class STATUS {
	IN_PROCESS = 0,
	COMPLETED,
	FAILED
};

enum class TASK_TYPE {
	MY_DAY = 1,
	IMPORTANT,
	ALL,
	PLANNED,
	COMPLETED,
	FAILED
};

class Task {
private:

protected:
	QString taskName;
	QString deadline;
	bool isMyDay;
	bool isImportant;
	STATUS status;
public:
	Task(QString taskName, QString deadline, int status, bool isMyDay, bool isImportant)
		:taskName{ taskName }, deadline{ deadline }, status{ status }, isMyDay{ isMyDay }, isImportant{ isImportant } {}
};

