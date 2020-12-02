#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      isModified(false),
      m_scaleFactor(1.0),
      m_darkmode(false),
      ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    if(QLocale::system().name().contains("de") ){
        on_action_Deutsch_triggered();
    } else {
        on_action_Englisch_triggered();
    }
    qDebug() << "system locale: "<< QLocale::system().name();
    //read settings
    readSettings();
    createActions();
    createMenue();


    setAcceptDrops(true);



    // Verknüpfung von signalMainImage und Label
    connect(this,SIGNAL(signalMainImage(QImage*)),&dialogWindow,SLOT(slotMainImage(QImage*)));
    connect(&dialogWindow,SIGNAL(signalOkPressed()),this,SLOT(slotOkPressed()));
    connect(this,SIGNAL(signalMainLabel(QLabel*)),&dialogWindow,SLOT(slotMainLabel(QLabel*)));
    connect(&dialogWindow,SIGNAL(signalChanged()),this,SLOT(slotChanged()));

    switchIcon(false);
    setTitleBar("Willkommen bei QT Image Manipulator");
    setStatusBar("Willkommen bei QT Image Manipulator",5000);

    for (int i  = 1; i < QImageReader::supportedMimeTypes().size(); ++i)
    {
        m_imageReaderFilter << QImageReader::supportedMimeTypes().at(i).data();
    }
    m_imageReaderFilter << "application/octet-stream";


    for (int i  = 1; i < QImageReader::supportedImageFormats().size(); ++i)
    {
        m_imageReaderFilterSuffix << QImageReader::supportedImageFormats().at(i).data();
    }
    m_imageReaderFilter << "application/octet-stream";


    for (int i  = 1; i < QImageWriter::supportedMimeTypes().size(); ++i)
    {
        m_imageWriterFilter << QImageWriter::supportedMimeTypes().at(i).data();
    }
    m_imageWriterFilter << "application/octet-stream";


    if (!m_filePathNamePrefix.isEmpty()) ReadAndLoad(m_filePathNamePrefix);

    m_pMainLabel = ui->labelMain;
    setTitleBar("Willkommen bei QT Image Manipulator");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::closeEvent (QCloseEvent *event)
{
    writeSettings();
    if( maybeSave() ) {
        event->accept();
    } else {
        event->ignore();
    }

}


void MainWindow::setStatusBar(QString text,int timer)
{
    ui->statusbar->showMessage(text,timer);

}

void MainWindow::setTitleBar(QString text)
{
    this->setWindowTitle(text);
}

QString MainWindow::getTitelBar()
{
    return windowTitle();
}


void MainWindow::on_actionE_xit_triggered()
{
    writeSettings();
    if( maybeSave() ) {
        QApplication::quit();;
    } else {
        return;
    }

}

void MainWindow::on_action_Open_triggered()
{

    if( !maybeSave() ) {
        return;
    }

    QFileDialog dialog(this);
    dialog.setMimeTypeFilters(m_imageReaderFilter);
    dialog.selectMimeTypeFilter("application/octet-stream");
    dialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).last());

    QStringList imageFiles;
    if (dialog.exec()) imageFiles = dialog.selectedFiles();
    if (!imageFiles.isEmpty()){
        ReadAndLoad(imageFiles[0]);
    }
}

void MainWindow::switchIcon(bool change)
{
    ui->action_Save->setEnabled(change);
    ui->actionSave_as->setEnabled(change);
    ui->action_Print->setEnabled(change);
    ui->action_Copy->setEnabled(change);
    ui->action_Zoom->setEnabled(change);
    ui->actionZoom_in->setEnabled(change);
    ui->actionZoom_out->setEnabled(change);
    ui->action_Fit_to_Windows->setEnabled(change);
    ui->action_Show_Dialog->setEnabled(change);
}


void MainWindow::ReadAndLoad(QString filePathNamePrefix, bool fileChanged){
    QFileInfo fi(filePathNamePrefix);
    if (!filePathNamePrefix.isEmpty()){
        QImageReader reader(filePathNamePrefix);
        QImage image = reader.read();
        if (image.isNull())
        {
            QMessageBox::critical(this, tr("Image Viewer"),tr("Cannot load %1\n%2").arg(m_filePathNamePrefix).arg(tr("May be you don't have write access")) );

        }else
        {
            alteration(false);
            m_filePathNamePrefix = filePathNamePrefix;
            m_baseName = fi.baseName();
            m_suffixName = fi.suffix();
            m_mainImage = image;
            setTitleBar(m_baseName);
            setStatusBar(m_filePathNamePrefix,5000);
            ui->labelMain->setPixmap(QPixmap::fromImage(image));
            switchIcon(true);
        }
    }
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox::about(this,tr("About the Creator"),tr("Hier könnte Ihre Werbung stehen."
                                                       "Und das ganz günstig!!"
                                                       "Call 999.999.QT"
                                                       ));
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this,tr("About Qt"));
}

void MainWindow::setModified(bool modified)
{
    isModified = modified;
}

void MainWindow::scaling(double factor)
{
    if(factor*100 <=25){
        ui->actionZoom_out->setEnabled(false);
        return;
    }
    if(factor*100 >= 400){
        ui->actionZoom_in->setEnabled(false);
        return;
    }
    ui->actionZoom_out->setEnabled(true);
    ui->actionZoom_in->setEnabled(true);
    scaleZoom(factor);

}

void MainWindow::scaleZoom(double factor){
    QPixmap pixmap = QPixmap::fromImage(m_mainImage).scaled(m_mainImage.size() * factor, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->labelMain->setPixmap(pixmap);

    m_scaleFactor = factor;
    int cutDouble = factor*100;
    setStatusBar(QString::number(cutDouble)+"%");
}

void MainWindow::on_action_Print_triggered()
{
    QPrinter printer;

    QPrintDialog printDialog(&printer,this);

    if (printDialog.exec() == QDialog::Rejected) {
        return;
    }

    QPainter painter(&printer);
    QRect rect = painter.viewport();
    QSize size = ui->labelMain->pixmap()->size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(ui->labelMain->pixmap()->rect());
    painter.drawPixmap(0, 0, *ui->labelMain->pixmap());
}

bool MainWindow::maybeSave()
{
    if( this->isModified ) {

        auto ret = QMessageBox::question( this, "QTWidgetImageManipulator",
                                          tr("The file was modified?\n" "Do you want to save it?"),
                                          QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
                                          );

        if( ret==QMessageBox::Save ) {

            save();

        } else if( ret==QMessageBox::Discard ) {

            return true;

        } else {

            return false;

        }
    }
    return true;
}

void MainWindow::save()
{


    QImage image  = ui->labelMain->pixmap()->toImage();
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setMimeTypeFilters(m_imageWriterFilter);
    dialog.setConfirmOverwrite (true);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setWindowIconText(tr("Save Image File"));
    QString imageFile;

    if (dialog.exec() == QDialog::Accepted)
    {
        QStringList files = dialog.selectedFiles();
        m_filePathNamePrefix = files[0];
        this->on_action_Save_triggered();

    }
}

void MainWindow::on_actionSave_as_triggered()
{

    save();

}

void MainWindow::on_action_Save_triggered()
{

    if(m_mainImage.save(m_filePathNamePrefix) == false)
    {
        QMessageBox::information(this,tr("Save Error"),tr("There went something wrong. Pls check write permissions."));
        return;
    }
    m_history << m_filePathNamePrefix;
    updateRecentFileActions();
    if (isModified) alteration(false);

}

void MainWindow::alteration(bool change)
{
    // Changes happen
    if (change) {
        setTitleBar(getTitelBar()+ "*");
        setModified(true);
        return;
    }

    // Changes NOT happen
    QString cutStar = getTitelBar();cutStar.chop(1);setTitleBar(cutStar);
    setModified(false);
    return;

}

void MainWindow::writeSettings()
{

    QSettings settings("Metalsoft","QtWidgetImageManipulator");
    settings.setValue("mainwindow/size", this->size());
    settings.setValue("mainwindow/fullScreen", this->isFullScreen());
    settings.setValue("mainwindow/darkMode", m_darkmode);
    settings.setValue("mainwindow/pos", this->pos());
    settings.setValue("others/image", m_filePathNamePrefix);
    settings.setValue("others/history", QVariant::fromValue(m_history));
    settings.sync();
}

void MainWindow::readSettings()
{
    QSettings settings("Metalsoft","QtWidgetImageManipulator");
    this->resize(settings.value("mainwindow/size", QSize(400, 400)).toSize());
    if (settings.value("mainwindow/fullScreen",false).toBool()) this->showFullScreen();
    this->move(settings.value("mainwindow/pos", QPoint(200, 200)).toPoint());
    if (settings.value("mainwindow/darkMode",false).toBool()) switchTheme(true);
    m_filePathNamePrefix = settings.value("others/image").toString();
    m_history = settings.value("others/history").toStringList();
}


void MainWindow::on_actionZoom_in_triggered()
{
    scaling(m_scaleFactor * 1.25);
}

void MainWindow::on_actionZoom_out_triggered()
{
    scaling(m_scaleFactor * 0.8);
}

void MainWindow::on_action_Zoom_triggered()
{
    scaling(1.0);
}

void MainWindow::on_action_Fit_to_Windows_triggered()
{
    double width = static_cast<double>(ui->scrollArea->width()) / m_mainImage.width();
    double height = static_cast<double>(ui->scrollArea->height()) / m_mainImage.height();
    // So that the picture fits with the smalest side
    double border = 0.95;
    // There is Space between QLabel and the Frame. So we reduce it a little bit
    // If we know the ratio to the resolution we can make it flexible
    scaling(qMin(height,width)*border);
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;
    QPoint numSteps = numDegrees / 15;

    if (numSteps.y() > 0) {
        on_actionZoom_in_triggered();
    } else {
        on_actionZoom_out_triggered();
    }
    event->accept();
}

void MainWindow::scrollWithPixels(const QPoint &pixel)
{
    int s = pixel.y();
    double z = std::pow(1.01, s);
    scaling(z);
}

void MainWindow::scrollWithDegrees(const QPoint &step)
{
    int s = step.y();
    double z = std::pow(1.1, s);
    scaling(z);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{

    if (event->button() == Qt::LeftButton)
    {
        _panStartX = event->x();
        _panStartY = event->y();
        setCursor(Qt::OpenHandCursor);
        event->accept();
        return;
    }
    event->ignore();
    setCursor(Qt::ClosedHandCursor);
    event->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // verschiebt leider die ganze scrollArea d.h. Anpassung der Bars *säufz*
    // ui->scrollArea->scroll(event->x()-_panStart.x(),event->y()-_panStart.y());
    ui->scrollArea->horizontalScrollBar()->setValue(ui->scrollArea->horizontalScrollBar()->value() - (event->x() - _panStartX));
    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->value() - (event->y() - _panStartY));
    _panStartX = event->x();
    _panStartY = event->y();
    event->accept();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    if( event->mimeData()->hasImage() || event->mimeData()->hasFormat( "text/uri-list" ) )
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent( QDropEvent* event )
{
    getImagesFromMimeData( event->mimeData() );
    event->acceptProposedAction();
}


void MainWindow::getImagesFromMimeData( const QMimeData* mimeData )
{
    QFileInfo f;
    QFileInfoList finalDropData;
    if( mimeData->hasUrls() )
    {
        for (auto c : mimeData->urls()){
            f.setFile(c.toLocalFile());
            for (auto suffix : m_imageReaderFilterSuffix) {
                if (f.suffix() == suffix){
                    finalDropData << f;
                }
            }
        }
    }

    if (finalDropData.isEmpty()) return;
    ReadAndLoad(finalDropData.last().filePath());
}


void MainWindow::on_action_Paste_triggered()
{
    QClipboard* clipboard = QApplication::clipboard();
    getImagesFromMimeData(clipboard->mimeData());
}


void MainWindow::on_action_Copy_triggered()
{
    QClipboard* clipboard = QApplication::clipboard();
    if(maybeSave()){
        clipboard->setImage(m_mainImage);
    }
}

void MainWindow::on_action_Show_Dialog_triggered()
{

    emit signalMainImage(&m_mainImage);
    emit signalMainLabel(m_pMainLabel);
    dialogWindow.setModal(false);
    dialogWindow.show();
}


void MainWindow::slotOkPressed()
{
    QImage image = ui->labelMain->pixmap()->toImage();
    m_mainImage = image;
}


// Recent Files

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadFile(action->data().toString());
}

void MainWindow::loadFile(const QString &fileName)
{
    ReadAndLoad(fileName);
}

void MainWindow::createMenue()
{
    separatorAct = ui->menu_File->addSeparator();
    rescentFiles = ui->menu_File->addMenu(tr("recent&Files"));
    for (int i = 0; i < MaxRecentFiles; ++i)
        rescentFiles->addAction(recentFileActs[i]);
    updateRecentFileActions();
}

void MainWindow::updateRecentFileActions()
{

    int j = 0;
    int numRecentFiles = qMin(m_history.size(), (int)MaxRecentFiles);

    for (int i= m_history.size();i>m_history.size()-numRecentFiles;i--) {
        QString text = tr("&%1 %2").arg(j + 1).arg(strippedName(m_history[i-1]));
        recentFileActs[j]->setText(text);
        recentFileActs[j]->setData(m_history[i-1]);
        recentFileActs[j]->setVisible(true);
        j++;
    }

    for (int k = numRecentFiles; k < MaxRecentFiles; ++k)
        recentFileActs[k]->setVisible(false);

    separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


void MainWindow::createActions()
{
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }
}


void MainWindow::loadLanguage(QString language){

    qApp->removeTranslator(&m_translator);

    if(language == tr("de_DE")){
        m_translator.load(":/languages/TranslationExample_de.qm");
    }else{
        //en_GB
        m_translator.load(":/languages/TranslationExample_en.qm");
    }
    if(!qApp->installTranslator(&m_translator)){
        qDebug() << "failes to install translator";
    } else  {
        qDebug() << "faild to load rLocale " << language;
    }
    statusBar()->showMessage(tr("Current Language changed to %1").arg(language));
}

void MainWindow::changeEvent(QEvent *event){
    if(0 != event){
        switch (event->type()) {
        //this event is send if a translator is loaded
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            dialogWindow.retranslateUI();
            break;
            //this event is send if a translator is loaded
        case QEvent::LocaleChange:
        {
            QString locale = QLocale::system().name();
            locale.truncate(locale.lastIndexOf('_'));
            loadLanguage(locale);
        }
            break;
        }
    }
}
void MainWindow::on_action_Deutsch_triggered()
{
    QString title = QWidget::windowTitle();
    loadLanguage(tr("de_DE"));
    setWindowTitle(title);
    ui->action_Deutsch->setChecked(true);
    ui->action_Englisch->setChecked(false);
}

void MainWindow::on_action_Englisch_triggered()
{
    QString title = QWidget::windowTitle();
    loadLanguage(tr("en_GB"));
    setWindowTitle(title);
    ui->action_Deutsch->setChecked(false);
    ui->action_Englisch->setChecked(true);
}

void MainWindow::switchTheme(bool value){

    if (value)
    {
        // Load an application style
        QFile styleFile(DARKMODE_QSS_FILE_PATH);
        styleFile.open( QFile::ReadOnly );

        // Apply the loaded stylesheet
        QString style( styleFile.readAll() );
        qApp->setStyleSheet(style);
        m_darkmode = true;
    }
    else{
        // Load an application style
        QFile styleFile(WHITEMODE_QSS_FILE_PATH);
        styleFile.open( QFile::ReadOnly );

        // Apply the loaded stylesheet
        QString style( styleFile.readAll() );
        qApp->setStyleSheet(style);
        m_darkmode = false;
         }
}

void MainWindow::on_action_DarkMode_triggered()
{
    switchTheme(!m_darkmode);
}

void MainWindow::slotChanged(){

    if (!isModified) alteration(true);
}
