#include "types.h"

/*
 * Complete actor placement and bounded motion-wait helper.  The packed value
 * at +40 carries the engine-owned pending halfword tested once per frame; the
 * helper waits for it to clear, but never for more than sixty frames.
 */
union ActorWait_02000970 {
    s32 packed;
    struct {
        u16 low;
        s16 pending;
    } halves;
};

struct Actor_02000970 {
    u8 reserved00[40];
    union ActorWait_02000970 wait;
    u8 reserved44[24];
    s32 motion_state;
    s32 speed;
};

extern struct Actor_02000970 *Func_0808a080(s32 actor);
extern void Func_0808a090(s32 actor, s32 width, s32 height);
extern void Func_080091e0(struct Actor_02000970 *actor, s32 enabled);
extern void Func_0808a0c0(s32 actor, s32 x, s32 y);
extern void Func_0808a0f0(s32 actor, s32 x_fixed, s32 y_fixed);
extern void Func_080000c0(s32 frames);

void Func_02000970(s32 actor_id, s32 x, s32 y, s32 wait_value)
{
    struct Actor_02000970 *actor;
    s32 frames_left;

    actor = Func_0808a080(actor_id);
    Func_0808a090(actor_id, 0x30000, 0x18000);
    actor->speed = 0x8000;
    actor->motion_state = 0;
    actor->wait.packed = wait_value;
    Func_080091e0(actor, 0);
    Func_0808a0c0(actor_id, x, y);
    Func_0808a0f0(actor_id, x << 16, y << 16);

    frames_left = 60;
    while (frames_left != 0) {
        Func_080000c0(1);
        if (actor->wait.halves.pending == 0) {
            break;
        }
        frames_left -= 1;
    }

    Func_080091e0(actor, 1);
    actor->speed = 0x10000;
}
