#include "ICalculations.hpp"
#include <limits>

using Calculation::ICalculation;

qreal ICalculation::diracDelta(qreal timePoint) const    //FIXME Interface with a source file? What is that?
{
    if(timePoint != 0)
    {
        return 0;
    }
    else
    {
        return std::numeric_limits<std::uint32_t>::max();   //FIXME probably conflicting return types
    }
}
