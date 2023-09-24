#pragma once

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QVector>
#include <QString>

#include "Task.hpp"

class TaskFrame : public QFrame {
    Q_OBJECT
signals:
    void clicked();
public:
    static const QVector<QString> colors;
    QHBoxLayout *taskLayout;
    QPushButton *myDayBtn;
    QFrame *VFrame;
    QVBoxLayout* taskDetails;
    QLabel* titleLabel;
    QLabel* taskLabel;
    QLabel* dateLabel;
    QSpacerItem* spacer;
    QFrame* buttons;
    QVBoxLayout* buttonsLayout;
    QPushButton* deleteBtn;
    QPushButton* doneBtn;

    void mousePressEvent(QMouseEvent* event) override {
        QFrame::mousePressEvent(event);
        emit clicked();
    }
    Task task;
    TaskFrame(Task newTask, QWidget* parent = nullptr) : task{ newTask }, QFrame(parent) {
        QPixmap failedBtnIcon("Assets/failed_icon.png");
        QPixmap doneBtnIcon("Assets/done_icon.png");
        QPixmap myDayBtnIcon("Assets/myday_icon.png");
        this->setFrameStyle(QFrame::StyledPanel);

        taskLayout = new QHBoxLayout(this);
        this->setLayout(taskLayout);

        myDayBtn = new QPushButton();
        taskLayout->addWidget(myDayBtn);

        VFrame = new QFrame();
        taskDetails = new QVBoxLayout(VFrame);
        VFrame->setLayout(taskDetails);

        titleLabel = new QLabel();
        taskLabel = new QLabel(task.taskName);
        dateLabel = new QLabel(task.deadline);

        taskDetails->addWidget(titleLabel);
        taskDetails->addWidget(taskLabel);
        taskDetails->addWidget(dateLabel);

        taskLayout->addWidget(VFrame);
        spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Minimum);
        taskLayout->addSpacerItem(spacer);

        buttons = new QFrame();
        buttonsLayout = new QVBoxLayout(buttons);
        buttons->setLayout(buttonsLayout);

        deleteBtn = new QPushButton();
        doneBtn = new QPushButton();

        buttonsLayout->addWidget(deleteBtn);
        buttonsLayout->addWidget(doneBtn);

        taskLayout->addWidget(buttons);

        int randomVal = (((rand() % 50) % 100) % (colors.size() - 1));

        this->setObjectName("NewTask");
        this->setStyleSheet("#NewTask { border-radius: 10px; border: 1px solid black; background-color: " + colors[randomVal] + "; }");

        if (static_cast<STATUS>(task.status) == STATUS::COMPLETED) {
            taskLabel->setObjectName("TaskName");
            taskLabel->setStyleSheet("#TaskName { font: bold 11pt 'Verdana'; color: gray; text-decoration: line-through; }");
        }
        else if (static_cast<STATUS>(task.status) == STATUS::FAILED) {
            taskLabel->setObjectName("TaskName");
            taskLabel->setStyleSheet("#TaskName { font: bold 11pt 'Verdana'; color: red; }");
        }
        else {
            taskLabel->setObjectName("TaskName");
            taskLabel->setStyleSheet("#TaskName { font: bold 11pt black 'Verdana'; }");
        }

        deleteBtn->setIcon(failedBtnIcon);
        deleteBtn->setIconSize(QSize(30, 30));
        deleteBtn->setStyleSheet("background-color: transparent; border: none;");

        doneBtn->setIcon(doneBtnIcon);
        doneBtn->setIconSize(QSize(30, 30));
        doneBtn->setStyleSheet("background-color: transparent; border: none;");

        myDayBtn->setIcon(myDayBtnIcon);
        myDayBtn->setIconSize(QSize(45, 45));
        myDayBtn->setStyleSheet("background-color: transparent; border: none;");
    }
    TaskFrame(QWidget* parent = nullptr) : QFrame(parent) {
        QPixmap failedBtnIcon("Assets/failed_icon.png");
        QPixmap doneBtnIcon("Assets/done_icon.png");
        QPixmap myDayBtnIcon("Assets/myday_icon.png");
        this->setFrameStyle(QFrame::StyledPanel);

        taskLayout = new QHBoxLayout(this);
        this->setLayout(taskLayout);

        myDayBtn = new QPushButton();
        taskLayout->addWidget(myDayBtn);

        VFrame = new QFrame();
        taskDetails = new QVBoxLayout(VFrame);
        VFrame->setLayout(taskDetails);

        titleLabel = new QLabel();
        taskLabel = new QLabel("Empty");
        dateLabel = new QLabel("Empty");

        taskDetails->addWidget(titleLabel);
        taskDetails->addWidget(taskLabel);
        taskDetails->addWidget(dateLabel);

        taskLayout->addWidget(VFrame);
        spacer = new QSpacerItem(100, 100, QSizePolicy::Expanding, QSizePolicy::Minimum);
        taskLayout->addSpacerItem(spacer);

        buttons = new QFrame();
        buttonsLayout = new QVBoxLayout(buttons);
        buttons->setLayout(buttonsLayout);

        deleteBtn = new QPushButton();
        doneBtn = new QPushButton();

        buttonsLayout->addWidget(deleteBtn);
        buttonsLayout->addWidget(doneBtn);

        taskLayout->addWidget(buttons);

        int randomVal = (((rand() % 50) % 100) % (colors.size() - 1));

        this->setObjectName("NewTask");
        this->setStyleSheet("#NewTask { border-radius: 10px; border: 1px solid black; background-color: " + colors[randomVal] + "; }");

        if (static_cast<STATUS>(task.status) == STATUS::COMPLETED) {
            taskLabel->setObjectName("TaskName");
            taskLabel->setStyleSheet("#TaskName { font: bold 11pt 'Verdana'; color: gray; text-decoration: line-through; }");
        }
        else if (static_cast<STATUS>(task.status) == STATUS::FAILED) {
            taskLabel->setObjectName("TaskName");
            taskLabel->setStyleSheet("#TaskName { font: bold 11pt 'Verdana'; color: red; }");
        }
        else {
            taskLabel->setObjectName("TaskName");
            taskLabel->setStyleSheet("#TaskName { font: bold 11pt black 'Verdana'; }");
        }

        deleteBtn->setIcon(failedBtnIcon);
        deleteBtn->setIconSize(QSize(30, 30));
        deleteBtn->setStyleSheet("background-color: transparent; border: none;");

        doneBtn->setIcon(doneBtnIcon);
        doneBtn->setIconSize(QSize(30, 30));
        doneBtn->setStyleSheet("background-color: transparent; border: none;");

        myDayBtn->setIcon(myDayBtnIcon);
        myDayBtn->setIconSize(QSize(45, 45));
        myDayBtn->setStyleSheet("background-color: transparent; border: none;");
    }
};

