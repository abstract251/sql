#include "HelpDialog.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QFrame>

HelpDialog::HelpDialog(QWidget* parent)
    : QDialog(parent)
    , m_scrollArea(nullptr)
{
    setupUi();
}

void HelpDialog::setupUi()
{
    setWindowTitle("常见问题解答");
    setMinimumSize(600, 500);
    resize(650, 550);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("常见问题解答", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    mainLayout->addSpacing(10);

    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* contentWidget = new QWidget(m_scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(15);

    contentLayout->addWidget(createFaqItem("如何登录系统？",
        "首次使用请使用管理员账号登录。默认账号：admin，密码：123456。如果您还没有账号，可以点击登录框中的\"注册\"按钮创建新账号。"));

    contentLayout->addWidget(createFaqItem("如何创建族谱？",
        "登录后，点击顶部菜单的\"族谱\"，选择\"创建族谱\"。填写谱名、姓氏、修谱时间等信息后即可创建新的族谱。"));

    contentLayout->addWidget(createFaqItem("如何添加家族成员？",
        "在\"成员管理\"页面中，点击\"添加成员\"按钮。填写成员的姓名、性别、出生年份等信息，其中辈分（代）字段用于表示该成员在家族中的世代位置。"));

    contentLayout->addWidget(createFaqItem("如何查看家族树形图？",
        "点击\"树形预览\"标签，输入起始成员的ID即可查看该成员及其后代的关系树。您也可以不输入ID，直接查看第一代成员及其整个家族树。"));

    contentLayout->addWidget(createFaqItem("如何查询祖先？",
        "点击\"祖先查询\"标签，输入成员的ID，系统将以树状图形式显示该成员的所有祖先（父辈及以上）。"));

    contentLayout->addWidget(createFaqItem("如何查询两个人之间的关系？",
        "点击\"亲缘关系\"标签，分别输入两个人的ID，系统将查找并显示两人之间的亲缘关系链路，如共同的祖先等。"));

    contentLayout->addWidget(createFaqItem("如何邀请他人参与族谱编辑？",
        "在\"族谱管理\"页面中，选择一个族谱后点击\"邀请用户\"按钮，输入要邀请的用户名即可。被邀请的用户将能够查看和编辑该族谱。"));

    contentLayout->addWidget(createFaqItem("如何修改成员信息？",
        "在\"成员管理\"页面中，双击要修改的成员行，或选中后点击\"编辑\"按钮，即可修改成员的姓名、出生年份等信息。"));

    contentLayout->addWidget(createFaqItem("删除成员会有什么影响？",
        "删除成员只会删除该成员的信息，不会影响其已建立的父子关系和家庭关系。如果该成员有子女，子女信息仍会保留。"));

    contentLayout->addWidget(createFaqItem("忘记密码怎么办？",
        "如果您忘记了密码，请联系系统管理员。管理员可以在数据库中重置您的密码。"));

    contentLayout->addWidget(createFaqItem("辈分（代）是什么意思？",
        "辈分用于表示家族中的世代顺序。数字越小表示越早的世代，例如第1代是最早的祖先，第10代是第1代的后代。系统会自动根据辈分进行排序和统计。"));

    contentLayout->addWidget(createFaqItem("如何正确设置成员的出生家庭？",
        "每个成员都有一个\"出生家庭\"字段，指向其父母的家庭。这个字段用于确定成员的父子关系和进行祖先追溯查询。请确保正确设置以便正常使用相关功能。"));

    contentLayout->addWidget(createFaqItem("本地数据库不完整导致查询祖先等功能不工作？",
                                           "请在本地数据库上执行Family\\database目录下的fix_data.sql和update_procedures.sql。"));

    contentLayout->addStretch();

    m_scrollArea->setWidget(contentWidget);
    mainLayout->addWidget(m_scrollArea);

    QPushButton* closeBtn = new QPushButton("关闭", this);
    closeBtn->setFixedWidth(100);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeBtn, 0, Qt::AlignRight);
}

QFrame* HelpDialog::createFaqItem(const QString& question, const QString& answer)
{
    QFrame* faqFrame = new QFrame();
    faqFrame->setFrameShape(QFrame::StyledPanel);
    faqFrame->setFrameShadow(QFrame::Raised);
    faqFrame->setStyleSheet("QFrame { background-color: #f5f5f5; border-radius: 5px; padding: 8px; }");

    QVBoxLayout* faqLayout = new QVBoxLayout(faqFrame);

    QLabel* questionLabel = new QLabel(question, faqFrame);
    QFont qFont = questionLabel->font();
    qFont.setBold(true);
    qFont.setPointSize(10);
    questionLabel->setFont(qFont);
    questionLabel->setStyleSheet("color: #2c3e50; background-color: transparent;");
    faqLayout->addWidget(questionLabel);

    QLabel* answerLabel = new QLabel(answer, faqFrame);
    answerLabel->setWordWrap(true);
    answerLabel->setStyleSheet("color: #555555; background-color: transparent;");
    faqLayout->addWidget(answerLabel);

    return faqFrame;
}
