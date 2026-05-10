#pragma once

#include "ui/ScalableLabel.h"

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class DonatePageClass : public ScalableLabel
{
    Q_OBJECT

public:
    DonatePageClass(QWidget* parent = nullptr);
    ~DonatePageClass();

};