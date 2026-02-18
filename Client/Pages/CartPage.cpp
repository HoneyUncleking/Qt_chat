#include "CartPage.h"
#include "ui_CartPage.h"
#include <QJsonObject>
#include <QMessageBox>

CartPage::CartPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CartPage)
    , m_totalPrice(0)
{
    ui->setupUi(this);
    clear();
}

CartPage::~CartPage()
{
    delete ui;
}

void CartPage::setCartData(const QJsonArray& cartItems)
{
    m_cartItems = cartItems;
    ui->list_cart->clear();
    m_totalPrice = 0;
    
    for (const auto& item : cartItems) {
        QJsonObject obj = item.toObject();
        addCartItem(obj);
    }
    
    updateTotalPrice();
}

void CartPage::addCartItem(const QJsonObject& item)
{
    QString title = item["title"].toString();
    int price = item["price"].toInt();
    QString seller = item["seller"].toString();
    int itemId = item["id"].toInt();
    
    QWidget* itemWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(itemWidget);
    layout->setContentsMargins(5, 5, 5, 5);
    
    QLabel* infoLabel = new QLabel(QString("%1\nSeller: %2").arg(title).arg(seller));
    QLabel* priceLabel = new QLabel(QString("%1 Toman").arg(price));
    
    QPushButton* removeBtn = new QPushButton("🗑️");
    removeBtn->setFixedSize(30, 30);
    removeBtn->setProperty("itemId", itemId);
    
    connect(removeBtn, &QPushButton::clicked, [this, itemId]() {
        emit removeItemRequested(itemId);
    });
    
    layout->addWidget(infoLabel, 3);
    layout->addWidget(priceLabel, 1);
    layout->addWidget(removeBtn);
    
    QListWidgetItem* listItem = new QListWidgetItem(ui->list_cart);
    listItem->setSizeHint(itemWidget->sizeHint());
    ui->list_cart->addItem(listItem);
    ui->list_cart->setItemWidget(listItem, itemWidget);
    
    m_totalPrice += price;
}

void CartPage::updateTotalPrice()
{
    ui->lbl_total_value->setText(QString("%1 Toman").arg(m_totalPrice));
}

void CartPage::clear()
{
    m_cartItems = QJsonArray();
    ui->list_cart->clear();
    m_totalPrice = 0;
    updateTotalPrice();
}

void CartPage::on_btn_back_clicked()
{
    emit backRequested();
}

void CartPage::on_btn_clear_clicked()
{
    if (m_cartItems.isEmpty()) return;
    
    if (QMessageBox::question(this, "Clear Cart", "Are you sure you want to clear your cart?",
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        emit clearCartRequested();
        clear();
    }
}

void CartPage::on_btn_clear_2_clicked()
{
    if (m_cartItems.isEmpty()) {
        QMessageBox::warning(this, "Empty Cart", "Your cart is empty.");
        return;
    }
    emit checkoutRequested();
}