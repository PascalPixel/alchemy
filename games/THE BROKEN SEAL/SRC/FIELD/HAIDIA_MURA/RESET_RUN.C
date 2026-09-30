#include "STAGED_MOTION.H"

void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode)
{
    s32 selector = first;
    u32 i;

    if (mode == 0) {
        for (i = 0; i < count; i++) {
            struct Resource373Actor *actor = Actor_Get(selector);

            actor->flag55 = 0;
            Actor_SetSpriteFlags(actor, 0);
            actor->field08 = 0x01860000;   /* 0xc3 << 17. */
            actor->field0c = 0x00a00000;   /* 0xa0 << 16. */
            actor->field10 = 0x034a0000;   /* The literal pool word. */
            selector++;
        }
        return;
    }

    for (i = 0; i < count; i++) {
        Actor_SetPosition(selector, 0, 0);
        selector++;
    }
}
