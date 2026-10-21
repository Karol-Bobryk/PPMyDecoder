#include "imageloader.h"
#include <QDataStream>
#include <QImage>
#include <QString>
#include <QVector>
#include <qdebug.h>

ImageLoader::ImageLoader(QFile *file)
    : file(file)
{}

QImage *ImageLoader::loadPPM3()
{
    qint32 width = 0;
    qint32 height = 0;
    qint32 max_value = 255;

    if (!file->open(QIODeviceBase::ReadOnly)) {
        qInfo() << "failed to open a p3 file";
        return nullptr;
    }

    if (readToken(file) != "P3") {
        qInfo() << "invalid ppm version :p";
        return nullptr;
    }

    //reading headers
    width = readToken(file).toInt();
    height = readToken(file).toInt();
    max_value = readToken(file).toInt();

    // TODO: add different formats
    QImage *image = new QImage(width, height, QImage::Format_RGB32);

    uchar *scanLine = nullptr;

    // QTextStream stream(file);
    // stream.seek(file->pos());

    for (int y = 0; y < height; ++y) {
        auto *row = reinterpret_cast<QRgb *>(image->scanLine(y));

        for (int x = 0; x < width; ++x) {
            qint32 red, green, blue;

            QString red_str = readToken(file);
            QString green_str = readToken(file);
            QString blue_str = readToken(file);

            red = red_str.toInt() * 255 / max_value;
            green = green_str.toInt() * 255 / max_value;
            blue = blue_str.toInt() * 255 / max_value;

            row[x] = qRgb(red, green, blue);
        }
    }

    file->close();
    return image;
}

QString ImageLoader::readToken(QIODevice *device)
{
    QByteArray token;

    while (!device->atEnd()) {
        char c;
        if (device->getChar(&c) == false)
            break;

        if (std::isspace(static_cast<unsigned char>(c)))
            continue;

        if (c == '#') {
            while (device->getChar(&c) && c != '\n')
                ;
            continue;
        }

        token += c;
        break;
    }

    while (!device->atEnd()) {
        char c;
        if (!device->getChar(&c))
            break;

        if (std::isspace(static_cast<unsigned char>(c)))
            break;

        if (c == '#') {
            while (device->getChar(&c) && c != '\n')
                ;
            break;
        }

        token += c;
    }

    return QString::fromLatin1(token);
}