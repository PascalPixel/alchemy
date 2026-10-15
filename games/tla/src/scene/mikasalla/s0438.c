#include "local.h"
#include "scene_ids.h"

void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Func_020006b8(s32 value);
void Func_020006f0(s32 value);

static __inline__ void Func_02000618_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02000618(a, b, c, d, e, f);
}

void Func_02000438(void)
{
    s32 sound;
    s32 drop;

    if (gPartyState.scene == (s32)&SceneId_Ov65dRoom1)
    {
        sound = 12;
        drop = 17;
    }
    else
    {
        sound = 3;
        drop = 9;
    }
    ObjectMotion_ArmCallback(4, 0xc000, 0);
    Battle_WaitMode0(10);
    Object_GetById(4)->actionFlags &= ~1;
    Func_02000618_(96, 14, 72, 14, 2, 1);
    ObjectMotion_OffsetPositionAndResetMotion(4, 0, 4);
    ObjectMotion_CommitCurrentPositionAndActivate(4);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(4), 0);
    Object_SetModeById(4, 13);
    ObjectMotion_OffsetPositionAndResetMotion(4, 0, drop);
    ObjectMotion_CommitCurrentPositionAndActivate(4);
    Object_SetModeById(4, 10);
    Battle_WaitMode0(10);
    Func_020006f0(123);
    Func_020006b8(sound);
    Func_02000650();
}
