
#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include "calculation.hpp"
#include <cmath>

QT_BEGIN_NAMESPACE
class QComboBox;
class QCheckBox;
class QLineEdit;
class QPushButton;
class Ui_ThemeWidgetForm;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE


QT_CHARTS_USE_NAMESPACE

class ThemeWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ThemeWidget(QWidget *parent = 0);
    ~ThemeWidget();

private Q_SLOTS:
    void updateUI();

private:
    Calculation::DataTable generateRandomData(int listCount, int valueMax, int valueCount) const;
    void populateThemeBox();
    void populateResponseTypeBox();
    void populateMemberTypeBox();
    void populateCharacteristicTypeBox();
    void connectSignals();
    QChart *createSplineChart() const;
    void showGraphGotPressed();
    void updateChart(Calculation::DataTable dataTable);
    bool isAllDataProvided();
    void connectCallbackToPushButton();
    void enableShowGraphButton();

private slots:
    void memberChangedCallback(int index);
    void responseChangedCallback(int index);
    void characteristicChangedCallback(int index);

private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;
    Calculation::DataAcquired_t _data;
    QList<QChartView *> m_charts;
    QChart *main_chart;
    Calculation::MemberType_t         _whichMemberIsPicked;
    Calculation::ResponseType_t       _whichResponseIsPicked;
    Calculation::CharacteristicType_t _whichCharactersiticIsPicked;
    Calculation::DataTable m_dataTable;
    Calculation::Calculator _calculator;
    Ui_ThemeWidgetForm *m_ui;
    QChartView *_chartView;

    Calculation::DataTable calculate(Calculation::DataAcquired_t& data);
};

#endif /* THEMEWIDGET_H */
