/* Stolen from https://stackoverflow.com/questions/60276971/how-to-set-qchart-aspect-ratio */

#include "chartFormatter.hpp"
#include <QtCharts/QValueAxis>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QXYSeries>
#include <QtCharts/QChartView>

#include <cmath>

using QtCharts::QValueAxis;
using QtCharts::QAbstractSeries;
using QtCharts::QXYSeries;
using QtCharts::QChartView;

void QChartView_scaledAxis::resizeEvent(QResizeEvent *event)
{
    QChartView::resizeEvent(event);

    // Get the axis of the graph
    QValueAxis* axisX = qobject_cast<QValueAxis*>(this->chart()->axes(Qt::Horizontal)[0]);
    QValueAxis* axisY = qobject_cast<QValueAxis*>(this->chart()->axes(Qt::Vertical)[0]);

    // Get the series displayed on the graph
    const QList<QAbstractSeries*> series = this->chart()->series();

    // get Min Max values (on X and Y) of all the points plotted on the graph
    float minX = std::numeric_limits<double>::max();
    float maxX = std::numeric_limits<double>::min();
    float minY = std::numeric_limits<double>::max();
    float maxY = std::numeric_limits<double>::min();
    for(QAbstractSeries *p_serie : series) { //iterate on all the series in the graph
        //Assuming all the series in the graph are QXYSeries...
        for(QPointF p : qobject_cast<QXYSeries*>(p_serie)->points()) { //iterate on each point of each serie
            minX = fmin(minX, p.x());
            maxX = fmax(maxX, p.x());
            minY = fmin(minY, p.y());
            maxY = fmax(maxY, p.y());
        }
    }

    // Get the points at both ends of the axis (will help to determine the plottable area in pixel)
    const QPointF minPosition = this->chart()->mapToPosition(QPointF(axisX->min(), axisY->min()));
    const QPointF maxPosition = this->chart()->mapToPosition(QPointF(axisX->max(), axisY->max()));

    // Ration between the size of the axis in pixel and in term of represented value
    const double axisX_PixToValue = (maxX - minX) / (maxPosition.x() - minPosition.x());
    const double axisY_PixToValue = (maxY - minY) / (maxPosition.y() - minPosition.y());

    // The smallest ratio must be 'kept' and applied to the other axis
    if(abs(axisX_PixToValue) > abs(axisY_PixToValue)) {
        axisY->setMin(minY);
        axisY->setMax(minY + (maxPosition.y() - minPosition.y()) * std::copysign(axisX_PixToValue, axisY_PixToValue));
    } else {
        axisX->setMin(minX);
        axisX->setMax(minX + (maxPosition.x() - minPosition.x()) * std::copysign(axisY_PixToValue, axisX_PixToValue));
    }
}
