#include "ChildrenQueryWidget.h"
#include "ui_children_query_widget.h"
#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

ChildrenQueryWidget::ChildrenQueryWidget(int genealogyId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChildrenQueryWidget)
    , m_currentGenealogyId(genealogyId)
{
    ui->setupUi(this);

    QSqlQueryModel* model = new QSqlQueryModel(this);
    ui->childrenTableView->setModel(model);
    ui->childrenTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->searchPushButton, &QPushButton::clicked, this, &ChildrenQueryWidget::onSearchClicked);
}

ChildrenQueryWidget::~ChildrenQueryWidget()
{
    delete ui;
}

void ChildrenQueryWidget::setGenealogyId(int genealogyId)
{
    m_currentGenealogyId = genealogyId;
}

void ChildrenQueryWidget::onSearchClicked()
{
    int personId = ui->idSpinBox->value();
    
    if (personId <= 0) {
        QMessageBox::warning(this, "输入错误", "请输入有效的成员ID!");
        return;
    }

    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->childrenTableView->model());
    
    QString queryStr;
    if (m_currentGenealogyId > 0) {
        queryStr = QString(R"(
            SELECT 
                p.person_id AS "成员ID",
                p.name AS "姓名",
                CASE p.gender WHEN 'M' THEN '男' ELSE '女' END AS "性别",
                p.birth_year AS "出生年份",
                COALESCE(p.death_year, 0) AS "卒年",
                p.generation AS "辈分"
            FROM persons p
            WHERE p.birth_family_id IN (
                SELECT family_id FROM families WHERE husband_id = %1 OR wife_id = %1
            )
            AND p.genealogy_id = %2
            ORDER BY p.generation, p.birth_year
        )").arg(personId).arg(m_currentGenealogyId);
    } else {
        queryStr = QString(R"(
            SELECT 
                p.person_id AS "成员ID",
                p.name AS "姓名",
                CASE p.gender WHEN 'M' THEN '男' ELSE '女' END AS "性别",
                p.birth_year AS "出生年份",
                COALESCE(p.death_year, 0) AS "卒年",
                p.generation AS "辈分"
            FROM persons p
            WHERE p.birth_family_id IN (
                SELECT family_id FROM families WHERE husband_id = %1 OR wife_id = %1
            )
            ORDER BY p.generation, p.birth_year
        )").arg(personId);
    }
    
    model->setQuery(queryStr);
    ui->childrenTableView->resizeColumnsToContents();
}
