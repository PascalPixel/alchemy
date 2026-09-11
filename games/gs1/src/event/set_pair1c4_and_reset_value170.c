#include "types.h"
#include "scene.h"
#include "abi/event/set_pair1c4_and_reset_value170.h"

struct State_08091e6c {
    u8 filler0[0x170];
    u16 value;
};

struct gRom {
    u8 filler0[0x1C4];
    u16 first;
    u16 second;
};

extern struct State_08091e6c *volatile gWork;
extern struct gRom gCell;

void Event_SetPair1c4AndResetValue170(u16 first, u16 second)
{
    struct State_08091e6c *state = gWork;
    state->value = 999;
    gCell.first = first;
    gCell.second = second;
}

void Sys_Run(u16 value)
{
    struct State_08091e6c *state = gWork;
    state->value = value;
}
