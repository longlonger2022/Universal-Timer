#pragma once

#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>

class WelcomePageClass : public QWidget
{
    Q_OBJECT
public:
    WelcomePageClass(QWidget* parent = nullptr);
    ~WelcomePageClass();

Q_SIGNALS:
    void finished();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:


    // Label
    QLabel* WelcomeLabel;

    // PushButton
    QPushButton* WelcomeButton;

};
