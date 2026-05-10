#include "ui/DonatePage.h"

DonatePageClass::DonatePageClass(QWidget* parent)
    : ScalableLabel(parent)
{

    this->setWindowTitle(tr("赞助"));

    this->OriginalPixmap = QPixmap(":/images/qrcode/WeChatDonate.png");
    
}

DonatePageClass::~DonatePageClass()
{}