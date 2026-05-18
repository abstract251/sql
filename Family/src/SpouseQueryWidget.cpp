#include "SpouseQueryWidget.h"
#include "ui_spouse_query_widget.h"
#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

SpouseQueryWidget::SpouseQueryWidget(int genealogyId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpouseQueryWidget)
    , m_currentGenealogyId(genealogyId)
{
    ui->setupUi(this);

    QSqlQueryModel* model = new QSqlQueryModel(this);
    ui->spouseTableView->setModel(model);
    ui->spouseTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->searchPushButton, &QPushButton::clicked, this, &SpouseQueryWidget::onSearchClicked);
}

SpouseQueryWidget::~SpouseQueryWidget()
{
    delete ui;
}

void SpouseQueryWidget::setGenealogyId(int genealogyId)
{
    m_currentGenealogyId = genealogyId;
}

void SpouseQueryWidget::onSearchClicked()
{
    int personId = ui->idSpinBox->value();
    
    if (personId <= 0) {
        QMessageBox::warning(this, "输入错误", "请输入有效的成员ID!");
        return;
    }

    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->spouseTableView->model());
    
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
            WHERE p.person_id IN (
                SELECT wife_id FROM families WHERE husband_id = %1 AND genealogy_id = %2
                UNION
                SELECT husband_id FROM families WHERE wife_id = %1 AND genealogy_id = %2
            )
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
            WHERE p.person_id IN (
                SELECT wife_id FROM families WHERE husband_id = %1
                UNION
                SELECT husband_id FROM families WHERE wife_id = %1
            )
        )").arg(personId);
    }
    
    model->setQuery(queryStr);
    ui->spouseTableView->resizeColumnsToContents();
}
