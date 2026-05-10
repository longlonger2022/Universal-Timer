#pragma once

#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>

class WelcomePageManager : public QWidget
{
    Q_OBJECT
public:
    WelcomePageManager(QWidget* parent = nullptr);
    ~WelcomePageManager();

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
