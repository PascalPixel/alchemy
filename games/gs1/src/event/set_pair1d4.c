#include "types.h"

#define Event_SetPair1d4 Func_08091fa8

struct State_08091fa8 {
    u8 filler0[0x170];
    u16 value;
};

struct Data_08091fa8 {
    u8 filler0[0x1C0];
    u16 first_1c0;
    u16 second_1c2;
    u8 filler1c4[14];
    u16 first_1d4;
    u16 second_1d6;
};

extern struct State_08091fa8 * volatile Data_03001ebc;
extern struct Data_08091fa8 Data_02000240;

void Event_SetPair1d4(u16 first, u16 second)
{
    Data_02000240.first_1d4 = first;
    Data_02000240.second_1d6 = second;
}

void Func_08091fc0(u16 first, u16 second)
{
    struct State_08091fa8 *state = Data_03001ebc;
    Data_02000240.first_1c0 = first;
    Data_02000240.second_1c2 = second;
    state->value = 999;
}
