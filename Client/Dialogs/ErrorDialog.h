#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ErrorDialog; }
QT_END_NAMESPACE

class ErrorDialog : public QDialog {
    Q_OBJECT

public:
    explicit ErrorDialog(QWidget *parent = nullptr);
    ~ErrorDialog();

    void setErrorMessage(const QString& title, const QString& message);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ErrorDialog *ui;
};

#endif