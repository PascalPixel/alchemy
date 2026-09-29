#include "HAIDIA_BABI.H"

/* The entry hook: how the house is set up for the entrance and the story. */

void Map_ClearLayerEntryFlag();
void FieldScene_RunPaletteRampSequence();
void FieldScene_RunComplexActorSequence();
void BattleFx_StartTwelveFrameBlend();
void BattleFx_SetBlock30Values12Zero();
void BattleFx_SetBlock30Values128One();

s32 HaidiaBabi_RestoreEntryState(void)
{
    u32 i;
    s32 record;
    s32 base5_3001ebc;

    if (gGameState.entrance == 19) {
        Call1(Engine_GameFlagClear, 0x12f);
        *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x209;
    } else {
        if (Value1(Engine_GameFlagIsSet, 0x834) != 0) {
            Engine_ActorSetPosition(11, 0, 0);
            Engine_ActorSetPosition(12, 0, 0);
            Engine_ActorSetPosition(13, 0, 0);
            Engine_ActorSetPosition(14, 0, 0);
            Engine_ActorSetPosition(15, 0, 0);
            Engine_ActorSetPosition(16, 0, 0);
        } else {
            ActorPresentation_SetTwoSceneCells();
        }
        Engine_ActorSetSpritePriority(13, 1);
        if (Value1(Engine_GameFlagIsSet, 0x87a) != 0) {
            record = (s32)Engine_ActorGet(17);
            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
            if (gGameState.entrance != 6 && gGameState.entrance != 7) {
                goto L_02000550;
            }
            if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
                record = Value1(Engine_GameFlagIsSet, 0x203);
                if (record == 0) {
                    goto L_02000550;
                }
                Map_ClearLayerEntryFlag(12);
                goto L_02000550;
            }
            Map_ClearLayerEntryFlag(11);
            record = (s32)Engine_ActorGet(8);
            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
            Engine_ActorSetAnimation(8, 10);
        } else {
            if (gGameState.entrance == 21) {
                FieldScene_RunPaletteRampSequence();
            } else {
                if (gGameState.entrance == 20) {
                    Call1(Engine_GameFlagSet, 0x834);
                    FieldScene_RunComplexActorSequence();
                } else {
                    if (gGameState.entrance == 22) {
                        FieldScene_RunSupplementalSequenceOne();
                    } else {
                        base5_3001ebc = (u32)&gEventWork;
                        *(s32 *)((*(s32 *)base5_3001ebc + 0x1c0)) = 0x209;
                        if (Value1(Engine_GameFlagIsSet, 0x834) != 0) {
                            BattleFx_StartTwelveFrameBlend();
                            {
                                u16 *target = (u16 *)((*(s32 *)(base5_3001ebc + 12) + 0x1f84));
                                s32 shown = 1;

                                *target = shown;
                            }
                            BattleFx_SetBlock30Values12Zero();
                            Engine_TaskWait(30);
                            Engine_EventOpenScreen();
                            Engine_EventWaitForScreen();
                            BattleFx_SetBlock30Values128One();
                        } else {
                            Engine_MapRedraw();
                            Engine_TaskWait(1);
                        }
                    }
                }
            }
        }
    }
    L_02000550:;
    return 0;
}
