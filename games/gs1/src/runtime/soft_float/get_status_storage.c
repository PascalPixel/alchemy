#include "types.h"

extern u8 Data_0200df90;

#define GetSoftFloatStatusStorage Func_02005ab8
u8 *GetSoftFloatStatusStorage(void)
{
    return &Data_0200df90;
}
