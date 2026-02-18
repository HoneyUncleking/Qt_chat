#include "PostAdPage.h"
#include "ui_PostAdPage.h"
#include <QFileDialog>
#include <QMessageBox>

PostAdPage::PostAdPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PostAdPage)
    , m_titleValid(false)
    , m_priceValid(false)
    , m_descriptionValid(false)
    , m_categorySelected(false)
{
    ui->setupUi(this);
    ui->txt_price->setValidator(new QIntValidator(1000, 1000000000, this));
    updatePostButtonState();
    
    connect(ui->txt_title, &QLineEdit::textChanged, this, &PostAdPage::validateInputs);
    connect(ui->txt_price, &QLineEdit::textChanged, this, &PostAdPage::validateInputs);
    connect(ui->txt_description, &QTextEdit::textChanged, this, &PostAdPage::validateInputs);
    connect(ui->cmb_category, &QComboBox::currentTextChanged, this, &PostAdPage::validateInputs);
}

PostAdPage::~PostAdPage()
{
    delete ui;
}

bool PostAdPage::validateTitle(const QString& title)
{
    return title.trimmed().length() >= 5 && title.length() <= 100;
}

bool PostAdPage::validatePrice(const QString& price)
{
    if (price.isEmpty()) return false;
    bool ok;
    int value = price.toInt(&ok);
    return ok && value >= 1000 && value <= 1000000000;
}

bool PostAdPage::validateDescription(const QString& description)
{
    return description.trimmed().length() >= 10;
}

void PostAdPage::setFieldValid(QWidget* field, bool valid)
{
    QString style;
    if (valid) {
        style = "border: 2px solid #27ae60;";
    } else {
        style = "border: 2px solid #dc3545;";
    }
    
    if (qobject_cast<QLineEdit*>(field)) {
        field->setStyleSheet("QLineEdit { " + style + " }");
    } else if (qobject_cast<QTextEdit*>(field)) {
        field->setStyleSheet("QTextEdit { " + style + " }");
    } else if (qobject_cast<QComboBox*>(field)) {
        field->setStyleSheet("QComboBox { " + style + " }");
    }
}

void PostAdPage::updatePostButtonState()
{
    m_titleValid = validateTitle(ui->txt_title->text());
    m_priceValid = validatePrice(ui->txt_price->text());
    m_descriptionValid = validateDescription(ui->txt_description->toPlainText());
    m_categorySelected = (ui->cmb_category->currentIndex() > 0);
    
    setFieldValid(ui->txt_title, m_titleValid);
    setFieldValid(ui->txt_price, m_priceValid);
    setFieldValid(ui->txt_description, m_descriptionValid);
    setFieldValid(ui->cmb_category, m_categorySelected);
    
    bool allValid = m_titleValid && m_priceValid && m_descriptionValid && m_categorySelected;
    ui->btn_post_ad->setEnabled(allValid);
}

void PostAdPage::validateInputs()
{
    updatePostButtonState();
}

void PostAdPage::on_btn_choose_image_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Image",
        QDir::homePath(), "Images (*.png *.jpg *.jpeg *.bmp)");
    
    if (!fileName.isEmpty()) {
        m_currentImagePath = fileName;
        QPixmap image(fileName);
        if (!image.isNull()) {
            ui->lbl_image_preview->setPixmap(image.scaled(171, 61, Qt::KeepAspectRatio));
        }
    }
}

void PostAdPage::on_btn_post_ad_clicked()
{
    if (!ui->btn_post_ad->isEnabled()) return;
    
    QJsonObject adData;
    adData["title"] = ui->txt_title->text().trimmed();
    adData["category"] = ui->cmb_category->currentText();
    adData["price"] = ui->txt_price->text().toInt();
    adData["description"] = ui->txt_description->toPlainText().trimmed();
    adData["imagePath"] = m_currentImagePath;
    
    emit postAdRequested(adData);
}

void PostAdPage::on_btn_back_clicked()
{
    emit backRequested();
}

void PostAdPage::clear()
{
    ui->txt_title->clear();
    ui->txt_price->clear();
    ui->txt_description->clear();
    ui->cmb_category->setCurrentIndex(0);
    m_currentImagePath.clear();
    ui->lbl_image_preview->clear();
    updatePostButtonState();
}