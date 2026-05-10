#pragma once

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class ScalableLabel : public QLabel
{

public:
    ScalableLabel(QWidget* parent = nullptr);
    ~ScalableLabel();

    QPixmap OriginalPixmap;

protected:
    void resizeEvent(QResizeEvent* event) override;

};