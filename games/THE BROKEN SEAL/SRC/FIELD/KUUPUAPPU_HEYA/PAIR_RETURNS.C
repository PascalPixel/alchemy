#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKuupuappuTheyreBack[];
extern u8 KuupuappuHeya_PairScriptB[];
extern u8 KuupuappuHeya_PairScriptO[];

void SceneActor_SetModeZeroAndValue(s32 a, s32 b);
void Object_RefreshSelectorById(s32);

/* The "shown" halfword of an actor record. */
#define ACTOR_SHOWN_OFFSET 100

/* Actors 24 and 25 come back: one line, then each takes up its script and
 * its shown state. */
void FieldScene_ConfigurePairedActors(void)
{
    Event_Wait(30);
    Actor_RunRepeatedMotion(24, 1);
    Event_Wait(20);
    Event_SetMessage((s32)MsgKuupuappuTheyreBack);
    SceneActor_SetModeZeroAndValue(24, 20);
    Actor_FaceDirection(25, 0, 20);
    Actor_SetAttachedEffect(25, 0x102);
    Actor_RunRepeatedMotion(25, 2);
    SceneActor_SetModeZeroAndValue(25, 20);
    Actor_SetAnimationAndWait(24, 4);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(24, 20);
    Actor_SetSpeed(24, 0x40000, 0x20000);
    Actor_SetSpeed(25, 0x38000, 0x1c000);
    ((s32 (*)())Engine_ActorEnableActionCallback)(25, (s32)KuupuappuHeya_PairScriptO);
    Actor_EnableActionCallback(24, KuupuappuHeya_PairScriptB);
    Object_RefreshSelectorById(24);
    /* FAKEMATCH: each shown state is parked in a word-sized local before
     * its halfword store, which keeps the constants' loads where the
     * reference has them. */
    {
        u8 *record = (u8 *)Object_GetById(24);
        s32 shown = 1;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    {
        u8 *record = (u8 *)((s32 (*)())Object_GetById)(25);
        s32 shown = 3;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    Map_CopyCellAttributes(14, 48, 4, 1, 14, 44);
}
