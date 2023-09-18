#pragma once

#include <QWidget>
#include <QFrame>

class ClickableFrame : public QFrame {
    Q_OBJECT

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        QFrame::mousePressEvent(event);
        emit clicked();
    }
};