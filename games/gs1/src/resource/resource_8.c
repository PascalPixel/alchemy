#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* resource/clear_owner_list_and_counters.c */
void Resource_ClearOwnerListAndCounters(void)
{
    void *state;

    state = *(void **)ADDR_03001E98;
    FIELD_AT_OFFSET(state, s32 *, 0x348) = 0;
    FIELD_AT_OFFSET(state, s16 *, 0x39A) = 0;
    if (0x80 & FIELD_AT_OFFSET(state, u16 *, 0x39E)) {
        FIELD_AT_OFFSET(state, s16 *, 0x39C) = 0;
        FIELD_AT_OFFSET(state, u16 *, 0x39E) = 0U;
    }
    FIELD_AT_OFFSET(state, s16 *, 0x3A0) = 0;
    FIELD_AT_OFFSET(state, s16 *, 0x394) = 0;
}

/* resource/push_pending_pair.c */
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
