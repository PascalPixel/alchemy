#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKuupuappuUhnnUhnn[];

/* FAKEMATCH: a call spelled through this wrapper sets r0 last of its
 * arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* The leader walks to the door; the first time, actor 8 groans before the
 * room is left through exit 14. */
void FieldScene_RunOpeningSequenceSecond(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 744, 408);
    if (GameFlag_IsSet(0x854) == 0) {
        Event_Begin();
        Value1((s32 (*)())Engine_EventSetMessage, (s32)MsgKuupuappuUhnnUhnn);
        Event_ShowMessage(8, 0);
        Event_End();
    }
    *(u32 *)((u8 *)gEventWork + 456) = 16;
    Value1((s32 (*)())Engine_AudioPlayCue, 123);
    Event_RequestExit(14);
}
