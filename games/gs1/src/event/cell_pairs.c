#include "types.h"
#include "event_runtime.h"

struct EventCellPairs {
    u8 unknown_000[0x1C0];
    u16 first_1c0;
    u16 second_1c2;
    u8 unknown_1c4[14];
    u16 first_1d4;
    u16 second_1d6;
};

extern s32 gCell[];
extern struct EventRuntime *Data_03001ebc;

void Event_SetPair1d4(u16 first, u16 second)
{
    ((struct EventCellPairs *)gCell)->first_1d4 = first;
    ((struct EventCellPairs *)gCell)->second_1d6 = second;
}

void Event_SetPair1c0AndSetValue170(u16 first, u16 second)
{
    struct EventRuntime *work = Data_03001ebc;
    ((struct EventCellPairs *)gCell)->first_1c0 = first;
    ((struct EventCellPairs *)gCell)->second_1c2 = second;
    work->value_170 = 999;
}
