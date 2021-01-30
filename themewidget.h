
#ifndef THEMEWIDGET_H
#define THEMEWIDGET_H

#include <QtWidgets/QWidget>
#include <QtCharts/QChartGlobal>
#include "types.h"
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

    template <typename Calculation::MemberType_t>
    qreal getValueOfOperation(Calculation::DataAcquired_t& data,
                                          int timeStamp);

    void setBorderValues(Calculation::DataAcquired_t& data,
                         std::vector<qreal> xValVector,
                         std::vector<qreal> yValVector);

};

template <>
qreal ThemeWidget::getValueOfOperation<Calculation::MemberType_t::InertionFirstOrder>(Calculation::DataAcquired_t& data,
                                                                                      int timeStamp)
{
    qreal result;

    if(this->_whichResponseIsPicked == Calculation::ResponseType_t::Step)
    {
        result = data.k * (1 - exp(-qreal(timeStamp)/qreal(data.t1)));
    }
    else
    {
        result = data.k / qreal(data.t1) * exp(-(qreal)timeStamp/(qreal)data.t1);
    }

    return result;
}

#endif /* THEMEWIDGET_H */
