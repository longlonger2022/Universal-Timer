#pragma once

#include <QLabel>


class FloatingBarClass : public QWidget
{
    Q_OBJECT

public:
    FloatingBarClass(QWidget* parent = nullptr);
    ~FloatingBarClass();

    QLabel* Bar;
};