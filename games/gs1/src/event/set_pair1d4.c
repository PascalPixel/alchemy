#include "types.h"
#include "event_runtime.h"

/* event/set_pair1d4.c */
struct State_08091fa8 {
    u8 filler0[0x170];
    u16 value;
};

struct EventCellPairs {
    u8 filler0[0x1C0];
    u16 first_1c0;
    u16 second_1c2;
    u8 filler1c4[14];
    u16 first_1d4;
    u16 second_1d6;
};

extern s32 gCell[];

void Event_SetPair1d4(u16 first, u16 second)
{
    ((struct EventCellPairs *)gCell)->first_1d4 = first;
    ((struct EventCellPairs *)gCell)->second_1d6 = second;
}
