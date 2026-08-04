#include "types.h"

/*
 * Complete scene-variant eleven actor-zero presentation sequence.  Two paired
 * tile/presentation updates are separated by four-frame pauses before actor
 * zero is normalized and placed for the scene.
 */
struct Actor_02000c1c {
    u8 reserved00[35];
    u8 control_flags;
    u8 reserved24[44];
    u8 *presentation;
};

extern struct Actor_02000c1c *Func_0808a080(s32 actor);
extern void Func_080f9010(s32 value);
extern void Func_08009180(s32 kind, s32 first_x, s32 second_x, s32 value,
                          s32 first_mode, s32 second_mode);
extern void Func_0808a010(s32 frames);
extern void Func_080091c0(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);
extern void Func_02000a80(s32 x, s32 y, s32 value);

void Func_02000c1c(void)
{
    struct Actor_02000c1c *actor;

    actor = Func_0808a080(0);
    Func_080f9010(188);
    Func_08009180(42, 33, 34, 16, 2, 2);
    Func_08009180(42, 35, 36, 16, 2, 2);
    Func_0808a010(4);
    Func_08009180(40, 33, 34, 16, 2, 2);
    Func_08009180(40, 35, 36, 16, 2, 2);
    Func_0808a010(4);
    Func_080091c0(33, 21, 2, 2, 3, 16);
    actor->control_flags &= (u8)~1;
    actor->presentation[9] |= 12;
    Func_02000a80(64, 272, 11);
}
