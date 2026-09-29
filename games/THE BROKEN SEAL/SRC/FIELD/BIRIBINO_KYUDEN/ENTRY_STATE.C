#include "KYUDEN.H"

/* McCoy's Palace entry: record the arrival, clear two layer flags and, by
 * the story flags and the entrance, run the matching scene or restore the
 * room (the four guards at row 0x2d8, the opened doorway cells). */
s32 BiribinoKyuden_ApplyEntryState(s32 a0, s32 a1)
{
    u32 i;
    s32 rec8;
    s32 record;

    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x209;
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Call1((void (*)())Engine_GameFlagSet, 0x84b);
    if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
        Call1(Engine_GameFlagClear, 0x200);
    }
    if (Value1(Engine_GameFlagIsSet, 0x84f) == 0) {
        record = Value1(Engine_GameFlagIsSet, 0x845);
        if (record != 0) {
            goto L_02000758;
        }
        if (gGameState.entrance == 29) {
            FieldScene_RunLongBranchingChoreography();
            goto L_02000888;
        }
        if (gGameState.entrance != 9) {
            goto L_02000888;
        }
        if (Value1(Engine_GameFlagIsSet, 0x321) == 0) {
            goto L_02000888;
        }
        FieldScene_RunPalaceGreeting();
    } else {
        L_02000758:;
        rec8 = Value1(Engine_GameFlagIsSet, 0x84e);
        if (rec8 != 0) {
        } else {
            if (gGameState.entrance == 29) {
                if (Value1(Engine_GameFlagIsSet, 0x85e) != 0) {
                    goto L_02000888;
                }
                record = Value1(Engine_GameFlagIsSet, 0x845);
                if (record == 0) {
                    goto L_02000888;
                }
                RunEventScript02();
            } else {
                if (gGameState.entrance == 28) {
                    if (Value1(Engine_GameFlagIsSet, 0x322) != 0) {
                        if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
                            Call6(Engine_MapCopyCellAttributes, 38, 55, 4, 1, 38, 45);
                            Call6(Engine_MapCopyCellAttributes, 42, 55, 4, 1, 38, 46);
                            Call3(Engine_ActorSetPosition, 21, 0x2680000, 0x2d80000);
                            Call3(Engine_ActorSetPosition, 22, 0x2780000, 0x2d80000);
                            Call3(Engine_ActorSetPosition, 23, 0x2880000, 0x2d80000);
                            Call3(Engine_ActorSetPosition, 24, 0x2980000, 0x2d80000);
                            record = (s32)Engine_ActorGet(21);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            record = (s32)Engine_ActorGet(22);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            record = (s32)Engine_ActorGet(23);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            record = (s32)Engine_ActorGet(24);
                            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
                            *((u8 *)Engine_ActorGet(21) + 85) = rec8;
                            *((u8 *)Engine_ActorGet(22) + 85) = rec8;
                            *((u8 *)Engine_ActorGet(23) + 85) = rec8;
                            *((u8 *)Engine_ActorGet(24) + 85) = rec8;
                            record = (s32)Engine_ActorGet(21);
                            *(s32 *)(record + 12) = -0x40000;
                            record = Value1((s32 (*)())Engine_ActorGet, 22);
                            *(s32 *)(record + 12) = -0x40000;
                            record = Value1((s32 (*)())Engine_ActorGet, 23);
                            *(s32 *)(record + 12) = -0x40000;
                            record = Value1((s32 (*)())Engine_ActorGet, 24);
                            *(s32 *)(record + 12) = -0x40000;
                        } else {
                            BiribinoKyuden_RunActorRowScene();
                        }
                    }
                }
            }
        }
    }
    L_02000888:;
    return 0;
}
