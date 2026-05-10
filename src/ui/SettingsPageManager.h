#pragma once

#include "core/ConfigManager.h"
#include "ui/FloatingBar.h"

#include <QGroupBox>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>


class SettingsPageManager : public QWidget
{
    Q_OBJECT

public:
    SettingsPageManager(QWidget* parent, ConfigManager& cfg, FloatingBarClass* bar);
    ~SettingsPageManager();

Q_SIGNALS:
    void clickedReminderPreviewButton();
    void clickedCloseButton();

private:

    void initializeObjects();
    void adjustObjects();
    void connectEmissions();

    // Config
    ConfigManager& config;

    // FloatingBar
    FloatingBarClass* FloatingBar;

    // GroupBox
    QGroupBox* GeneralSettingsGroupBox; // 文本和时间设置分组框
    QGroupBox* ReminderSettingsGroupBox; // 全屏提醒设置分组框
    QGroupBox* FloatingBarSettingsGroupBox; // 悬浮条设置分组框

    // ButtonGroup
    QButtonGroup* FloatingBarLevelButtonGroup; // 悬浮条层级按钮组
    QButtonGroup* FloatingBarPositionButtonGroup; // 悬浮条位置按钮组

    // LineEdit
    QLineEdit* FloatingBarTextLineEdit; // 设置悬浮条文本输入框
    QLineEdit* ReminderTitleLineEdit; // 设置全屏窗口文本输入框
    QLineEdit* ReminderTextLineEdit; // 设置全屏窗口小文本输入框

    // DateTimeEdit
    QDateTimeEdit* TargetDateTimeEdit; // 设置目标时间输入框

    // ComboBox
    QComboBox* LanguageComboBox; // 设置语言下拉框

    // SpinBox
    QSpinBox* ReminderRemainingDaysToPlayCountdownSoundSpinBox; // 设置剩余天数播放倒计时声音输入框
    QSpinBox* ReminderRemainingDaysToPlayHeartbeatSoundSpinBox; // 设置剩余天数播放心跳声音输入框
    QSpinBox* ReminderBlockShowTimesSpinBox; // 设置红色块显示次数输入框

    QSpinBox* FloatingBarHeightSpinBox; // 设置悬浮条高度输入框
    QSpinBox* FloatingBarBorderRadiusSpinBox; // 设置窗口圆角输入框

    QPushButton* ReminderPreviewButton; // 全屏提醒预览按钮
    QPushButton* CloseButton; // 设置关闭按钮

    // CheckBox
    QCheckBox* IsShowReminderCheckBox; // 设置是否显示全屏窗口复选框

    QCheckBox* IsShowFloatingBarCheckBox; // 设置是否显示悬浮条复选框

    // RadioButton
    QRadioButton* FloatingBarPositionTopLeftRadioButton; // 设置悬浮条位置左上单选按钮
    QRadioButton* FloatingBarPositionTopCenterRadioButton; // 设置悬浮条位置中上单选按钮
    QRadioButton* FloatingBarPositionTopRightRadioButton; // 设置悬浮条位置右上单选按钮

    QRadioButton* FloatingBarOnTopRadioButton; // 设置悬浮条置顶单选按钮
    QRadioButton* FloatingBarOnBottomRadioButton; // 设置悬浮条置底单选按钮


};