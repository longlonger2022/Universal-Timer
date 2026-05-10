#include "FloatingBar.h"

FloatingBarClass::FloatingBarClass(QWidget* parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground); // 设置窗口背景透明

    Bar = new QLabel(this);
    Bar->setAlignment(Qt::AlignCenter);
    Bar->setStyleSheet("font-family: Microsoft YaHei UI, Microsoft YaHei; background: rgba(255, 255, 255, 0.75); color: red;");
    Bar->show();
}

FloatingBarClass::~FloatingBarClass()
{}