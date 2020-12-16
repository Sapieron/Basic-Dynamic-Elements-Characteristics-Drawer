
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

private:
    int m_listCount;
    int m_valueMax;
    int m_valueCount;
    QList<QChartView *> m_charts;
    QChart *main_chart;
    Calculation::MemberType_t _whichMemberIsPicked;
    Calculation::DataTable m_dataTable;
    Ui_ThemeWidgetForm *m_ui;

    Calculation::DataTable calculate(Calculation::DataAcquired_t data,
                                     Calculation::ResponseType_t response,
                                     Calculation::MemberType_t   member);
};

#endif /* THEMEWIDGET_H */
