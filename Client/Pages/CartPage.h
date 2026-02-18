#ifndef CARTPAGE_H
#define CARTPAGE_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class CartPage; }
QT_END_NAMESPACE

class CartPage : public QWidget {
    Q_OBJECT

public:
    explicit CartPage(QWidget *parent = nullptr);
    ~CartPage();
    void setCartData(const QJsonArray& cartItems);
    void clear();

signals:
    void backRequested();
    void checkoutRequested();
    void clearCartRequested();
    void removeItemRequested(int itemId);

private slots:
    void on_btn_back_clicked();
    void on_btn_clear_clicked();
    void on_btn_clear_2_clicked();

private:
    void updateTotalPrice();
    void addCartItem(const QJsonObject& item);
    
    Ui::CartPage *ui;
    QJsonArray m_cartItems;
    int m_totalPrice;
};

#endif