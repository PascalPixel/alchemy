#include "types.h"

#define Party_SetFields1ceAnd1d0 Func_08091f90

struct SharedData_08091f90 {
    u8 data[0x1ce];
    u16 first;
    u16 second;
};

extern struct SharedData_08091f90 Data_02000240;

void Party_SetFields1ceAnd1d0(u16 first, u16 second)
{
    Data_02000240.first = first;
    Data_02000240.second = second;
}
