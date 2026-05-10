#pragma once

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

class DonatePageClass : public QLabel
{
    Q_OBJECT

public:
    DonatePageClass(QWidget* parent = nullptr);
    ~DonatePageClass();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QPixmap OriginalPixmap;

};