#ifndef Dialog_H
#define Dialog_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtPrintSupport>
#include <Qt>


#include <colorPick.h>
#include <cmath>


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void retranslateUI();

    QString origFilePath="";
    QString fileSuffix="";

    bool imageManipulated = false;
    bool radioButtonRGBEnabled = false;

    void calculateRGB();
    void calculateYUV();
    bool checkValues();
public slots:
    void slotMainImage(QImage *image);
    void slotMainLabel(QLabel *label);
    void slotRefreshPix();

signals:
    void signalChanged();
    void signalOkPressed();
    void signalRefreshPix();

private slots:
    void on_radioButtonRGB_pressed();

    void on_radioButtonYUV_pressed();

    void on_sliderRGB_R_valueChanged(int value);

    void on_sliderRGB_G_valueChanged(int value);

    void on_sliderRGB_B_valueChanged(int value);

    void on_sliderYUV_Y_valueChanged(int value);

    void on_sliderYUV_U_valueChanged(int value);

    void on_sliderYUV_V_valueChanged(int value);

    void on_spinBoxRGB_R_valueChanged(int value);

    void on_spinBoxRGB_G_valueChanged(int value);

    void on_spinBoxRGB_B_valueChanged(int value);

    void on_spinBoxYUV_Y_valueChanged(int value);

    void on_spinBoxYUV_U_valueChanged(int value);

    void on_spinBoxYUV_V_valueChanged(int value);

    void on_pushButton_ok_pressed();

    void on_pushButton_preview_pressed();

    void on_pushButton_cancel_pressed();

    void on_Reset_released();

private:
    Ui::Dialog *ui;
    void enableRGB(bool value);
    void enableYUV(bool value);
    void resetSlider();
    QImage *m_pMainImage;
    QLabel *m_pMainLabel;
    QImage m_pChangedImage;
    QImage *m_pRefeshPix;
    ColorPick config;
    bool m_previewToggle;

};

#endif // Dialog_H
