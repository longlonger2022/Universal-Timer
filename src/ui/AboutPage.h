#pragma once

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

    class ScalableLabel : public QLabel
    {

    public:
        ScalableLabel(QWidget* parent = nullptr);
        ~ScalableLabel();

    protected:
        void resizeEvent(QResizeEvent* event) override;

    private:
        QPixmap OriginalPixmap;

    };

private:

    ScalableLabel* IconLabel;

    // Label
    QLabel* AboutLabel;

    // TextEdit
    QTextEdit* AboutTextEdit;

};