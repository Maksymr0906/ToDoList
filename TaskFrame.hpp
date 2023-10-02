#pragma once

#include <QWidget>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QVector>
#include <QString>
#include <QMessageBox>
#include <QTSql>
#include <memory>

#include "Task.hpp"
#include "EditTaskDialog.hpp"

class TaskFrame : public QFrame {
    Q_OBJECT
private slots:
    void doneBtnPressed();
    void failedBtnPressed();
    void myDayBtnPressed();
    void failedBtnReleased();
    void myDayBtnReleased();
    void doneBtnReleased();
    void editTask();
signals:
    void clicked();
public:
    DateValidator* dateValidator;
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
    QPushButton* failedBtn;
    QPushButton* doneBtn;
    Task task;

    void mousePressEvent(QMouseEvent* event) override {
        QFrame::mousePressEvent(event);
        emit clicked();
    }

    void setTitleLabelText(QString titleText) {
        titleLabel->setText(titleText);
    }

    TaskFrame(Task newTask, QWidget* parent = nullptr) : task{ newTask }, QFrame(parent) {
        dateValidator = new DateValidator();
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

        failedBtn = new QPushButton();
        doneBtn = new QPushButton();

        buttonsLayout->addWidget(failedBtn);
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

        failedBtn->setIcon(failedBtnIcon);
        failedBtn->setIconSize(QSize(30, 30));
        failedBtn->setStyleSheet("background-color: transparent; border: none;");

        doneBtn->setIcon(doneBtnIcon);
        doneBtn->setIconSize(QSize(30, 30));
        doneBtn->setStyleSheet("background-color: transparent; border: none;");

        myDayBtn->setIcon(myDayBtnIcon);
        myDayBtn->setIconSize(QSize(45, 45));
        myDayBtn->setStyleSheet("background-color: transparent; border: none;");

        connect(myDayBtn, &QPushButton::pressed, this, &TaskFrame::myDayBtnPressed);
        connect(doneBtn, &QPushButton::pressed, this, &TaskFrame::doneBtnPressed);
        connect(failedBtn, &QPushButton::pressed, this, &TaskFrame::failedBtnPressed);
        connect(failedBtn, &QPushButton::released, this, &TaskFrame::failedBtnReleased);
        connect(doneBtn, &QPushButton::released, this, &TaskFrame::doneBtnReleased);
        connect(myDayBtn, &QPushButton::released, this, &TaskFrame::myDayBtnReleased);
        connect(this, &TaskFrame::clicked, this, &TaskFrame::editTask);
    }
    TaskFrame(QWidget* parent = nullptr) : QFrame(parent) {
        dateValidator = new DateValidator();
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

        failedBtn = new QPushButton();
        doneBtn = new QPushButton();

        buttonsLayout->addWidget(failedBtn);
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

        failedBtn->setIcon(failedBtnIcon);
        failedBtn->setIconSize(QSize(30, 30));
        failedBtn->setStyleSheet("background-color: transparent; border: none;");

        doneBtn->setIcon(doneBtnIcon);
        doneBtn->setIconSize(QSize(30, 30));
        doneBtn->setStyleSheet("background-color: transparent; border: none;");

        myDayBtn->setIcon(myDayBtnIcon);
        myDayBtn->setIconSize(QSize(45, 45));
        myDayBtn->setStyleSheet("background-color: transparent; border: none;");

        connect(myDayBtn, SIGNAL(clicked()), this, SLOT(myDayBtnClicked()));
        connect(doneBtn, SIGNAL(clicked()), this, SLOT(doneBtnClicked()));
        connect(failedBtn, SIGNAL(clicked()), this, SLOT(failedBtnClicked()));
        connect(this, &TaskFrame::clicked, this, &TaskFrame::editTask);
    }
};

