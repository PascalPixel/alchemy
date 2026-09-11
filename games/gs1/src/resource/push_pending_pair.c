#include "types.h"
#include "scene.h"
#include "abi/resource/push_pending_pair.h"

struct State_0801a7c0 {
    u8 filler0[0x354];
    u16 first[16];
    u16 second[16];
    u16 cnt;
};

extern struct State_0801a7c0 *volatile gIw;

void Resource_PushPendingPair(u32 first, u32 second)
{
    struct State_0801a7c0 *state = gIw;
    u16 cnt = state->cnt;

    if (cnt != 16) {
        state->first[cnt] = first;
        state->second[cnt] = second;
        state->cnt++;
    }
}
