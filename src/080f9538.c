#include "types.h"

extern u16 Data_02003034;
extern u16 Data_02003010;

void Func_080f9538(u16 primary, u16 secondary)
{
    Data_02003034 = primary;
    Data_02003010 = secondary;
}
