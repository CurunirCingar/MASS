#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QDebug>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QPixmap>
#include <QIcon>
#include <QJsonObject>
#include <QRgb>
#include <QVariant>

#include "../NeuralNetworkModel/ineuralnetworkmodel.h"

namespace Ui {
class MainForm;
}
//! \addtogroup PerceptronNetworkViewPlugin_imp
//! \{
struct NetworkParams{
    int maxEpoch;
    float trainErrorThreshold;
    float testErrorThreshold;
    float minWeight;
    float maxWeight;
};

struct LayerParams{
    int size;
    float LearnSpeed;
    float Moment;
    float FuncIndent;
    float Bias;
};

enum LayerType{
    Input,
    Hidden,
    Output
};

class MainForm : public QWidget
{
    Q_OBJECT

public:
    QVector<InputSampleF> trainingSamples;
    QVector<InputSampleF> testSamples;
    QStandardItemModel itemModel;
    QStandardItemModel trainImagesModel;
    QStandardItemModel testImagesModel;

    explicit MainForm(QWidget *parent = 0);
    ~MainForm();
    void SetModel(INeuralNetworkModel *Model);
signals:
    void onClose();

private slots:
    void MarkNetworkStatsToUpdate();
    void on_buttonRunTrain_clicked();
    void on_buttonClose_clicked();
    void on_buttonAdd_clicked();
    void on_buttonRemove_clicked();

    void on_listView_clicked(const QModelIndex &index);
    void on_spinSize_valueChanged(int arg1);
    void on_spinLearnSpeed_valueChanged(double arg1);
    void on_spinMoment_valueChanged(double arg1);
    void on_spinFuncIndent_valueChanged(double arg1);
    void on_spinBias_valueChanged(double arg1);

    void on_buttonLoadImage_clicked();
    void on_buttonRemoveImages_clicked();
    void on_buttonLoadImage_2_clicked();
    void on_buttonRemoveImages_2_clicked();
    void on_spinEpoch_editingFinished();
    void on_spinErrorThreshold_editingFinished();
    void on_spinMinWeight_editingFinished();
    void on_spinMaxWeight_editingFinished();
    void on_spinTestErrorThreshold_editingFinished();
    void on_buttonResumeTraining_clicked();

private:
    Ui::MainForm *ui;
    INeuralNetworkModel *model;
    INeuralNetworkModel::INeuralNetwork *network;
    QModelIndex currentIndex;
    bool isStatsChanged;
    QList<LayerParams> layersList;
    QList<QImage> trainImages;
    QList<QImage> testImages;
    QVector<double> trainErrorVector;
    QVector<double> testErrorVector;
    int epoch;
    QVector<double> xValues;

    void RunTrainFromEpoch(int maxEpoch);
    bool UpdateNetworkStats();
    void UpdateLayerStatsGUI();
    void MakePlot(int graph, QVector<double> &x, QVector<double> &y);
    void ReplotPlot();
    QJsonObject ConvertNetworkParams();
    QJsonObject ConvertLayerParams(LayerType type, LayerParams &params);
};
//! \}
#endif // MAINFORM_H
