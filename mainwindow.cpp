#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "imageloader.h"
#include "imageviewer.h"

#include <QFile>
#include <QFileDialog>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , image{nullptr}
    , ui(new Ui::MainWindow)
    , image_viewer(new ImageViewer(this))
{
    ui->setupUi(this);
    ui->imageLayout->addWidget(image_viewer);

    connect(ui->buttonPPM3, &QPushButton::clicked, this, &MainWindow::loadPPM3);
}

MainWindow::~MainWindow()
{
    delete image;
    delete ui;
}

void MainWindow::loadPPM3()
{
    QString filter = "PP3 files (*.ppm)";
    QString caption = "Load a ppm file";
    QUrl file_url = QFileDialog::getOpenFileUrl(this, caption, QUrl(), filter);

    if (file_url.isEmpty()) {
        qInfo() << "pp3 file path is empty";
        return;
    }

    QFile file(file_url.path(), this);

    ImageLoader loader(&file);

    QImage *loadedImage = loader.loadPPM3();

    if (!loadedImage || loadedImage->isNull()) {
        qInfo() << "Image load failed";
        delete loadedImage;
        return;
    }

    delete image;
    image = loadedImage;

    image_viewer->setImage(*image);
}
