#include "types.h"
#include "scene.h"

/* overlays/scene/state/interaction/dispatch_by_actor_zero_depth.c */
struct Actor {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
};

extern struct Actor *State_Run(s32);

void State_DispatchByActorZeroDepth(void)
{
    struct Actor *p = State_Run(0);

    if (p->f0c >= 0x100000) {
        State_unk2_4();
    } else {
        State_unk3_4();
    }
}

/* overlays/scene/state/interaction/set_entries_16_21_byte_35.c */
extern u8 *State_Run(s32);

void State_SetEntries16To21Byte35(void)
{
    s32 index = 16;
    s32 flag = 1;
    s32 remaining = 5;

    do {
        u8 *entry = State_Run(index);

        remaining--;
        entry[35] = flag;
        index++;
    } while (remaining >= 0);
}
