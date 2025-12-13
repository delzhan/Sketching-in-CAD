#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QStack>

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    enum Tool { Pen, Hatching };

    ScribbleArea(QWidget *parent = nullptr);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setCurrentTool(Tool newTool);
    void setHatchAngle(int angle);  // Новый метод для установки угла штриховки
    void setHatchSpacing(int spacing); // Новый метод для установки расстояния между линиями

    bool isModified() const { return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const { return myPenWidth; }
    Tool currentTool() const { return m_currentTool; }

public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void hatchFill(const QPoint &startPoint);  // Новая функция для заливки штриховкой
    void resizeImage(QImage *image, const QSize &newSize);

    // Вспомогательные функции для заливки
    bool canHatchAt(int x, int y, const QColor &targetColor);
    void hatchArea(int startX, int startY, const QColor &targetColor);

    bool modified = false;
    bool scribbling = false;
    int myPenWidth = 1;
    QColor myPenColor = Qt::blue;
    QImage image;
    QPoint lastPoint;
    Tool m_currentTool = Pen;

    // Параметры штриховки
    int hatchAngle = 45;      // Угол штриховки в градусах
    int hatchSpacing = 10;    // Расстояние между линиями штриховки
};

#endif
