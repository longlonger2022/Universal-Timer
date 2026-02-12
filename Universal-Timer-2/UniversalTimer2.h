#pragma once

#include <QtWidgets/QWidget>
#include "ui_UniversalTimer2.h"
#include <QLabel>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QMessageBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDateTime>
#include <QTimer>

class UniversalTimer2 : public QWidget
{
    Q_OBJECT

public:
    UniversalTimer2(QWidget *parent = nullptr);
    ~UniversalTimer2();

private slots:
    void readConfig(); // 读取配置
    void writeConfig(); // 写入配置
    void readTimeList(); // 读取时间列表
    void writeTimeList(); // 写入时间列表
    void writeLog(QString type, QString log); // 写入日志
    void showBigWindow(); // 显示全屏窗口
    void updateObjects(); // 更新对象
    void adjustReminderSize(qreal scale); // 调整全屏提醒大小

private:
    Ui::UniversalTimer2Class ui;

    bool is_show_BigWindow = true; // 是否显示全屏窗口，默认显示
    bool is_show_SmallWindow = true; // 是否显示悬浮条，默认显示
    bool SmallWindow_on_top = true; // 悬浮条是否置顶，默认置顶

    bool is_setting = false; // 是否在设置界面，默认不在
    bool is_welcome = false; // 是否在欢迎界面，默认在

    int border_radius = 10; // 窗口圆角，默认10
    unsigned short SmallWindow_height = 50; // 悬浮条高度，默认50
    int remaining_days_to_play_countdown_sound = 30; // 剩余天数播放倒计时声音，默认30
    int remaining_days_to_play_heartbeat_sound = 14; // 剩余天数播放心跳声音，默认14

    QFont font; // 字体

    QRect desktop; // 屏幕分辨率

    QDateTime currentDateTime; // 当前时间
    QDateTime targetDateTime = QDateTime::fromString("2025-06-30 00:00:00", "yyyy-MM-dd hh:mm:ss"); // 目标时间，默认2025-06-30 00:00:00
    int timeLeft = 0; // 剩余时间

    unsigned short update_interval = 1000; // 更新间隔时间，默认1秒

    QString SmallWindow_text = "距会考还剩："; // 悬浮条文本
    QString BigWindow_text = "距会考"; // 全屏窗口文本
    QString BigWindow_small_text = "THE EXAM IN "; // 全屏窗口小文本

    unsigned short SmallWindow_position = 1; // 悬浮条位置，0为左上，1为中上，2为右上

    QList<QTime> timeList = { QTime(8, 13), QTime(9, 3), QTime(9, 53), QTime(10, 43), QTime(11, 38), QTime(14, 20), QTime(15, 23), QTime(16, 13), QTime(17, 3), QTime(18, 6) }; // 时间列表

    // SystemTrayIcon
    QSystemTrayIcon* TrayIcon; // 系统托盘图标

    // Widget
    QWidget* BigWindow; // 全屏窗口

    // GroupBox
    QGroupBox* SettingsTextAndDateGroupBox; // 文本和时间设置分组框
    QGroupBox* SettingsReminderGroupBox; // 全屏提醒设置分组框
    QGroupBox* SettingsSmallWindowGroupBox; // 悬浮条设置分组框

    // ButtonGroup
    QButtonGroup* SettingsSmallWindowLevelButtonGroup; // 悬浮条层级按钮组
    QButtonGroup* SettingsSmallWindowPositionButtonGroup; // 悬浮条位置按钮组
    
    // Label
    QLabel* WelcomeLabel; // 欢迎标签
    QLabel* SmallWindowLabel; // 悬浮条背景标签
    QLabel* FullScreenAnimationLabel; // 红色全屏动画标签
    QLabel* BigWindowBackgroundLabel; // 全屏窗口背景标签

    QLabel* SettingsUnderlyingLabel; // 设置背景标签
    QLabel* SettingsSmallWindowTextLabel; // 设置悬浮条文本标签
    QLabel* SettingsBigWindowTextLabel; // 设置全屏窗口文本标签
    QLabel* SettingsBigWindowSmallTextLabel; // 设置全屏窗口小文本标签
    QLabel* SettingsTargetDateTimeLabel; // 设置目标时间标签

    QLabel* ReminderUnderlyingLabel; // 全屏提醒背景标签
    QLabel* ReminderTextLabel; // 全屏提醒自定义文本标签
    QLabel* ReminderRemainingLabel; // 全屏提醒“还剩”文本标签
    QLabel* ReminderDaysLabel; // 全屏提醒“天”文本标签
    QLabel* ReminderNumberLabel; // 全屏提醒剩余天数标签
    QLabel* ReminderSmallTextLabel; // 全屏提醒自定义小文本标签
    QLabel* ReminderColorLabel; // 全屏提醒颜色标签

    // LineEdit
    QLineEdit* SettingsSmallWindowTextLineEdit; // 设置悬浮条文本输入框
    QLineEdit* SettingsBigWindowTextLineEdit; // 设置全屏窗口文本输入框
    QLineEdit* SettingsBigWindowSmallTextLineEdit; // 设置全屏窗口小文本输入框

    // DateTimeEdit
    QDateTimeEdit* SettingsTargetDateTimeEdit; // 设置目标时间输入框

    // SpinBox
    QSpinBox* SettingsRemainingDaysToPlayCountdownSoundSpinBox; // 设置剩余天数播放倒计时声音输入框
    QSpinBox* SettingsRemainingDaysToPlayHeartbeatSoundSpinBox; // 设置剩余天数播放心跳声音输入框

    QSpinBox* SettingsSmallWindowHeightSpinBox; // 设置悬浮条高度输入框
    QSpinBox* SettingsSmallWindowBorderRadiusSpinBox; // 设置窗口圆角输入框

    // PushButton
    QPushButton* WelcomeButton; // 欢迎按钮

    QPushButton* SettingsReminderPreviewButton; // 全屏提醒预览按钮
    QPushButton* SettingsCloseButton; // 设置关闭按钮

    // CheckBox
    QCheckBox* SettingsIsShowBigWindowCheckBox; // 设置是否显示全屏窗口复选框

    QCheckBox* SettingsIsShowSmallWindowCheckBox; // 设置是否显示悬浮条复选框

    // RadioButton
    QRadioButton* SettingsSmallWindowPositionTopLeftRadioButton; // 设置悬浮条位置左上单选按钮
    QRadioButton* SettingsSmallWindowPositionTopCenterRadioButton; // 设置悬浮条位置中上单选按钮
    QRadioButton* SettingsSmallWindowPositionTopRightRadioButton; // 设置悬浮条位置右上单选按钮

    QRadioButton* SettingsSmallWindowOnTopRadioButton; // 设置悬浮条置顶单选按钮
    QRadioButton* SettingsSmallWindowOnBottomRadioButton; // 设置悬浮条置底单选按钮

    // Timer
    QTimer* Timer; // 定时器

    // Animation
    QPropertyAnimation* FullScreenAnimation1; // 红色全屏动画前段
    QPropertyAnimation* FullScreenAnimation2; // 红色全屏动画后段

    QPropertyAnimation* BigWindowHideAnimation; // 全屏窗口隐藏动画

    // Animation Group
    QSequentialAnimationGroup* FullScreenAnimationGroup; // 红色全屏动画组

};
