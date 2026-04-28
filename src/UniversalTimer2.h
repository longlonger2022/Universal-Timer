#pragma once

#include "Logger.h"
#include "ConfigManager.h"

#include <QtWidgets/QWidget>
#include <numbers>
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
#include <QComboBox>
#include <QTranslator>
#include <QSoundEffect>
#include <QDateTime>
#include <QTimer>

inline constexpr double GOLDEN_RATIO_INV = 1.0 / std::numbers::phi; // 黄金比例倒数

class UniversalTimer2 : public QWidget
{
    Q_OBJECT

public:
    UniversalTimer2(QWidget* parent = nullptr);
    ~UniversalTimer2();

    // 欢迎
    void showWelcome(); // 显示欢迎界面

private:


    // 函数声明
    // 刷新
    void refresh();

    // 语言
    void scanLanguage(); // 扫描语言
    void changeLanguage(); // 切换语言

    // 全屏窗口
    void showReminder(); // 显示全屏窗口

    // 更新
    void updateFloatingBar(); // 更新悬浮条
    void updateReminder(); // 更新全屏提醒
    void updateSettings(); // 更新设置界面
    void updateObjects(); // 更新对象
    void updateText(); // 更新文本
    void adjustReminderSize(const qreal scale); // 调整全屏提醒大小

    // 闪烁
    void showBlocks(unsigned short times = 1); // 显示红色块


    // 配置
    ConfigManager Config;


    // 强枚举
    // 全屏窗口模式
    enum class FullScreenWidgetMode {
        None,
        Welcome,
        Settings,
        Reminder
    };
    FullScreenWidgetMode FullScreenWidget_mode = FullScreenWidgetMode::None; // 全屏窗口模式，默认无


    QFont font; // 字体

    QRect desktop; // 屏幕分辨率

    int timeLeft = 0; // 剩余时间

    QTranslator* Translator; // 翻译器


    // Sound
    QSoundEffect* CountdownSound; // 倒计时声音
    QSoundEffect* HeartbeatSound; // 心跳声音

    // SystemTrayIcon
    QSystemTrayIcon* TrayIcon; // 系统托盘图标

    // Widget
    QWidget* FullScreenWidget; // 全屏窗口

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
    QLabel* FullScreenWidgetBackgroundLabel; // 全屏窗口背景标签

    QLabel* SettingsUnderlyingLabel; // 设置背景标签
    QLabel* SettingsSmallWindowTextLabel; // 设置悬浮条文本标签
    QLabel* SettingsReminderTextLabel; // 设置全屏窗口文本标签
    QLabel* SettingsReminderSmallTextLabel; // 设置全屏窗口小文本标签
    QLabel* SettingsTargetDateTimeLabel; // 设置目标时间标签

    QLabel* ReminderUnderlyingLabel; // 全屏提醒背景标签
    QLabel* ReminderTextLabel; // 全屏提醒自定义文本标签
    QLabel* ReminderRemainingLabel; // 全屏提醒“还剩”文本标签
    QLabel* ReminderDaysLabel; // 全屏提醒“天”文本标签
    QLabel* ReminderNumberLabel; // 全屏提醒剩余天数标签
    QLabel* ReminderSmallTextLabel; // 全屏提醒自定义小文本标签
    QLabel* ReminderColorLabel; // 全屏提醒颜色标签

    QList<QLabel*> ReminderBlockLabels; // 全屏提醒红色块标签列表

    // LineEdit
    QLineEdit* SettingsSmallWindowTextLineEdit; // 设置悬浮条文本输入框
    QLineEdit* SettingsReminderTextLineEdit; // 设置全屏窗口文本输入框
    QLineEdit* SettingsReminderSmallTextLineEdit; // 设置全屏窗口小文本输入框

    // DateTimeEdit
    QDateTimeEdit* SettingsTargetDateTimeEdit; // 设置目标时间输入框

    // ComboBox
    QComboBox* SettingsLanguageComboBox; // 设置语言下拉框

    // SpinBox
    QSpinBox* SettingsRemainingDaysToPlayCountdownSoundSpinBox; // 设置剩余天数播放倒计时声音输入框
    QSpinBox* SettingsRemainingDaysToPlayHeartbeatSoundSpinBox; // 设置剩余天数播放心跳声音输入框
    QSpinBox* SettingsBlockShowTimesSpinBox; // 设置红色块显示次数输入框

    QSpinBox* SettingsSmallWindowHeightSpinBox; // 设置悬浮条高度输入框
    QSpinBox* SettingsSmallWindowBorderRadiusSpinBox; // 设置窗口圆角输入框

    // PushButton
    QPushButton* WelcomeButton; // 欢迎按钮

    QPushButton* SettingsReminderPreviewButton; // 全屏提醒预览按钮
    QPushButton* SettingsCloseButton; // 设置关闭按钮

    // CheckBox
    QCheckBox* SettingsIsShowReminderCheckBox; // 设置是否显示全屏窗口复选框

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

    QPropertyAnimation* FullScreenWidgetHideAnimation; // 全屏窗口隐藏动画

    // Animation Group
    QSequentialAnimationGroup* FullScreenAnimationGroup; // 红色全屏动画组

};
