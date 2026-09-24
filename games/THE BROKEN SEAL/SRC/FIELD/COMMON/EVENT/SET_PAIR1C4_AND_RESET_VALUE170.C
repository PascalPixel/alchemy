#include "TYPES.H"

struct State_08091e6c {
    u8 filler0[0x170];
    u16 value;
};

struct Data_08091e6c {
    u8 filler0[0x1C4];
    u16 first;
    u16 second;
};

extern struct State_08091e6c *gEventWork;
extern struct Data_08091e6c gGameState;

void Event_SetPair1c4AndResetValue170(u16 first, u16 second)
{
    struct State_08091e6c *state = gEventWork;
    state->value = 999;
    gGameState.first = first;
    gGameState.second = second;
}

void Func_08091e9c(u16 value)
{
    struct State_08091e6c *state = gEventWork;
    state->value = value;
}
