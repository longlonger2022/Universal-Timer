#pragma once

#include "ui/ScalableLabel.h"

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPixmap>
#include <QResizeEvent>

class AboutPageClass : public QWidget
{
    Q_OBJECT

public:
    AboutPageClass(QWidget* parent = nullptr);
    ~AboutPageClass();

private:

    ScalableLabel* IconLabel;

    // Label
    QLabel* AboutLabel;

    // TextEdit
    QTextEdit* AboutTextEdit;

};