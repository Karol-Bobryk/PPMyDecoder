#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include "imageviewer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QImage image;
    ImageViewer *image_viewer;
private slots:
    void loadPPM3();
    void loadPPM6();
    void loadJPEG();
    void saveJPEG();
};
#endif // MAINWINDOW_H
