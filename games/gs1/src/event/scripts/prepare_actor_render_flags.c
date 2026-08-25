#include "types.h"

struct EventActor {
    u8 reserved_00[0x23];
    u8 flags;
    u8 reserved_24[0x2c];
    u8 *render_state;
};

#define prepare_event_actor_render_flags Func_02000030

s32 prepare_event_actor_render_flags(struct EventActor *actor)
{
    actor->flags &= ~1;
    actor->render_state[9] |= 0xc;
    actor->render_state[21] |= 0xc;
    return 0;
}
