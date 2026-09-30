/* Scene tables, the primary sequence and the facing actors. */
#include "TOREBI.H"
#include "CALL.H"
extern u8 MsgTorebiHeWontSailShipEven[];
extern u8 MsgTorebiHeyWhatsThis[];

u8 *SceneData_GetTable8BB4(void)
{
    return Data_02008bb4;
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *SceneData_GetTable8dac(void)
{
    return Data_02008dac;
}

s32 SceneData_SelectTable8e00ByFlag(void)
{
    if (GameFlag_IsSet(0x950) != 0) {
        return (s32)Data_02009040;
    }
    return (s32)Data_02008e00;
}

u8 *SceneData_SelectTable9310ByFlags(void)
{
    if (GameFlag_IsSet(0x950) != 0) {
        return Data_020099d0;
    }
    if (GameFlag_IsSet(0x962) != 0) {
        return Data_02009670;
    }
    return Data_02009310;
}

void SceneState_SetWork1c0AndRun(void)
{
    extern u8 *Data_03001ebc;

    u8 *state = Data_03001ebc;

    *(s32 *)(state + 0x1C0) = 0x201;
    *(s32 *)(state + 0x1C8) = 24;
    ((void (*)(void))Engine_EventRequestExit)();
}

void FieldScene_RunPrimarySequence(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    u8 *p6;
    s32 n;

    p6 = *(u8 **)Data_03001ebc;
    for (i = 8; i < 66; i++) {
        record = Engine_ActorGet(i);
        if (record != 0) {
            *(u8 *)(record + 85) = 0;
        }
    }
    p6 = p6 + 0x16c;
    n = *(s16 *)p6 - 14;
    Audio_PlayCue(158);
    Value3(Engine_MapAnimateCells, Data_02009dcc[n].a, Data_02009dcc[n].b, Data_02009dcc[n].c);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    *(u8 *)((s32)Engine_ActorGet(0) + 85) = 0;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Event_RequestExit(*(s16 *)p6);
}

void FieldScene_RunScene3b6SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgTorebiHeyWhatsThis);
    Event_Wait(40);
    rec7 = Engine_ObjectCreate(0x11c, 0x2580000, 0, 0x3380000);
    Actor_SetSpriteFlags(rec7, 0);
    Object_SetAnimation(rec7, 6);
    Event_Wait(10);
    Object_SetAnimation(rec7, 1);
    Event_Wait(40);
    Engine_ObjectDispatchRelease(rec7);
    Event_Wait(2);
    Actor_ShowEmote(25, 0x100, 50);
    Actor_SetSpeed(25, 0x10000, 0x8000);
    Actor_WalkToAndWait(25, 0x258, 0x350);
    Actor_FaceDirection(25, 0xc000, 0);
    Event_Wait(40);
    Event_ShowMessage(25, 0);
    Actor_RunRepeatedMotion(25, 2);
    Event_Wait(30);
    Actor_WalkToAndWait(25, 0x238, 0x350);
    Actor_FaceDirection(25, 0xc000, 0);
    Event_Wait(30);
    Actor_ShowEmote(25, 0x108, 50);
    Event_Wait(20);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    Event_Wait(20);
    Actor_FaceDirection(25, 0x3000, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(25, 2);
    Event_Wait(20);
    Event_ShowMessage(25, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 50);
    Event_Wait(20);
    Actor_SetAnimationAndWait(25, 4);
    Event_Wait(20);
    Event_ShowMessage(25, 0);
    Event_Wait(30);
    Actor_ShowEmote(25, 0x102, 50);
    Event_ShowMessage(25, 0);
    Actor_SetSpeed(25, 0x16666, 0xb333);
    Actor_WalkByAndWait(25, 16, 0);
    Actor_WalkByAndWait(25, 0, 32);
    Event_Wait(20);
    Actor_SetAnimationAndWait(25, 3);
    Event_Wait(20);
    Event_ShowMessage(25, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Event_Wait(20);
    Actor_SetSpeed(25, 0x1cccc, 0xe666);
    Actor_WalkByAndWait(25, 0, 48);
    Actor_SetPosition(25, 0, 0);
    Event_End();
}

void FieldScene_RunActorsThirtyOneToThirtyThreeChoreography(void)
{
    void Event_Wait(s32);
    void Event_SetMessage(s32);

    Event_Begin();
    Event_SetMessage((s32)MsgTorebiHeWontSailShipEven);
    Event_Wait(30);
    /* Same import, same first two arguments, differing only in the third.
     * Two call sites, not a loop. */
    Actor_Jump(31, 4, 13);
    Actor_Jump(31, 4, 30);
    Event_ShowMessage(31, 0);
    Event_Wait(10);
    /* r1 = 129 << 1 = 0x102. Argument registers are set r1, r2, r0. */
    Actor_ShowEmote(32, 0x102, 50);
    Event_Wait(10);
    Actor_SetAnimationAndWait(32, 3);
    Event_Wait(30);
    Event_ShowMessage(32, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(33, 4);
    Event_Wait(20);
    Event_ShowMessage(33, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(31, 2);
    Event_Wait(20);
    Event_ShowMessage(31, 0);
    Event_Wait(10);
    /* Repeats the (32, 3) call made above; a second site, deliberately not
     * folded with the first. */
    Actor_SetAnimationAndWait(32, 3);
    Event_Wait(30);
    Event_End();
}

/* Keep the first byte store and zero initialization as one assignment. */
s32 Scene_InitFacingActors(void)
{
    u8 *record;
    s32 none;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (Engine_GameFlagIsSet(0x950) != 0) {
        Call6(Engine_MapCopyCellAttributes, 51, 47, 3, 1, 51, 45);
        record = Engine_ActorGet(31);
        record[35] = none = 0;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        record = Engine_ActorGet(32);
        record[35] = none;
        (*(s8 **)(record + 80))[9] = ((-13 & (*(s8 **)(record + 80))[9]) | 8);
        if (Value1(Engine_GameFlagIsSet, 0x8bc) != 0) {
            Call3(Engine_ActorSetPosition, 25, 0x2300000, 0x2a80000);
            Call3(Engine_ActorFaceDirection, 25, 0x8000, 0);
        }
        if (gGameState.entrance == 19) {
            if (Value1(Engine_GameFlagIsSet, 0x8bc) == 0) {
                Engine_GameFlagSet(0x8bc);
                Event_OpenScreen();
                FieldScene_RunScene3b6SequenceA();
            }
        }
        if (gGameState.entrance == 16) {
            if (Value1(Engine_GameFlagIsSet, 0x300) == 0) {
                Engine_GameFlagSet(0x300);
                Event_OpenScreen();
                FieldScene_RunActorsThirtyOneToThirtyThreeChoreography();
            }
        }
        if (Engine_GameFlagIsSet(0x8ab) != 0) {
            Actor_SetPosition(35, 0, 0);
            Actor_SetPosition(36, 0, 0);
        }
    }
    return 0;
}
