#include "mainwindow.h"
#include "paintview.h"

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QInputDialog>
#include <QActionGroup>
#include <QMenuBar>
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), paintView(new PaintView(this))
{
    setCentralWidget(paintView);

    createActions();
    createMenus();

    setWindowTitle(tr("Scribble"));
    resize(500, 500);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}

void MainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            paintView->openImage(fileName);
    }
}

void MainWindow::setPenTool()
{
    paintView->usePencilTool();
}

void MainWindow::setHatchingTool()
{
    paintView->useHatchingTool();
}

void MainWindow::setHatchingMetal()
{
    paintView->useHatchingTool();
    paintView->setHatchType(HatchingTool::Metal);
}

void MainWindow::setHatchingNonMetal()
{
    paintView->useHatchingTool();
    paintView->setHatchType(HatchingTool::NonMetal);
}

void MainWindow::setHatchingWood()
{
    paintView->useHatchingTool();
    paintView->setHatchType(HatchingTool::Wood);
}

void MainWindow::setHatchingStone()
{
    paintView->useHatchingTool();
    paintView->setHatchType(HatchingTool::Stone);
}

void MainWindow::setHatchingCeramic()
{
    paintView->useHatchingTool();
    paintView->setHatchType(HatchingTool::Ceramic);
}

void MainWindow::setHatchingConcrete()
{
    paintView->useHatchingTool();
    paintView->setHatchType(HatchingTool::Concrete);
}

void MainWindow::setHatchingGlass()
{
    paintView->useHatchingTool();
    paintView->setHatchType(HatchingTool::Glass);
}

void MainWindow::setHatchingLiquid()
{
    paintView->useHatchingTool();
    paintView->setHatchType(HatchingTool::Liquid);
}

void MainWindow::setHatchingSoil()
{
    paintView->useHatchingTool();
    paintView->setHatchType(HatchingTool::Soil);
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(paintView->penColor());
    if (newColor.isValid())
        paintView->setPenColor(newColor);
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width:"),
                                        paintView->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        paintView->setPenWidth(newWidth);
}

void MainWindow::setHatchAngle()
{
    bool ok;
    int angle = QInputDialog::getInt(this, tr("Угол штриховки"),
                                     tr("Введите угол штриховки (0-180 градусов):"),
                                     45, 0, 180, 1, &ok);
    if (ok)
        paintView->setHatchAngle(angle);
}

void MainWindow::setHatchSpacing()
{
    bool ok;
    int spacing = QInputDialog::getInt(this, tr("Расстояние между линиями"),
                                       tr("Введите расстояние между линиями штриховки:"),
                                       10, 1, 50, 1, &ok);
    if (ok)
        paintView->setHatchSpacing(spacing);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Scribble"),
                       tr("<p>The <b>Scribble</b> example shows how to use QMainWindow as the "
                          "base widget for an application, and how to reimplement some of "
                          "QWidget's event handlers to receive the events generated for "
                          "the application's widgets:</p><p> We reimplement the mouse event "
                          "handlers to facilitate drawing, the paint event handler to "
                          "update the application and the resize event handler to optimize "
                          "the application's appearance. In addition we reimplement the "
                          "close event handler to intercept the close events before "
                          "terminating the application.</p><p> The example also demonstrates "
                          "how to use QPainter to draw an image in real time, as well as "
                          "to repaint widgets.</p>"));
}

void MainWindow::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    const QList<QByteArray> imageFormats = QImageWriter::supportedImageFormats();
    for (const QByteArray &format : imageFormats) {
        QString text = tr("%1...").arg(QString::fromLatin1(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, &QAction::triggered, this, &MainWindow::save);
        saveAsActs.append(action);
    }

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, &QAction::triggered, this, &MainWindow::penColor);

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, &QAction::triggered, this, &MainWindow::penWidth);

    penToolAct = new QAction(tr("&Карандаш"), this);
    penToolAct->setCheckable(true);
    penToolAct->setChecked(true);
    penToolAct->setShortcut(tr("Ctrl+1"));
    connect(penToolAct, &QAction::triggered, this, &MainWindow::setPenTool);

    hatchingToolAct = new QAction(tr("&Штриховка"), this);
    hatchingToolAct->setCheckable(true);
    hatchingToolAct->setShortcut(tr("Ctrl+2"));
    connect(hatchingToolAct, &QAction::triggered, this, &MainWindow::setHatchingTool);

    hatchingMetalAct = new QAction(tr("&Металлы и твёрдые сплавы"), this);
    hatchingMetalAct->setCheckable(true);
    connect(hatchingMetalAct, &QAction::triggered, this, &MainWindow::setHatchingMetal);

    hatchingNonMetalAct = new QAction(tr("&Неметаллические материалы"), this);
    hatchingNonMetalAct->setCheckable(true);
    connect(hatchingNonMetalAct, &QAction::triggered, this, &MainWindow::setHatchingNonMetal);

    hatchingWoodAct = new QAction(tr("&Древесина"), this);
    hatchingWoodAct->setCheckable(true);
    connect(hatchingWoodAct, &QAction::triggered, this, &MainWindow::setHatchingWood);

    hatchingStoneAct = new QAction(tr("&Камень естественный"), this);
    hatchingStoneAct->setCheckable(true);
    connect(hatchingStoneAct, &QAction::triggered, this, &MainWindow::setHatchingStone);

    hatchingCeramicAct = new QAction(tr("&Керамика и силикатные материалы"), this);
    hatchingCeramicAct->setCheckable(true);
    connect(hatchingCeramicAct, &QAction::triggered, this, &MainWindow::setHatchingCeramic);

    hatchingConcreteAct = new QAction(tr("&Бетон"), this);
    hatchingConcreteAct->setCheckable(true);
    connect(hatchingConcreteAct, &QAction::triggered, this, &MainWindow::setHatchingConcrete);

    hatchingGlassAct = new QAction(tr("&Стекло и светопрозрачные материалы"), this);
    hatchingGlassAct->setCheckable(true);
    connect(hatchingGlassAct, &QAction::triggered, this, &MainWindow::setHatchingGlass);

    hatchingLiquidAct = new QAction(tr("&Жидкости"), this);
    hatchingLiquidAct->setCheckable(true);
    connect(hatchingLiquidAct, &QAction::triggered, this, &MainWindow::setHatchingLiquid);

    hatchingSoilAct = new QAction(tr("&Грунт естественный"), this);
    hatchingSoilAct->setCheckable(true);
    connect(hatchingSoilAct, &QAction::triggered, this, &MainWindow::setHatchingSoil);

    QActionGroup *toolGroup = new QActionGroup(this);
    toolGroup->addAction(penToolAct);
    toolGroup->addAction(hatchingToolAct);
    toolGroup->addAction(hatchingMetalAct);
    toolGroup->addAction(hatchingNonMetalAct);
    toolGroup->addAction(hatchingWoodAct);
    toolGroup->addAction(hatchingStoneAct);
    toolGroup->addAction(hatchingCeramicAct);
    toolGroup->addAction(hatchingConcreteAct);
    toolGroup->addAction(hatchingGlassAct);
    toolGroup->addAction(hatchingLiquidAct);
    toolGroup->addAction(hatchingSoilAct);
    toolGroup->setExclusive(true);

    hatchAngleAct = new QAction(tr("Угол &штриховки..."), this);
    connect(hatchAngleAct, &QAction::triggered, this, &MainWindow::setHatchAngle);

    hatchSpacingAct = new QAction(tr("&Расстояние между линиями..."), this);
    connect(hatchSpacingAct, &QAction::triggered, this, &MainWindow::setHatchSpacing);

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, &QAction::triggered,
            paintView, &PaintView::clearImage);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createMenus()
{
    saveAsMenu = new QMenu(tr("&Save As"), this);
    for (QAction *action : std::as_const(saveAsActs))
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    hatchingSubMenu = new QMenu(tr("&Штриховка"), this);
    hatchingSubMenu->addAction(hatchingMetalAct);
    hatchingSubMenu->addAction(hatchingNonMetalAct);
    hatchingSubMenu->addAction(hatchingWoodAct);
    hatchingSubMenu->addAction(hatchingStoneAct);
    hatchingSubMenu->addAction(hatchingCeramicAct);
    hatchingSubMenu->addAction(hatchingConcreteAct);
    hatchingSubMenu->addAction(hatchingGlassAct);
    hatchingSubMenu->addAction(hatchingLiquidAct);
    hatchingSubMenu->addAction(hatchingSoilAct);

    toolsMenu = new QMenu(tr("&Инструменты"), this);
    toolsMenu->addAction(penToolAct);
    toolsMenu->addAction(hatchingToolAct);
    toolsMenu->addMenu(hatchingSubMenu);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(hatchAngleAct);
    optionMenu->addAction(hatchSpacingAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave()
{
    if (paintView->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("The image has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard
                                       | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile("png");
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    initialPath,
                                                    tr("%1 Files (*.%2);;All Files (*)")
                                                        .arg(QString::fromLatin1(fileFormat.toUpper()))
                                                        .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
        return false;
    return paintView->saveImage(fileName, fileFormat.constData());
}
