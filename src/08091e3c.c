#include "types.h"

struct State {
    u8 unknown_000[368];
    u16 value;
};

struct Work {
    u8 unknown_000[448];
    u16 first;
    u16 second;
};

extern struct State *Data_03001ebc;
extern struct Work Data_02000240;

void Func_08091e3c(u16 first, u16 second)
{
    Data_03001ebc->value = 999;
    Data_02000240.first = first;
    Data_02000240.second = second;
}
