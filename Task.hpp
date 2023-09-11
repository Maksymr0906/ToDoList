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

struct Task {
	QString taskName;
	QString deadline;
	bool isMyDay;
	bool isImportant;
	STATUS status;

	Task(QString taskName = "Empty", QString deadline = "2000.01.01", int status = 0, bool isMyDay = false, bool isImportant = false)
		:taskName{ taskName }, deadline{ deadline }, status{ status }, isMyDay{ isMyDay }, isImportant{ isImportant } {}
};

