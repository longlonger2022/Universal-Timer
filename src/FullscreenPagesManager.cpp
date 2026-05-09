#include "FullscreenPagesManager.h"

#include <QVBoxLayout>
#include <QTimer>

FullscreenPagesManager::FullscreenPagesManager(QWidget* parent, ConfigManager& cfg, FloatingBarClass* bar)
    : QWidget(parent), config(cfg), FloatingBar(bar)
{

    this->setAttribute(Qt::WA_TranslucentBackground);

    BackgroundWidget = new QWidget(this);
    BackgroundWidget->setStyleSheet("background: rgba(0, 0, 0, 0.75);");

    SlideAnimationWidget = new QWidget(this);
    SlideAnimationWidget->setStyleSheet("background: red;");
    BackgroundWidget->hide();

    SlideInAnimation = new QPropertyAnimation(SlideAnimationWidget, "pos", this);
    SlideInAnimation->setDuration(250);
    SlideOutAnimation = new QPropertyAnimation(SlideAnimationWidget, "pos", this);
    SlideOutAnimation->setDuration(1000);
    SlideOutAnimation->setEasingCurve(QEasingCurve::OutCubic);
    SlideInOutAnimationGroup = new QSequentialAnimationGroup(this);
    SlideInOutAnimationGroup->addAnimation(SlideInAnimation);
    SlideInOutAnimationGroup->addAnimation(SlideOutAnimation);

    FadeOutAnimation = new QPropertyAnimation(this, "windowOpacity", this);
    FadeOutAnimation->setDuration(1000);
    FadeOutAnimation->setStartValue(1);
    FadeOutAnimation->setEndValue(0);
    FadeOutAnimation->setEasingCurve(QEasingCurve::InCubic);

    connect(SlideInAnimation, &QPropertyAnimation::finished, [this] {
        switch (fullscreen_pages_mode) {
            case FullscreenPagesMode::Reminder: {
                ReminderPageManager* ReminderPage = new ReminderPageManager(this, config);
                ReminderPage->move((this->width() - ReminderPage->width()) / 2, (this->height() - ReminderPage->height()) / 2);
                SlideAnimationWidget->raise();
                BackgroundWidget->show();
                ReminderPage->show();
                connect(ReminderPage, &ReminderPageManager::finished, this, [this, ReminderPage] {
                    FadeOutAnimation->start();
                    connect(FadeOutAnimation, &QPropertyAnimation::finished, ReminderPage, &ReminderPageManager::deleteLater);
                    }, Qt::SingleShotConnection);
                break;
            }
            case FullscreenPagesMode::Settings: {
                SettingsPageManager* SettingsPage = new SettingsPageManager(this, config, FloatingBar);
                SettingsPage->resize(this->size());
                SlideAnimationWidget->raise();
                BackgroundWidget->show();
                SettingsPage->show();
                connect(SettingsPage, &SettingsPageManager::clickedCloseButton, this, [this, SettingsPage] {
                    FadeOutAnimation->start();
                    connect(FadeOutAnimation, &QPropertyAnimation::finished, SettingsPage, &SettingsPageManager::deleteLater, Qt::SingleShotConnection);
                    }, Qt::SingleShotConnection);
                connect(SettingsPage, &SettingsPageManager::clickedReminderPreviewButton, this, [this, SettingsPage] {
                    fullscreen_pages_mode = FullscreenPagesMode::None;
                    showReminder();
                    connect(SlideInAnimation, &QPropertyAnimation::finished, SettingsPage, &SettingsPageManager::deleteLater, Qt::SingleShotConnection);
                    }, Qt::SingleShotConnection);
                break;
            }
            case FullscreenPagesMode::Welcome: {
                WelcomePageManager* WelcomePage = new WelcomePageManager(this);
                WelcomePage->resize(this->size());
                SlideAnimationWidget->raise();
                BackgroundWidget->show();
                WelcomePage->show();
                connect(WelcomePage, &WelcomePageManager::finished, this, [this, WelcomePage] {
                    FadeOutAnimation->start();
                    connect(FadeOutAnimation, &QPropertyAnimation::finished, WelcomePage, &WelcomePageManager::deleteLater);
                    }, Qt::SingleShotConnection);
                break;
            }
        }
        });
    connect(FadeOutAnimation, &QPropertyAnimation::finished, [this] {
        this->hide();
        BackgroundWidget->hide();
        fullscreen_pages_mode = FullscreenPagesMode::None;
        });
}

FullscreenPagesManager::~FullscreenPagesManager()
{}

void FullscreenPagesManager::refresh() {

}

void FullscreenPagesManager::showReminder() {
    if (fullscreen_pages_mode == FullscreenPagesMode::None) {
        fullscreen_pages_mode = FullscreenPagesMode::Reminder;
        startAnimation();
    }
}

void FullscreenPagesManager::showSettings() {
    if (fullscreen_pages_mode == FullscreenPagesMode::None) {
        fullscreen_pages_mode = FullscreenPagesMode::Settings;
        startAnimation();
    }
}

void FullscreenPagesManager::showWelcome() {
    if (fullscreen_pages_mode == FullscreenPagesMode::None) {
        fullscreen_pages_mode = FullscreenPagesMode::Welcome;
        startAnimation();
    }
}

void FullscreenPagesManager::startAnimation() {
    this->setWindowOpacity(1);
    this->show();
    SlideInOutAnimationGroup->start();
}

void FullscreenPagesManager::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    BackgroundWidget->resize(this->size());

    SlideAnimationWidget->resize(event->size());

    SlideInAnimation->setStartValue(QPoint(-event->size().width(), 0));
    SlideInAnimation->setEndValue(QPoint(0, 0));
    SlideOutAnimation->setStartValue(QPoint(0, 0));
    SlideOutAnimation->setEndValue(QPoint(event->size().width(), 0));
    
}