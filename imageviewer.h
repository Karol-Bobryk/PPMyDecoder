#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QAbstractScrollArea>
#include <QImage>
#include <QPainter>
#include <QWheelEvent>

class ImageViewer : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);

    void setImage(const QImage &image);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateScrollBars();

    qreal zoom = 1.0;
    QImage image;
};

#endif // IMAGEVIEWER_H
