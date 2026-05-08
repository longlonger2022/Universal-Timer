#pragma once

#include "LogManager.h"
#include "ConfigManager.h"
#include "FloatingBar.h"
#include "FullscreenPagesManager.h"

#include <QtWidgets/QWidget>
#include <numbers>
#include <QSystemTrayIcon>
#include <QTimer>

inline constexpr double GOLDEN_RATIO_INV = 1.0 / std::numbers::phi; // 黄金比例倒数

class UniversalTimer2 : public QObject
{
    Q_OBJECT

public:
    UniversalTimer2(QObject* parent = nullptr);
    ~UniversalTimer2();

private:


    // 函数声明
    // 刷新
    void refresh();

    // 更新
    void updateFloatingBar(); // 更新悬浮条
    void updateObjects(); // 更新对象


    // 剩余时间
    int left_time = 0;

    // 屏幕分辨率
    QRect desktop;

    // 计时器
    QTimer timer;

    // 配置
    ConfigManager Config;

    // 悬浮条
    FloatingBarClass* FloatingBar;

    // 全屏窗口
    FullscreenPagesManager* FullscreenPages;


    // SystemTrayIcon
    QSystemTrayIcon* TrayIcon; // 系统托盘图标


};
