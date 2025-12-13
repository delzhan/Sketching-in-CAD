// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "scribblearea.h"

#include <QMouseEvent>
#include <QPainter>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QtMath>
#include <stack>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QPrintDialog>
#include <QPrinter>
#endif

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);

    image = QImage(500, 500, QImage::Format_ARGB32);
    image.fill(Qt::white);
}

void ScribbleArea::setCurrentTool(Tool newTool)
{
    m_currentTool = newTool;
}

void ScribbleArea::setHatchAngle(int angle)
{
    hatchAngle = angle;
}

void ScribbleArea::setHatchSpacing(int spacing)
{
    hatchSpacing = spacing;
}

bool ScribbleArea::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    }
    return false;
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->position().toPoint();
        scribbling = true;

        if (m_currentTool == Hatching) {
            hatchFill(event->position().toPoint());
            scribbling = false;
        }
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling && m_currentTool == Pen) {
        drawLineTo(event->position().toPoint());
    }
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling && m_currentTool == Pen) {
        drawLineTo(event->position().toPoint());
        scribbling = false;
    }
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    if (m_currentTool == Pen) {
        QPainter painter(&image);
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter.drawLine(lastPoint, endPoint);
        modified = true;

        int rad = (myPenWidth / 2) + 2;
        update(QRect(lastPoint, endPoint).normalized()
                   .adjusted(-rad, -rad, +rad, +rad));
        lastPoint = endPoint;
    }
}

void ScribbleArea::hatchFill(const QPoint &startPoint)
{
    if (!image.rect().contains(startPoint))
        return;

    QColor targetColor = image.pixelColor(startPoint);

    if (targetColor == myPenColor)
        return;

    hatchArea(startPoint.x(), startPoint.y(), targetColor);

    update();
    modified = true;
}

bool ScribbleArea::canHatchAt(int x, int y, const QColor &targetColor)
{
    if (!image.rect().contains(x, y))
        return false;

    QColor pixelColor = image.pixelColor(x, y);
    return pixelColor == targetColor;
}

void ScribbleArea::hatchArea(int startX, int startY, const QColor &targetColor)
{
    // Алгоритм заливки с затравкой (Flood Fill)
    std::stack<QPoint> stack;
    stack.push(QPoint(startX, startY));

    QImage visited(image.size(), QImage::Format_Mono);
    visited.fill(0);

    QVector<QPoint> areaPixels;

    while (!stack.empty()) {
        QPoint p = stack.top();
        stack.pop();

        int x = p.x();
        int y = p.y();

        if (!image.rect().contains(x, y) || visited.pixelIndex(x, y))
            continue;

        if (canHatchAt(x, y, targetColor)) {
            visited.setPixel(x, y, 1);
            areaPixels.append(QPoint(x, y));

            stack.push(QPoint(x + 1, y));
            stack.push(QPoint(x - 1, y));
            stack.push(QPoint(x, y + 1));
            stack.push(QPoint(x, y - 1));
        }
    }

    if (areaPixels.isEmpty())
        return;

    // Находим границы области
    int minX = image.width(), maxX = 0;
    int minY = image.height(), maxY = 0;

    for (const QPoint &p : areaPixels) {
        minX = qMin(minX, p.x());
        maxX = qMax(maxX, p.x());
        minY = qMin(minY, p.y());
        maxY = qMax(maxY, p.y());
    }

    int width = maxX - minX + 1;
    int height = maxY - minY + 1;

    // Создаем временное изображение для штриховки
    QImage hatchImage(width, height, QImage::Format_ARGB32);
    hatchImage.fill(Qt::transparent);

    QPainter hatchPainter(&hatchImage);
    hatchPainter.setPen(QPen(myPenColor, myPenWidth));

    // Параметры для поворота штриховки
    double angleRad = qDegreesToRadians((double)hatchAngle);
    double cosAngle = qCos(angleRad);
    double sinAngle = qSin(angleRad);

    // Диагональ области для полного покрытия
    int diagonal = qSqrt(width * width + height * height);

    // Рисуем линии штриховки во временном изображении
    for (int i = -diagonal; i < diagonal; i += hatchSpacing) {
        QPointF p1, p2;

        if (qAbs(sinAngle) > qAbs(cosAngle)) {
            p1 = QPointF(i / sinAngle, 0);
            p2 = QPointF((i - height * cosAngle) / sinAngle, height);
        } else {
            p1 = QPointF(0, i / cosAngle);
            p2 = QPointF(width, (i - width * sinAngle) / cosAngle);
        }

        hatchPainter.drawLine(p1, p2);
    }

    // Возвращаем пиксели, не затрагивая контур штриховки
    for (const QPoint &p : areaPixels) {
        int localX = p.x() - minX;
        int localY = p.y() - minY;

        if (localX >= 0 && localX < width && localY >= 0 && localY < height) {
            QColor hatchColor = hatchImage.pixelColor(localX, localY);

            // Копируем цвет только если пиксель в hatchImage не прозрачный
            // и если этот пиксель действительно внутри области
            if (hatchColor.alpha() > 0) {
                image.setPixelColor(p, hatchColor);
            }
        }
    }
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ScribbleArea::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}
