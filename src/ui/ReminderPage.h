#pragma once

#include "core/ConfigManager.h"

#include <QLabel>
#include <QSoundEffect>

class ReminderPageClass : public QWidget
{
    Q_OBJECT

public:
    ReminderPageClass(QWidget* parent, const ConfigManager& cfg);
    ~ReminderPageClass();

Q_SIGNALS:

    void finished();

private:

    void initializeObjects();
    void adjustObjects(const QWidget* parent, const qreal scale, const unsigned short depth = 0);
    void showBlocks(const unsigned short times = 1);


    // Font
    QFont font;

    // Config
    const ConfigManager& config;

    // Sound
    QSoundEffect* CountdownSound; // 倒计时声音
    QSoundEffect* HeartbeatSound; // 心跳声音

    QLabel* TitleLabel; // 全屏提醒自定义标题标签
    QLabel* ConjunctionLabel; // 全屏提醒“还剩”文本标签
    QLabel* DaysLabel; // 全屏提醒“天”文本标签
    QLabel* NumberLabel; // 全屏提醒剩余天数标签
    QLabel* TextLabel; // 全屏提醒自定义小文本标签
    QLabel* ColorLabel; // 全屏提醒颜色标签

    QList<QLabel*> ReminderBlockLabels; // 全屏提醒红色块标签列表

};