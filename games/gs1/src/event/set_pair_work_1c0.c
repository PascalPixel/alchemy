#include "event_runtime.h"

extern struct EventPairWork1c0 Data_02000240;

void Event_SetPairWork1c0(u16 first, u16 second)
{
    Data_03001ebc->value_170 = 999;
    Data_02000240.first = first;
    Data_02000240.second = second;
}
