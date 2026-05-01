#pragma once

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>
#include <QMessageLogContext>


static QtMessageHandler defaultMessageHandler;

// 输出颜色函数
inline QString colorForTypeToFill(QtMsgType type) {
    switch (type) {
        case QtDebugMsg:    return "\033[34m%1\033[0m";   // 蓝色
        case QtInfoMsg:     return "\033[32m%1\033[0m";   // 绿色
        case QtWarningMsg:  return "\033[33m%1\033[0m";   // 黄色
        case QtCriticalMsg: return "\033[31m%1\033[0m";   // 红色
        case QtFatalMsg:    return "\033[41m%1\033[0m";   // 红底
        default:            return "%1";    // 默认
    }
}

// 重定向函数
inline void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& message) {
    static QMutex mutex;
    QMutexLocker locker(&mutex); // 线程锁
    QFile file("Universal-Timer.log");
    QString text;
    QTextStream out(&text);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << " - ";
    switch (type) {
        case QtDebugMsg: out << QString("[DEBUG]").leftJustified(10); break;
        case QtInfoMsg: out << QString("[INFO]").leftJustified(10); break;
        case QtWarningMsg: out << QString("[WARNING]").leftJustified(10); break;
        case QtCriticalMsg: out << QString("[CRITICAL]").leftJustified(10); break;
        case QtFatalMsg: out << QString("[FATAL]").leftJustified(10); break;
        default: out << QString("[UNKNOWN]").leftJustified(10); break;
    }
    out << " - " << message;

    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream(&file) << text << Qt::endl;
        file.flush();
    }

    if (defaultMessageHandler)
        defaultMessageHandler(type, context, colorForTypeToFill(type).arg(text));
}