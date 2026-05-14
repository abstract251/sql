#include "DashboardWidget.h"
#include "ui_dashboard_widget.h"
#include "DatabaseManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTableView>
#include <QHeaderView>
#include <QGroupBox>
#include <QDebug>
#include <QSqlQuery>
#include <cmath>

DashboardWidget::DashboardWidget(int genealogyId, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashboardWidget)
    , m_currentGenealogyId(genealogyId)
    , m_statsModel(nullptr)
{
    ui->setupUi(this);

    QVariantList genealogies = DatabaseManager::instance().getAllGenealogies();
    for (const QVariant& g : genealogies) {
        QVariantMap map = g.toMap();
        ui->genealogyComboBox->addItem(map["name"].toString(), map["genealogy_id"]);
    }

    if (m_currentGenealogyId > 0) {
        int index = ui->genealogyComboBox->findData(m_currentGenealogyId);
        if (index >= 0) {
            ui->genealogyComboBox->setCurrentIndex(index);
        }
    }

    connect(ui->genealogyComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &DashboardWidget::onGenealogyComboChanged);

    refreshData();
}

DashboardWidget::~DashboardWidget()
{
    delete ui;
}

void DashboardWidget::refreshData()
{
    if (m_currentGenealogyId <= 0) {
        ui->totalMembersLabel->setText("0");
        ui->maleCountLabel->setText("0 （0%）");
        ui->femaleCountLabel->setText("0 （0%）");
        ui->genderRatioLabel->setText("0:0");
        ui->totalGenerationsLabel->setText("0");
        ui->avgLifespanLabel->setText("0");
        return;
    }

    QSqlQuery query(DatabaseManager::instance().database());
    
    query.prepare(R"(
        SELECT 
            COUNT(*) as total,
            COALESCE(SUM(CASE WHEN gender = 'M' THEN 1 ELSE 0 END), 0) as male_count,
            COALESCE(SUM(CASE WHEN gender = 'F' THEN 1 ELSE 0 END), 0) as female_count
        FROM persons
        WHERE genealogy_id = ?
    )");
    query.addBindValue(m_currentGenealogyId);
    
    int total = 0;
    int maleCount = 0;
    int femaleCount = 0;
    
    if (query.exec() && query.next()) {
        total = query.value("total").toInt();
        maleCount = query.value("male_count").toInt();
        femaleCount = query.value("female_count").toInt();
    }

    ui->totalMembersLabel->setText(QString::number(total));

    if (total > 0) {
        double maleRatio = (double)maleCount / total * 100;
        double femaleRatio = (double)femaleCount / total * 100;
        
        ui->maleCountLabel->setText(QString::number(maleCount) + " （" + 
            QString::number(maleRatio, 'f', 1) + "%）");
        ui->femaleCountLabel->setText(QString::number(femaleCount) + " （" + 
            QString::number(femaleRatio, 'f', 1) + "%）");
        
        double roundedMale = std::round(maleRatio * 10) / 10.0;
        double roundedFemale = 100.0 - roundedMale;
        
        ui->genderRatioLabel->setText(
            QString::number(roundedMale, 'f', 1) + ":" + 
            QString::number(roundedFemale, 'f', 1)
        );
    } else {
        ui->maleCountLabel->setText("0 （0%）");
        ui->femaleCountLabel->setText("0 （0%）");
        ui->genderRatioLabel->setText("0:0");
    }
    
    query.prepare(R"(
        SELECT 
            COUNT(DISTINCT generation) as total_generations,
            CASE 
                WHEN AVG(death_year - birth_year) IS NOT NULL 
                THEN ROUND(AVG(death_year - birth_year), 1)
                ELSE 0 
            END as avg_lifespan
        FROM persons
        WHERE genealogy_id = ? AND death_year IS NOT NULL AND death_year > 0
    )");
    query.addBindValue(m_currentGenealogyId);
    
    if (query.exec() && query.next()) {
        ui->totalGenerationsLabel->setText(query.value("total_generations").toString());
        ui->avgLifespanLabel->setText(query.value("avg_lifespan").toString() + " 岁");
    } else {
        ui->totalGenerationsLabel->setText("0");
        ui->avgLifespanLabel->setText("0");
    }

    updateStatistics();
}

void DashboardWidget::onGenealogyComboChanged(int index)
{
    if (index >= 0) {
        m_currentGenealogyId = ui->genealogyComboBox->currentData().toInt();
        emit genealogySelectionChanged(m_currentGenealogyId);
        refreshData();
    }
}

void DashboardWidget::updateStatistics()
{
    if (m_currentGenealogyId <= 0) return;

    QVariantList genStats = DatabaseManager::instance().getGenerationStats(m_currentGenealogyId);

    if (ui->statsTableWidget->rowCount() > 0) {
        ui->statsTableWidget->setRowCount(0);
    }

    ui->statsTableWidget->setColumnCount(5);
    ui->statsTableWidget->setHorizontalHeaderLabels({"辈分", "人数", "平均寿命", "最早出生", "最晚出生"});
    ui->statsTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->statsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->statsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    for (const QVariant& v : genStats) {
        QVariantMap map = v.toMap();
        int row = ui->statsTableWidget->rowCount();
        ui->statsTableWidget->insertRow(row);

        ui->statsTableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(map["generation"].toInt())));
        ui->statsTableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(map["count"].toInt())));
        ui->statsTableWidget->setItem(row, 2, new QTableWidgetItem(map["avg_lifespan"].toString()));
        ui->statsTableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(map["earliest_birth"].toInt())));
        ui->statsTableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(map["latest_birth"].toInt())));
    }

    if (genStats.isEmpty()) {
        ui->statsTableWidget->insertRow(0);
        ui->statsTableWidget->setItem(0, 0, new QTableWidgetItem("暂无数据"));
    }
}
