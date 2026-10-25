#include "imageloader.h"
#include <QDataStream>
#include <QImage>
#include <QString>
#include <QVector>
#include <qdebug.h>

ImageLoader::ImageLoader(QFile *file)
    : file(file)
{}

QImage ImageLoader::loadPPM3()
{
    qint32 width = 0;
    qint32 height = 0;
    qint32 max_value = 255;

    if (!file->open(QIODeviceBase::ReadOnly | QIODeviceBase::Text)) {
        qInfo() << "failed to open a p3 file";
        return QImage();
    }

    if (readToken(file) != "P3") {
        qInfo() << "invalid ppm version (should be P3) :p";
        return QImage();
    }

    //reading headers
    width = readToken(file).toInt();
    height = readToken(file).toInt();
    max_value = readToken(file).toInt();

    QImage image(width, height, QImage::Format_RGB32);

    // QTextStream stream(file);

    for (int y = 0; y < height; ++y) {
        auto *row = reinterpret_cast<QRgb *>(image.scanLine(y));

        for (int x = 0; x < width; ++x) {
            qint32 red = readToken(file).toInt() * 255 / max_value;
            qint32 green = readToken(file).toInt() * 255 / max_value;
            qint32 blue = readToken(file).toInt() * 255 / max_value;

            row[x] = qRgb(red, green, blue);
        }
    }

    file->close();
    return image;
}

QImage ImageLoader::loadPPM6()
{
    qint32 width = 0;
    qint32 height = 0;
    qint32 max_value = 255;

    if (!file->open(QIODeviceBase::ReadOnly)) {
        qInfo() << "failed to open a p6 file";
        return QImage();
    }

    if (readToken(file) != "P6") {
        qInfo() << "invalid ppm version (should be P6) :p";
        return QImage();
    }

    //reading headers
    width = readToken(file).toInt();
    height = readToken(file).toInt();
    max_value = readToken(file).toInt();

    if (max_value != 255) {
        qInfo() << "invalid p6 max_value :0";
        return QImage();
    }

    QImage image(width, height, QImage::Format_RGB32);

    if (file->peek(1) == '\n') {
        file->seek(file->pos() + 1);
    }

    for (int y = 0; y < height; ++y) {
        auto *row = reinterpret_cast<QRgb *>(image.scanLine(y));

        for (int x = 0; x < width; ++x) {
            QByteArray arr = file->read(3);

            row[x] = qRgb(arr[0], arr[1], arr[2]);
            // qInfo() << arr;
        }
    }

    file->close();
    return image;
}

QByteArray ImageLoader::readToken(QIODevice *device)
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

    return token;
}