#ifndef HATCHINGTOOL_H
#define HATCHINGTOOL_H

#include "tool.h"
#include <QImage>
#include <QPoint>
#include <QRect>
#include <QVector>

class HatchingTool : public Tool
{
    Q_OBJECT
public:
    enum HatchType {
        Metal,
        NonMetal,
        Wood,
        Stone,
        Ceramic,
        Concrete,
        Glass,
        Liquid,
        Soil
    };

    explicit HatchingTool(QObject *parent = nullptr);

    void onMousePress(QMouseEvent *event, QPainter &painter) override;
    void onMouseMove(QMouseEvent *event, QPainter &painter) override;
    void onMouseRelease(QMouseEvent *event, QPainter &painter) override;

    void setHatchAngle(int angle) { m_hatchAngle = angle; }
    void setHatchSpacing(int spacing) { m_hatchSpacing = spacing; }
    void setCrossHatching(bool cross) { m_crossHatching = cross; }
    void setHatchType(HatchType type);

    int getHatchAngle() const { return m_hatchAngle; }
    int getHatchSpacing() const { return m_hatchSpacing; }
    bool isCrossHatching() const { return m_crossHatching; }
    HatchType getHatchType() const { return m_hatchType; }

private:
    void floodFillHatch(const QPoint &startPoint, QImage &image, QPainter &painter);

    void drawHatchOnImage(QImage &hatchImage, int width, int height);

    int m_hatchAngle = 45;
    int m_hatchSpacing = 10;
    bool m_crossHatching = false;
    HatchType m_hatchType = Metal;

    bool m_isDrawing = false;
};

#endif // HATCHINGTOOL_H
