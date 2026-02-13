#include "penciltool.h"
#include <QPainter>
#include <QMouseEvent>

PencilTool::PencilTool(QObject *parent) : Tool(parent)
{
}

void PencilTool::onMousePress(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint)
{
    Q_UNUSED(lastPoint);
    if (event->button() == Qt::LeftButton) {
        m_scribbling = true;
        m_lastPoint = event->pos();

        painter.setPen(QPen(m_penColor, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPoint(m_lastPoint);
    }
}

void PencilTool::onMouseMove(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint)
{
    Q_UNUSED(lastPoint);
    if ((event->buttons() & Qt::LeftButton) && m_scribbling) {
        drawLineTo(event->pos(), painter, m_lastPoint);
        m_lastPoint = event->pos();
    }
}

void PencilTool::onMouseRelease(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint)
{
    Q_UNUSED(lastPoint);
    if (event->button() == Qt::LeftButton && m_scribbling) {
        drawLineTo(event->pos(), painter, m_lastPoint);
        m_scribbling = false;
    }
}

void PencilTool::drawLineTo(const QPoint &endPoint, QPainter &painter, const QPoint &startPoint)
{
    painter.setPen(QPen(m_penColor, m_penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(startPoint, endPoint);
}
