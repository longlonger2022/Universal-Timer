#pragma once

#include <QtWidgets/QWidget>
#include "ui_ExamCountdown_v1.h"
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsEffect>

class ExamCountdown_v1 : public QWidget
{
    Q_OBJECT

public:
    ExamCountdown_v1(QWidget *parent = nullptr);
    ~ExamCountdown_v1();

private:
    Ui::ExamCountdown_v1Class ui;

    void updateLabel();

    QDateTime currentDateTime;
    QString currentDateTimeString;
    QDateTime targetDateTime;
    int timeDifference;
    QString timeDifferenceString;

    int BlockLabelShowTimes;


    QGraphicsOpacityEffect* NumberLabelOpacityEffect;
    QGraphicsOpacityEffect* StartWindowBlockLabel1OpacityEffect;
    QGraphicsOpacityEffect* StartWindowBlockLabel2OpacityEffect;
    QGraphicsOpacityEffect* StartWindowBlockLabel3OpacityEffect;
    QGraphicsOpacityEffect* StartWindowBlockLabel4OpacityEffect;


    QPropertyAnimation* SmallWindowStartAnimation;
    QPropertyAnimation* SmallWindowStartOpacityAnimation;
    QPropertyAnimation* SmallWindowCloseOpacityAnimation;
    QPropertyAnimation* StartWindowStartOpacityAnimation;
    QPropertyAnimation* StartWindowColorLabelAnimation1;
    QPropertyAnimation* StartWindowColorLabelAnimation2;
    QPropertyAnimation* StartWindowTextColorLabelAnimation;
    QPropertyAnimation* StartWindowNumberLabelOpacityAnimation1;
    QPropertyAnimation* StartWindowNumberLabelOpacityAnimation2;
    QPropertyAnimation* StartWindowNumberLabelOpacityAnimation3;
    QPropertyAnimation* StartWindowNumberLabelOpacityAnimation4;
    QPropertyAnimation* StartWindowNumberLabelOpacityAnimation5;
    QPropertyAnimation* StartWindowCloseOpacityAnimation;

    QPropertyAnimation* StartWindowBlockLabel1OpacityAnimation1;
    QPropertyAnimation* StartWindowBlockLabel1OpacityAnimation2;
    QPropertyAnimation* StartWindowBlockLabel1OpacityAnimation3;
    QPropertyAnimation* StartWindowBlockLabel1OpacityAnimation4;
    QPropertyAnimation* StartWindowBlockLabel2OpacityAnimation1;
    QPropertyAnimation* StartWindowBlockLabel2OpacityAnimation2;
    QPropertyAnimation* StartWindowBlockLabel2OpacityAnimation3;
    QPropertyAnimation* StartWindowBlockLabel2OpacityAnimation4;
    QPropertyAnimation* StartWindowBlockLabel3OpacityAnimation1;
    QPropertyAnimation* StartWindowBlockLabel3OpacityAnimation2;
    QPropertyAnimation* StartWindowBlockLabel3OpacityAnimation3;
    QPropertyAnimation* StartWindowBlockLabel3OpacityAnimation4;
    QPropertyAnimation* StartWindowBlockLabel4OpacityAnimation1;
    QPropertyAnimation* StartWindowBlockLabel4OpacityAnimation2;
    QPropertyAnimation* StartWindowBlockLabel4OpacityAnimation3;
    QPropertyAnimation* StartWindowBlockLabel4OpacityAnimation4;


    QSequentialAnimationGroup* StartWindowAnimationGroup;
    QSequentialAnimationGroup* StartWindowNumberLabelAnimationGroup;

    QSequentialAnimationGroup* StartWindowBlockLabel1AnimationGroup;
    QSequentialAnimationGroup* StartWindowBlockLabel2AnimationGroup;
    QSequentialAnimationGroup* StartWindowBlockLabel3AnimationGroup;
    QSequentialAnimationGroup* StartWindowBlockLabel4AnimationGroup;



    QParallelAnimationGroup* SmallWindowStartAnimationGroup;


    QWidget* StartWindow;


    QLabel* SmallWindowLabel;
    QLabel* StartWindowUnderlyingLabel;
    QLabel* StartWindowColorLabel;
    QLabel* StartWindowTextColorLabel;
    QLabel* StartWindowTextLabel1;
    QLabel* StartWindowTextLabel2;
    QLabel* StartWindowTextLabel3;
    QLabel* StartWindowNumberLabel;
    QLabel* StartWindowTextLabelEnglish;

    QLabel* StartWindowBlockLabel1;
    QLabel* StartWindowBlockLabel2;
    QLabel* StartWindowBlockLabel3;
    QLabel* StartWindowBlockLabel4;

};
