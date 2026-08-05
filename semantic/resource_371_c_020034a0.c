#include "types.h"

/*
 * Complete actor-8 follower-effect spawner.  It runs only on the quiet input
 * phase, creates an effect just above/left of actor 8, gives it a randomized
 * lateral offset, then inherits the actor's presentation bits and variant.
 */
struct Effect_020034a0 {
    u8 reserved00[8];
    s32 x;
    s32 auxiliary;
    s32 y;
    u8 reserved14[4];
    s32 width;
    s32 height;
    u8 reserved20[3];
    u8 control_flags;
    u8 reserved24[44];
    u8 *presentation;
    u8 reserved54;
    u8 variant;
};

extern volatile s32 Data_03001e40;
extern const u8 Data_0200e73c[];
extern struct Effect_020034a0 *Func_0808a080(s32 actor);
extern struct Effect_020034a0 *Func_080090c8(s32 kind, s32 x, s32 auxiliary, s32 y);
extern u32 Func_080000f8(void);
extern void Func_0808a160(struct Effect_020034a0 *effect, s32 mode);
extern void Func_08009080(struct Effect_020034a0 *effect, s32 mode);
extern void Func_08009098(struct Effect_020034a0 *effect, const void *descriptor);

void Func_020034a0(void)
{
    struct Effect_020034a0 *actor;
    struct Effect_020034a0 *effect;
    u8 *presentation;
    u32 random_value;
    s32 offset;

    if ((Data_03001e40 & 15) != 0) {
        return;
    }

    actor = Func_0808a080(8);
    effect = Func_080090c8(222, actor->x - 0x200000,
                          actor->auxiliary, actor->y - 0x100000);
    if (effect == 0) {
        return;
    }

    effect->width = 0x8000;
    effect->height = 0x8000;
    presentation = effect->presentation;

    random_value = Func_080000f8();
    if (((random_value << 1) >> 16) != 0) {
        random_value = Func_080000f8();
        offset = (s32)((((random_value * 3u) << 4) >> 16) << 16);
        effect->x -= offset >> 1;
        effect->y -= offset;
    } else {
        offset = (s32)(((random_value << 5) >> 16) << 16);
        effect->x += offset;
        effect->y += offset >> 1;
        random_value = Func_080000f8();
    }

    presentation[38] = 0;
    presentation[9] = (presentation[9] & (u8)~12)
                    | (actor->presentation[9] & 12);
    effect->control_flags |= 2;
    effect->variant = actor->variant;
    Func_0808a160(effect, 9);
    Func_08009080(effect, 2);
    Func_08009098(effect, Data_0200e73c);
}
