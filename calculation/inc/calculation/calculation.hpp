#ifndef CALCULATION_HPP
#define CALCULATION_HPP

#include "types.h"
#include <vector>

#include "timeCalc.hpp"
#include "ampPhaseCalc.hpp"
#include "pidCalc.hpp"


namespace Calculation
{
    using Data = QPair<QPointF, QString>;
    using DataList = QList<Data>;
    using DataTable = QList<DataList>;

    class Calculator
    {
      public:
        Calculator() = default;

        DataTable calculate(DataAcquired_t& data, QPair<int, int> span);

      private:
        DataList calculateXAxis(DataTable *dataTable);
        DataList calculateYAxis(DataTable *dataTable);

        AmplitudePhaseCalculation _amplitudePhaseChartCalculation;
        TimeChartCalculation      _timeChartCalculation;
        PIDCalculation            _pidCalculation;
    };
}

#endif // CALCULATION_HPP
