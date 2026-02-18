#ifndef POSTADPAGE_H
#define POSTADPAGE_H

#include <QWidget>
#include <QJsonObject>
#include <QPixmap>
#include <QIntValidator>

QT_BEGIN_NAMESPACE
namespace Ui { class PostAdPage; }
QT_END_NAMESPACE

class PostAdPage : public QWidget {
    Q_OBJECT

public:
    explicit PostAdPage(QWidget *parent = nullptr);
    ~PostAdPage();
    void clear();

signals:
    void backRequested();
    void postAdRequested(const QJsonObject& adData);

private slots:
    void on_btn_back_clicked();
    void on_btn_post_ad_clicked();
    void on_btn_choose_image_clicked();
    void validateInputs();

private:
    void updatePostButtonState();
    void setFieldValid(QWidget* field, bool valid);
    bool validateTitle(const QString& title);
    bool validatePrice(const QString& price);
    bool validateDescription(const QString& description);
    
    Ui::PostAdPage *ui;
    bool m_titleValid;
    bool m_priceValid;
    bool m_descriptionValid;
    bool m_categorySelected;
    QString m_currentImagePath;
};

#endif