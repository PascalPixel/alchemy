#include "ENTRY_SETUP.H"

void SceneActor_ClearActorModeAndSetState5(s32 no)
{
    u8 *p;
    s32 mask;

    p = Actor_Get(no);
    p[0x55] = 0;
    mask = 252;
    mask &= p[0x59];
    p[0x59] = mask;
    Actor_SetSpriteFlags(p, 0);
    Object_SetAnimation(p, 5);
    Actor_SetSpritePriority(no, 3);
    {
        s32 v = 2;
        v |= p[0x23];
        p[0x23] = v;
    }
}

void SceneState_ApplyStepToSlots15To18(void)
{
    u32 i;

    i = 15;
    do {
        Actor_Get(i);
        i++;
    } while (i <= 18);
}
