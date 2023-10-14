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
	int id;
	QString taskName;
	QString deadline;
	QString responsible;
	QString email;
	bool isMyDay;
	bool isImportant;
	STATUS status;

	Task(int id = 1, QString taskName = "Empty", QString deadline = "2000.01.01", QString responsible = "None", QString email = "Empty", int status = 0, bool isMyDay = false, bool isImportant = false)
		:id{ id }, taskName{ taskName }, deadline{ deadline }, responsible{responsible}, email{email}, status{status}, isMyDay{isMyDay}, isImportant{isImportant} {}

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
		
		return "";
	}

	void updateStatus() {
		if (deadline.isEmpty())
			return;

		QString deadlineString = deadline;

		int year = deadlineString.mid(0, 4).toInt();
		int month = deadlineString.mid(5, 2).toInt();
		int day = deadlineString.mid(8, 2).toInt();
		if (status == STATUS::IN_PROCESS) {
			if (QDate::currentDate() <= QDate(year, month, day))
				status = STATUS::IN_PROCESS;
			else status = STATUS::FAILED;
		}
	}

	void updateIsMyDay() {
		QString deadlineString = deadline;

		int year = deadlineString.mid(0, 4).toInt();
		int month = deadlineString.mid(5, 2).toInt();
		int day = deadlineString.mid(8, 2).toInt();

		if (QDate::currentDate() == QDate(year, month, day)) {
			isMyDay = true;
		}
	}
};

