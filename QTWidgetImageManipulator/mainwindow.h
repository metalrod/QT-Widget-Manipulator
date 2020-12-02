#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define DARKMODE_QSS_FILE_PATH ":/qss/darkMode.qss"
#define WHITEMODE_QSS_FILE_PATH ":/qss/whiteMode.qss"

#include <QMainWindow>
#include <dialog.h>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtPrintSupport>
#include <Qt>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void save();
    bool maybeSave();

    ~MainWindow();

    void ReadAndLoad(QString filePathNamePrefix, bool fileChanged=false);

    void switchIcon(bool change);
    void setStatusBar(QString text, int timer=0);
    void setTitleBar(QString text);
    QString getTitelBar();
    void alteration(bool change);
    void setModified(bool modified);
    void scaling(double value);
    void writeSettings();
    void readSettings();

    void wheelEvent(QWheelEvent *event);
    void scrollWithPixels(const QPoint &pixel);
    void scrollWithDegrees(const QPoint &step);

    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void getImagesFromMimeData(const QMimeData *mimeData);

    void on_actionShow_Dialog_triggered();
    void switchTheme(bool value);

    void changeEvent(QEvent *event);
    void scaleZoom(double factor);



signals:
    void signalMainLabel(QLabel *image);
    void signalMainImage(QImage *label);

public slots:
    void slotChanged();
    void slotOkPressed();
    void loadLanguage(QString language);

private slots:

    void mousePressEvent(QMouseEvent *event);

    void on_actionE_xit_triggered();

    void on_action_Open_triggered();

    void on_action_About_triggered();

    void on_actionAbout_Qt_triggered();

    void closeEvent(QCloseEvent *event);

    void on_action_Print_triggered();

    void on_action_Save_triggered();

    void on_actionSave_as_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_action_Zoom_triggered();

    void on_action_Fit_to_Windows_triggered();

    void on_action_Paste_triggered();

    void on_action_Copy_triggered();

    void on_action_Show_Dialog_triggered();

    //Rescent Files
    void openRecentFile();

    void on_action_Deutsch_triggered();

    void on_action_Englisch_triggered();

    void on_action_DarkMode_triggered();

private:
    bool isModified;
    Ui::MainWindow *ui;
    QStringList m_imageWriterFilter;
    QStringList m_imageReaderFilter;
    QStringList m_imageReaderFilterSuffix;
    QString m_filePathNamePrefix;
    QString m_baseName, m_suffixName;
    QImage m_mainImage;
    double m_scaleFactor;
    QStringList m_history;
    int _panStartX,_panStartY;
    Dialog dialogWindow;
    QLabel *m_pMainLabel;

    //Rescent Files
    void createActions();
    QString strippedName(const QString &fullFileName);
     void updateRecentFileActions();
    void loadFile(const QString &fileName);

    QMenu *rescentFiles;
    QAction *separatorAct;
    void createMenue();
    enum { MaxRecentFiles = 10 };
    QAction *recentFileActs[MaxRecentFiles];
    QTranslator m_translator;
    bool m_darkmode;
};
#endif // MAINWINDOW_H
