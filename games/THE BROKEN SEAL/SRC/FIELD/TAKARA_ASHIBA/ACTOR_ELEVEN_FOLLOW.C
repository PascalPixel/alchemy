#include "TYPES.H"
#include "CALL.H"

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
        ObjectMotion_SetSpeedParameters(11, 0x1b333, 0xd999);
        AudioCommand_Play(188);
        v3 = *(s32 *)(Engine_ActorGet(0) + 8) / 0x100000;
        if (v3 > *(s32 *)(Engine_ActorGet(11) + 8) / 0x100000) {
            ObjectMotion_OffsetPositionAndResetMotion(11, 8, 0);
        }
        v3 = *(s32 *)(Engine_ActorGet(0) + 8) / 0x100000;
        if (v3 < *(s32 *)(Engine_ActorGet(11) + 8) / 0x100000) {
            Call3(ObjectMotion_OffsetPositionAndResetMotion, 11, -8, 0);
        }
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        record = Engine_ActorGet(0);
        if (record != 0) {
            ObjectMotion_ResetAndSetPosition(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        ObjectMotion_OffsetPositionAndResetMotion(0, 0, 24);
        Battle_WaitMode0(4);
        AudioCommand_Play(188);
        ObjectMotion_OffsetPositionAndResetMotion(11, 0, 16);
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        ObjectMotion_ResetAndSetPosition(11, 0x158, 0x168);
        ObjectMotion_CommitCurrentPositionAndActivate(11);
        Battle_WaitMode0(10);
        BattleFx_FinishAction();
    }
}
