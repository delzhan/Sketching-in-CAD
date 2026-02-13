#include "paintview.h"
#include "penciltool.h"
#include "hatchingtool.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QFileDialog>

PaintView::PaintView(QWidget *parent)
    : QWidget(parent)
    , m_modified(false)
    , m_image(500, 500, QImage::Format_ARGB32)
    , m_lastPoint(0, 0)
{
    setAttribute(Qt::WA_StaticContents);
    setMouseTracking(true);

    m_image.fill(Qt::white);

    m_pencilTool = std::make_unique<PencilTool>();
    m_hatchingTool = std::make_unique<HatchingTool>();

    m_currentTool = m_pencilTool.get();
}

PaintView::~PaintView()
{
}

bool PaintView::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(newSize);
    m_image = loadedImage;
    m_modified = false;
    update();

    return true;
}

bool PaintView::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = m_image;
    resizeImage(size());

    if (visibleImage.save(fileName, fileFormat)) {
        m_modified = false;
        return true;
    }
    return false;
}

void PaintView::clearImage()
{
    m_image.fill(Qt::white);
    m_modified = true;
    update();
}

void PaintView::setCurrentTool(Tool *tool)
{
    if (tool && tool != m_currentTool) {
        if (m_currentTool) {
        }

        m_currentTool = tool;
        emit toolChanged(tool);
    }
}

void PaintView::usePencilTool()
{
    setCurrentTool(m_pencilTool.get());
}

void PaintView::useHatchingTool()
{
    setCurrentTool(m_hatchingTool.get());
}

void PaintView::setPenColor(const QColor &color)
{
    if (m_pencilTool) m_pencilTool->setPenColor(color);
    if (m_hatchingTool) m_hatchingTool->setPenColor(color);
}

void PaintView::setPenWidth(int width)
{
    if (m_pencilTool) m_pencilTool->setPenWidth(width);
    if (m_hatchingTool) m_hatchingTool->setPenWidth(width);
}

QColor PaintView::penColor() const
{
    return m_currentTool ? m_currentTool->penColor() : Qt::blue;
}

int PaintView::penWidth() const
{
    return m_currentTool ? m_currentTool->penWidth() : 1;
}

void PaintView::setHatchAngle(int angle)
{
    if (m_hatchingTool) {
        m_hatchingTool->setHatchAngle(angle);
    }
}

void PaintView::setHatchSpacing(int spacing)
{
    if (m_hatchingTool) {
        m_hatchingTool->setHatchSpacing(spacing);
    }
}

void PaintView::setCrossHatching(bool cross)
{
    if (m_hatchingTool) {
        m_hatchingTool->setCrossHatching(cross);
    }
}

void PaintView::setHatchType(HatchingTool::HatchType type)
{
    if (m_hatchingTool) {
        m_hatchingTool->setHatchType(type);
    }
}

void PaintView::mousePressEvent(QMouseEvent *event)
{
    if (!m_currentTool) return;

    m_lastPoint = event->pos();
    QPainter painter(&m_image);
    painter.setRenderHint(QPainter::Antialiasing, true);

    m_currentTool->onMousePress(event, painter, m_lastPoint);

    m_modified = true;
    update();
}

void PaintView::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_currentTool) return;

    if (event->buttons() & Qt::LeftButton) {
        QPainter painter(&m_image);
        painter.setRenderHint(QPainter::Antialiasing, true);

        m_currentTool->onMouseMove(event, painter, m_lastPoint);

        m_lastPoint = event->pos();
        m_modified = true;
        update();
    }
}

void PaintView::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_currentTool) return;

    QPainter painter(&m_image);
    painter.setRenderHint(QPainter::Antialiasing, true);

    m_currentTool->onMouseRelease(event, painter, m_lastPoint);

    m_modified = true;
    update();
}

void PaintView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, m_image, dirtyRect);
}

void PaintView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (width() > m_image.width() || height() > m_image.height()) {
        int newWidth = qMax(width() + 128, m_image.width());
        int newHeight = qMax(height() + 128, m_image.height());
        resizeImage(QSize(newWidth, newHeight));
    }
}

void PaintView::resizeImage(const QSize &newSize)
{
    if (m_image.size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32);
    newImage.fill(Qt::white);

    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), m_image);
    m_image = newImage;

    update();
}
