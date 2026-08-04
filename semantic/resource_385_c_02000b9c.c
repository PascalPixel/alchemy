#include "types.h"

/* Complete scene-variant nine actor-zero presentation setup through its pool. */
struct Actor_02000b9c {
    u8 reserved00[35];
    u8 control_flags;
    u8 reserved24[44];
    u8 *presentation;
};

extern const u8 Data_02009798[];
extern struct Actor_02000b9c *Func_0808a080(s32 actor);
extern void Func_080f9010(s32 value);
extern void Func_08009178(const void *descriptor, s32 value, s32 actor);
extern void Func_080091c0(s32 kind, s32 x, s32 enabled, s32 mode,
                          s32 first_value, s32 second_value);
extern void Func_02000a80(s32 x, s32 y, s32 value);

void Func_02000b9c(void)
{
    struct Actor_02000b9c *actor;

    actor = Func_0808a080(0);
    Func_080f9010(158);
    Func_08009178(Data_02009798, 49, 10);
    Func_080091c0(33, 20, 1, 3, 18, 10);
    actor->control_flags &= (u8)~1;
    actor->presentation[9] |= 12;
    Func_02000a80(296, 176, 9);
}
