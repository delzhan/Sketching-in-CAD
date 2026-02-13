#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>

class Tool : public QObject
{
    Q_OBJECT
public:
    explicit Tool(QObject *parent = nullptr);
    virtual ~Tool() = default;

    virtual void onMousePress(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint) = 0;
    virtual void onMouseMove(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint) = 0;
    virtual void onMouseRelease(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint) = 0;

    virtual void setPenColor(const QColor &color) { m_penColor = color; }
    virtual void setPenWidth(int width) { m_penWidth = width; }

    QColor penColor() const { return m_penColor; }
    int penWidth() const { return m_penWidth; }

protected:
    QColor m_penColor = Qt::blue;
    int m_penWidth = 1;
};

#endif // TOOL_H
