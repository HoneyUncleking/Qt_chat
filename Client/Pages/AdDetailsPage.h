#ifndef ADDETAILSPAGE_H
#define ADDETAILSPAGE_H

#include <QWidget>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class AdDetailsPage; }
QT_END_NAMESPACE

class AdDetailsPage : public QWidget {
    Q_OBJECT

public:
    explicit AdDetailsPage(QWidget *parent = nullptr);
    ~AdDetailsPage();
    void setAdData(const QJsonObject& adData);
    void clear();

signals:
    void backRequested();
    void cartRequested();
    void addToCartRequested(int adId);
    void buyNowRequested(int adId);

private slots:
    void on_btn_back_clicked();
    void on_btn_cart_clicked();
    void on_btn_add_to_cart_clicked();
    void on_btn_buy_clicked();

private:
    Ui::AdDetailsPage *ui;
    QJsonObject m_currentAd;
    int m_currentAdId;
};

#endif