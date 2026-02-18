#ifndef ADSLISTPAGE_H
#define ADSLISTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QFrame>
#include <QEvent>

class AdsListPage : public QWidget {
    Q_OBJECT

public:
    explicit AdsListPage(QWidget *parent = nullptr);
    ~AdsListPage();
    void setupUi();
    void retranslateUi();
    void setAdSold(int index, bool sold);

signals:
    void searchClicked(const QString& text);
    void filterApplied(const QString& category, double minPrice, double maxPrice);
    void adClicked(int adIndex);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onSearchButtonClicked();
    void onApplyFilterClicked();

private:
    QLabel *searchLabel;
    QPushButton *searchButton;
    QLineEdit *searchAds;
    QComboBox *comboBox;
    QLabel *lbl_price_from;
    QLineEdit *txt_price_from;
    QLabel *toLabel;
    QLineEdit *txt_price_to;
    QPushButton *btn_apply_filter;
    QLabel *lbl_ads_title;
    QFrame *frame;
    QLabel *lbl_img_1;
    QLabel *lbl_info_1;
    QLabel *lbl_sold_1;
    QFrame *frame_2;
    QLabel *lbl_img_2;
    QLabel *lbl_info_2;
    QLabel *lbl_sold_2;
};

#endif