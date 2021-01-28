#ifndef CALCULATION_HPP
#define CALCULATION_HPP

#include "types.h"

namespace Calculation
{
    class Calculatator
    {
      public:
        Calculatator();

        DataTable* GetProportional();

        DataTable* GetInertionFirstOrder(Calculation::DataAcquired_t data,
                                         Calculation::ResponseType_t response);
    };
}

#endif // CALCULATION_HPP
