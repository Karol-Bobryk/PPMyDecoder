#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "imageloader.h"
#include "imageviewer.h"

#include <QFile>
#include <QFileDialog>
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

    image_viewer->setImage(loadedImage);
}

void MainWindow::loadPPM6()
{
    QString filter = "P6 files (*.ppm)";
    QString caption = "Load a ppm file";
    QUrl file_url = QFileDialog::getOpenFileUrl(this, caption, QUrl(), filter);

    if (file_url.isEmpty()) {
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

    image_viewer->setImage(loadedImage);
}