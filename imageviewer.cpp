#include "imageviewer.h"

#include <QResizeEvent>
#include <QScrollBar>
#include <QtMath>

ImageViewer::ImageViewer(QWidget *parent)
    : QAbstractScrollArea(parent)
{
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
}

void ImageViewer::setImage(const QImage &newImage)
{
    image = newImage;
    zoom = 1.0;

    horizontalScrollBar()->setValue(0);
    verticalScrollBar()->setValue(0);

    updateScrollBars();
    viewport()->update();
}

void ImageViewer::wheelEvent(QWheelEvent *event)
{
    if (image.isNull()) {
        event->ignore();
        return;
    }

    const int delta = event->angleDelta().y();

    if (delta == 0) {
        event->ignore();
        return;
    }

    const QPoint mousePos = event->position().toPoint();

    const QPointF imagePos((mousePos.x() + horizontalScrollBar()->value()) / zoom,
                           (mousePos.y() + verticalScrollBar()->value()) / zoom);

    const qreal oldZoom = zoom;

    if (delta > 0)
        zoom *= 1.25;
    else
        zoom /= 1.25;

    zoom = qBound(0.1, zoom, 50.0);

    if (qFuzzyCompare(oldZoom, zoom)) {
        event->accept();
        return;
    }

    updateScrollBars();

    horizontalScrollBar()->setValue(qRound(imagePos.x() * zoom - mousePos.x()));

    verticalScrollBar()->setValue(qRound(imagePos.y() * zoom - mousePos.y()));

    viewport()->update();

    event->accept();
}

void ImageViewer::updateScrollBars()
{
    if (image.isNull())
        return;

    const QSize scaledSize(qCeil(image.width() * zoom), qCeil(image.height() * zoom));

    horizontalScrollBar()->setPageStep(viewport()->width());
    verticalScrollBar()->setPageStep(viewport()->height());

    horizontalScrollBar()->setRange(0, qMax(0, scaledSize.width() - viewport()->width()));

    verticalScrollBar()->setRange(0, qMax(0, scaledSize.height() - viewport()->height()));
}

void ImageViewer::resizeEvent(QResizeEvent *event)
{
    QAbstractScrollArea::resizeEvent(event);
    updateScrollBars();
}

void ImageViewer::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(viewport());
    painter.setRenderHint(QPainter::LosslessImageRendering, true);

    if (image.isNull())
        return;

    painter.scale(zoom, zoom);

    painter.translate(-horizontalScrollBar()->value() / zoom, -verticalScrollBar()->value() / zoom);
    painter.drawImage(QPoint(0, 0), image);
}
