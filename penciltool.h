#ifndef PENCILTOOL_H
#define PENCILTOOL_H

#include "tool.h"

class PencilTool : public Tool
{
    Q_OBJECT
public:
    explicit PencilTool(QObject *parent = nullptr);

    void onMousePress(QMouseEvent *event, QPainter &painter) override;
    void onMouseMove(QMouseEvent *event, QPainter &painter) override;
    void onMouseRelease(QMouseEvent *event, QPainter &painter) override;

private:
    void drawLineTo(const QPoint &endPoint, QPainter &painter);

    bool m_scribbling = false;
    QPoint m_lastPoint;
};

#endif // PENCILTOOL_H
