
#include "VisualStation.h"

#include "SolidModel.h"
#include "PointsModel.h"
#include "ToolpathModel.h"
#include "FileModel.h"

#include "vtkTextActor.h"
#include "vtkTextProperty.h"

#include "qfiledialog.h"

#include "ribbon.h"
#include "ribbontabcontent.h"
#include "ribbonbuttongroup.h"

VisualStation::VisualStation(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VisualStationClass())
{
    ui->setupUi(this);
    this->setAcceptDrops(true);

    // 导入文件
    connect(ui->actionOpen, &QAction::triggered, [=]() {
        QString filePath = QFileDialog::getOpenFileName(
            this, tr("Open file"), "./", "file (*.csv *.cls *.stl)");
        if (ui->stackedWidget_Views->currentWidget() == ui->stackedWidget_ViewsPage_2D)
            ui->viewWidget_2D->loadFile(filePath);
        else if (ui->stackedWidget_Views->currentWidget() == ui->stackedWidget_ViewsPage_3D)
            ui->viewWidget_3D->loadFile(filePath);
        });

    // 清空视图删除所选项
    connect(ui->actionClear, &QAction::triggered, [=]() {
        if (ui->stackedWidget_Views->currentWidget() == ui->stackedWidget_ViewsPage_2D)
            ui->viewWidget_2D->clear();
        else if (ui->stackedWidget_Views->currentWidget() == ui->stackedWidget_ViewsPage_3D)
            ui->viewWidget_3D->clear();
        });

    //// 删除所选项
    //connect(ui->actionDelete, &QAction::triggered, [=]() {
    //   if (ui->stackedWidget_Views->currentWidget() == ui->stackedWidget_ViewsPage_3D)
    //        ui->viewWidget_3D->removePickedActor();
    //    });

    // 切换视图
    connect(ui->actionSwitchTo2D, &QAction::triggered,
        [=]() { ui->stackedWidget_Views->setCurrentWidget(ui->stackedWidget_ViewsPage_2D); });
    connect(ui->actionSwitchTo3D, &QAction::triggered,
        [=]() { ui->stackedWidget_Views->setCurrentWidget(ui->stackedWidget_ViewsPage_3D); });

    // 载入点云模型
    qRegisterMetaType<PointsModel*>("PointsModel*");
    connect(ui->viewWidget_3D, &ViewWidget_3D::pointsModelAdded, [=](PointsModel* pointsModel) {
        auto action = ui->menuBar->addMenu(pointsModel->buildFeatureMenu());
        connect(pointsModel, &PointsModel::removedFromView , [=]() {
            if (ui->stackedWidget_Views->currentWidget() == ui->stackedWidget_ViewsPage_3D)
            {
                ui->menuBar->removeAction(action);
                delete action;
            };
            });
        });

    ui->stackedWidget_Views->setCurrentWidget(ui->stackedWidget_ViewsPage_3D);
}

VisualStation::~VisualStation()
{
    delete ui;
}
