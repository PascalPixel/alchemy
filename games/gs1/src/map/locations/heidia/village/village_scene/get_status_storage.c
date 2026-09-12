#include "types.h"

extern u8 Data_0200df90;

u8 *GetSoftFloatStatusStorage(void)
{
    return &Data_0200df90;
}
