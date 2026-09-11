#include "types.h"
#include "scene.h"
#include "abi/event/set_pair1d4.h"

struct State_08091fa8 {
    u8 filler0[0x170];
    u16 value;
};

struct gRom {
    u8 filler0[0x1C0];
    u16 first_1c0;
    u16 second_1c2;
    u8 filler1c4[14];
    u16 first_1d4;
    u16 second_1d6;
};

extern struct State_08091fa8 *volatile gWork;
extern struct gRom gCell;

void Event_SetPair1d4(u16 first, u16 second)
{
    gCell.first_1d4 = first;
    gCell.second_1d6 = second;
}

void Sys_Run(u16 first, u16 second)
{
    struct State_08091fa8 *state = gWork;
    gCell.first_1c0 = first;
    gCell.second_1c2 = second;
    state->value = 999;
}
