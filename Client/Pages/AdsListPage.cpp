#include "AdsListPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

AdsListPage::AdsListPage(QWidget *parent) : QWidget(parent)
{
    setupUi();
    retranslateUi();
    connect(searchButton, &QPushButton::clicked, this, &AdsListPage::onSearchButtonClicked);
    connect(btn_apply_filter, &QPushButton::clicked, this, &AdsListPage::onApplyFilterClicked);
    
    lbl_img_1->installEventFilter(this);
    lbl_info_1->installEventFilter(this);
    lbl_img_2->installEventFilter(this);
    lbl_info_2->installEventFilter(this);
}

AdsListPage::~AdsListPage() {}

void AdsListPage::setupUi()
{
    searchLabel = new QLabel(this);
    searchLabel->setObjectName("searchLabel");
    searchLabel->setGeometry(110, 40, 121, 31);
    
    searchButton = new QPushButton(this);
    searchButton->setObjectName("searchButton");
    searchButton->setGeometry(70, 90, 51, 20);
    
    searchAds = new QLineEdit(this);
    searchAds->setObjectName("searchAds");
    searchAds->setGeometry(130, 90, 171, 20);
    
    comboBox = new QComboBox(this);
    comboBox->setObjectName("comboBox");
    comboBox->setGeometry(70, 120, 231, 21);
    
    lbl_price_from = new QLabel(this);
    lbl_price_from->setObjectName("lbl_price_from");
    lbl_price_from->setGeometry(70, 150, 51, 41);
    
    txt_price_from = new QLineEdit(this);
    txt_price_from->setObjectName("txt_price_from");
    txt_price_from->setGeometry(130, 160, 71, 16);
    
    toLabel = new QLabel(this);
    toLabel->setObjectName("toLabel");
    toLabel->setGeometry(210, 160, 21, 20);
    
    txt_price_to = new QLineEdit(this);
    txt_price_to->setObjectName("txt_price_to");
    txt_price_to->setGeometry(230, 160, 71, 16);
    
    btn_apply_filter = new QPushButton(this);
    btn_apply_filter->setObjectName("btn_apply_filter");
    btn_apply_filter->setGeometry(70, 190, 231, 20);
    
    lbl_ads_title = new QLabel(this);
    lbl_ads_title->setObjectName("lbl_ads_title");
    lbl_ads_title->setGeometry(70, 220, 81, 16);
    
    frame = new QFrame(this);
    frame->setObjectName("frame");
    frame->setGeometry(70, 250, 111, 191);
    frame->setFrameShape(QFrame::StyledPanel);
    
    lbl_img_1 = new QLabel(frame);
    lbl_img_1->setObjectName("lbl_img_1");
    lbl_img_1->setGeometry(10, 10, 91, 81);
    lbl_img_1->setScaledContents(true);
    
    lbl_info_1 = new QLabel(frame);
    lbl_info_1->setObjectName("lbl_info_1");
    lbl_info_1->setGeometry(20, 90, 81, 81);
    
    lbl_sold_1 = new QLabel(frame);
    lbl_sold_1->setObjectName("lbl_sold_1");
    lbl_sold_1->setGeometry(40, 170, 37, 12);
    lbl_sold_1->setVisible(false);
    
    frame_2 = new QFrame(this);
    frame_2->setObjectName("frame_2");
    frame_2->setGeometry(190, 249, 121, 191);
    frame_2->setFrameShape(QFrame::StyledPanel);
    
    lbl_img_2 = new QLabel(frame_2);
    lbl_img_2->setObjectName("lbl_img_2");
    lbl_img_2->setGeometry(10, 10, 101, 91);
    lbl_img_2->setScaledContents(true);
    
    lbl_info_2 = new QLabel(frame_2);
    lbl_info_2->setObjectName("lbl_info_2");
    lbl_info_2->setGeometry(6, 101, 111, 81);
    
    lbl_sold_2 = new QLabel(frame_2);
    lbl_sold_2->setObjectName("lbl_sold_2");
    lbl_sold_2->setGeometry(40, 170, 37, 12);
    lbl_sold_2->setVisible(false);
}

void AdsListPage::retranslateUi()
{
    searchLabel->setText(tr(" 🔍 Online Shop - Search Ads  "));
    searchButton->setText(tr("Search"));
    searchAds->setPlaceholderText(tr("Search ads..."));
    lbl_price_from->setText(tr("Price From:"));
    toLabel->setText(tr("To:"));
    btn_apply_filter->setText(tr("Apply"));
    lbl_ads_title->setText(tr("📋 Approved Ads:"));
    lbl_info_1->setText(tr("📱 Samsung A53\nDual SIM\n256GB\n8GB RAM\n💰 $450"));
    lbl_sold_1->setText(tr("Sold❌"));
    lbl_info_2->setText(tr("📚 Harry Potter Collection\nBoxed Edition\nby J. K. Rowling\nForest Publishing\n7 volumes💰 $30"));
    lbl_sold_2->setText(tr("Sold❌"));
    
    comboBox->clear();
    comboBox->addItem(tr("All categories"));
    comboBox->addItem(tr("💻 Digital Goods"));
    comboBox->addItem(tr("🏠 Home & Kitchen"));
    comboBox->addItem(tr("🔧 Household Appliances"));
    comboBox->addItem(tr("💄 Beauty"));
    comboBox->addItem(tr("👗 Fashion & Clothing"));
    comboBox->addItem(tr("💍 Gold & Silver"));
    comboBox->addItem(tr("🚗 Cars & Motorcycles"));
    comboBox->addItem(tr("🏥 Health & Medical"));
    comboBox->addItem(tr("📚 Books & Stationery"));
    comboBox->addItem(tr("🛒 Online Supermarket"));
    comboBox->addItem(tr("🧸 Toys"));
}

void AdsListPage::setAdSold(int index, bool sold)
{
    if (index == 0) lbl_sold_1->setVisible(sold);
    else if (index == 1) lbl_sold_2->setVisible(sold);
}

void AdsListPage::onSearchButtonClicked()
{
    emit searchClicked(searchAds->text());
}

void AdsListPage::onApplyFilterClicked()
{
    QString category = comboBox->currentText();
    double minPrice = txt_price_from->text().toDouble();
    double maxPrice = txt_price_to->text().toDouble();
    emit filterApplied(category, minPrice, maxPrice);
}

bool AdsListPage::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == lbl_img_1 || obj == lbl_info_1) {
            emit adClicked(0);
            return true;
        } else if (obj == lbl_img_2 || obj == lbl_info_2) {
            emit adClicked(1);
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}