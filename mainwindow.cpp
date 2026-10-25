#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "imageloader.h"
#include "imageviewer.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , image_viewer(new ImageViewer(this))
{
    ui->setupUi(this);
    ui->imageLayout->addWidget(image_viewer);

    connect(ui->buttonPPM3, &QPushButton::clicked, this, &MainWindow::loadPPM3);
    connect(ui->buttonPPM6, &QPushButton::clicked, this, &MainWindow::loadPPM6);
    connect(ui->buttonJPEG, &QPushButton::clicked, this, &MainWindow::loadJPEG);
    connect(ui->saveJPEG, &QPushButton::clicked, this, &MainWindow::saveJPEG);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadPPM3()
{
    QString filter = "P3 files (*.ppm)";
    QString caption = "Load a ppm file";
    QUrl file_url = QFileDialog::getOpenFileUrl(this, caption, QUrl(), filter);

    if (file_url.isEmpty()) {
        QMessageBox::warning(this, "Warning", "p3 file path is empty");
        qInfo() << "pp3 file path is empty";
        return;
    }

    QFile file(file_url.path(), this);

    ImageLoader loader(&file);

    QImage loadedImage = loader.loadPPM3();

    if (loadedImage.isNull()) {
        qInfo() << "Image load failed";
        return;
    }

    image = loadedImage;
    image_viewer->setImage(image);
}

void MainWindow::loadPPM6()
{
    QString filter = "P6 files (*.ppm)";
    QString caption = "Load a ppm file";
    QUrl file_url = QFileDialog::getOpenFileUrl(this, caption, QUrl(), filter);

    if (file_url.isEmpty()) {
        QMessageBox::warning(this, "Warning", "p6 file path is empty");
        qInfo() << "p6 file path is empty";
        return;
    }

    QFile file(file_url.path(), this);

    ImageLoader loader(&file);
    QImage loadedImage = loader.loadPPM6();

    if (loadedImage.isNull()) {
        qInfo() << "Image load failed";
        return;
    }

    image = loadedImage;
    image_viewer->setImage(image);
}

void MainWindow::loadJPEG()
{
    QString filter = "Jpeg files (*.jpg *.jpeg)";
    QString caption = "Load a jpeg file";
    QUrl file_url = QFileDialog::getOpenFileUrl(this, caption, QUrl(), filter);

    if (file_url.isEmpty()) {
        QMessageBox::warning(this, "Warning", "jpeg file path is empty");
        qInfo() << "jpeg file path is empty";
        return;
    }

    QFile file(file_url.path(), this);
    image = QImage(file_url.path());
    image_viewer->setImage(image);
}

void MainWindow::saveJPEG()
{
    if (image.isNull()) {
        QMessageBox::about(this, "Info", "Cannot save an empty image");
    }
    QString filter = "Jpeg files (*.jpg *.jpeg)";
    QString caption = "Save to a jpeg file";
    QUrl file_url = QFileDialog::getSaveFileUrl(this, caption, QUrl(), filter);

    if (file_url.isEmpty()) {
        QMessageBox::warning(this, "Warning", "jpeg file path is empty");
        qInfo() << "jpeg file path is empty";
        return;
    }

    qInfo() << image.save(file_url.path(), "JPEG", 100 - ui->compressionEdit->text().toInt());
}