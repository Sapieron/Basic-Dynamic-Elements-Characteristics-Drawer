#ifndef CALCULATION_HPP
#define CALCULATION_HPP

#include "types.h"
#include <vector>

#include "timeCharts.hpp"


namespace Calculation
{
    using Data = QPair<QPointF, QString>;
    using DataList = QList<Data>;
    using DataTable = QList<DataList>;

    class Calculator
    {
      public:
//        Calculator(ICalculator& timeChartCalculation,
//                   ICalculator& amplitudePhaseChartCalculation);
        Calculator() = default;

        DataTable calculate(DataAcquired_t& data, QPair<int, int> span);

      private:
//        ICalculator& _timeChartCalculation;
//        ICalculator& _amplitudePhaseChartCalculation;
        TimeChartCalculation _timeChartCalculation;

    };
}

#endif // CALCULATION_HPP
