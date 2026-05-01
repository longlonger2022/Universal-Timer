#include "ConfigManager.h"
#include "LogManager.h"

ConfigManager::ConfigManager(QObject* parent) : QObject(parent)
{
    read();
}

ConfigManager::~ConfigManager()
{}


void ConfigManager::read() {
    if (QFile::exists("config.ini")) {
        qDebug() << "读取配置文件……";

        QSettings Settings("config.ini", QSettings::IniFormat);

        Settings.beginGroup("General");
        General.target_date_time = Settings.value("target_date_time", QDateTime(QDate(2025, 6, 30), QTime(0, 0, 0))).toDateTime();
        General.update_interval = Settings.value("update_interval", 1000).toInt();
        General.language = Settings.value("language", "zh-CN").toString();
        General.language_code_list = Settings.value("language_code_list", language_code_list_default).toStringList();
        General.language_name_list = Settings.value("language_name_list", language_name_list_default).toStringList();
        Settings.endGroup();

        Settings.beginGroup("FloatingBar");
        FloatingBar.is_show_floating_bar = Settings.value("is_show_floating_bar", true).toBool();
        FloatingBar.floating_bar_on_top = Settings.value("floating_bar_on_top", true).toBool();
        FloatingBar.floating_bar_position = static_cast<FloatingBarPosition>(Settings.value("floating_bar_position", static_cast<int>(FloatingBarPosition::TopCenter)).toInt());
        FloatingBar.floating_bar_border_radius = Settings.value("floating_bar_border_radius", 10).toUInt();
        FloatingBar.floating_bar_height = Settings.value("floating_bar_height", 50).toUInt();
        FloatingBar.floating_bar_text = Settings.value("floating_bar_text", "距会考还剩：").toString();
        Settings.endGroup();

        Settings.beginGroup("Reminder");
        Reminder.is_show_reminder = Settings.value("is_show_reminder", true).toBool();
        Reminder.reminder_text = Settings.value("reminder_text", "距会考").toString();
        Reminder.reminder_small_text = Settings.value("reminder_small_text", "THE EXAM IN ").toString();
        Reminder.remaining_days_to_play_countdown_sound = Settings.value("remaining_days_to_play_countdown_sound", 30).toInt();
        Reminder.remaining_days_to_play_heartbeat_sound = Settings.value("remaining_days_to_play_heartbeat_sound", 14).toInt();
        Reminder.block_show_times = Settings.value("block_show_times", 4).toUInt();
        Reminder.reminder_time_list = Settings.value("reminder_time_list").toList();
        Settings.endGroup();

        qInfo() << "配置文件读取成功";
    }
}

void ConfigManager::write() {
    qDebug() << "写入配置文件……";

    QSettings Settings("config.ini", QSettings::IniFormat);

    Settings.beginGroup("General");
    Settings.setValue("target_date_time", General.target_date_time);
    Settings.setValue("update_interval", General.update_interval);
    Settings.setValue("language", General.language);
    Settings.setValue("language_code_list", General.language_code_list);
    Settings.setValue("language_name_list", General.language_name_list);
    Settings.endGroup();

    Settings.beginGroup("FloatingBar");
    Settings.setValue("is_show_floating_bar", FloatingBar.is_show_floating_bar);
    Settings.setValue("floating_bar_on_top", FloatingBar.floating_bar_on_top);
    Settings.setValue("floating_bar_position", static_cast<int>(FloatingBar.floating_bar_position));
    Settings.setValue("floating_bar_border_radius", FloatingBar.floating_bar_border_radius);
    Settings.setValue("floating_bar_height", FloatingBar.floating_bar_height);
    Settings.setValue("floating_bar_text", FloatingBar.floating_bar_text);
    Settings.endGroup();

    Settings.beginGroup("Reminder");
    Settings.setValue("is_show_reminder", Reminder.is_show_reminder);
    Settings.setValue("reminder_text", Reminder.reminder_text);
    Settings.setValue("reminder_small_text", Reminder.reminder_small_text);
    Settings.setValue("remaining_days_to_play_countdown_sound", Reminder.remaining_days_to_play_countdown_sound);
    Settings.setValue("remaining_days_to_play_heartbeat_sound", Reminder.remaining_days_to_play_heartbeat_sound);
    Settings.setValue("block_show_times", Reminder.block_show_times);
    Settings.setValue("reminder_time_list", Reminder.reminder_time_list);
    Settings.endGroup();

    Settings.sync();

    if (Settings.status() != QSettings::NoError) {
        QMessageBox::critical(NULL, tr("错误"), tr("配置文件写入失败<br>错误代码：") + QString::number(Settings.status()), QMessageBox::Ok);
        qWarning() << "配置文件写入失败，错误代码：" << QString::number(Settings.status());
    }
    else {
        qInfo() << "配置文件写入成功";
    }
}