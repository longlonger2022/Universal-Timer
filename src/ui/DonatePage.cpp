#include "ui/DonatePage.h"

DonatePageClass::DonatePageClass(QWidget* parent)
    : QLabel(parent)
{

    this->setWindowTitle(tr("赞助"));

    OriginalPixmap = QPixmap(":/images/qrcode/WeChatDonate.png");
    if (!OriginalPixmap.isNull()) {
        // 初始按当前尺寸缩放，保持比例
        this->setPixmap(OriginalPixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    this->setAlignment(Qt::AlignCenter);
}

DonatePageClass::~DonatePageClass()
{}

void DonatePageClass::resizeEvent(QResizeEvent* event) {
    QLabel::resizeEvent(event);

    if (!OriginalPixmap.isNull()) {
        this->setPixmap(OriginalPixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}