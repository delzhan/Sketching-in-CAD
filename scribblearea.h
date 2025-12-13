#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

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
    void setHatchAngle(int angle);
    void setHatchSpacing(int spacing);
    void setCrossHatching(bool cross);

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
    void hatchFill(const QPoint &startPoint);
    void resizeImage(QImage *image, const QSize &newSize);

    // Вспомогательные функции для заливки
    bool canHatchAt(int x, int y, const QColor &targetColor);
    void hatchArea(int startX, int startY, const QColor &targetColor);

    // Метод для перекрестной штриховки
    void drawCrossHatching(QImage &hatchImage, int width, int height);

    bool modified = false;
    bool scribbling = false;
    int myPenWidth = 1;
    QColor myPenColor = Qt::blue;
    QImage image;
    QPoint lastPoint;
    Tool m_currentTool = Pen;

    // Параметры штриховки
    int hatchAngle = 45;
    int hatchSpacing = 10;
    bool m_crossHatching = false;
};

#endif
