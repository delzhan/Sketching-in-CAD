#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class ScribbleArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void open();
    void save();
    void penColor();
    void penWidth();
    void about();
    void setPenTool();

    void setHatchingMetal();
    void setHatchingNonMetal();
    void setHatchingWood();
    void setHatchingStone();
    void setHatchingCeramic();
    void setHatchingConcrete();
    void setHatchingGlass();
    void setHatchingLiquid();
    void setHatchingSoil();

    void setHatchAngle();
    void setHatchSpacing();

private:
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    ScribbleArea *scribbleArea;

    QMenu *toolsMenu;
    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;
    QMenu *hatchingSubMenu;

    QList<QAction *> saveAsActs;
    QAction *penToolAct;

    QAction *hatchingMetalAct;
    QAction *hatchingNonMetalAct;
    QAction *hatchingWoodAct;
    QAction *hatchingStoneAct;
    QAction *hatchingCeramicAct;
    QAction *hatchingConcreteAct;
    QAction *hatchingGlassAct;
    QAction *hatchingLiquidAct;
    QAction *hatchingSoilAct;

    QAction *openAct;
    QAction *exitAct;
    QAction *penColorAct;
    QAction *penWidthAct;
    QAction *printAct;
    QAction *clearScreenAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *hatchAngleAct;
    QAction *hatchSpacingAct;
};

#endif
