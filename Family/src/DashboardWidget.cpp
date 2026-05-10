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
        ui->maleCountLabel->setText("0");
        ui->femaleCountLabel->setText("0");
        ui->maleRatioLabel->setText("0%");
        ui->femaleRatioLabel->setText("0%");
        return;
    }

    QVariantList genderStats = DatabaseManager::instance().getGenderStats(m_currentGenealogyId);
    if (!genderStats.isEmpty()) {
        QVariantMap stats = genderStats.first().toMap();
        int total = stats["total"].toInt();
        int maleCount = stats["male_count"].toInt();
        int femaleCount = stats["female_count"].toInt();

        ui->totalMembersLabel->setText(QString::number(total));
        ui->maleCountLabel->setText(QString::number(maleCount));
        ui->femaleCountLabel->setText(QString::number(femaleCount));

        if (total > 0) {
            double maleRatio = (double)maleCount / total * 100;
            double femaleRatio = (double)femaleCount / total * 100;
            ui->maleRatioLabel->setText(QString::number(maleRatio, 'f', 1) + "%");
            ui->femaleRatioLabel->setText(QString::number(femaleRatio, 'f', 1) + "%");
        } else {
            ui->maleRatioLabel->setText("0%");
            ui->femaleRatioLabel->setText("0%");
        }
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
