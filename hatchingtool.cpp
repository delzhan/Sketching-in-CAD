#include "hatchingtool.h"
#include <QPainter>
#include <QMouseEvent>
#include <QStack>
#include <QDebug>
#include <cmath>

HatchingTool::HatchingTool(QObject *parent) : Tool(parent)
{
}

void HatchingTool::onMousePress(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint)
{
    Q_UNUSED(painter);
    Q_UNUSED(lastPoint);
    if (event->button() == Qt::LeftButton) {
        m_isDrawing = true;
    }
}

void HatchingTool::onMouseMove(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint)
{
    Q_UNUSED(event);
    Q_UNUSED(painter);
    Q_UNUSED(lastPoint);
}

void HatchingTool::onMouseRelease(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint)
{
    Q_UNUSED(lastPoint);
    if (event->button() == Qt::LeftButton && m_isDrawing) {
        m_isDrawing = false;
        QPoint clickPoint = event->pos();

        QPaintDevice *device = painter.device();
        if (!device) return;

        QImage *image = static_cast<QImage*>(device);
        if (!image) return;

        floodFillHatch(clickPoint, *image, painter);
    }
}

void HatchingTool::setHatchType(HatchType type)
{
    m_hatchType = type;

    switch (type) {
    case Metal:
        m_hatchAngle = 45;
        m_hatchSpacing = 5;
        m_crossHatching = false;
        break;
    case NonMetal:
        m_hatchAngle = 45;
        m_hatchSpacing = 4;
        m_crossHatching = true;
        break;
    case Wood:
        m_hatchAngle = 0;
        m_hatchSpacing = 4;
        m_crossHatching = false;
        break;
    case Stone:
        m_hatchAngle = 45;
        m_hatchSpacing = 6;
        m_crossHatching = false;
        break;
    case Ceramic:
        m_hatchAngle = 45;
        m_hatchSpacing = 5;
        m_crossHatching = false;
        break;
    case Concrete:
        m_hatchAngle = 45;
        m_hatchSpacing = 8;
        m_crossHatching = false;
        break;
    case Glass:
        m_hatchAngle = 45;
        m_hatchSpacing = 12;
        m_crossHatching = false;
        break;
    case Liquid:
        m_hatchAngle = 45;
        m_hatchSpacing = 2;
        m_crossHatching = false;
        break;
    case Soil:
        m_hatchAngle = 45;
        m_hatchSpacing = 5;
        m_crossHatching = false;
        break;
    }
}

void HatchingTool::floodFillHatch(const QPoint &startPoint, QImage &image, QPainter &painter)
{
    Q_UNUSED(painter);

    if (!image.rect().contains(startPoint))
        return;

    QColor targetColor = image.pixelColor(startPoint);

    if (targetColor == m_penColor)
        return;

    QStack<QPoint> stack;
    stack.push(startPoint);

    QImage visited(image.size(), QImage::Format_Mono);
    visited.fill(0);

    QVector<QPoint> areaPixels;

    while (!stack.isEmpty()) {
        QPoint p = stack.pop();
        int x = p.x();
        int y = p.y();

        if (!image.rect().contains(x, y))
            continue;

        if (visited.pixelIndex(x, y))
            continue;

        if (image.pixelColor(x, y) == targetColor) {
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

    QImage hatchImage(width, height, QImage::Format_ARGB32);
    hatchImage.fill(Qt::transparent);

    drawHatchOnImage(hatchImage, width, height);

    for (const QPoint &p : areaPixels) {
        int localX = p.x() - minX;
        int localY = p.y() - minY;

        if (localX >= 0 && localX < width && localY >= 0 && localY < height) {
            QColor hatchColor = hatchImage.pixelColor(localX, localY);
            if (hatchColor.alpha() > 0) {
                image.setPixelColor(p, hatchColor);
            }
        }
    }

    qDebug() << "HatchingTool: filled" << areaPixels.size() << "pixels";
}

void HatchingTool::drawHatchOnImage(QImage &hatchImage, int width, int height)
{
    QPainter painter(&hatchImage);
    painter.setPen(QPen(m_penColor, m_penWidth));

    int diagonal = static_cast<int>(std::sqrt(width * width + height * height));

    if (m_crossHatching) {
        double angle1Rad = qDegreesToRadians(static_cast<double>(m_hatchAngle));
        double cosAngle1 = std::cos(angle1Rad);
        double sinAngle1 = std::sin(angle1Rad);

        double angle2Rad = qDegreesToRadians(static_cast<double>(-m_hatchAngle));
        double cosAngle2 = std::cos(angle2Rad);
        double sinAngle2 = std::sin(angle2Rad);

        for (int i = -diagonal; i < diagonal; i += m_hatchSpacing) {
            QPointF p1, p2;
            if (std::abs(sinAngle1) > std::abs(cosAngle1)) {
                p1 = QPointF(i / sinAngle1, 0);
                p2 = QPointF((i - height * cosAngle1) / sinAngle1, height);
            } else {
                p1 = QPointF(0, i / cosAngle1);
                p2 = QPointF(width, (i - width * sinAngle1) / cosAngle1);
            }
            painter.drawLine(p1, p2);
        }

        for (int i = -diagonal; i < diagonal; i += m_hatchSpacing) {
            QPointF p1, p2;
            if (std::abs(sinAngle2) > std::abs(cosAngle2)) {
                p1 = QPointF(i / sinAngle2, 0);
                p2 = QPointF((i - height * cosAngle2) / sinAngle2, height);
            } else {
                p1 = QPointF(0, i / cosAngle2);
                p2 = QPointF(width, (i - width * sinAngle2) / cosAngle2);
            }
            painter.drawLine(p1, p2);
        }
    } else {
        double angleRad = qDegreesToRadians(static_cast<double>(m_hatchAngle));
        double cosAngle = std::cos(angleRad);
        double sinAngle = std::sin(angleRad);

        for (int i = -diagonal; i < diagonal; i += m_hatchSpacing) {
            QPointF p1, p2;
            if (std::abs(sinAngle) > std::abs(cosAngle)) {
                p1 = QPointF(i / sinAngle, 0);
                p2 = QPointF((i - height * cosAngle) / sinAngle, height);
            } else {
                p1 = QPointF(0, i / cosAngle);
                p2 = QPointF(width, (i - width * sinAngle) / cosAngle);
            }
            painter.drawLine(p1, p2);
        }
    }
}
