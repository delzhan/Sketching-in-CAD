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

    void onMousePress(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint) override;
    void onMouseMove(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint) override;
    void onMouseRelease(QMouseEvent *event, QPainter &painter, const QPoint &lastPoint) override;

    void setHatchAngle(int angle) { m_hatchAngle = angle; }
    void setHatchSpacing(int spacing) { m_hatchSpacing = spacing; }
    void setCrossHatching(bool cross) { m_crossHatching = cross; }
    void setHatchType(HatchType type);

    int getHatchAngle() const { return m_hatchAngle; }
    int getHatchSpacing() const { return m_hatchSpacing; }
    bool isCrossHatching() const { return m_crossHatching; }
    HatchType getHatchType() const { return m_hatchType; }

private:
    // Основной метод заливки области штриховкой (точная копия логики из ScribbleArea)
    void floodFillHatch(const QPoint &startPoint, QImage &image, QPainter &painter);

    // Рисование штриховки на отдельном изображении (как в drawCrossHatching и hatchArea)
    void drawHatchOnImage(QImage &hatchImage, int width, int height);

    // Параметры штриховки
    int m_hatchAngle = 45;
    int m_hatchSpacing = 10;
    bool m_crossHatching = false;
    HatchType m_hatchType = Metal;

    // Состояние инструмента
    bool m_isDrawing = false;
};

#endif // HATCHINGTOOL_H
