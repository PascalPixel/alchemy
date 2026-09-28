#include "TYPES.H"

#define TakaraAshiba_RunActorElevenFollowScene Func_02001694

extern u8 Data_02000240[];
void Battle_Reset();
void ObjectMotion_SetSpeedParameters();
s32 Engine_ActorGet();
void ObjectMotion_OffsetPositionAndResetMotion();
void Battle_WaitMode0();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_CommitCurrentPositionAndActivate();
void BattleFx_FinishAction();
void AudioCommand_Play();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Unless the scene is 11, bring actor 11 level with the leader (0.85
 * speed), play cue 188 twice around a facing beat and walk it to (0x158,
 * 0x168). */
void TakaraAshiba_RunActorElevenFollowScene(void)
{
    s32 record;
    s32 base3_2000240;
    s32 v3;

    base3_2000240 = (s32)Data_02000240;
    if (*(s16 *)((base3_2000240 + 0x24a)) != 11) {
        Battle_Reset();
        Call3(ObjectMotion_SetSpeedParameters, 0, 0x1b333, 0xd999);
        Call3(ObjectMotion_SetSpeedParameters, 11, 0x1b333, 0xd999);
        AudioCommand_Play(188);
        v3 = *(s32 *)(Value1(Engine_ActorGet, 0) + 8) / 0x100000;
        if (v3 > *(s32 *)(Value1(Engine_ActorGet, 11) + 8) / 0x100000) {
            ObjectMotion_OffsetPositionAndResetMotion(11, 8, 0);
        }
        v3 = *(s32 *)(Value1(Engine_ActorGet, 0) + 8) / 0x100000;
        if (v3 < *(s32 *)(Value1(Engine_ActorGet, 11) + 8) / 0x100000) {
            Call3(ObjectMotion_OffsetPositionAndResetMotion, 11, -8, 0);
        }
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            ObjectMotion_ResetAndSetPosition(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        ObjectMotion_OffsetPositionAndResetMotion(0, 0, 24);
        Battle_WaitMode0(4);
        AudioCommand_Play(188);
        ObjectMotion_OffsetPositionAndResetMotion(11, 0, 16);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        Call3(ObjectMotion_ResetAndSetPosition, 11, 0x158, 0x168);
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        Battle_WaitMode0(10);
        BattleFx_FinishAction();
    }
}
