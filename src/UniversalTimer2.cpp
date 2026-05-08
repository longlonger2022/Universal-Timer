#include "UniversalTimer2.h"

#include <QApplication>
#include <QScreen>
#include <QMenu>

UniversalTimer2::UniversalTimer2(QObject* parent)
    : QObject(parent)
{

    desktop = QApplication::primaryScreen()->geometry();

    // Floating Bar
    FloatingBar = new FloatingBarClass;
    FloatingBar->show();

    // Fullscreen Pages
    FullscreenPages = new FullscreenPagesManager(nullptr, Config, FloatingBar);
    FullscreenPages->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    FullscreenPages->resize(desktop.size());

    // Tray Icon
    TrayIcon = new QSystemTrayIcon(QIcon(":/images/icons/Universal-Timer-2_icon.512px.png"), this); // 系统托盘图标
    TrayIcon->setToolTip(tr("万能倒计时"));
    QMenu* TrayIconMenu = new QMenu; // 系统托盘菜单
    TrayIconMenu->addAction(tr("设置"), FullscreenPages, &FullscreenPagesManager::showSettings); // 系统托盘菜单项：设置
    TrayIconMenu->addAction(tr("刷新"), this, &UniversalTimer2::refresh); // 系统托盘菜单项：刷新
    TrayIconMenu->addAction(tr("退出"), this, &qApp->quit); // 系统托盘菜单项：退出
    TrayIcon->setContextMenu(TrayIconMenu);
    TrayIcon->show();

    refresh();

    // Connections
    timer.start(Config.General.update_interval);
    connect(&timer, &QTimer::timeout, this, &UniversalTimer2::updateObjects);

}

UniversalTimer2::~UniversalTimer2()
{
    delete FullscreenPages;
    delete FloatingBar;
}


// 刷新函数
void UniversalTimer2::refresh() {

    qDebug() << "刷新……";

    if (!QFile::exists("config.ini")) FullscreenPages->showWelcome();

    Config.read();
    
    // FloatingBar
    if (!Config.FloatingBar.is_show_floating_bar) {
        FloatingBar->hide();
    } // todo)) if not, show FloatingBar.
    FloatingBar->Bar->setStyleSheet("background: rgba(255, 255, 255, 0.75); border-radius: " + QString::number(Config.FloatingBar.floating_bar_border_radius) + "px; color: red;"); // 更新悬浮条样式
    FloatingBar->setWindowFlags((Config.FloatingBar.floating_bar_on_top ? Qt::WindowStaysOnTopHint : Qt::WindowStaysOnBottomHint) | Qt::FramelessWindowHint | Qt::Tool);
    FloatingBar->setFixedHeight(Config.FloatingBar.floating_bar_height);
    QFont font;
    font.setPixelSize(Config.FloatingBar.floating_bar_height * GOLDEN_RATIO_INV);
    FloatingBar->Bar->setFont(font);

    // Reminder
    if (Config.Reminder.is_show_reminder) {
        FullscreenPages->showReminder();
    }

    qDebug() << "刷新结束";
}

// 更新函数
// 悬浮条更新函数
void UniversalTimer2::updateFloatingBar() {
    // 更新标签文本
    FloatingBar->Bar->setText(" " + Config.FloatingBar.floating_bar_text + QString::number(left_time / 86400) + tr("天", "Floating Bar") + QString::number((left_time % 86400) / 3600) + tr("时") + QString::number((left_time % 3600) / 60) + tr("分") + QString::number(left_time % 60) + tr("秒 "));

    // 更新大小
    FloatingBar->Bar->adjustSize();
    FloatingBar->Bar->resize(FloatingBar->Bar->width() + 20, Config.FloatingBar.floating_bar_height);
    FloatingBar->adjustSize();

    // 更新位置
    switch (Config.FloatingBar.floating_bar_position) {
        case FloatingBarPosition::TopCenter:
            FloatingBar->move((desktop.width() - FloatingBar->width()) / 2, 0);
            break;
        case FloatingBarPosition::TopRight:
            FloatingBar->move(desktop.width() - FloatingBar->width(), 0);
            break;
        case FloatingBarPosition::TopLeft:
            FloatingBar->move(0, 0);
            break;
    }
}

// 更新函数
void UniversalTimer2::updateObjects() {
    left_time = QDateTime::currentDateTime().secsTo(Config.General.target_date_time);
    if (Config.FloatingBar.is_show_floating_bar) {
        updateFloatingBar();
    }
    // 定时显示全屏提醒
    if (Config.Reminder.is_show_reminder) {
        QTime current_time = QTime::currentTime();
        if (Config.Reminder.reminder_time_list.contains(QTime(current_time.hour(), current_time.minute(), current_time.second()))) {
            FullscreenPages->showReminder();
        }
    }
}