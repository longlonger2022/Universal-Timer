#include "ui/ScalableLabel.h"

ScalableLabel::ScalableLabel(QWidget* parent)
    : QLabel(parent)
{
    if (!OriginalPixmap.isNull()) {
        // 初始按当前尺寸缩放，保持比例
        this->setPixmap(OriginalPixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    this->setAlignment(Qt::AlignCenter);
}

ScalableLabel::~ScalableLabel()
{}

void ScalableLabel::resizeEvent(QResizeEvent* event) {
    QLabel::resizeEvent(event);

    if (!OriginalPixmap.isNull()) {
        this->setPixmap(OriginalPixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}