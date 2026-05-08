#include "SettingsPageManager.h"
#include "UniversalTimer2.h"

#include <QVBoxLayout>
#include <QFormLayout>

SettingsPageManager::SettingsPageManager(QWidget* parent, ConfigManager& cfg, FloatingBarClass* bar)
    : QWidget(parent), Config(cfg), FloatingBar(bar)
{

    this->setStyleSheet("QGroupBox {\
                            border: 1px solid rgba(255, 0, 0, 0.5);\
                            color: red;\
                        }\
                        QGroupBox::title {\
                            subcontrol-origin: margin;\
                            subcontrol-position: top left;\
                        }\
                        QLabel, QCheckBox, QRadioButton {\
                            color: white;\
                        }");
    
    initializeObjects();
    adjustObjects();
    connectEmissions();

}

SettingsPageManager::~SettingsPageManager()
{}

void SettingsPageManager::initializeObjects() {

    // Creates
    // GroupBox
    GeneralSettingsGroupBox = new QGroupBox(tr("总设置"), this); // 设置文本和日期分组框
    ReminderSettingsGroupBox = new QGroupBox(tr("全屏提醒设置"), this); // 设置全屏提醒分组框
    FloatingBarSettingsGroupBox = new QGroupBox(tr("悬浮条设置"), this); // 设置悬浮条分组框

    // ButtonGroup
    FloatingBarLevelButtonGroup = new QButtonGroup(FloatingBarSettingsGroupBox); // 悬浮条位置按钮组
    FloatingBarPositionButtonGroup = new QButtonGroup(FloatingBarSettingsGroupBox); // 悬浮条位置按钮组
    
    // PushButton
    ReminderPreviewButton = new QPushButton(tr("预览"), ReminderSettingsGroupBox); // 全屏提醒预览按钮
    CloseButton = new QPushButton(tr("关闭设置"), this); // 设置关闭按钮
    
    // LineEdit
    FloatingBarTextLineEdit = new QLineEdit(Config.FloatingBar.floating_bar_text, FloatingBarSettingsGroupBox); // 悬浮条文本输入框
    ReminderTitleLineEdit = new QLineEdit(Config.Reminder.reminder_text, ReminderSettingsGroupBox); // 全屏提醒文本输入框
    ReminderTextLineEdit = new QLineEdit(Config.Reminder.reminder_small_text, ReminderSettingsGroupBox); // 全屏提醒小文本输入框
    
    // DateTimeEdit
    TargetDateTimeEdit = new QDateTimeEdit(Config.General.target_date_time, GeneralSettingsGroupBox); // 目标时间输入框
    
    // CheckBox
    IsShowReminderCheckBox = new QCheckBox(tr("是否显示全屏提醒"), ReminderSettingsGroupBox); // 播报全屏提醒复选框
    IsShowFloatingBarCheckBox = new QCheckBox(tr("是否显示悬浮条"), FloatingBarSettingsGroupBox); // 显示悬浮条复选框
    
    // ComboBox
    LanguageComboBox = new QComboBox(GeneralSettingsGroupBox); // 设置语言下拉框
    
    // SpinBox
    ReminderRemainingDaysToPlayCountdownSoundSpinBox = new QSpinBox(ReminderSettingsGroupBox); // 剩余天数播放倒计时声音
    ReminderRemainingDaysToPlayHeartbeatSoundSpinBox = new QSpinBox(ReminderSettingsGroupBox); // 剩余天数播放心跳声音
    ReminderBlockShowTimesSpinBox = new QSpinBox(ReminderSettingsGroupBox); // 方块闪烁次数
    FloatingBarHeightSpinBox = new QSpinBox(FloatingBarSettingsGroupBox); // 悬浮条高度
    FloatingBarBorderRadiusSpinBox = new QSpinBox(FloatingBarSettingsGroupBox); // 悬浮条圆角
    
    // RadioButton
    FloatingBarOnTopRadioButton = new QRadioButton(tr("悬浮条置顶"), FloatingBarSettingsGroupBox); // 悬浮条置顶单选按钮
    FloatingBarOnBottomRadioButton = new QRadioButton(tr("悬浮条置底"), FloatingBarSettingsGroupBox); // 悬浮条置底单选按钮
    FloatingBarPositionTopLeftRadioButton = new QRadioButton(tr("悬浮条位于左上"), FloatingBarSettingsGroupBox); // 悬浮条位置左上单选按钮
    FloatingBarPositionTopCenterRadioButton = new QRadioButton(tr("悬浮条位于中上"), FloatingBarSettingsGroupBox); // 悬浮条位置中上单选按钮
    FloatingBarPositionTopRightRadioButton = new QRadioButton(tr("悬浮条位于右上"), FloatingBarSettingsGroupBox); // 悬浮条位置右上



    // Others
    // LineEdit
    FloatingBarTextLineEdit->setPlaceholderText(tr("悬浮条文本"));
    ReminderTitleLineEdit->setPlaceholderText(tr("全屏提醒标题"));
    ReminderTextLineEdit->setPlaceholderText(tr("全屏提醒文本"));

    // ButtonGroup
    FloatingBarLevelButtonGroup->addButton(FloatingBarOnTopRadioButton);
    FloatingBarLevelButtonGroup->addButton(FloatingBarOnBottomRadioButton);
    FloatingBarPositionButtonGroup->addButton(FloatingBarPositionTopLeftRadioButton);
    FloatingBarPositionButtonGroup->addButton(FloatingBarPositionTopCenterRadioButton);
    FloatingBarPositionButtonGroup->addButton(FloatingBarPositionTopRightRadioButton);
    FloatingBarLevelButtonGroup->setExclusive(true);
    FloatingBarPositionButtonGroup->setExclusive(true);

    // DateTimeEdit
    TargetDateTimeEdit->setDateTime(Config.General.target_date_time);
    TargetDateTimeEdit->setCalendarPopup(true);
    TargetDateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss"); // 设置显示格式

    // CheckBox
    IsShowReminderCheckBox->setChecked(Config.Reminder.is_show_reminder);
    IsShowFloatingBarCheckBox->setChecked(Config.FloatingBar.is_show_floating_bar);

    // SpinBox
    ReminderRemainingDaysToPlayCountdownSoundSpinBox->setRange(INT_MIN, INT_MAX);
    ReminderRemainingDaysToPlayCountdownSoundSpinBox->setValue(Config.Reminder.remaining_days_to_play_countdown_sound);
    ReminderRemainingDaysToPlayCountdownSoundSpinBox->setPrefix(tr("剩余天数≤ ")); // 剩余天数播放倒计时音效前缀文本
    ReminderRemainingDaysToPlayCountdownSoundSpinBox->setSuffix(tr(" 天时播放倒计时提醒音")); // 剩余天数播放倒计时音效后缀文本
    ReminderRemainingDaysToPlayHeartbeatSoundSpinBox->setRange(INT_MIN, INT_MAX);
    ReminderRemainingDaysToPlayHeartbeatSoundSpinBox->setValue(Config.Reminder.remaining_days_to_play_heartbeat_sound);
    ReminderRemainingDaysToPlayHeartbeatSoundSpinBox->setPrefix(tr("剩余天数≤ ")); // 剩余天数播放心跳音效前缀文本
    ReminderRemainingDaysToPlayHeartbeatSoundSpinBox->setSuffix(tr(" 天时播放心跳提醒音")); // 剩余天数播放心跳音效后缀文本
    ReminderBlockShowTimesSpinBox->setRange(0, USHRT_MAX);
    ReminderBlockShowTimesSpinBox->setValue(Config.Reminder.block_show_times);
    ReminderBlockShowTimesSpinBox->setPrefix(tr("提醒音播放次数和方块闪烁次数：")); // 提醒音播放次数和方块闪烁次数前缀文本
    ReminderBlockShowTimesSpinBox->setSuffix(tr(" 次")); // 提醒音播放次数和方块闪烁次数后缀文本
    FloatingBarHeightSpinBox->setRange(0, 5714);
    FloatingBarHeightSpinBox->setValue(Config.FloatingBar.floating_bar_height);
    FloatingBarHeightSpinBox->setPrefix(tr("悬浮条高度：")); // 悬浮条高度前缀文本
    FloatingBarHeightSpinBox->setSuffix(tr(" 像素")); // 悬浮条高度后缀文本
    FloatingBarBorderRadiusSpinBox->setRange(0, Config.FloatingBar.floating_bar_height / 2);
    FloatingBarBorderRadiusSpinBox->setValue(Config.FloatingBar.floating_bar_border_radius);
    FloatingBarBorderRadiusSpinBox->setPrefix(tr("悬浮条圆角半径：")); // 悬浮条圆角半径前缀文本
    FloatingBarBorderRadiusSpinBox->setSuffix(tr(" 像素")); // 悬浮条圆角半径后缀文本

    // RadioButton
    FloatingBarOnTopRadioButton->setChecked(Config.FloatingBar.floating_bar_on_top);
    FloatingBarOnBottomRadioButton->setChecked(!Config.FloatingBar.floating_bar_on_top);
    FloatingBarPositionTopLeftRadioButton->setChecked(Config.FloatingBar.floating_bar_position == FloatingBarPosition::TopLeft);
    FloatingBarPositionTopCenterRadioButton->setChecked(Config.FloatingBar.floating_bar_position == FloatingBarPosition::TopCenter);
    FloatingBarPositionTopRightRadioButton->setChecked(Config.FloatingBar.floating_bar_position == FloatingBarPosition::TopRight);


    // Show
    for (QWidget* child : findChildren<QWidget*>())
        child->show();

    // Is show
    for (QWidget* child : FloatingBarSettingsGroupBox->findChildren<QWidget*>())
        if (child != IsShowFloatingBarCheckBox)
            child->setEnabled(Config.FloatingBar.is_show_floating_bar);

    for (QWidget* child : ReminderSettingsGroupBox->findChildren<QWidget*>())
        if (child != IsShowReminderCheckBox)
            child->setEnabled(Config.Reminder.is_show_reminder);

}

void SettingsPageManager::adjustObjects() {


    QFormLayout* GeneralSettingsFormLayout = new QFormLayout(GeneralSettingsGroupBox);
    GeneralSettingsFormLayout->addRow(tr("目标时间："), TargetDateTimeEdit);
    // todo)) update_interval...
    GeneralSettingsFormLayout->addRow(tr("语言："), LanguageComboBox);
    GeneralSettingsGroupBox->setLayout(GeneralSettingsFormLayout);

    QFormLayout* FloatingBarSettingsFormLayout = new QFormLayout(FloatingBarSettingsGroupBox);
    FloatingBarSettingsFormLayout->addRow(IsShowFloatingBarCheckBox);
    FloatingBarSettingsFormLayout->addRow(tr("悬浮条文本："), FloatingBarTextLineEdit);
    FloatingBarSettingsFormLayout->addRow(FloatingBarOnTopRadioButton, FloatingBarOnBottomRadioButton);
    FloatingBarSettingsFormLayout->addRow(FloatingBarPositionTopLeftRadioButton);
    FloatingBarSettingsFormLayout->addRow(FloatingBarPositionTopCenterRadioButton);
    FloatingBarSettingsFormLayout->addRow(FloatingBarPositionTopRightRadioButton);
    FloatingBarSettingsFormLayout->addRow(tr("悬浮条高度："), FloatingBarHeightSpinBox);
    FloatingBarSettingsFormLayout->addRow(tr("悬浮条圆角："), FloatingBarBorderRadiusSpinBox);
    FloatingBarSettingsGroupBox->setLayout(FloatingBarSettingsFormLayout);

    QFormLayout* ReminderSettingsFormLayout = new QFormLayout(ReminderSettingsGroupBox);
    ReminderSettingsFormLayout->addRow(IsShowReminderCheckBox);
    ReminderSettingsFormLayout->addRow(tr("全屏提醒标题："), ReminderTitleLineEdit);
    ReminderSettingsFormLayout->addRow(tr("全屏提醒文本："), ReminderTextLineEdit);
    ReminderSettingsFormLayout->addRow(ReminderRemainingDaysToPlayCountdownSoundSpinBox);
    ReminderSettingsFormLayout->addRow(ReminderRemainingDaysToPlayHeartbeatSoundSpinBox);
    ReminderSettingsFormLayout->addRow(ReminderBlockShowTimesSpinBox);
    ReminderSettingsFormLayout->addRow(ReminderPreviewButton);
    ReminderSettingsGroupBox->setLayout(ReminderSettingsFormLayout);

    QVBoxLayout* SettingsPageLayout = new QVBoxLayout(this);
    SettingsPageLayout->setContentsMargins(30, 10, 30, 10);
    SettingsPageLayout->setSpacing(15);
    SettingsPageLayout->setAlignment(Qt::AlignCenter);
    SettingsPageLayout->addWidget(GeneralSettingsGroupBox);
    SettingsPageLayout->addWidget(FloatingBarSettingsGroupBox);
    SettingsPageLayout->addWidget(ReminderSettingsGroupBox);
    SettingsPageLayout->addWidget(CloseButton);
    this->setLayout(SettingsPageLayout);

}

void SettingsPageManager::connectEmissions() {

    // General
    connect(TargetDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, [this](const QDateTime& date_time) {
        Config.set(Config.General.target_date_time, date_time);
        });
    // todo)) update_interval...

    // FloatingBar
    connect(IsShowFloatingBarCheckBox, &QCheckBox::checkStateChanged, this, [this] {
        Config.set(Config.FloatingBar.is_show_floating_bar, IsShowFloatingBarCheckBox->isChecked());
        if (!Config.FloatingBar.is_show_floating_bar) {
            FloatingBar->hide();
        }
        else FloatingBar->show();
        for (QWidget* child : FloatingBarSettingsGroupBox->findChildren<QWidget*>())
            if (child != IsShowFloatingBarCheckBox)
                child->setEnabled(Config.FloatingBar.is_show_floating_bar);
        });
    connect(FloatingBarTextLineEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        Config.set(Config.FloatingBar.floating_bar_text, text);
        });
    connect(FloatingBarLevelButtonGroup, &QButtonGroup::buttonClicked, this, [this] {
        Config.set(Config.FloatingBar.floating_bar_on_top, FloatingBarOnTopRadioButton->isChecked());
        FloatingBar->setWindowFlags((Config.FloatingBar.floating_bar_on_top ? Qt::WindowStaysOnTopHint : Qt::WindowStaysOnBottomHint) | Qt::FramelessWindowHint | Qt::Tool);
        FloatingBar->hide();
        FloatingBar->show();
#ifdef Q_OS_WIN
        if (Config.FloatingBar.floating_bar_on_top) {
            QMessageBox::information(FullScreenWidget, tr("提示"), tr("已设置悬浮条置顶，需要重新打开程序才可生效。"));
        }
#endif
        });
    connect(FloatingBarPositionButtonGroup, &QButtonGroup::buttonClicked, this, [this] {
        Config.set(Config.FloatingBar.floating_bar_position, FloatingBarPositionTopLeftRadioButton->isChecked() ? FloatingBarPosition::TopLeft : (FloatingBarPositionTopCenterRadioButton->isChecked() ? FloatingBarPosition::TopCenter : FloatingBarPosition::TopRight));
        });
    connect(FloatingBarHeightSpinBox, &QSpinBox::valueChanged, this, [this](int value) { // todo)) 解决修改悬浮条高度之后悬浮条圆角SpinBox上限不会修改的问题
        Config.set(Config.FloatingBar.floating_bar_height, value);
        FloatingBar->setFixedHeight(value);
        QFont font;
        font.setPixelSize(value * GOLDEN_RATIO_INV);
        FloatingBar->Bar->setFont(font);
        });
    connect(FloatingBarBorderRadiusSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        Config.set(Config.FloatingBar.floating_bar_border_radius, value);
        FloatingBar->Bar->setStyleSheet("background: rgba(255, 255, 255, 0.75); border-radius: " + QString::number(value) + "px; color: red;"); // 更新悬浮条样式
        });

    // Reminder
    connect(IsShowReminderCheckBox, &QCheckBox::checkStateChanged, this, [this] {
        Config.set(Config.Reminder.is_show_reminder, IsShowReminderCheckBox->isChecked());
        for (QWidget* child : ReminderSettingsGroupBox->findChildren<QWidget*>())
            if (child != IsShowReminderCheckBox)
                child->setEnabled(Config.Reminder.is_show_reminder);
        });
    connect(ReminderTitleLineEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        Config.set(Config.Reminder.reminder_text, text);
        });
    connect(ReminderTextLineEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        Config.set(Config.Reminder.reminder_small_text, text);
        });
    connect(ReminderRemainingDaysToPlayCountdownSoundSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        Config.set(Config.Reminder.remaining_days_to_play_countdown_sound, value);
        });
    connect(ReminderRemainingDaysToPlayHeartbeatSoundSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        Config.set(Config.Reminder.remaining_days_to_play_heartbeat_sound, value);
        });
    connect(ReminderBlockShowTimesSpinBox, &QSpinBox::valueChanged, this, [this](int value) {
        Config.set(Config.Reminder.block_show_times, value);
        });
    connect(ReminderPreviewButton, &QPushButton::clicked, this, [this] {
        emit clickedReminderPreviewButton();
        });

    // Other
    connect(CloseButton, &QPushButton::clicked, this, [this] {
        emit clickedCloseButton();
        });

}