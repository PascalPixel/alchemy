#include "types.h"

extern u16 Data_02003030;
extern u16 Data_0200300c;

void Audio_SetWorkPairB(u16 primary, u16 secondary)
{
    Data_02003030 = primary;
    Data_0200300c = secondary;
}
