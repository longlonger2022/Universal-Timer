#include "UniversalTimer2.h"
#include <QScreen>
#include <QMenu>
#include <QTranslator>
#include <QSettings>
#include <QFile>

UniversalTimer2::UniversalTimer2(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    /*QTranslator *translator = new QTranslator(this);
    translator->load("Translation_en.qm");
    qApp->installTranslator(translator);*/

    desktop = QApplication::primaryScreen()->geometry(); // 获取屏幕分辨率

    currentDateTime = QDateTime::currentDateTime(); // 获取当前时间

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool); // 无边框窗口
    this->setAttribute(Qt::WA_TranslucentBackground); // 设置窗口背景透明


    TrayIcon = new QSystemTrayIcon(QIcon(":/img/Universal-Timer-2_icon.512px.png"), this); // 系统托盘图标
    TrayIcon->setToolTip(tr("万能倒计时"));
    QMenu *TrayIconMenu = new QMenu(this); // 系统托盘菜单
    TrayIconMenu->addAction(tr("设置"), this, SLOT(showNormal())); // 系统托盘菜单项：设置
    TrayIconMenu->addAction(tr("刷新"), this, SLOT(readConfig())); // 系统托盘菜单项：刷新
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

    BigWindowSettingsUnderlyingLabel = new QLabel(BigWindow); // 设置背景标签
    BigWindowSettingsUnderlyingLabel->hide();

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

    FullScreenAnimationLabel = new QLabel(BigWindow); // 红色全屏动画标签
    FullScreenAnimationLabel->resize(desktop.size());
    FullScreenAnimationLabel->setStyleSheet("background: red");


    // GroupBox
    BigWindowSettingsTextAndDateGroupBox = new QGroupBox(tr("文本和时间"), BigWindowSettingsUnderlyingLabel); // 设置文本和日期组框
    BigWindowSettingsTextAndDateGroupBox->show();
    

    // Label in GroupBox
    BigWindowSettingsSmallWindowTextLabel = new QLabel(tr("悬浮条文本："), BigWindowSettingsTextAndDateGroupBox); // 悬浮条文本标签
    BigWindowSettingsSmallWindowTextLabel->show();
    BigWindowSettingsBigWindowTextLabel = new QLabel(tr("全屏提醒文本："), BigWindowSettingsTextAndDateGroupBox); // 全屏窗口文本标签
    BigWindowSettingsBigWindowTextLabel->show();
    BigWindowSettingsBigWindowSmallTextLabel = new QLabel(tr("全屏提醒小文本："), BigWindowSettingsTextAndDateGroupBox); // 全屏窗口小文本标签
    BigWindowSettingsBigWindowSmallTextLabel->show();
    BigWindowSettingsTargetDateTimeLabel = new QLabel(tr("目标时间："), BigWindowSettingsTextAndDateGroupBox); // 目标时间标签
    BigWindowSettingsTargetDateTimeLabel->show();


    // LineEdit in GroupBox
    BigWindowSettingsSmallWindowTextLineEdit = new QLineEdit(SmallWindow_text, BigWindowSettingsTextAndDateGroupBox); // 悬浮条文本输入框
    BigWindowSettingsSmallWindowTextLineEdit->show();
    BigWindowSettingsBigWindowTextLineEdit = new QLineEdit(BigWindow_text, BigWindowSettingsTextAndDateGroupBox); // 全屏窗口


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
        }
        else if (!is_setting) {
            ReminderUnderlyingLabel->show();
        }
        });
    connect(FullScreenAnimationGroup, &QSequentialAnimationGroup::finished, [&] {
        QTimer::singleShot(3000, this, [&] {BigWindowHideAnimation->start(); });
        });
    connect(BigWindowHideAnimation, &QPropertyAnimation::finished, [&] {
        BigWindow->setWindowOpacity(1);
        BigWindow->hide();
        ReminderUnderlyingLabel->hide();
        });
}

UniversalTimer2::~UniversalTimer2()
{}

// 配置文件读取函数
void UniversalTimer2::readConfig() {
    // 若文件存在，则读取配置文件
    if (QFile::exists("config.ini")) {
        QSettings settings("config.ini", QSettings::IniFormat);

        is_show_BigWindow = settings.value("is_show_BigWindow").toBool(); // 是否显示全屏窗口

        is_show_SmallWindow = settings.value("is_show_SmallWindow").toBool(); // 是否显示悬浮条
        if (!is_show_SmallWindow) {
            this->hide();
            SmallWindowLabel->hide();
        }

        border_radius = settings.value("border_radius").toInt(); // 窗口圆角
        SmallWindowLabel->setStyleSheet("background: rgba(255, 255, 255, 0.75); border-radius: " + QString::number(border_radius) + "px; color: red;"); // 更新悬浮条样式

        targetDateTime = QDateTime::fromString(settings.value("targetDateTime").toString(), "yyyy-MM-dd hh:mm:ss"); // 目标时间
        if (!targetDateTime.isValid()) {
            targetDateTime = QDateTime::currentDateTime().addDays(1); // 默认设置为明天
            writeLog("WARNING", "目标时间无效，已设置为默认值：明天");
        }

        SmallWindow_text = settings.value("SmallWindow_text").toString(); // 悬浮条文本

        SmallWindow_position = settings.value("SmallWindow_position").toInt(); // 悬浮条位置

        SmallWindow_height = settings.value("SmallWindow_height").toInt(); // 悬浮条高度
        font.setPixelSize(SmallWindow_height * 0.618);
        SmallWindowLabel->setFont(font);

        BigWindow_text = settings.value("BigWindow_text").toString(); // 全屏窗口文本
        ReminderTextLabel->setText(BigWindow_text);
        ReminderTextLabel->adjustSize();

        BigWindow_small_text = settings.value("BigWindow_small_text").toString(); // 全屏窗口小文本

        update_interval = settings.value("update_interval").toInt(); // 更新间隔时间

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
        is_welcome = true;
        WelcomeLabel = new QLabel(tr("欢迎使用万能倒计时!<br>简单设置，并开始使用吧!"), BigWindowBackgroundLabel);
        WelcomeLabel->setAlignment(Qt::AlignCenter);
        WelcomeLabel->resize(desktop.size());
        WelcomeLabel->setStyleSheet("color: white; font-size: 40px; font-weight: bold; background: transparent;");
        WelcomeLabel->show();
        WelcomeButton = new QPushButton(tr("开始"), WelcomeLabel);
        WelcomeButton->setGeometry(desktop.width() * 0.45, desktop.height() * 0.6, desktop.width() * 0.1, desktop.height() * 0.1);
        WelcomeButton->setStyleSheet("color: white; font-size: 20px; font-weight: bold; background: rgba(0, 0, 0, 0.5); border-radius: 10px;");
        WelcomeButton->show();
        connect(WelcomeButton, &QPushButton::clicked, [&] {
            BigWindowHideAnimation->start();
            WelcomeLabel->deleteLater();
            WelcomeButton->deleteLater();
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
    settings.setValue("border_radius", border_radius);
    settings.setValue("targetDateTime", targetDateTime.toString("yyyy-MM-dd hh:mm:ss"));
    settings.setValue("SmallWindow_text", SmallWindow_text);
    settings.setValue("SmallWindow_position", SmallWindow_position);
    settings.setValue("SmallWindow_height", SmallWindow_height);
    settings.setValue("BigWindow_text", BigWindow_text);
    settings.setValue("BigWindow_small_text", BigWindow_small_text);
    settings.setValue("update_interval", update_interval);
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


// 日志写入函数
void UniversalTimer2::writeLog(QString type, QString log) {
    QFile file("Universal-Timer-2.log");
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
    updateObjects();
    adjustReminderSize(1.0);
    ReminderUnderlyingLabel->move((desktop.width() - ReminderUnderlyingLabel->width()) / 2, (desktop.height() - ReminderUnderlyingLabel->height()) / 2);


    ReminderUnderlyingLabel->hide();
    BigWindowBackgroundLabel->hide();
    FullScreenAnimationLabel->show();
    BigWindow->show();
    FullScreenAnimationGroup->start();
}

// 全屏提醒大小调整函数
void UniversalTimer2::adjustReminderSize(qreal scale) {
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
    if (is_show_BigWindow && (!BigWindow->isVisible())) {
        if (timeList.contains(QTime::fromString(currentDateTime.toString("hh:mm:ss")))) {
            showBigWindow();
        }
    }
}