#ifndef CHARTFORMATTER_HPP
#define CHARTFORMATTER_HPP

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QWidget>
#include <QResizeEvent>

using QtCharts::QChartView;
using QtCharts::QChart;

class QChartView_scaledAxis : public QChartView
{
    Q_OBJECT
public:
    virtual ~QChartView_scaledAxis() {};

    void resizeEvent(QResizeEvent *event) override;
};

#endif // CHARTFORMATTER_HPP
