#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>
#include <QJsonArray>
#include <QListWidgetItem>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class HistoryPage; }
QT_END_NAMESPACE

class HistoryPage : public QWidget {
    Q_OBJECT

public:
    explicit HistoryPage(QWidget *parent = nullptr);
    ~HistoryPage();
    void setHistoryData(const QJsonArray& historyItems);
    void clear();

signals:
    void backRequested();

private slots:
    void on_btn_back_clicked();
    void on_btn_all_clicked();
    void on_btn_buys_clicked();
    void on_btn_sales_clicked();
    void on_btn_ads_clicked();

private:
    void addHistoryItem(const QJsonObject& item);
    void setActiveFilterButton(QPushButton* activeButton);
    
    Ui::HistoryPage *ui;
    QJsonArray m_allItems;
    QString m_currentFilter;
};

#endif