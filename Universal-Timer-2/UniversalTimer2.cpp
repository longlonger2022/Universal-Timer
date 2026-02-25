#include "UniversalTimer2.h"
#include <QScreen>
#include <QMenu>
#include <QSettings>
#include <QFile>

UniversalTimer2::UniversalTimer2(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);


    desktop = QApplication::primaryScreen()->geometry(); // 获取屏幕分辨率

    currentDateTime = QDateTime::currentDateTime(); // 获取当前时间

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool); // 无边框窗口
    this->setAttribute(Qt::WA_TranslucentBackground); // 设置窗口背景透明


    // Translator
    Translator = new QTranslator(this);


    // Sound
    CountdownSound = new QSoundEffect(this);
    CountdownSound->setSource(QUrl::fromLocalFile("./sound/countdown.wav"));
    HeartbeatSound = new QSoundEffect(this);
    HeartbeatSound->setSource(QUrl::fromLocalFile("./sound/heartbeat.wav"));


    TrayIcon = new QSystemTrayIcon(QIcon(":/img/Universal-Timer-2_icon.512px.png"), this); // 系统托盘图标
    TrayIcon->setToolTip(tr("万能倒计时"));
    QMenu *TrayIconMenu = new QMenu(this); // 系统托盘菜单
    TrayIconMenu->addAction(tr("设置"), this, [&] {
        is_setting = true;
        BigWindow->show();
        FullScreenAnimationGroup->start();
        }); // 系统托盘菜单项：设置
    TrayIconMenu->addAction(tr("刷新"), this, &UniversalTimer2::readConfig); // 系统托盘菜单项：刷新
    TrayIconMenu->addAction(tr("退出"), this, &qApp->quit); // 系统托盘菜单项：退出
    TrayIcon->setContextMenu(TrayIconMenu);
    TrayIcon->show();


    // Widget
    BigWindow = new QWidget; // 全屏窗口
    BigWindow->setGeometry(desktop);
    BigWindow->setAttribute(Qt::WA_TranslucentBackground);
    BigWindow->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);


    // Label
    SmallWindowLabel = new QLabel(this); // 悬浮条标签
    SmallWindowLabel->setStyleSheet("font-family: Microsoft YaHei UI, Microsoft YaHei; background: rgba(255, 255, 255, 0.75); border-radius: " + QString::number(border_radius) + "px; color: red;");
    SmallWindowLabel->setAlignment(Qt::AlignCenter);
    SmallWindowLabel->show();
    BigWindowBackgroundLabel = new QLabel(BigWindow); // 全屏窗口背景标签
    BigWindowBackgroundLabel->resize(desktop.size());
    BigWindowBackgroundLabel->setStyleSheet("background: rgba(0, 0, 0, 0.75)");

    SettingsUnderlyingLabel = new QLabel(BigWindow); // 设置背景标签
    // 在Debug下编译时
#ifdef QT_DEBUG
    SettingsUnderlyingLabel->setStyleSheet("border: 1px solid red;");
#endif
    SettingsUnderlyingLabel->hide();

    ReminderUnderlyingLabel = new QLabel(BigWindow); // 提醒背景标签
    // 在Debug下编译时
#ifdef QT_DEBUG
    ReminderUnderlyingLabel->setStyleSheet("border: 1px solid red;");
#endif
    ReminderUnderlyingLabel->hide();
    ReminderTextLabel = new QLabel(BigWindow_text, ReminderUnderlyingLabel); // 全屏提醒自定义文本标签
    font.setPixelSize(desktop.height() * 0.2);
    ReminderTextLabel->adjustSize();
    ReminderTextLabel->setFont(font);
    ReminderTextLabel->setStyleSheet("font-family: DIN1451, zihun59hao-chuangcuhei, Microsoft YaHei UI, Microsoft YaHei; color: white;");
    ReminderTextLabel->show();
    ReminderRemainingLabel = new QLabel(tr("还剩"), ReminderUnderlyingLabel); // 全屏提醒剩余时间标签
    ReminderRemainingLabel->setFont(font);
    ReminderRemainingLabel->setStyleSheet("font-family: DIN1451, zihun59hao-chuangcuhei, Microsoft YaHei UI, Microsoft YaHei; color: white;");
    ReminderRemainingLabel->adjustSize();
    ReminderRemainingLabel->show();
    ReminderDaysLabel = new QLabel(tr("天"), ReminderUnderlyingLabel); // 全屏提醒剩余天数标签
    ReminderDaysLabel->setFont(font);
    ReminderDaysLabel->setStyleSheet("font-family: DIN1451, zihun59hao-chuangcuhei, Microsoft YaHei UI, Microsoft YaHei; color: white;");
    ReminderDaysLabel->adjustSize();
    ReminderDaysLabel->show();
    ReminderNumberLabel = new QLabel("0", ReminderUnderlyingLabel); // 全屏提醒剩余天数数字标签
    font.setPixelSize(desktop.height() * 0.5);
    ReminderNumberLabel->setFont(font);
    ReminderNumberLabel->setStyleSheet("font-family: DIN1451, zihun59hao-chuangcuhei, Microsoft YaHei UI, Microsoft YaHei; color: red;");
    ReminderNumberLabel->adjustSize();
    ReminderNumberLabel->show();
    ReminderSmallTextLabel = new QLabel(BigWindow_small_text, ReminderUnderlyingLabel); // 全屏提醒小文本标签
    font.setPixelSize(desktop.height() * 0.05);
    ReminderSmallTextLabel->setFont(font);
    ReminderSmallTextLabel->setStyleSheet("font-family: DIN1451, zihun59hao-chuangcuhei, Microsoft YaHei UI, Microsoft YaHei; color: white;");
    ReminderSmallTextLabel->adjustSize();
    ReminderSmallTextLabel->show();
    ReminderColorLabel = new QLabel(ReminderUnderlyingLabel); // 全屏提醒颜色标签
    ReminderColorLabel->setStyleSheet("background: red");
    ReminderColorLabel->resize(desktop.width() * 0.01, ReminderRemainingLabel->height());
    ReminderColorLabel->show();

    for (int i = 0; i < 4; i++) {
        ReminderBlockLabels.append(new QLabel(BigWindowBackgroundLabel)); // 全屏提醒块标签
        ReminderBlockLabels[i]->setStyleSheet("background: red");
        ReminderBlockLabels[i]->hide();
    }
    ReminderBlockLabels[0]->setGeometry(desktop.height() * 0.05, desktop.height() * 0.05, desktop.height() * 0.1, desktop.height() * 0.1); // 左上
    ReminderBlockLabels[1]->setGeometry(desktop.width() - desktop.height() * 0.15, desktop.height() * 0.05, desktop.height() * 0.1, desktop.height() * 0.1); // 右上
    ReminderBlockLabels[2]->setGeometry(desktop.height() * 0.05, desktop.height() * 0.85, desktop.height() * 0.1, desktop.height() * 0.1); // 左下
    ReminderBlockLabels[3]->setGeometry(desktop.width() - desktop.height() * 0.15, desktop.height() * 0.85, desktop.height() * 0.1, desktop.height() * 0.1); // 右下

    FullScreenAnimationLabel = new QLabel(BigWindow); // 红色全屏动画标签
    FullScreenAnimationLabel->resize(desktop.size());
    FullScreenAnimationLabel->setStyleSheet("background: red");




    // GroupBox
    SettingsTextAndDateGroupBox = new QGroupBox(tr("文本和时间"), SettingsUnderlyingLabel); // 设置文本和日期分组框
    SettingsTextAndDateGroupBox->resize(desktop.width() * 0.4, 0);
    font.setPixelSize(desktop.height() * 0.015);
    SettingsTextAndDateGroupBox->setFont(font);
    SettingsTextAndDateGroupBox->setStyleSheet("QGroupBox {border: 1px solid rgba(255, 0, 0, 0.5); color: red; margin-top: " + QString::number(font.pixelSize() / 2) + "px; border-radius: " + QString::number(font.pixelSize() / 2) + "px;} QGroupBox::title {subcontrol-origin: margin; subcontrol-position: top left;} QLabel{color: white}");
    SettingsTextAndDateGroupBox->show();
    SettingsReminderGroupBox = new QGroupBox(tr("全屏提醒"), SettingsUnderlyingLabel); // 设置全屏提醒分组框
    SettingsReminderGroupBox->setGeometry(SettingsTextAndDateGroupBox->width() + desktop.width() * 0.1, 0, desktop.width() * 0.4, 0);
    SettingsReminderGroupBox->setFont(font);
    SettingsReminderGroupBox->setStyleSheet("QGroupBox {border: 1px solid rgba(255, 0, 0, 0.5); color: red; margin-top: " + QString::number(font.pixelSize() / 2) + "px; border-radius: " + QString::number(font.pixelSize() / 2) + "px;} QGroupBox::title {subcontrol-origin: margin; subcontrol-position: top left;} QCheckBox {color: white;}");
    SettingsReminderGroupBox->show();
    SettingsSmallWindowGroupBox = new QGroupBox(tr("悬浮条设置"), SettingsUnderlyingLabel); // 设置悬浮条分组框
    SettingsSmallWindowGroupBox->setGeometry(0, SettingsTextAndDateGroupBox->height() + desktop.height() * 0.1, desktop.width() * 0.4, 0);
    SettingsSmallWindowGroupBox->setFont(font);
    SettingsSmallWindowGroupBox->setStyleSheet("QGroupBox {border: 1px solid rgba(255, 0, 0, 0.5); color: red; margin-top: " + QString::number(font.pixelSize() / 2) + "px; border-radius: " + QString::number(font.pixelSize() / 2) + "px;} QGroupBox::title {subcontrol-origin: margin; subcontrol-position: top left;} QRadioButton {color: white;} QCheckBox {color: white;}");
    SettingsSmallWindowGroupBox->show();

    
    // ButtonGroup
    SettingsSmallWindowLevelButtonGroup = new QButtonGroup(SettingsSmallWindowGroupBox); // 悬浮条位置按钮组
    SettingsSmallWindowLevelButtonGroup->setExclusive(true);
    SettingsSmallWindowPositionButtonGroup = new QButtonGroup(SettingsSmallWindowGroupBox); // 悬浮条位置按钮组
    SettingsSmallWindowPositionButtonGroup->setExclusive(true);
    

    // Label in GroupBox
    SettingsSmallWindowTextLabel = new QLabel(tr("悬浮条文本："), SettingsTextAndDateGroupBox); // 悬浮条文本标签
    font.setPixelSize(desktop.height() * 0.025);
    SettingsSmallWindowTextLabel->setFont(font);
    SettingsSmallWindowTextLabel->show();
    SettingsBigWindowTextLabel = new QLabel(tr("全屏提醒文本："), SettingsTextAndDateGroupBox); // 全屏窗口文本标签
    SettingsBigWindowTextLabel->setFont(font);
    SettingsBigWindowTextLabel->show();
    SettingsBigWindowSmallTextLabel = new QLabel(tr("全屏提醒小文本："), SettingsTextAndDateGroupBox); // 全屏窗口小文本标签
    SettingsBigWindowSmallTextLabel->setFont(font);
    SettingsBigWindowSmallTextLabel->show();
    SettingsTargetDateTimeLabel = new QLabel(tr("目标时间："), SettingsTextAndDateGroupBox); // 目标时间标签
    SettingsTargetDateTimeLabel->setFont(font);
    SettingsTargetDateTimeLabel->show();

    
    // PushButton in GroupBox
    SettingsReminderPreviewButton = new QPushButton(tr("预览"), SettingsReminderGroupBox); // 全屏提醒预览按钮
    SettingsReminderPreviewButton->show();
    SettingsCloseButton = new QPushButton(tr("关闭设置"), SettingsUnderlyingLabel); // 设置关闭按钮
    SettingsCloseButton->show();


    // LineEdit in GroupBox
    SettingsSmallWindowTextLineEdit = new QLineEdit(SmallWindow_text, SettingsTextAndDateGroupBox); // 悬浮条文本输入框
    SettingsSmallWindowTextLineEdit->show();
    SettingsBigWindowTextLineEdit = new QLineEdit(BigWindow_text, SettingsTextAndDateGroupBox); // 全屏窗口文本输入框
    SettingsBigWindowTextLineEdit->show();
    SettingsBigWindowSmallTextLineEdit = new QLineEdit(BigWindow_small_text, SettingsTextAndDateGroupBox); // 全屏窗口小文本输入框
    SettingsBigWindowSmallTextLineEdit->show();

    // DateTimeEdit in GroupBox
    SettingsTargetDateTimeEdit = new QDateTimeEdit(SettingsTextAndDateGroupBox); // 目标时间输入框
    SettingsTargetDateTimeEdit->setDateTime(targetDateTime);
    SettingsTargetDateTimeEdit->setCalendarPopup(true);
    // 精确到秒
    SettingsTargetDateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    SettingsTargetDateTimeEdit->show();

    // CheckBox in GroupBox
    SettingsIsShowBigWindowCheckBox = new QCheckBox(tr("播报全屏提醒"), SettingsReminderGroupBox); // 播报全屏提醒复选框
    SettingsIsShowBigWindowCheckBox->setChecked(is_show_BigWindow);
    SettingsIsShowBigWindowCheckBox->show();

    SettingsIsShowSmallWindowCheckBox = new QCheckBox(tr("显示悬浮条"), SettingsSmallWindowGroupBox); // 显示悬浮条复选框
    SettingsIsShowSmallWindowCheckBox->setChecked(is_show_SmallWindow);
    SettingsIsShowSmallWindowCheckBox->show();


    // ComboBox
    SettingsLanguageComboBox = new QComboBox(SettingsTextAndDateGroupBox); // 设置语言下拉框
    SettingsLanguageComboBox->show();

    // SpinBox in GroupBox
    SettingsRemainingDaysToPlayCountdownSoundSpinBox = new QSpinBox(SettingsReminderGroupBox); // 剩余天数播放倒计时声音
    SettingsRemainingDaysToPlayCountdownSoundSpinBox->setValue(remaining_days_to_play_countdown_sound);
    SettingsRemainingDaysToPlayCountdownSoundSpinBox->setPrefix(tr("剩余天数≤ "));
    SettingsRemainingDaysToPlayCountdownSoundSpinBox->setSuffix(tr(" 天时播放倒计时提醒音"));
    SettingsRemainingDaysToPlayCountdownSoundSpinBox->setRange(INT_MIN, INT_MAX);
    SettingsRemainingDaysToPlayCountdownSoundSpinBox->show();
    SettingsRemainingDaysToPlayHeartbeatSoundSpinBox = new QSpinBox(SettingsReminderGroupBox); // 剩余天数播放心跳声音
    SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->setValue(remaining_days_to_play_heartbeat_sound);
    SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->setPrefix(tr("剩余天数≤ "));
    SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->setSuffix(tr(" 天时播放心跳提醒音"));
    SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->setRange(INT_MIN, INT_MAX);
    SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->show();

    SettingsBlockShowTimesSpinBox = new QSpinBox(SettingsReminderGroupBox); // 方块闪烁次数
    SettingsBlockShowTimesSpinBox->setValue(block_show_times);
    SettingsBlockShowTimesSpinBox->setPrefix(tr("提醒音播放次数和方块闪烁次数："));
    SettingsBlockShowTimesSpinBox->setSuffix(tr(" 次"));
    SettingsBlockShowTimesSpinBox->setRange(0, USHRT_MAX);
    SettingsBlockShowTimesSpinBox->show();


    SettingsSmallWindowHeightSpinBox = new QSpinBox(SettingsSmallWindowGroupBox); // 悬浮条高度
    SettingsSmallWindowHeightSpinBox->setValue(SmallWindow_height);
    SettingsSmallWindowHeightSpinBox->setPrefix(tr("悬浮条高度："));
    SettingsSmallWindowHeightSpinBox->setSuffix(tr(" 像素"));
    SettingsSmallWindowHeightSpinBox->setRange(0, 5714);
    SettingsSmallWindowHeightSpinBox->show();
    SettingsSmallWindowBorderRadiusSpinBox = new QSpinBox(SettingsSmallWindowGroupBox); // 悬浮条圆角
    SettingsSmallWindowBorderRadiusSpinBox->setValue(border_radius);
    SettingsSmallWindowBorderRadiusSpinBox->setPrefix(tr("悬浮条圆角半径："));
    SettingsSmallWindowBorderRadiusSpinBox->setSuffix(tr(" 像素"));
    SettingsSmallWindowBorderRadiusSpinBox->setRange(0, SmallWindow_height / 2);
    SettingsSmallWindowBorderRadiusSpinBox->show();

    // RadioButton in GroupBox
    SettingsSmallWindowOnTopRadioButton = new QRadioButton(tr("悬浮条置顶"), SettingsSmallWindowGroupBox); // 悬浮条置顶单选按钮
    SettingsSmallWindowOnTopRadioButton->setChecked(SmallWindow_on_top);
    SettingsSmallWindowOnTopRadioButton->show();
    SettingsSmallWindowOnBottomRadioButton = new QRadioButton(tr("悬浮条置底"), SettingsSmallWindowGroupBox); // 悬浮条置底单选按钮
    SettingsSmallWindowOnBottomRadioButton->setChecked(!SmallWindow_on_top);
    SettingsSmallWindowOnBottomRadioButton->show();
    SettingsSmallWindowLevelButtonGroup->addButton(SettingsSmallWindowOnTopRadioButton);
    SettingsSmallWindowLevelButtonGroup->addButton(SettingsSmallWindowOnBottomRadioButton);

    SettingsSmallWindowPositionTopLeftRadioButton = new QRadioButton(tr("悬浮条位于左上"), SettingsSmallWindowGroupBox); // 悬浮条位置左上单选按钮
    SettingsSmallWindowPositionTopLeftRadioButton->setChecked(SmallWindow_position == 0);
    SettingsSmallWindowPositionTopLeftRadioButton->show();
    SettingsSmallWindowPositionTopCenterRadioButton = new QRadioButton(tr("悬浮条位于中上"), SettingsSmallWindowGroupBox); // 悬浮条位置中上单选按钮
    SettingsSmallWindowPositionTopCenterRadioButton->setChecked(SmallWindow_position == 1);
    SettingsSmallWindowPositionTopCenterRadioButton->show();
    SettingsSmallWindowPositionTopRightRadioButton = new QRadioButton(tr("悬浮条位于右上"), SettingsSmallWindowGroupBox); // 悬浮条位置右上
    SettingsSmallWindowPositionTopRightRadioButton->setChecked(SmallWindow_position == 2);
    SettingsSmallWindowPositionTopRightRadioButton->show();
    SettingsSmallWindowPositionButtonGroup->addButton(SettingsSmallWindowPositionTopLeftRadioButton);
    SettingsSmallWindowPositionButtonGroup->addButton(SettingsSmallWindowPositionTopCenterRadioButton);
    SettingsSmallWindowPositionButtonGroup->addButton(SettingsSmallWindowPositionTopRightRadioButton);


    // Animation
    FullScreenAnimation1 = new QPropertyAnimation(FullScreenAnimationLabel, "pos"); // 红色全屏动画前段
    FullScreenAnimation1->setDuration(250);
    FullScreenAnimation1->setStartValue(QPoint(-desktop.width(), 0));
    FullScreenAnimation1->setEndValue(desktop.topLeft());
    FullScreenAnimation2 = new QPropertyAnimation(FullScreenAnimationLabel, "pos"); // 红色全屏动画后段
    FullScreenAnimation2->setDuration(1000);
    FullScreenAnimation2->setStartValue(desktop.topLeft());
    FullScreenAnimation2->setEndValue(QPoint(desktop.width(), 0));
    FullScreenAnimation2->setEasingCurve(QEasingCurve::OutCubic);

    BigWindowHideAnimation = new QPropertyAnimation(BigWindow, "windowOpacity"); // 全屏窗口隐藏动画
    BigWindowHideAnimation->setDuration(1000);
    BigWindowHideAnimation->setStartValue(1);
    BigWindowHideAnimation->setEndValue(0);
    BigWindowHideAnimation->setEasingCurve(QEasingCurve::InCubic);


    // Animation Group
    FullScreenAnimationGroup = new QSequentialAnimationGroup; // 红色全屏动画组
    FullScreenAnimationGroup->addAnimation(FullScreenAnimation1);
    FullScreenAnimationGroup->addAnimation(FullScreenAnimation2);

    readConfig(); // 读取配置文件
    
    // connection
    Timer = new QTimer(this); // 定时器
    connect(Timer, &QTimer::timeout, this, &UniversalTimer2::updateObjects);
    Timer->start(update_interval);
    connect(FullScreenAnimation1, &QPropertyAnimation::finished, [&] {
        BigWindowBackgroundLabel->show();
        if (is_welcome) {
            ReminderUnderlyingLabel->hide();
            SettingsUnderlyingLabel->hide();
        }
        else if (!is_setting) {
            ReminderUnderlyingLabel->show();
            SettingsUnderlyingLabel->hide();
        }
        else {
            ReminderUnderlyingLabel->hide();
            SettingsUnderlyingLabel->show();
        }
        updateObjects();
        });
    connect(FullScreenAnimationGroup, &QSequentialAnimationGroup::finished, [&] {
        if (!is_welcome && !is_setting) {
            if ((timeLeft / 86400) <= remaining_days_to_play_countdown_sound) showBlocks();
            else QTimer::singleShot(3000, this, [&] {BigWindowHideAnimation->start(); });
        }
        });
    connect(BigWindowHideAnimation, &QPropertyAnimation::finished, [&] {
        BigWindow->hide();
        BigWindow->setWindowOpacity(1);
        ReminderUnderlyingLabel->hide();
        SettingsUnderlyingLabel->hide();
        });

    connect(SettingsSmallWindowTextLineEdit, &QLineEdit::textChanged, this, [&] {
        SmallWindow_text = SettingsSmallWindowTextLineEdit->text();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsBigWindowTextLineEdit, &QLineEdit::textChanged, this, [&] {
        BigWindow_text = SettingsBigWindowTextLineEdit->text();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsBigWindowSmallTextLineEdit, &QLineEdit::textChanged, this, [&] {
        BigWindow_small_text = SettingsBigWindowSmallTextLineEdit->text();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsTargetDateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, [&] {
        targetDateTime = SettingsTargetDateTimeEdit->dateTime();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsLanguageComboBox, &QComboBox::currentTextChanged, this, [&] {
        language = SettingsLanguageComboBox->currentText();
        changeLanguage();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsIsShowBigWindowCheckBox, &QCheckBox::checkStateChanged, this, [&] {
        is_show_BigWindow = SettingsIsShowBigWindowCheckBox->isChecked();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsRemainingDaysToPlayCountdownSoundSpinBox, &QSpinBox::valueChanged, this, [&] {
        remaining_days_to_play_countdown_sound = SettingsRemainingDaysToPlayCountdownSoundSpinBox->value();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsRemainingDaysToPlayHeartbeatSoundSpinBox, &QSpinBox::valueChanged, this, [&] {
        remaining_days_to_play_heartbeat_sound = SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->value();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsSmallWindowLevelButtonGroup, &QButtonGroup::buttonClicked, this, [&] {
        SmallWindow_on_top = SettingsSmallWindowOnTopRadioButton->isChecked();
        this->setWindowFlags((SmallWindow_on_top ? Qt::WindowStaysOnTopHint : Qt::WindowStaysOnBottomHint) | Qt::FramelessWindowHint | Qt::Tool);
        this->show();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsSmallWindowPositionButtonGroup, &QButtonGroup::buttonClicked, this, [&] {
        SmallWindow_position = SettingsSmallWindowPositionTopLeftRadioButton->isChecked() ? 0 : (SettingsSmallWindowPositionTopCenterRadioButton->isChecked() ? 1 : 2);
        writeConfig(); // 写入配置文件
        });
    connect(SettingsCloseButton, &QPushButton::clicked, this, [&] {
        is_setting = false;
        BigWindowHideAnimation->start();
        });
    connect(SettingsSmallWindowHeightSpinBox, &QSpinBox::valueChanged, this, [&] {
        SmallWindow_height = SettingsSmallWindowHeightSpinBox->value();
        SmallWindowLabel->setFixedHeight(SmallWindow_height);
        font.setPixelSize(SmallWindow_height * 0.618);
        SmallWindowLabel->setFont(font);
        updateObjects();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsSmallWindowBorderRadiusSpinBox, &QSpinBox::valueChanged, this, [&] {
        border_radius = SettingsSmallWindowBorderRadiusSpinBox->value();
        SmallWindowLabel->setStyleSheet("background: rgba(255, 255, 255, 0.75); border-radius: " + QString::number(border_radius) + "px; color: red;"); // 更新悬浮条样式
        writeConfig(); // 写入配置文件
        });
    connect(SettingsBlockShowTimesSpinBox, &QSpinBox::valueChanged, this, [&] {
        block_show_times = SettingsBlockShowTimesSpinBox->value();
        writeConfig(); // 写入配置文件
        });
    connect(SettingsIsShowBigWindowCheckBox, &QCheckBox::checkStateChanged, this, [&] {
        is_show_BigWindow = SettingsIsShowBigWindowCheckBox->isChecked();
        SettingsRemainingDaysToPlayCountdownSoundSpinBox->setEnabled(is_show_BigWindow);
        SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->setEnabled(is_show_BigWindow);
        SettingsReminderPreviewButton->setEnabled(is_show_BigWindow);
        writeConfig(); // 写入配置文件
        });
    connect(SettingsIsShowSmallWindowCheckBox, &QCheckBox::checkStateChanged, this, [&] {
        is_show_SmallWindow = SettingsIsShowSmallWindowCheckBox->isChecked();
        if (!is_show_SmallWindow) {
            this->hide();
        }
        else this->show();
        SettingsSmallWindowOnTopRadioButton->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowOnBottomRadioButton->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowPositionTopLeftRadioButton->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowPositionTopCenterRadioButton->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowPositionTopRightRadioButton->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowHeightSpinBox->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowBorderRadiusSpinBox->setEnabled(is_show_SmallWindow);
        writeConfig(); // 写入配置文件
        });
    connect(SettingsReminderPreviewButton, &QPushButton::clicked, this, [&] {
        SettingsUnderlyingLabel->hide();
        is_setting = false;
        showBigWindow();
        });
}

UniversalTimer2::~UniversalTimer2()
{
    delete BigWindow;
}

// 配置文件读取函数
void UniversalTimer2::readConfig() {
    // 若文件存在，则读取配置文件
    if (QFile::exists("config.ini")) {
        QSettings settings("config.ini", QSettings::IniFormat);

        is_show_BigWindow = settings.value("is_show_BigWindow").toBool(); // 是否显示全屏窗口
        SettingsIsShowBigWindowCheckBox->setChecked(is_show_BigWindow);
        SettingsRemainingDaysToPlayCountdownSoundSpinBox->setEnabled(is_show_BigWindow);
        SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->setEnabled(is_show_BigWindow);
        SettingsReminderPreviewButton->setEnabled(is_show_BigWindow);

        is_show_SmallWindow = settings.value("is_show_SmallWindow").toBool(); // 是否显示悬浮条
        if (!is_show_SmallWindow) {
            this->hide();
            SmallWindowLabel->hide();
        }
        SettingsIsShowSmallWindowCheckBox->setChecked(is_show_SmallWindow);
        SettingsSmallWindowOnTopRadioButton->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowOnBottomRadioButton->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowPositionTopLeftRadioButton->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowPositionTopCenterRadioButton->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowPositionTopRightRadioButton->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowHeightSpinBox->setEnabled(is_show_SmallWindow);
        SettingsSmallWindowBorderRadiusSpinBox->setEnabled(is_show_SmallWindow);

        SmallWindow_on_top = settings.value("SmallWindow_on_top").toBool(); // 悬浮条是否置顶
        this->setWindowFlags((SmallWindow_on_top ? Qt::WindowStaysOnTopHint : Qt::WindowStaysOnBottomHint) | Qt::FramelessWindowHint | Qt::Tool);
        SettingsSmallWindowOnTopRadioButton->setChecked(SmallWindow_on_top);
        SettingsSmallWindowOnBottomRadioButton->setChecked(!SmallWindow_on_top);

        border_radius = settings.value("border_radius").toInt(); // 窗口圆角
        SmallWindowLabel->setStyleSheet("background: rgba(255, 255, 255, 0.75); border-radius: " + QString::number(border_radius) + "px; color: red;"); // 更新悬浮条样式
        SettingsSmallWindowBorderRadiusSpinBox->setValue(border_radius);

        block_show_times = settings.value("block_show_times").toInt(); // 倒计时显示次数
        SettingsBlockShowTimesSpinBox->setValue(block_show_times);

        targetDateTime = QDateTime::fromString(settings.value("targetDateTime").toString(), "yyyy-MM-dd hh:mm:ss"); // 目标时间
        if (!targetDateTime.isValid()) {
            targetDateTime = QDateTime::currentDateTime().addDays(1); // 默认设置为明天
            writeLog("WARNING", "目标时间无效，已设置为默认值：明天");
        }
        SettingsTargetDateTimeEdit->setDateTime(targetDateTime); // 更新目标时间输入框

        SmallWindow_text = settings.value("SmallWindow_text").toString(); // 悬浮条文本
        SettingsSmallWindowTextLineEdit->setText(SmallWindow_text);

        SmallWindow_position = settings.value("SmallWindow_position").toInt(); // 悬浮条位置
        SettingsSmallWindowPositionTopLeftRadioButton->setChecked(SmallWindow_position == 0);
        SettingsSmallWindowPositionTopCenterRadioButton->setChecked(SmallWindow_position == 1);
        SettingsSmallWindowPositionTopRightRadioButton->setChecked(SmallWindow_position == 2);

        SmallWindow_height = settings.value("SmallWindow_height").toInt(); // 悬浮条高度
        font.setPixelSize(SmallWindow_height * 0.618);
        SmallWindowLabel->setFont(font);
        SettingsSmallWindowHeightSpinBox->setValue(SmallWindow_height);
        SettingsSmallWindowBorderRadiusSpinBox->setRange(0, SmallWindow_height / 2);

        BigWindow_text = settings.value("BigWindow_text").toString(); // 全屏窗口文本
        ReminderTextLabel->setText(BigWindow_text);
        ReminderTextLabel->adjustSize();
        SettingsBigWindowTextLineEdit->setText(BigWindow_text);

        BigWindow_small_text = settings.value("BigWindow_small_text").toString(); // 全屏窗口小文本
        SettingsBigWindowSmallTextLineEdit->setText(BigWindow_small_text);

        remaining_days_to_play_countdown_sound = settings.value("remaining_days_to_play_countdown_sound").toInt(); // 倒计时声音剩余天数
        SettingsRemainingDaysToPlayCountdownSoundSpinBox->setValue(remaining_days_to_play_countdown_sound);

        remaining_days_to_play_heartbeat_sound = settings.value("remaining_days_to_play_heartbeat_sound").toInt(); // 心跳声音剩余天数
        SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->setValue(remaining_days_to_play_heartbeat_sound);

        update_interval = settings.value("update_interval").toInt(); // 更新间隔时间

        LanguageCodeList = settings.value("LanguageCodeList").toStringList(); // 语言代码列表
        LanguageNameList = settings.value("LanguageNameList").toStringList(); // 语言名称列表
        language = settings.value("language").toString(); // 当前语言
        scanLanguage(); // 扫描语言
        SettingsLanguageComboBox->setCurrentText(LanguageNameList.at(LanguageCodeList.indexOf(language)));
        changeLanguage();

        // 错误处理
        if (settings.status() != QSettings::NoError) {
            QMessageBox::critical(NULL, tr("错误"), tr("配置文件读取失败！") + "\nError Code: " + QString::number(settings.status()), QMessageBox::Ok);
            writeLog("ERROR", "配置文件读取失败，错误代码：" + QString::number(settings.status()));
        }
        else {
            writeLog("INFO", "配置文件读取成功！");
        }
        readTimeList(); // 读取时间列表
    }
    // 否则，为第一次使用，显示欢迎界面，并创建配置文件
    else {
        // 获取系统语言
        if (QLocale::system().language() == QLocale::Chinese) language = "zh-CN";
        else language = "en";
        scanLanguage();
        SettingsLanguageComboBox->setCurrentText(LanguageNameList.at(LanguageCodeList.indexOf(language)));
        changeLanguage();
        is_welcome = true;
        WelcomeLabel = new QLabel(tr("欢迎使用万能倒计时!<br>简单设置，并开始使用吧!"), BigWindowBackgroundLabel);
        WelcomeLabel->setAlignment(Qt::AlignCenter);
        WelcomeLabel->resize(desktop.size());
        WelcomeLabel->setStyleSheet("color: white; font-size: 40px; font-weight: bold; background: transparent;");
        BigWindowBackgroundLabel->setPixmap(QPixmap(":/img/background/Universal-Timer.png"));
        BigWindowBackgroundLabel->setScaledContents(true);
        WelcomeLabel->show();
        WelcomeButton = new QPushButton(tr("开始"), WelcomeLabel);
        WelcomeButton->setGeometry(desktop.width() * 0.45, desktop.height() * 0.6, desktop.width() * 0.1, desktop.height() * 0.1);
        WelcomeButton->setStyleSheet("color: white; font-size: 20px; font-weight: bold; background: rgba(0, 0, 0, 0.5); border-radius: 10px;");
        WelcomeButton->show();
        connect(WelcomeButton, &QPushButton::clicked, [&] {
            is_welcome = false;
            BigWindowHideAnimation->start();
            WelcomeLabel->deleteLater();
            WelcomeButton->deleteLater();
            // 删除背景图片
            BigWindowBackgroundLabel->setPixmap(QPixmap());
            });
        // 创建配置文件
        writeConfig();
    }
    if (is_show_BigWindow) {
        showBigWindow();
    }
}

// 配置文件写入函数
void UniversalTimer2::writeConfig() {
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.setValue("is_show_BigWindow", is_show_BigWindow);
    settings.setValue("is_show_SmallWindow", is_show_SmallWindow);
    settings.setValue("SmallWindow_on_top", SmallWindow_on_top);
    settings.setValue("border_radius", border_radius);
    settings.setValue("targetDateTime", targetDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    settings.setValue("SmallWindow_text", SmallWindow_text);
    settings.setValue("SmallWindow_position", SmallWindow_position);
    settings.setValue("SmallWindow_height", SmallWindow_height);
    settings.setValue("BigWindow_text", BigWindow_text);
    settings.setValue("BigWindow_small_text", BigWindow_small_text);
    settings.setValue("remaining_days_to_play_countdown_sound", remaining_days_to_play_countdown_sound);
    settings.setValue("remaining_days_to_play_heartbeat_sound", remaining_days_to_play_heartbeat_sound);
    settings.setValue("block_show_times", block_show_times);
    settings.setValue("update_interval", update_interval);
    settings.setValue("LanguageCodeList", LanguageCodeList);
    settings.setValue("LanguageNameList", LanguageNameList);
    settings.setValue("language", language);
    settings.sync();
    if (settings.status() != QSettings::NoError) {
        QMessageBox::critical(NULL, tr("错误"), tr("配置文件写入失败！") + "\nError Code: " + QString::number(settings.status()), QMessageBox::Ok);
        writeLog("ERROR", "配置文件写入失败，错误代码：" + QString::number(settings.status()));
    }
    else {
        writeLog("INFO", "配置文件写入成功！");
    }
    writeTimeList(); // 写入时间列表
}

// 时间列表读取函数
void UniversalTimer2::readTimeList() {
    // 若文件存在，则读取时间列表
    if (QFile::exists("timelist")) {
        QFile file("timelist");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            timeList.clear(); // 清空时间列表
            while (!in.atEnd()) {
                QString line = in.readLine();
                timeList.append(QTime::fromString(line, "hh:mm:ss"));
            }
            writeLog("INFO", "时间列表读取成功！");
            file.close();
        }
        // 出错时
        else {
            QMessageBox::critical(NULL, tr("错误"), tr("时间列表读取失败！") + "\nError Code: " + QString::number(file.error()), QMessageBox::Ok);
            writeLog("ERROR", "时间列表读取失败，错误代码：" + QString::number(file.error()));
        }
    }
    // 否则，为第一次使用，创建时间列表
    else {
        writeTimeList();
    }
}

// 时间列表写入函数
void UniversalTimer2::writeTimeList() {
    QFile file("timelist");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int i = 0; i < timeList.size(); i++) {
            out << timeList[i].toString("hh:mm:ss") << "\n";
        }
    }
}


// 语言扫描函数
void UniversalTimer2::scanLanguage() {
    for (int i = 0; i < LanguageCodeList.size(); i++) {
        if (QFile::exists("./lang/" + LanguageCodeList[i] + ".qm")) {
            SettingsLanguageComboBox->addItem(LanguageNameList[i]);
        }
        if (LanguageCodeList[i] == "za") {
            SettingsLanguageComboBox->addItem("简体中文（中国大陆）");
        }
    }
}

// 语言切换函数
void UniversalTimer2::changeLanguage() {
    language = LanguageCodeList.at(LanguageNameList.indexOf(SettingsLanguageComboBox->currentText()));
    if (language != "zh-CN") {
        Translator->load("./lang/" + language + ".qm");
        qApp->installTranslator(Translator);
    }
    else {
        // 卸载
        qApp->removeTranslator(Translator);
    }
    updateText();
}


// 日志写入函数
void UniversalTimer2::writeLog(QString type, QString log) {
    QFile file("Universal-Timer.log");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << currentDateTime.toString("yyyy-MM-dd hh:mm:ss") << " - [" << type << "] - " << log << "\n";
    }
}

// 动画函数
void UniversalTimer2::showBigWindow() {
    currentDateTime = QDateTime::currentDateTime();
    timeLeft = currentDateTime.secsTo(targetDateTime);
    // 更新标签文本
    ReminderNumberLabel->setText(QString::number(timeLeft / 86400));
    ReminderSmallTextLabel->setText(BigWindow_small_text + QString::number(timeLeft / 86400) + " DAYS");
    adjustReminderSize(1.0);
    ReminderUnderlyingLabel->move((desktop.width() - ReminderUnderlyingLabel->width()) / 2, (desktop.height() - ReminderUnderlyingLabel->height()) / 2);


    ReminderUnderlyingLabel->hide();
    BigWindowBackgroundLabel->hide();
    FullScreenAnimationLabel->show();
    BigWindow->show();
    FullScreenAnimationGroup->start();
}

// 全屏提醒大小调整函数
void UniversalTimer2::adjustReminderSize(const qreal scale) {
    font.setPixelSize(desktop.height() * 0.2 * scale);
    ReminderTextLabel->setFont(font);
    ReminderRemainingLabel->setFont(font);
    ReminderDaysLabel->setFont(font);
    font.setPixelSize(desktop.height() * 0.05 * scale);
    ReminderSmallTextLabel->setFont(font);
    font.setPixelSize(desktop.height() * 0.5 * scale);
    ReminderNumberLabel->setFont(font);
    ReminderTextLabel->adjustSize();
    ReminderRemainingLabel->adjustSize();
    ReminderDaysLabel->adjustSize();
    ReminderSmallTextLabel->adjustSize();
    ReminderNumberLabel->adjustSize();
    ReminderTextLabel->move(0, 0);
    ReminderRemainingLabel->move(ReminderTextLabel->width() - ReminderRemainingLabel->width(), desktop.height() * 0.2);
    ReminderNumberLabel->move(ReminderTextLabel->width(), ReminderRemainingLabel->y() + ReminderRemainingLabel->height() - ReminderNumberLabel->height() * 4 / 5);
    ReminderDaysLabel->move(ReminderNumberLabel->x() + ReminderNumberLabel->width(), ReminderRemainingLabel->y());
    ReminderSmallTextLabel->move(ReminderRemainingLabel->x() + desktop.height() * 0.05, ReminderRemainingLabel->y() + ReminderRemainingLabel->height());
    ReminderColorLabel->move(ReminderRemainingLabel->x() - ReminderColorLabel->width(), ReminderRemainingLabel->y() + ReminderSmallTextLabel->height());
    ReminderUnderlyingLabel->resize((ReminderDaysLabel->x() + ReminderDaysLabel->width()) > (ReminderSmallTextLabel->x() + ReminderSmallTextLabel->width()) ? (ReminderDaysLabel->x() + ReminderDaysLabel->width()) : (ReminderSmallTextLabel->x() + ReminderSmallTextLabel->width()), ReminderSmallTextLabel->y() + ReminderSmallTextLabel->height());
    // 当超过屏幕范围时
    if (ReminderUnderlyingLabel->width() > desktop.width()) {
        adjustReminderSize(scale - 0.1);
    }
}

void UniversalTimer2::showBlocks(unsigned short times) {
    if ((timeLeft / 86400) <= remaining_days_to_play_heartbeat_sound) {
        CountdownSound->play();
        if(times % 2) HeartbeatSound->play();
    }
    else if ((timeLeft / 86400) <= remaining_days_to_play_countdown_sound) CountdownSound->play();
    for (int i = 0; i < ReminderBlockLabels.size(); i++) {
        ReminderBlockLabels[i]->show();
    }
    QTimer::singleShot(500, this, [=] {
        for (int i = 0; i < ReminderBlockLabels.size(); i++) {
            ReminderBlockLabels[i]->hide();
        }
        QTimer::singleShot(500, this, [=] {
            if (times < block_show_times) {
                showBlocks(times + 1);
            }
            else BigWindowHideAnimation->start();
            });
        });
}

// 文本更新函数
void UniversalTimer2::updateText() {
    TrayIcon->setToolTip(tr("万能倒计时"));
    TrayIcon->contextMenu()->actions()[0]->setText(tr("设置"));
    TrayIcon->contextMenu()->actions()[1]->setText(tr("刷新"));
    TrayIcon->contextMenu()->actions()[2]->setText(tr("退出"));
    ReminderRemainingLabel->setText(tr("还剩")); // 全屏提醒剩余时间标签
    ReminderDaysLabel->setText(tr("天")); // 全屏提醒剩余天数标签
    SettingsTextAndDateGroupBox->setTitle(tr("文本和时间")); // 设置文本和日期分组框
    SettingsReminderGroupBox->setTitle(tr("全屏提醒")); // 设置全屏提醒分组框
    SettingsSmallWindowGroupBox->setTitle(tr("悬浮条设置")); // 设置悬浮条分组框
    SettingsSmallWindowTextLabel->setText(tr("悬浮条文本：")); // 悬浮条文本标签
    SettingsBigWindowTextLabel->setText(tr("全屏提醒文本：")); // 全屏窗口文本标签
    SettingsBigWindowSmallTextLabel->setText(tr("全屏提醒小文本：")); // 全屏窗口小文本标签
    SettingsTargetDateTimeLabel->setText(tr("目标时间：")); // 目标时间标签
    SettingsReminderPreviewButton->setText(tr("预览")); // 全屏提醒预览按钮
    SettingsCloseButton->setText(tr("关闭设置")); // 设置关闭按钮
    SettingsIsShowBigWindowCheckBox->setText(tr("播报全屏提醒")); // 播报全屏提醒复选框
    SettingsIsShowSmallWindowCheckBox->setText(tr("显示悬浮条")); // 显示悬浮条复选框
    SettingsRemainingDaysToPlayCountdownSoundSpinBox->setPrefix(tr("剩余天数≤ "));
    SettingsRemainingDaysToPlayCountdownSoundSpinBox->setSuffix(tr(" 天时播放倒计时提醒音"));
    SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->setPrefix(tr("剩余天数≤ "));
    SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->setSuffix(tr(" 天时播放心跳提醒音"));
    SettingsBlockShowTimesSpinBox->setPrefix(tr("提醒音播放次数和方块闪烁次数："));
    SettingsBlockShowTimesSpinBox->setSuffix(tr(" 次"));
    SettingsSmallWindowHeightSpinBox->setPrefix(tr("悬浮条高度："));
    SettingsSmallWindowHeightSpinBox->setSuffix(tr(" 像素"));
    SettingsSmallWindowBorderRadiusSpinBox->setPrefix(tr("悬浮条圆角半径："));
    SettingsSmallWindowBorderRadiusSpinBox->setSuffix(tr(" 像素"));
    SettingsSmallWindowOnTopRadioButton->setText(tr("悬浮条置顶")); // 悬浮条置顶单选按钮
    SettingsSmallWindowOnBottomRadioButton->setText(tr("悬浮条置底")); // 悬浮条置底单选按钮
    SettingsSmallWindowPositionTopLeftRadioButton->setText(tr("悬浮条位于左上")); // 悬浮条位置左上单选按钮
    SettingsSmallWindowPositionTopCenterRadioButton->setText(tr("悬浮条位于中上")); // 悬浮条位置中上单选按钮
    SettingsSmallWindowPositionTopRightRadioButton->setText(tr("悬浮条位于右上")); // 悬浮条位置右上单选按钮
}

// 更新控件
void UniversalTimer2::updateObjects() {
    currentDateTime = QDateTime::currentDateTime();
    timeLeft = currentDateTime.secsTo(targetDateTime);

    
    if (is_show_SmallWindow) {
        // 更新标签文本
        SmallWindowLabel->setText(" " + SmallWindow_text + QString::number(timeLeft / 86400) + tr("天") + QString::number((timeLeft % 86400) / 3600) + tr("时") + QString::number((timeLeft % 3600) / 60) + tr("分") + QString::number(timeLeft % 60) + tr("秒 "));
        // 更新大小
        SmallWindowLabel->adjustSize();
        SmallWindowLabel->resize(SmallWindowLabel->width() + 20, SmallWindow_height);
        this->adjustSize();
        if (SmallWindow_position == 1) { // 中上
            this->move((desktop.width() - this->width()) / 2, 0);
        }
        else if (SmallWindow_position == 2) { // 右上
            this->move(desktop.width() - this->width(), 0);
        }
        else {
            this->move(0, 0);
        }
    }
    if (ReminderUnderlyingLabel->isVisible()) {
        // 更新标签文本
        ReminderNumberLabel->setText(QString::number(timeLeft / 86400));
        ReminderSmallTextLabel->setText(BigWindow_small_text + QString::number(timeLeft / 86400) + " DAYS");
        // 更新大小
        adjustReminderSize(1.0);
        ReminderUnderlyingLabel->move((desktop.width() - ReminderUnderlyingLabel->width()) / 2, (desktop.height() - ReminderUnderlyingLabel->height()) / 2);
    }
    else if (SettingsUnderlyingLabel->isVisible()) {
        // 更新位置大小
        SettingsSmallWindowTextLabel->adjustSize();
        SettingsSmallWindowTextLabel->move(0, desktop.height() * 0.02);
        SettingsSmallWindowTextLineEdit->setGeometry(SettingsSmallWindowTextLabel->x() + SettingsSmallWindowTextLabel->width(), SettingsSmallWindowTextLabel->y(), SettingsTextAndDateGroupBox->width() - SettingsSmallWindowTextLabel->width(), SettingsSmallWindowTextLabel->height());
        SettingsBigWindowTextLabel->adjustSize();
        SettingsBigWindowTextLabel->move(SettingsSmallWindowTextLabel->x(), SettingsSmallWindowTextLabel->y() + SettingsSmallWindowTextLabel->height());
        SettingsBigWindowTextLineEdit->setGeometry(SettingsBigWindowTextLabel->x() + SettingsBigWindowTextLabel->width(), SettingsBigWindowTextLabel->y(), SettingsTextAndDateGroupBox->width() - SettingsBigWindowTextLabel->width(), SettingsBigWindowTextLabel->height());
        SettingsBigWindowSmallTextLabel->adjustSize();
        SettingsBigWindowSmallTextLabel->move(SettingsBigWindowTextLabel->x(), SettingsBigWindowTextLineEdit->y() + SettingsBigWindowTextLineEdit->height());
        SettingsBigWindowSmallTextLineEdit->setGeometry(SettingsBigWindowSmallTextLabel->x() + SettingsBigWindowSmallTextLabel->width(), SettingsBigWindowSmallTextLabel->y(), SettingsTextAndDateGroupBox->width() - SettingsBigWindowSmallTextLabel->width(), SettingsBigWindowSmallTextLabel->height());
        SettingsTargetDateTimeLabel->adjustSize();
        SettingsTargetDateTimeLabel->move(SettingsBigWindowSmallTextLabel->x(), SettingsBigWindowSmallTextLineEdit->y() + SettingsBigWindowSmallTextLineEdit->height());
        SettingsTargetDateTimeEdit->setGeometry(SettingsTargetDateTimeLabel->x() + SettingsTargetDateTimeLabel->width(), SettingsTargetDateTimeLabel->y(), SettingsTextAndDateGroupBox->width() - SettingsTargetDateTimeLabel->width(), SettingsTargetDateTimeLabel->height());
        SettingsLanguageComboBox->adjustSize();
        SettingsLanguageComboBox->setGeometry(SettingsTargetDateTimeLabel->x(), SettingsTargetDateTimeLabel->y() + SettingsTargetDateTimeLabel->height(), SettingsTextAndDateGroupBox->width(), SettingsLanguageComboBox->height());
        SettingsTextAndDateGroupBox->resize(SettingsTextAndDateGroupBox->width(), SettingsLanguageComboBox->y() + SettingsLanguageComboBox->height());

        SettingsIsShowBigWindowCheckBox->adjustSize();
        SettingsIsShowBigWindowCheckBox->setGeometry(0, desktop.height() * 0.02, SettingsReminderGroupBox->width(), SettingsIsShowBigWindowCheckBox->height());
        SettingsRemainingDaysToPlayCountdownSoundSpinBox->adjustSize();
        SettingsRemainingDaysToPlayCountdownSoundSpinBox->setGeometry(SettingsIsShowBigWindowCheckBox->x(), SettingsIsShowBigWindowCheckBox->y() + SettingsIsShowBigWindowCheckBox->height(), SettingsReminderGroupBox->width(), SettingsRemainingDaysToPlayCountdownSoundSpinBox->height());
        SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->adjustSize();
        SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->setGeometry(SettingsRemainingDaysToPlayCountdownSoundSpinBox->x(), SettingsRemainingDaysToPlayCountdownSoundSpinBox->y() + SettingsRemainingDaysToPlayCountdownSoundSpinBox->height(), SettingsReminderGroupBox->width(), SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->height());
        SettingsBlockShowTimesSpinBox->adjustSize();
        SettingsBlockShowTimesSpinBox->setGeometry(SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->x(), SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->y() + SettingsRemainingDaysToPlayHeartbeatSoundSpinBox->height(), SettingsReminderGroupBox->width(), SettingsBlockShowTimesSpinBox->height());
        SettingsReminderPreviewButton->adjustSize();
        SettingsReminderPreviewButton->setGeometry(SettingsBlockShowTimesSpinBox->x(), SettingsBlockShowTimesSpinBox->y() + SettingsBlockShowTimesSpinBox->height(), SettingsReminderGroupBox->width(), SettingsReminderPreviewButton->height());
        SettingsReminderGroupBox->resize(SettingsReminderGroupBox->width(), SettingsReminderPreviewButton->y() + SettingsReminderPreviewButton->height());
        SettingsReminderGroupBox->move(SettingsTextAndDateGroupBox->x() + SettingsTextAndDateGroupBox->width() + desktop.width() * 0.1, SettingsTextAndDateGroupBox->y());

        SettingsIsShowSmallWindowCheckBox->adjustSize();
        SettingsIsShowSmallWindowCheckBox->setGeometry(0, desktop.height() * 0.02, SettingsSmallWindowGroupBox->width(), SettingsIsShowSmallWindowCheckBox->height());
        SettingsSmallWindowOnTopRadioButton->adjustSize();
        SettingsSmallWindowOnTopRadioButton->setGeometry(SettingsIsShowSmallWindowCheckBox->x(), SettingsIsShowSmallWindowCheckBox->y() + SettingsIsShowSmallWindowCheckBox->height(), SettingsSmallWindowGroupBox->width() / 2, SettingsSmallWindowOnTopRadioButton->height());
        SettingsSmallWindowOnBottomRadioButton->adjustSize();
        SettingsSmallWindowOnBottomRadioButton->setGeometry(SettingsSmallWindowGroupBox->width() / 2, SettingsSmallWindowOnTopRadioButton->y(), SettingsSmallWindowGroupBox->width() / 2, SettingsSmallWindowOnBottomRadioButton->height());
        SettingsSmallWindowPositionTopLeftRadioButton->adjustSize();
        SettingsSmallWindowPositionTopLeftRadioButton->setGeometry(SettingsSmallWindowOnTopRadioButton->x(), SettingsSmallWindowOnTopRadioButton->y() + SettingsSmallWindowOnTopRadioButton->height(), SettingsSmallWindowGroupBox->width(), SettingsSmallWindowPositionTopLeftRadioButton->height());
        SettingsSmallWindowPositionTopCenterRadioButton->adjustSize();
        SettingsSmallWindowPositionTopCenterRadioButton->setGeometry(SettingsSmallWindowPositionTopLeftRadioButton->x(), SettingsSmallWindowPositionTopLeftRadioButton->y() + SettingsSmallWindowPositionTopLeftRadioButton->height(), SettingsSmallWindowGroupBox->width(), SettingsSmallWindowPositionTopCenterRadioButton->height());
        SettingsSmallWindowPositionTopRightRadioButton->adjustSize();
        SettingsSmallWindowPositionTopRightRadioButton->setGeometry(SettingsSmallWindowPositionTopCenterRadioButton->x(), SettingsSmallWindowPositionTopCenterRadioButton->y() + SettingsSmallWindowPositionTopCenterRadioButton->height(), SettingsSmallWindowGroupBox->width(), SettingsSmallWindowPositionTopRightRadioButton->height());
        SettingsSmallWindowHeightSpinBox->adjustSize();
        SettingsSmallWindowHeightSpinBox->setGeometry(SettingsSmallWindowPositionTopRightRadioButton->x(), SettingsSmallWindowPositionTopRightRadioButton->y() + SettingsSmallWindowPositionTopRightRadioButton->height(), SettingsSmallWindowGroupBox->width(), SettingsSmallWindowHeightSpinBox->height());
        SettingsSmallWindowBorderRadiusSpinBox->adjustSize();
        SettingsSmallWindowBorderRadiusSpinBox->setGeometry(SettingsSmallWindowHeightSpinBox->x(), SettingsSmallWindowHeightSpinBox->y() + SettingsSmallWindowHeightSpinBox->height(), SettingsSmallWindowGroupBox->width(), SettingsSmallWindowBorderRadiusSpinBox->height());
        SettingsSmallWindowGroupBox->resize(SettingsSmallWindowGroupBox->width(), SettingsSmallWindowBorderRadiusSpinBox->y() + SettingsSmallWindowBorderRadiusSpinBox->height());
        SettingsSmallWindowGroupBox->move(SettingsTextAndDateGroupBox->x(), SettingsTextAndDateGroupBox->y() + SettingsTextAndDateGroupBox->height() + desktop.height() * 0.1);

        SettingsCloseButton->adjustSize();
        SettingsCloseButton->move(SettingsReminderGroupBox->x() + (SettingsReminderGroupBox->width() - SettingsCloseButton->width()) / 2, SettingsSmallWindowGroupBox->y() + (SettingsSmallWindowGroupBox->height() - SettingsCloseButton->height()) / 2);

        SettingsUnderlyingLabel->resize(SettingsReminderGroupBox->x() + SettingsReminderGroupBox->width(), SettingsSmallWindowGroupBox->y() + SettingsSmallWindowGroupBox->height());
        SettingsUnderlyingLabel->move((desktop.width() - SettingsUnderlyingLabel->width()) / 2, (desktop.height() - SettingsUnderlyingLabel->height()) / 2);
    }
    // 定时显示全屏提醒
    if (is_show_BigWindow && (!BigWindow->isVisible())) {
        if (timeList.contains(QTime::fromString(currentDateTime.toString("hh:mm:ss")))) {
            showBigWindow();
        }
    }
}