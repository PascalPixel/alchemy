#include "types.h"

extern u16 Data_02003030;
extern u16 Data_0200300c;

void Func_080f94f8(u16 primary, u16 secondary)
{
    Data_02003030 = primary;
    Data_0200300c = secondary;
}
