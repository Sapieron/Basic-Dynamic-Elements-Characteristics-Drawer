
#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include "types.h"


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

private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;
    Calculation::DataAcquired_t _data;
    QList<QChartView *> m_charts;
    QChart *main_chart;
    Calculation::MemberType_t   _whichMemberIsPicked;
    Calculation::ResponseType_t _whichResponseIsPicked;
    Calculation::DataTable m_dataTable;
    Ui_ThemeWidgetForm *m_ui;

    Calculation::DataTable calculate(Calculation::DataAcquired_t& data);

    Calculation::DataTable proportionalCalculation(Calculation::DataAcquired_t& data);
    Calculation::DataTable inertionFirstOrderCalculation(Calculation::DataAcquired_t& data);
    Calculation::DataTable inertionSecondOrderCalculation(Calculation::DataAcquired_t& data);

    void setBorderValues(Calculation::DataAcquired_t& data,
                         std::vector<qreal> xValVector,
                         std::vector<qreal> yValVector);

    qreal getInertionFirstOrderValue(Calculation::DataAcquired_t& data,
                                     int timeStamp);
    qreal getInertionSecondOrderValue(Calculation::DataAcquired_t& data,
                                      int timeStamp);
    qreal getProportional(Calculation::DataAcquired_t& data,
                                                   int timeStamp);  //FIXME these 3 functions are temporary
};

#endif /* THEMEWIDGET_H */
