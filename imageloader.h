#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QFile>
#include <QPixmap>

class ImageLoader
{
public:
    ImageLoader(QFile *file);

    QImage loadPPM3();
    QImage loadPPM6();

private:
    QFile *file;
    QByteArray readToken(QIODevice *device);
};

#endif // IMAGELOADER_H