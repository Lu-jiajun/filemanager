#include "findwidget.h"
#include "qdebug.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMimeDatabase>
#include <QProgressDialog>
#include <QSettings>
FindWidget::FindWidget(QWidget *parent)
{
    setWindowTitle(tr("Find Files"));
    qDebug()<<"FindWidgetwork";
   // connect(ui->pushButton_5, &QAbstractButton::clicked, this, &::FindWidget::browser);


    loadFindWidgetInfo();

}

enum { absoluteFileNameRole = Qt::UserRole + 1 };
static inline QString fileNameOfItem(const QTableWidgetItem *item)
{
    return item->data(absoluteFileNameRole).toString();
}

void FindWidget::setFindInfo(const QString &path, const QString &find)
{
    //    fileComboBox->addItem(find);
    //    directoryComboBox->addItem(path);
    ui->comboBox_3->lineEdit()->setText(find);
    //textComboBox->lineEdit()->setText("");
    ui->comboBox_3->lineEdit()->setText(path);
}


void FindWidget::browser()
{
    QString directory =ui->comboBox_2->lineEdit()->text();
    if (!QFileInfo::exists(directory)) {
        directory = QDir::currentPath();
    }

    directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Find Files"), directory));
    if (!directory.isEmpty()) {
        if (ui->comboBox_2->findText(directory) == -1)
            ui->comboBox_2->addItem(directory);
        ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findText(directory));
    }
}

static void updateComboBox(QComboBox *comboBox)
{
    if (comboBox->findText(comboBox->currentText()) == -1)
        comboBox->addItem(comboBox->currentText());
}

void FindWidget::find()
{
    ui->tableWidget->setRowCount(0);

    QString fileName = ui->comboBox_3->currentText();
    //QString text = textComboBox->currentText();
    QString path = QDir::cleanPath(ui->comboBox_2->currentText());
    if (path.isEmpty()) {
        qDebug() << "find path empty";
        return;
    }

    currentDir = QDir(path);
    updateComboBox(ui->comboBox_3);
    //updateComboBox(textComboBox);
    updateComboBox(ui->comboBox_2);

    qDebug() << QString("find fileName: %1, path %2").arg(fileName, path);

    QStringList filter;
    if (!fileName.isEmpty())
        filter << fileName;
    //    QDirIterator it(path, filter, QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator it(path, filter, QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QStringList files;
    while (it.hasNext())
        files << it.next();
//        if (!text.isEmpty())
//            files = findFiles(files, text);
    files.sort();
    showFiles(files);

}

void FindWidget::animateFindClick()
{
    ui->pushButton_4->animateClick();
}

QStringList FindWidget::findFiles(const QStringList &files, const QString &text)
{
    QProgressDialog progressDialog(ui->dockWidget_3);
    progressDialog.setCancelButtonText(tr("&Cancel"));
    progressDialog.setRange(0, files.size());
    progressDialog.setWindowTitle(tr("Find Files"));

    QMimeDatabase mimeDatabase;
    QStringList foundFiles;

    for (int i = 0; i < files.size(); ++i) {
        progressDialog.setValue(i);
        progressDialog.setLabelText(tr("Searching file number %1 of %n...", nullptr, files.size()).arg(i));
        QCoreApplication::processEvents();

        if (progressDialog.wasCanceled())
            break;

        const QString fileName = files.at(i);
        const QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileName);
        if (mimeType.isValid() && !mimeType.inherits(QStringLiteral("text/plain"))) {
            //            qWarning() << "Not searching binary file " << QDir::toNativeSeparators(fileName);
            continue;
        }
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            QString line;
            QTextStream in(&file);
            in.setCodec("UTF-8");   // set codec to match Chinese
            while (!in.atEnd()) {
                if (progressDialog.wasCanceled())
                    break;
                line = in.readLine();
                if (line.contains(text, Qt::CaseInsensitive)) {
                    foundFiles << files[i];
                    break;
                }
            }
        }
    }
    return foundFiles;
}

void FindWidget::showFiles(const QStringList &paths)
{
    for (const QString &filePath : paths) {
        const QString toolTip = QDir::toNativeSeparators(filePath);
        const QString relativePath = QDir::toNativeSeparators(currentDir.relativeFilePath(filePath));
        const qint64 size = QFileInfo(filePath).size();
        QTableWidgetItem *fileNameItem = new QTableWidgetItem(relativePath);
        fileNameItem->setData(absoluteFileNameRole, QVariant(filePath));
        fileNameItem->setToolTip(toolTip);
        fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem *sizeItem = new QTableWidgetItem(QLocale().formattedDataSize(size));
        sizeItem->setData(absoluteFileNameRole, QVariant(filePath));
        sizeItem->setToolTip(toolTip);
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, fileNameItem);
        ui->tableWidget->setItem(row, 1, sizeItem);
    }
    if(tablewidgetcount<1){
        ui->tableWidget->setuplable();
        tablewidgetcount++;
    }else{
        ui->tableWidget->freshlable();
    }

    //    filesFoundLabel->setText(tr("%n file(s) found (Double click on a file to open it)", nullptr, paths.size()));
    //    filesFoundLabel->setWordWrap(true);
}

void FindWidget::onCellActivated(int row, int column)
{
    const QTableWidgetItem *item = ui->tableWidget->item(row, 0);
    qDebug() << QString("onCellActivated %1").arg(fileNameOfItem(item));
    emit cellActivated(fileNameOfItem(item));
}

void FindWidget::contextMenu(const QPoint &pos)
{
    const QTableWidgetItem *item = ui->tableWidget->itemAt(pos);
    if (!item)
        return;
    QMenu menu;
    QAction *openAction = menu.addAction(tr("&Open"));
    QAction *openDirAction = menu.addAction(tr("Open Containing &Folder"));

    QAction *action = menu.exec(ui->tableWidget->mapToGlobal(pos));
    if (!action)
        return;

    const QString fileName = fileNameOfItem(item);
    if (action == openAction) {
        emit cellActivated(fileNameOfItem(item));
    } else if (action == openDirAction) {
        QString filename = fileNameOfItem(item);
        emit cellActivated(QFileInfo(filename).absolutePath());
    }
}
QStringList readArraySettings(const QString &group)
{
    QString configFilePath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(configFilePath, QSettings::IniFormat);

    QStringList list;
    int size = settings.beginReadArray(group);
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        list.append(settings.value("item").toString());
    }
    settings.endArray();

    return list;
}
void FindWidget::loadFindWidgetInfo()
{
    //    qDebug() << QString("loadFindWidgetInfo");

    QStringList findList = readArraySettings("FindWidget");
    //        qDebug() << findList;
    if (findList.count() < 3) {
        qDebug() << QString("findList.count() error %1").arg(findList.count());
        return;
    }

    ui->comboBox_3->lineEdit()->setText(findList.at(0));
    //textComboBox->lineEdit()->setText(findList.at(1));
    ui->comboBox_2->lineEdit()->setText(findList.at(2));
}

void writeArraySettings(const QString &group, QStringList list)
{
    QString configFilePath = QCoreApplication::applicationDirPath() + "/config.ini";
    QSettings settings(configFilePath, QSettings::IniFormat);

    settings.remove(group);

    settings.beginWriteArray(group);
    for (int i = 0; i < list.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("item", list.at(i));
    }
    settings.endArray();
}

void FindWidget::saveFindWidgetInfo()
{
    //    qDebug() << QString("saveFindWidgetInfo");

    QStringList findList = {ui->comboBox_3->currentText(), ui->comboBox_2->currentText()};
    writeArraySettings("FindWidget", findList);
}
