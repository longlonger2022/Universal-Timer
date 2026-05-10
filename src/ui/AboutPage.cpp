#include "ui/AboutPage.h"
#include "core/Global.h"

#include <QGridLayout>

AboutPageClass::AboutPageClass(QWidget* parent)
    : QWidget(parent)
{

    this->setWindowTitle(tr("关于"));

    IconLabel = new ScalableLabel(this);
    IconLabel->setPixmap(QPixmap(":/images/icons/Universal-Timer-2_icon.512px.png"));
    IconLabel->show();

    AboutLabel = new QLabel(
        tr("万能倒计时<br>\
            版本：%1<br>\
            开发者：<br>\
            龙ger_longer (B站同名，GitHub用户名longlonger2022)<br>\
            new_pointer (B站同名，GitHub用户名new5Fpointer)").arg(CURRENT_VERSION_STRING),
            this
        );
    AboutLabel->setWordWrap(true);
    AboutLabel->setAlignment(Qt::AlignLeft);
    AboutLabel->show();

    AboutTextEdit = new QTextEdit(
        tr("欢迎使用万能倒计时！<br>\
            本软件代码使用 GPLv3 许可证，如果您是开发者，请遵守许可证。<br>\
            本软件完全免费且开源，赞助完全取决于自愿！<br>\
            您可以通过托盘图标进入设置界面，根据您的喜好个性化本软件。<br>\
            截至该版本发布，仍暂时没有自动更新的功能。由此查看万能倒计时的各个版本：%1<br>\
            官网（可能会变动，请注意关注最新动向）：%2 或 %3").arg(GITHUB_RELEASE_URL).arg(GITHUB_PAGES_DOMAIN_URL).arg(CLOUDFLARE_PAGES_DOMAIN_URL),
        this
        );
    AboutTextEdit->setReadOnly(true);
    AboutLabel->show();

    QGridLayout* Layout = new QGridLayout(this);
    Layout->addWidget(IconLabel, 0, 0, 1, 1);
    Layout->addWidget(AboutLabel, 0, 1, 1, 1);
    Layout->addWidget(AboutTextEdit, 1, 0, 1, 2);
    Layout->setSpacing(25);
    this->setLayout(Layout);

}

AboutPageClass::~AboutPageClass()
{}

AboutPageClass::ScalableLabel::ScalableLabel(QWidget* parent)
    : QLabel(parent)
{
    if (!OriginalPixmap.isNull()) {
        // 初始按当前尺寸缩放，保持比例
        this->setPixmap(OriginalPixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    this->setAlignment(Qt::AlignCenter);
}

AboutPageClass::ScalableLabel::~ScalableLabel()
{}

void AboutPageClass::ScalableLabel::resizeEvent(QResizeEvent* event) {
    QLabel::resizeEvent(event);

    if (!OriginalPixmap.isNull()) {
        this->setPixmap(OriginalPixmap.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}