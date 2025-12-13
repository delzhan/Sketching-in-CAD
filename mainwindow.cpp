// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwindow.h"
#include "scribblearea.h"

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
    : QMainWindow(parent), scribbleArea(new ScribbleArea(this))
{
    setCentralWidget(scribbleArea);

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
            scribbleArea->openImage(fileName);
    }
}

void MainWindow::setPenTool()
{
    scribbleArea->setCurrentTool(ScribbleArea::Pen);
}

// ============================================================================
// СЛОТЫ ДЛЯ РАЗЛИЧНЫХ ТИПОВ ШТРИХОВКИ
// ============================================================================

// Металлы и твёрдые сплавы
void MainWindow::setHatchingMetal()
{
    scribbleArea->setCurrentTool(ScribbleArea::Hatching);
    // Стандартные параметры для металлов
    scribbleArea->setHatchAngle(45);
    scribbleArea->setHatchSpacing(5);
}

// Неметаллические материалы
void MainWindow::setHatchingNonMetal()
{
    scribbleArea->setCurrentTool(ScribbleArea::Hatching);
    // Более частая штриховка для неметаллов
    scribbleArea->setHatchAngle(45);
    scribbleArea->setHatchSpacing(3);
}

// Древесина
void MainWindow::setHatchingWood()
{
    scribbleArea->setCurrentTool(ScribbleArea::Hatching);
    // Древесина - часто используется под углом 0 градусов (горизонтально)
    scribbleArea->setHatchAngle(0);
    scribbleArea->setHatchSpacing(4);
}

// Камень естественный
void MainWindow::setHatchingStone()
{
    scribbleArea->setCurrentTool(ScribbleArea::Hatching);
    // Камень - более редкая штриховка
    scribbleArea->setHatchAngle(45);
    scribbleArea->setHatchSpacing(6);
}

// Керамика и силикатные материалы для кладки
void MainWindow::setHatchingCeramic()
{
    scribbleArea->setCurrentTool(ScribbleArea::Hatching);
    // Керамика - стандартная штриховка
    scribbleArea->setHatchAngle(45);
    scribbleArea->setHatchSpacing(5);
}

// Бетон
void MainWindow::setHatchingConcrete()
{
    scribbleArea->setCurrentTool(ScribbleArea::Hatching);
    // Бетон - редкая прерывистая штриховка
    scribbleArea->setHatchAngle(45);
    scribbleArea->setHatchSpacing(8);
}

// Стекло и другие светопрозрачные материалы
void MainWindow::setHatchingGlass()
{
    scribbleArea->setCurrentTool(ScribbleArea::Hatching);
    // Стекло - очень редкая штриховка (почти прозрачно)
    scribbleArea->setHatchAngle(45);
    scribbleArea->setHatchSpacing(12);
}

// Жидкости
void MainWindow::setHatchingLiquid()
{
    scribbleArea->setCurrentTool(ScribbleArea::Hatching);
    // Жидкости - очень частая штриховка
    scribbleArea->setHatchAngle(45);
    scribbleArea->setHatchSpacing(2);
}

// Грунт естественный
void MainWindow::setHatchingSoil()
{
    scribbleArea->setCurrentTool(ScribbleArea::Hatching);
    // Грунт - неравномерная штриховка
    scribbleArea->setHatchAngle(45);
    scribbleArea->setHatchSpacing(5);
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"),
                                        tr("Select pen width:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}

void MainWindow::setHatchAngle()
{
    bool ok;
    int angle = QInputDialog::getInt(this, tr("Угол штриховки"),
                                     tr("Введите угол штриховки (0-180 градусов):"),
                                     45, 0, 180, 1, &ok);
    if (ok)
        scribbleArea->setHatchAngle(angle);
}

void MainWindow::setHatchSpacing()
{
    bool ok;
    int spacing = QInputDialog::getInt(this, tr("Расстояние между линиями"),
                                       tr("Введите расстояние между линиями штриховки:"),
                                       10, 1, 50, 1, &ok);
    if (ok)
        scribbleArea->setHatchSpacing(spacing);
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

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, &QAction::triggered, scribbleArea, &ScribbleArea::print);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &MainWindow::close);

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, &QAction::triggered, this, &MainWindow::penColor);

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, &QAction::triggered, this, &MainWindow::penWidth);

    // Инструмент "Карандаш"
    penToolAct = new QAction(tr("&Карандаш"), this);
    penToolAct->setCheckable(true);
    penToolAct->setChecked(true);
    penToolAct->setShortcut(tr("Ctrl+1"));
    connect(penToolAct, &QAction::triggered, this, &MainWindow::setPenTool);

    // ========================================================================
    // ДЕЙСТВИЯ ДЛЯ РАЗЛИЧНЫХ ТИПОВ ШТРИХОВКИ
    // ========================================================================

    // Металлы и твёрдые сплавы
    hatchingMetalAct = new QAction(tr("&Металлы и твёрдые сплавы"), this);
    hatchingMetalAct->setCheckable(true);
    connect(hatchingMetalAct, &QAction::triggered, this, &MainWindow::setHatchingMetal);

    // Неметаллические материалы
    hatchingNonMetalAct = new QAction(tr("&Неметаллические материалы"), this);
    hatchingNonMetalAct->setCheckable(true);
    connect(hatchingNonMetalAct, &QAction::triggered, this, &MainWindow::setHatchingNonMetal);

    // Древесина
    hatchingWoodAct = new QAction(tr("&Древесина"), this);
    hatchingWoodAct->setCheckable(true);
    connect(hatchingWoodAct, &QAction::triggered, this, &MainWindow::setHatchingWood);

    // Камень естественный
    hatchingStoneAct = new QAction(tr("&Камень естественный"), this);
    hatchingStoneAct->setCheckable(true);
    connect(hatchingStoneAct, &QAction::triggered, this, &MainWindow::setHatchingStone);

    // Керамика и силикатные материалы для кладки
    hatchingCeramicAct = new QAction(tr("&Керамика и силикатные материалы"), this);
    hatchingCeramicAct->setCheckable(true);
    connect(hatchingCeramicAct, &QAction::triggered, this, &MainWindow::setHatchingCeramic);

    // Бетон
    hatchingConcreteAct = new QAction(tr("&Бетон"), this);
    hatchingConcreteAct->setCheckable(true);
    connect(hatchingConcreteAct, &QAction::triggered, this, &MainWindow::setHatchingConcrete);

    // Стекло и другие светопрозрачные материалы
    hatchingGlassAct = new QAction(tr("&Стекло и светопрозрачные материалы"), this);
    hatchingGlassAct->setCheckable(true);
    connect(hatchingGlassAct, &QAction::triggered, this, &MainWindow::setHatchingGlass);

    // Жидкости
    hatchingLiquidAct = new QAction(tr("&Жидкости"), this);
    hatchingLiquidAct->setCheckable(true);
    connect(hatchingLiquidAct, &QAction::triggered, this, &MainWindow::setHatchingLiquid);

    // Грунт естественный
    hatchingSoilAct = new QAction(tr("&Грунт естественный"), this);
    hatchingSoilAct->setCheckable(true);
    connect(hatchingSoilAct, &QAction::triggered, this, &MainWindow::setHatchingSoil);

    // Создаем группу действий для инструментов
    QActionGroup *toolGroup = new QActionGroup(this);
    toolGroup->addAction(penToolAct);

    // Добавляем все типы штриховки в группу инструментов
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

    // Действия для настройки штриховки
    hatchAngleAct = new QAction(tr("Угол &штриховки..."), this);
    connect(hatchAngleAct, &QAction::triggered, this, &MainWindow::setHatchAngle);

    hatchSpacingAct = new QAction(tr("&Расстояние между линиями..."), this);
    connect(hatchSpacingAct, &QAction::triggered, this, &MainWindow::setHatchSpacing);

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, &QAction::triggered,
            scribbleArea, &ScribbleArea::clearImage);

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
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    // ========================================================================
    // СОЗДАНИЕ ПОДМЕНЮ ДЛЯ ШТРИХОВКИ
    // ========================================================================
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

    // Меню инструментов
    toolsMenu = new QMenu(tr("&Инструменты"), this);
    toolsMenu->addAction(penToolAct);
    toolsMenu->addMenu(hatchingSubMenu);  // Добавляем подменю штриховки

    // Меню настроек
    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();

    // Добавляем настройки штриховки в меню Options
    optionMenu->addAction(hatchAngleAct);
    optionMenu->addAction(hatchSpacingAct);

    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    // Меню помощи
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    // Добавляем все меню в строку меню
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(toolsMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave()
{
    if (scribbleArea->isModified()) {
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
    return scribbleArea->saveImage(fileName, fileFormat.constData());
}
