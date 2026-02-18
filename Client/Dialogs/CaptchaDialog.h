#ifndef CAPTCHADIALOG_H
#define CAPTCHADIALOG_H

#include <QDialog>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class CaptchaDialog; }
QT_END_NAMESPACE

class CaptchaDialog : public QDialog {
    Q_OBJECT

public:
    explicit CaptchaDialog(QWidget *parent = nullptr);
    ~CaptchaDialog();

    QString getCaptchaCode() const { return m_captchaCode; }

private slots:
    void on_btn_refresh_clicked();
    void on_btn_submit_clicked();

private:
    QString generateCaptcha();
    
    Ui::CaptchaDialog *ui;
    QString m_captchaCode;
};

#endif