#include "TYPES.H"

extern u8 Data_03001e98[];

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Resource_PushPendingPair(u32 first, u32 second)
{
    struct State_0801a7c0 *state = gResQueueWork;
    u16 cnt = state->cnt;

    if (cnt != 16) {
        state->first[cnt] = first;
        state->second[cnt] = second;
        state->cnt++;
    }
}
