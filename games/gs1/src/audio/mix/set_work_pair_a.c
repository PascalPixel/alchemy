#include "types.h"

extern u16 Data_02003034;
extern u16 Data_02003010;

void Audio_SetWorkPairA(u16 primary, u16 secondary)
{
    Data_02003034 = primary;
    Data_02003010 = secondary;
}
