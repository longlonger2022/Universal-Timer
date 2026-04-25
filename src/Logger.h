#pragma once

#include <QFile>
#include <QTextStream>
#include <QDateTime>

// 日志写入函数
inline void writeLog(QString type, QString log) {
    QFile file("Universal-Timer.log");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " - [" << type << "] - " << log << Qt::endl;
    }
}
