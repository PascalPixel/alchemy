#include "types.h"

#define SceneActor_ResetActorRun Func_0200345c
/*
 * Reset or tear down a consecutive run of scene actors -- resource_373.
 */
struct Resource373Actor {
    u8 unknown_00[8];
    s32 field08;
    s32 field0c;
    s32 field10;
    u8 unknown_14[0x41];
    u8 flag55;
};

struct Resource373Actor *Func_02009458(s32 selector);
void Func_020093d0(struct Resource373Actor *actor, s32 mode);
void Func_020094ec(s32 selector, s32 a, s32 b);

/*
 * The owner extends through its one literal pool word. The run length is
 * unsigned and the selector advances once per iteration. The Func_ aliases
 * name the call words encoded in the overlay image, not verified runtime
 * addresses.
 */
void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode)
{
    s32 selector = first;
    u32 i;

    if (mode == 0) {
        for (i = 0; i < count; i++) {
            struct Resource373Actor *actor = Func_02009458(selector);

            actor->flag55 = 0;
            Func_020093d0(actor, 0);
            actor->field08 = 0x01860000;   /* 0xc3 << 17. */
            actor->field0c = 0x00a00000;   /* 0xa0 << 16. */
            actor->field10 = 0x034a0000;   /* The literal pool word. */
            selector++;
        }
        return;
    }

    for (i = 0; i < count; i++) {
        Func_020094ec(selector, 0, 0);
        selector++;
    }
}
