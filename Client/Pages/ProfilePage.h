#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#include <QWidget>
#include <QJsonObject>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class ProfilePage; }
QT_END_NAMESPACE

class ProfilePage : public QWidget {
    Q_OBJECT

public:
    explicit ProfilePage(QWidget *parent = nullptr);
    ~ProfilePage();
    void setProfileData(const QJsonObject& profileData);
    void clear();

signals:
    void backRequested();
    void editRequested();
    void historyRequested();
    void walletRequested();

private slots:
    void on_btn_back_clicked();
    void on_btn_edit_clicked();
    void on_btn_history_clicked();
    void on_btn_wallet_clicked();

private:
    Ui::ProfilePage *ui;
    QJsonObject m_profileData;
};

#endif