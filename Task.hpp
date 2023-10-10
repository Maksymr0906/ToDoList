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
	QString responsible;
	QString email;
	bool isMyDay;
	bool isImportant;
	STATUS status;

	Task(QString taskName = "Empty", QString deadline = "2000.01.01", QString responsible = "None", QString email = "Empty", int status = 0, bool isMyDay = false, bool isImportant = false)
		:taskName{ taskName }, deadline{ deadline }, responsible{responsible}, email{email}, status{status}, isMyDay{isMyDay}, isImportant{isImportant} {}

	QString statusToString() {
		if (status == STATUS::IN_PROCESS) {
			return "In Process";
		}
		if (status == STATUS::COMPLETED) {
			return "Completed";
		}
		if (status == STATUS::FAILED) {
			return "Failed";
		}
	}
};

