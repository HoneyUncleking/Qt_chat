#include "HistoryPage.h"
#include "ui_HistoryPage.h"
#include <QJsonObject>

HistoryPage::HistoryPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HistoryPage)
    , m_currentFilter("all")
{
    ui->setupUi(this);
    clear();
}

HistoryPage::~HistoryPage()
{
    delete ui;
}

void HistoryPage::setHistoryData(const QJsonArray& historyItems)
{
    m_allItems = historyItems;
    ui->list_history->clear();
    
    for (const auto& item : historyItems) {
        QJsonObject obj = item.toObject();
        addHistoryItem(obj);
    }
}

void HistoryPage::addHistoryItem(const QJsonObject& item)
{
    QString type = item["type"].toString();
    QString title = item["title"].toString();
    QString amount = item["amount"].toString();
    QString date = item["date"].toString();
    
    QString displayText;
    QString icon;
    
    if (type == "buy") {
        icon = "🛒";
        displayText = QString("%1 Buy: %2 - %3 Toman\n   %4").arg(icon).arg(title).arg(amount).arg(date);
    } else if (type == "sell") {
        icon = "💰";
        displayText = QString("%1 Sale: %2 - %3 Toman\n   %4").arg(icon).arg(title).arg(amount).arg(date);
    } else if (type == "ad") {
        icon = "📝";
        displayText = QString("%1 Ad: %2\n   %3").arg(icon).arg(title).arg(date);
    } else if (type == "charge") {
        icon = "💳";
        displayText = QString("%1 Wallet Charge: +%2 Toman - %3").arg(icon).arg(amount).arg(date);
    }
    
    QListWidgetItem* listItem = new QListWidgetItem(displayText);
    listItem->setData(Qt::UserRole, type);
    ui->list_history->addItem(listItem);
}

void HistoryPage::setActiveFilterButton(QPushButton* activeButton)
{
    QString defaultStyle = "QPushButton { background-color: #e0e0e0; }";
    QString activeStyle = "QPushButton { background-color: #3498db; color: white; }";
    
    ui->btn_all->setStyleSheet(defaultStyle);
    ui->btn_buys->setStyleSheet(defaultStyle);
    ui->btn_sales->setStyleSheet(defaultStyle);
    ui->btn_ads->setStyleSheet(defaultStyle);
    
    activeButton->setStyleSheet(activeStyle);
}

void HistoryPage::clear()
{
    m_allItems = QJsonArray();
    ui->list_history->clear();
    setActiveFilterButton(ui->btn_all);
    m_currentFilter = "all";
}

void HistoryPage::on_btn_back_clicked()
{
    emit backRequested();
}

void HistoryPage::on_btn_all_clicked()
{
    setActiveFilterButton(ui->btn_all);
    m_currentFilter = "all";
    ui->list_history->clear();
    for (const auto& item : m_allItems) {
        QJsonObject obj = item.toObject();
        addHistoryItem(obj);
    }
}

void HistoryPage::on_btn_buys_clicked()
{
    setActiveFilterButton(ui->btn_buys);
    m_currentFilter = "buys";
    ui->list_history->clear();
    for (const auto& item : m_allItems) {
        QJsonObject obj = item.toObject();
        if (obj["type"].toString() == "buy") {
            addHistoryItem(obj);
        }
    }
}

void HistoryPage::on_btn_sales_clicked()
{
    setActiveFilterButton(ui->btn_sales);
    m_currentFilter = "sales";
    ui->list_history->clear();
    for (const auto& item : m_allItems) {
        QJsonObject obj = item.toObject();
        if (obj["type"].toString() == "sell") {
            addHistoryItem(obj);
        }
    }
}

void HistoryPage::on_btn_ads_clicked()
{
    setActiveFilterButton(ui->btn_ads);
    m_currentFilter = "ads";
    ui->list_history->clear();
    for (const auto& item : m_allItems) {
        QJsonObject obj = item.toObject();
        if (obj["type"].toString() == "ad") {
            addHistoryItem(obj);
        }
    }
}