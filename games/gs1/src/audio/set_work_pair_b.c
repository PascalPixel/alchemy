#include "types.h"

#define Audio_SetWorkPairB Func_080f94f8

extern u16 Data_02003030;
extern u16 Data_0200300c;

void Audio_SetWorkPairB(u16 primary, u16 secondary)
{
    Data_02003030 = primary;
    Data_0200300c = secondary;
}
