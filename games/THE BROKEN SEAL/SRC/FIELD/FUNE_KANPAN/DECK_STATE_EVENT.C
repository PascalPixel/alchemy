#include "TYPES.H"
extern struct EventWork *gEventWork;


void FieldScene_RunScene3af_02000bb8();
void FieldScene_RunScene3af_02000bf0();
void Battle_Reset();
void Battle_WaitMode0();
void ObjectMotion_SetSpeedParameters();
void ObjectMotion_SnapHeadingAndOffset();
void Event_SetValue170();
void AudioCommand_Play();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FuneKanpan_RunDeckStateEvent(void)
{

    s32 v5;
    u8 *p6;

    p6 = *(s32 *)&gEventWork;
    Battle_Reset();
    v5 = 0;
    switch (*(s16 *)(((s32)p6 + 0x16c))) {
    case 1:
        v5 = 1;
        FieldScene_RunScene3af_02000bb8();
        break;
    case 3:
        v5 = 1;
        FieldScene_RunScene3af_02000bf0();
        break;
    }
    if (v5 != 0) {
        Call3(ObjectMotion_SetSpeedParameters, 0, 0x9999, 0x4ccc);
        Call3(ObjectMotion_SnapHeadingAndOffset, 0, 1, -10);
        Battle_WaitMode0(10);
    } else {
        AudioCommand_Play(123);
    }
    Event_SetValue170(*(s16 *)(((s32)p6 + 0x16c)));
}
