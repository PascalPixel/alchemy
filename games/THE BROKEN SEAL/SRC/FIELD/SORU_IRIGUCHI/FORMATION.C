#include "SORU.H"
extern u8 MsgFieldDoorTightlyLocked[];
extern u8 MsgSoruMoreStatuesOutOfReach[];

void Scene_RunTransitionCue(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x204;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetPosition(8, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(8, 0x10000, 0x8000);
    Actor_SetAnimation(8, 2);
    Actor_SetDestinationOffset(8, 24, -10);
    Actor_WaitForMove(8);
    Actor_SetAnimation(8, 1);
    Event_Wait(6);
    Actor_FaceDirection(8, 0xb000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 40);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x6880000, -1, 0x20c0000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x7580000, -1, 0x20c0000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0x6e90000, -1, 0x2240000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_RunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0, 30);
    Event_SetMessage((s32)MsgSoruMoreStatuesOutOfReach);
    Event_ShowMessageAndWait(0x4008, 0, 10);
    Actor_ShowEmote(8, 0x100, 40);
    Actor_RunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(0x4008, 0, 10);
    Actor_SetAnimation(8, 2);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetDestination(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(8);
    Actor_SetPosition(8, 0, 0);
    GameFlag_Set(0x825);
    Event_End();
}

void Scene_RunActorFormation(s32 a0)
{
    u32 i;
    s32 record;

    Value6(Engine_MapCopyCellAttributes, 122, 20, 1, 1, 100, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 104, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 108, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 112, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 116, 32);
    Map_CopyCellAttributes(122, 20, 1, 1, 120, 32);
    if (GameFlag_IsSet(0x311) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
        if (a0 == 0) {
            goto L_02001890;
        }
        Actor_SetPosition(9, 0x6380000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x310) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
            if (a0 != 0) {
                Actor_SetPosition(9, 0x6580000, 0x2080000);
            }
        }
    }
    L_02001890:;
    if (GameFlag_IsSet(0x313) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
        if (a0 == 0) {
            goto L_020018f2;
        }
        Actor_SetPosition(10, 0x6780000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x312) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
            if (a0 != 0) {
                Actor_SetPosition(10, 0x6980000, 0x2080000);
            }
        }
    }
    L_020018f2:;
    if (GameFlag_IsSet(0x315) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
        if (a0 == 0) {
            goto L_02001956;
        }
        Actor_SetPosition(11, 0x6b80000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x314) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
            if (a0 != 0) {
                Actor_SetPosition(11, 0x6d80000, 0x2080000);
            }
        }
    }
    L_02001956:;
    if (GameFlag_IsSet(0x317) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
        if (a0 == 0) {
            goto L_020019b8;
        }
        Actor_SetPosition(12, 0x6f80000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x316) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
            if (a0 != 0) {
                Actor_SetPosition(12, 0x7180000, 0x2080000);
            }
        }
    }
    L_020019b8:;
    if (GameFlag_IsSet(0x319) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
        if (a0 == 0) {
            goto L_02001a1c;
        }
        Actor_SetPosition(13, 0x7380000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x318) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
            if (a0 != 0) {
                Actor_SetPosition(13, 0x7580000, 0x2080000);
            }
        }
    }
    L_02001a1c:;
    if (GameFlag_IsSet(0x31b) != 0) {
        Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
        if (a0 == 0) {
            goto L_02001a7e;
        }
        Actor_SetPosition(14, 0x7780000, 0x2080000);
    } else {
        if (GameFlag_IsSet(0x31a) != 0) {
            Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
            if (a0 != 0) {
                Actor_SetPosition(14, 0x7980000, 0x2080000);
            }
        }
    }
    L_02001a7e:;
}

void FieldScene_RunScriptedStep953(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldDoorTightlyLocked, 1);
    Event_End();
}

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 record;
    s32 value;
    s32 *timer;
    s32 v3;

    timer = &SoruIriguchi_CueTimer;
    if (*timer != 0) {
        v3 = (*timer - 1);
        *timer = (*timer - 1);
        if (v3 != 40) {
            goto L_02001b14;
        }
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    } else {
        value = Value0(Engine_RandomNext);
        if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
            Audio_PlayCue(138);
            Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
            *timer = 80;
        }
    }
    L_02001b14:;
}
