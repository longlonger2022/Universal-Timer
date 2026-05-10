#include "ui/WelcomePage.h"

WelcomePageClass::WelcomePageClass(QWidget* parent)
    : QWidget(parent)
{
    WelcomeLabel = new QLabel(tr("欢迎使用万能倒计时!<br>简单设置，并开始使用吧!"), this);
    WelcomeLabel->setAlignment(Qt::AlignCenter);
    WelcomeLabel->resize(this->size());
    WelcomeLabel->setStyleSheet("color: white; font-size: 40px; font-weight: bold; background: transparent; border-image: url(:/images/background/Universal-Timer.png);");
    WelcomeLabel->show();

    WelcomeButton = new QPushButton(tr("开始"), this);
    WelcomeButton->setGeometry(this->width() * 0.45, this->height() * 0.6, this->width() * 0.1, this->height() * 0.1);
    WelcomeButton->setStyleSheet("color: white; font-size: 20px; font-weight: bold; background: rgba(0, 0, 0, 0.5); border-radius: 10px;");
    WelcomeButton->show();
    
    connect(WelcomeButton, &QPushButton::clicked, [this] {
        emit finished();
        });
}

WelcomePageClass::~WelcomePageClass()
{}

void WelcomePageClass::resizeEvent(QResizeEvent* event) {
    WelcomeLabel->resize(this->size());
    WelcomeButton->setGeometry(this->width() * 0.45, this->height() * 0.6, this->width() * 0.1, this->height() * 0.1);
}