#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <QWidget>
#include <QImage>
#include <QColor>
#include <QPoint>
#include <memory>

class Tool;
class PencilTool;
#include "hatchingtool.h"

class PaintView : public QWidget
{
    Q_OBJECT

public:
    explicit PaintView(QWidget *parent = nullptr);
    ~PaintView();

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void clearImage();

    void setCurrentTool(Tool *tool);
    Tool* currentTool() const { return m_currentTool; }

    void usePencilTool();
    void useHatchingTool();

    void setPenColor(const QColor &color);
    void setPenWidth(int width);

    void setHatchAngle(int angle);
    void setHatchSpacing(int spacing);
    void setCrossHatching(bool cross);
    void setHatchType(HatchingTool::HatchType type);

    bool isModified() const { return m_modified; }
    QColor penColor() const;
    int penWidth() const;
    const QImage& image() const { return m_image; }

signals:
    void toolChanged(Tool *newTool);
    void imageModified();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void resizeImage(const QSize &newSize);

    bool m_modified = false;
    QImage m_image;
    QPoint m_lastPoint;

    Tool *m_currentTool = nullptr;
    std::unique_ptr<PencilTool> m_pencilTool;
    std::unique_ptr<HatchingTool> m_hatchingTool;
};

#endif // PAINTVIEW_H
