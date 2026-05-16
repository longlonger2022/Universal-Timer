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
        tr("万能倒计时<br>"
            "版本：%1<br>"
            "开发者：<br>"
            "    龙ger_longer (B站同名，GitHub用户名longlonger2022)<br>"
            "    new_pointer (B站同名，GitHub用户名new5Fpointer)").arg(CURRENT_VERSION_STRING),
            this
        );
    AboutLabel->setWordWrap(true);
    AboutLabel->setAlignment(Qt::AlignLeft);
    AboutLabel->show();

    AboutTextEdit = new QTextEdit(
        tr("欢迎使用万能倒计时！<br>"
            "本软件代码使用 GPLv3 许可证，如果您是开发者，请遵守许可证。<br>"
            "本软件完全免费且开源，赞助完全取决于自愿！<br>"
            "您可以通过托盘图标进入设置界面，根据您的喜好个性化本软件。<br>"
            "截至该版本发布，仍暂时没有自动更新的功能。由此查看万能倒计时的各个版本：%1<br>"
            "官网（可能会变动，请注意关注最新动向）：%2 或 %3<br><br>"
            "若此地为开发者曾经学习之处，请往下读：<br>"
            "这个程序最初只是为了满足2025届会考倒计时需求，后经过许多更新，走到了这里，截至该版本发布，已经有一年之多了。由于开发者的学业影响，该程序在匆忙中进行着并不大的“大”更新，可能并不完美甚至说不好用，还可能有许多bug。这一版本发布 (2026年5月16日) 后，为学业着想，将会有数十天的几乎不更新，而等到暂时摆脱影响之后，又几乎不再会线下前往亲手更新，故还请关注官网动向还有GitHub仓库，以获取最新的版本。同时，也欢迎来反馈！"
            ).arg(GITHUB_RELEASE_URL).arg(GITHUB_PAGES_DOMAIN_URL).arg(CLOUDFLARE_PAGES_DOMAIN_URL),
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
