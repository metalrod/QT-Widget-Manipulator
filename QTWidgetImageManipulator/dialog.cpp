#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"






Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    m_previewToggle(true),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(this,SIGNAL(signalRefreshPix()),this,SLOT(slotRefreshPix()));
    enableRGB(true);
}

Dialog::~Dialog()
{

    delete ui;
}

void Dialog::enableRGB(bool enable)
{
    radioButtonRGBEnabled = enable;

    ui->radioButtonRGB->setChecked(enable);
    ui->sliderRGB_R->setEnabled(enable);
    ui->sliderRGB_G->setEnabled(enable);
    ui->sliderRGB_B->setEnabled(enable);
    ui->spinBoxRGB_R->setEnabled(enable);
    ui->spinBoxRGB_G->setEnabled(enable);
    ui->spinBoxRGB_B->setEnabled(enable);
    enableYUV(!enable);
}
void Dialog::enableYUV(bool enable)
{
    ui->radioButtonRGB->setChecked(enable);
    ui->sliderYUV_Y->setEnabled(enable);
    ui->sliderYUV_Y->setEnabled(enable);
    ui->sliderYUV_U->setEnabled(enable);
    ui->sliderYUV_V->setEnabled(enable);
    ui->spinBoxYUV_Y->setEnabled(enable);
    ui->spinBoxYUV_U->setEnabled(enable);
    ui->spinBoxYUV_V->setEnabled(enable);
}
void Dialog::resetSlider(){

    int reset = 100;
    ui->sliderRGB_R->setValue(reset);
    ui->sliderRGB_G->setValue(reset);
    ui->sliderRGB_B->setValue(reset);
    ui->sliderYUV_Y->setValue(reset);
    ui->sliderYUV_U->setValue(0);
    ui->sliderYUV_V->setValue(0);
}

void Dialog::slotMainImage(QImage *image)
{
    m_pMainImage = image;
    m_pChangedImage = *image;
    m_pRefeshPix = m_pMainImage;
}

void Dialog::slotMainLabel(QLabel *label)
{
    m_pMainLabel = label;
}


void Dialog::on_radioButtonRGB_pressed()
{
    enableRGB(true);
    calculateRGB();
    if (!m_previewToggle)  {
        emit signalRefreshPix();
    }
}
void Dialog::on_radioButtonYUV_pressed()
{
    enableRGB(false);
    calculateYUV();
    if (!m_previewToggle)  {
        emit signalRefreshPix();
    }
}

void Dialog::slotRefreshPix(){
    QPixmap px = QPixmap::fromImage(*m_pRefeshPix);
    m_pMainLabel->setPixmap(px);
}


void Dialog::on_sliderRGB_R_valueChanged(int position)
{
    ui->spinBoxRGB_R->setValue(position);
    imageManipulated = true;
    if (!m_previewToggle)  {
        calculateRGB();
        emit signalRefreshPix();
    }
}


void Dialog::on_sliderRGB_G_valueChanged(int position)
{
    ui->spinBoxRGB_G->setValue(position);
    imageManipulated = true;
    if (!m_previewToggle)  {
        calculateRGB();
        emit signalRefreshPix();
    }
}


void Dialog::on_sliderRGB_B_valueChanged(int position)
{
    ui->spinBoxRGB_B->setValue(position);
    imageManipulated = true;
    if (!m_previewToggle)  {
        calculateRGB();
        emit signalRefreshPix();
    }
}
void Dialog::on_sliderYUV_Y_valueChanged(int position)
{
    ui->spinBoxYUV_Y->setValue(position);
    imageManipulated = true;
    if (!m_previewToggle)  {
        calculateYUV();
        emit signalRefreshPix();
    }
}

void Dialog::calculateRGB(){

    QColor var;
    for(int i = 0; i < m_pMainImage->size().width(); i++)
    {
        for(int j = 0; j < m_pMainImage->size().height(); j++)
        {
            config.setR(ui->sliderRGB_R->value());
            config.setG(ui->sliderRGB_G->value());
            config.setB(ui->sliderRGB_B->value());

            config.setR(qBound(0.0,std::round(m_pMainImage -> pixelColor(i, j).red()*config.getR()/100.), 255.0));
            config.setG(qBound(0.0,std::round(m_pMainImage -> pixelColor(i, j).green()*config.getG()/100.), 255.0));
            config.setB(qBound(0.0,std::round(m_pMainImage -> pixelColor(i, j).blue()*config.getB()/100.), 255.0));

            var.setRed(config.getR());
            var.setGreen(config.getG());
            var.setBlue(config.getB());
            m_pChangedImage.setPixelColor(i, j, var);
        }
    }
}

void Dialog::calculateYUV(){
    QColor var;
    for(int i = 0; i < m_pMainImage->size().width(); i++)
    {
        for(int j = 0; j < m_pMainImage->size().height(); j++)
        {
            config.setY(ui->sliderYUV_Y->value());
            config.setU(ui->sliderYUV_U->value());
            config.setV(ui->sliderYUV_V->value());
            config.calculateYUV();

            config.setR(qBound(0.0,std::round(m_pMainImage -> pixelColor(i, j).red()*config.getCR()/100.), 255.0));
            config.setG(qBound(0.0,std::round(m_pMainImage -> pixelColor(i, j).green()*config.getCG()/100.), 255.0));
            config.setB(qBound(0.0,std::round(m_pMainImage -> pixelColor(i, j).blue()*config.getCB()/100.), 255.0));

            var.setRed(config.getR());
            var.setGreen(config.getG());
            var.setBlue(config.getB());
            m_pChangedImage.setPixelColor(i, j, var);
        }
    }
}

void Dialog::on_sliderYUV_U_valueChanged(int position)
{
    ui->spinBoxYUV_U->setValue(position);
    imageManipulated = true;
    if (!m_previewToggle)  {
        calculateYUV();
        emit signalRefreshPix();
    }
}

void Dialog::on_sliderYUV_V_valueChanged(int position)
{
    ui->spinBoxYUV_V->setValue(position);
    imageManipulated = true;
    if (!m_previewToggle)  {
        calculateYUV();
        emit signalRefreshPix();
    }
}

void Dialog::on_spinBoxRGB_R_valueChanged(int position)
{
    ui->sliderRGB_R->setValue(position);
    imageManipulated = true;
}

void Dialog::on_spinBoxRGB_G_valueChanged(int position)
{
    ui->sliderRGB_G->setValue(position);
    imageManipulated = true;
}

void Dialog::on_spinBoxRGB_B_valueChanged(int position)
{
    ui->sliderRGB_B->setValue(position);
    imageManipulated = true;
}

void Dialog::on_spinBoxYUV_Y_valueChanged(int position)
{
    ui->sliderYUV_Y->setValue(position);
    imageManipulated = true;
}

void Dialog::on_spinBoxYUV_U_valueChanged(int position)
{
    ui->sliderYUV_U->setValue(position);
    imageManipulated = true;
}

void Dialog::on_spinBoxYUV_V_valueChanged(int position)
{
    ui->sliderYUV_V->setValue(position);
    imageManipulated = true;
}

void Dialog::on_pushButton_ok_pressed()
{
    if (ui->radioButtonRGB->isChecked()) {
        calculateRGB();
    } else {
        calculateYUV();
    }
    *m_pMainImage = m_pChangedImage;
    emit signalRefreshPix();

    if (checkValues()) emit signalChanged();
    resetSlider();
    this->close();
}
void Dialog::on_pushButton_preview_pressed()
{
    if (ui->radioButtonRGB->isChecked()) {
        calculateRGB();
    } else {
        calculateYUV();
    }

    if (m_previewToggle){
        m_pRefeshPix = &m_pChangedImage;
        m_previewToggle = false;

        emit signalRefreshPix();
        return;
    }
    QPixmap px = QPixmap::fromImage(*m_pMainImage);
    m_pRefeshPix = m_pMainImage;
    emit signalRefreshPix();
    m_previewToggle = true;
}

void Dialog::on_pushButton_cancel_pressed()
{
    this->close();
}


bool Dialog::checkValues(){

    int count = 0;
    if (ui->radioButtonRGB->isChecked()) {

    count += ui->sliderRGB_R->value();
    count += ui->sliderRGB_G->value();
    count += ui->sliderRGB_B->value();

    if (count != 300) return true;
    }
    else {

    count += ui->sliderYUV_Y->value();
    ui->sliderYUV_U->value(); //0
    ui->sliderYUV_V->value(); //0

    if (count != 100) return true;
    }

    return false;
}

void Dialog::on_Reset_released()
{
    resetSlider();
}

void Dialog::retranslateUI(){
    ui->retranslateUi(this);
}
