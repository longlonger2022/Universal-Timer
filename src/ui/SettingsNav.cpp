#include "core/Global.h"
#include "ui/SettingsNav.h"

#include <QVBoxLayout>

SettingsNavClass::SettingsNavClass(QWidget* parent, SettingsContentClass* content)
    : QWidget(parent), SettingsContent(content)
{

    ChosenLabel = new QLabel(this);

    GeneralSettingsButton = new QPushButton(tr("总设置"), this);
    FloatingBarSettingsButton = new QPushButton(tr("悬浮条设置"), this);
    ReminderSettingsButton = new QPushButton(tr("全屏提醒设置"), this);
    DonateButton = new QPushButton(tr("赞助"), this);
    AboutButton = new QPushButton(tr("关于"), this);
    CloseButton = new QPushButton(tr("关闭设置"), this);

    ChosenLabelMoveAnimation = new QPropertyAnimation(ChosenLabel, "pos", this);
    ChosenLabelMoveAnimation->setDuration(500);
    ChosenLabelMoveAnimation->setEasingCurve(QEasingCurve::OutExpo);

    // Layout
    QVBoxLayout* Layout = new QVBoxLayout(this);
    Layout->addWidget(GeneralSettingsButton);
    Layout->addWidget(FloatingBarSettingsButton);
    Layout->addWidget(ReminderSettingsButton);
    Layout->addStretch();
    Layout->addWidget(DonateButton);
    Layout->addWidget(AboutButton);
    Layout->addWidget(CloseButton);
    Layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(Layout);

    connect(GeneralSettingsButton, &QPushButton::clicked, this, [this] {
        ChosenLabelMoveAnimation->setStartValue(ChosenLabel->pos());
        ChosenLabelMoveAnimation->setEndValue(GeneralSettingsButton->pos());
        ChosenLabelMoveAnimation->start();
        SettingsContent->setCurrentPage(SettingsContentClass::Page::GeneralSettingsPage);
        });
    connect(FloatingBarSettingsButton, &QPushButton::clicked, this, [this] {
        ChosenLabelMoveAnimation->setStartValue(ChosenLabel->pos());
        ChosenLabelMoveAnimation->setEndValue(FloatingBarSettingsButton->pos());
        ChosenLabelMoveAnimation->start();
        SettingsContent->setCurrentPage(SettingsContentClass::Page::FloatingBarSettingsPage);
        });
    connect(ReminderSettingsButton, &QPushButton::clicked, this,  [this] {
        ChosenLabelMoveAnimation->setStartValue(ChosenLabel->pos());
        ChosenLabelMoveAnimation->setEndValue(ReminderSettingsButton->pos());
        ChosenLabelMoveAnimation->start();
        SettingsContent->setCurrentPage(SettingsContentClass::Page::ReminderSettingsPage);
        });
    connect(DonateButton, &QPushButton::clicked, this, [this] {
        ChosenLabelMoveAnimation->setStartValue(ChosenLabel->pos());
        ChosenLabelMoveAnimation->setEndValue(DonateButton->pos());
        ChosenLabelMoveAnimation->start();
        SettingsContent->setCurrentPage(SettingsContentClass::Page::DonatePage);
        });
    connect(AboutButton, &QPushButton::clicked, this, [this] {
        ChosenLabelMoveAnimation->setStartValue(ChosenLabel->pos());
        ChosenLabelMoveAnimation->setEndValue(AboutButton->pos());
        ChosenLabelMoveAnimation->start();
        SettingsContent->setCurrentPage(SettingsContentClass::Page::AboutPage);
        });
    connect(CloseButton, &QPushButton::clicked, this, [this] {
        ChosenLabelMoveAnimation->setStartValue(ChosenLabel->pos());
        ChosenLabelMoveAnimation->setEndValue(CloseButton->pos());
        ChosenLabelMoveAnimation->start();
        SettingsContent->setCurrentPage(SettingsContentClass::Page::None);
        emit clickedCloseButton();
        });

}

SettingsNavClass::~SettingsNavClass()
{}

void SettingsNavClass::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    
    this->setStyleSheet(QString(R"(
        QPushButton {
            color: white;
            background-color: transparent;
            border: none;
            font-size: %1px;
            padding: %2px;
        }
        QPushButton:hover {
            background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 255, 255, 0.5), stop:1 transparent);
        }
    )").arg(this->height() * 0.03).arg((this->height() * 0.03 / GOLDEN_RATIO_INV - this->height() * 0.03) / 2));

    this->layout()->activate();

    ChosenLabel->setStyleSheet("background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 0.5), stop:1 transparent); border-top: 3px solid qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 0.75), stop:1 transparent); border-bottom: 3px solid qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255, 0, 0, 0.75), stop:1 transparent);");
    ChosenLabel->setGeometry(CloseButton->geometry());


}