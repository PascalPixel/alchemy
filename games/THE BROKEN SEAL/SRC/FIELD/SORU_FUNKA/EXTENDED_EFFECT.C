/* Scene tables, the hostage scene and small callbacks. */
#include "FUNKA.H"
extern u8 MsgSoruIKnowItsARock[];
extern u8 MsgSoruJasmineWhatHappened[];
extern u8 MsgSoruSomeoneIsLiftingIt[];
extern u8 MsgSoruSukuretaCouldThatBeThe[];

u8 *SceneData_GetScriptTable(void)
{
    return Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

u8 *SceneData_GetActorTable(void)
{
    return Placement_Actors;
}

u8 *SceneData_GetEffectTable(void)
{
    return Placement_Effects;
}

void Scene_SaturosTakesHostages(void)
{
    void Actor_ShowEmote();
    void ColorBuffer_ApplyTarget();
    void ColorBuffer_Interpolate();
    void Audio_PlayCue();

    u32 i;
    u8 *record;
    s32 base5_3001ec4;
    s32 base5_3001ebc;
    u8 *p7;

    base5_3001ec4 = (s32)gParticleWork;
    p7 = *(u8 **)base5_3001ec4;
    Event_Begin();
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x9c0000);
    Actor_SetPosition(ACTOR_GERALD, 0xda0000, 0xac0000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
    Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
    Actor_SetSpritePriority(15, 1);
    base5_3001ebc = base5_3001ec4 - 8;
    Camera_MoveTo(0xe80000, -1, 0x9c0000, 0);
    Map_Redraw();
    *(s32 *)((*(s32 *)base5_3001ebc + 0x1c8)) = 8;
    Event_OpenScreen();
    Event_WaitForScreen();
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    SoruFunka_ThrowEruptionRing();
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(16);
    Audio_PlayCue(144);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(4);
    Event_Wait(4);
    Audio_PlayCue(144);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(48);
    Event_Wait(48);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_Jump(ACTOR_GERALD, 6, 20);
    FieldScene_RunVariantStep(1, 20, 20);
    FieldScene_RunVariantStep(0, 20, 40);
    Event_SetMessage((s32)MsgSoruJasmineWhatHappened);
    Call3((void (*)())Engine_EventShowMessageAndWait, 11, 0, 20);
    Call2((void (*)())Engine_EventShowMessage, 10, 0);
    FieldScene_RunVariantStep(1, 20, 0);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
    FieldScene_RunVariantStep(0, 20, 0);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Event_Wait(40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 30);
    FieldScene_RunVariantStep(1, 20, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    FieldScene_RunVariantStep(0, 20, 20);
    record = Engine_ActorGet(15);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetPosition(15, 0x1450000, 0x12e0000);
    record = Engine_ActorGet(15);
    record[85] = 5;
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Event_Wait(150);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
    Event_ShowMessage(ACTOR_JASMINE, 0);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x3000, 10);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
    Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
    Camera_SetSpeed(0x66666, 0xcccc);
    Camera_MoveTo(0x1480000, -1, 0x12b0000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(167);
    ColorBuffer_ApplyTarget(0x205294, 2);
    ColorBuffer_Interpolate(20);
    Task_Wait(20);
    ColorBuffer_ApplyTarget(0x10000, 2);
    ColorBuffer_Interpolate(20);
    Event_Wait(200);
    Event_OpenMessage(0x1001, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage((s32)MsgSoruIKnowItsARock);
    } else {
        Event_SetMessage((s32)MsgSoruSomeoneIsLiftingIt);
    }
    Event_ShowMessageAndWait(0x1001, 0, 80);
    Event_SetMessage((s32)MsgSoruSukuretaCouldThatBeThe);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
    FieldScene_RunVariantStep(1, 20, 0);
    *(s32 *)(((s32)p7 + 0x40c)) = 0;
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(80);
    *(s32 *)(((s32)p7 + 0x40c)) = 1;
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    FieldScene_RunVariantStep(0, 20, 60);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
    Actor_FaceDirection(15, 0xa000, 40);
    FieldScene_RunVariantStep(1, 20, 20);
    FieldScene_RunVariantStep(0, 20, 20);
    Event_ShowMessageAndWait(0x1001, 0, 30);
    Actor_FaceDirection(15, 0x1000, 40);
    FieldScene_RunVariantStep(1, 20, 20);
    FieldScene_RunVariantStep(0, 20, 20);
    Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 30);
    SceneState_InitStateWordsAndSlots();
    InstallTask(Engine_TaskAddCallback, (void (*)())SoruFunka_StepEmbers, 0xc80);
    InstallTask(Engine_TaskAddCallback, (void (*)())SceneState_UpdateRandomTimerLevel, 0xc80);
    Event_Wait(240);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_GARCIA, 0x1d70000, 0x1320000);
    Actor_SetPosition(ACTOR_ALEX, 0x1df0000, 0x16a0000);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x8000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x8000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x8000, 0);
    Actor_FaceDirection(ACTOR_ALEX, 0x8000, 0);
    record = Engine_ActorGet(5);
    Camera_MoveTo((*(s16 *)((s32)record + 10) << 16), -1, (*(s16 *)((s32)record + 18) << 16), 1);
    Camera_WaitForMove();
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GARCIA, 0);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_SetAnimation(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 20);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    Actor_SetAnimation(ACTOR_SATUROS, 4);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 40);
    Event_ShowMessageAndWait(0x4005, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 40);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    Actor_ShowEmote(ACTOR_SATUROS, 0x105, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 10);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 10);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 40);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 60);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 20);
    Actor_FaceDirection(ACTOR_ALEX, 0xd000, 20);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
    Actor_ShowEmote(ACTOR_GARCIA, 0x103, 0);
    Actor_SetSpeed(ACTOR_GARCIA, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_GARCIA, 0x1d7, 0x13a);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 3);
    Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 10);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 10);
    Actor_SetAnimation(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 3);
    Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 20);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 30);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 30);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 4);
    Event_Wait(10);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Actor_FaceDirection(ACTOR_GARCIA, 0x5000, 20);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 2);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(ACTOR_GARCIA, 1);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_GARCIA, 0x8000, 0);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_GARCIA, 0x102, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_ShowMessageAndWait(0x200e, 0, 30);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 20);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 40);
    Event_ShowMessageAndWait(0x2005, 0, 40);
    Actor_ShowEmote(ACTOR_GARCIA, 0x105, 60);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Actor_ShowEmote(ACTOR_SATUROS, 0x101, 0);
    Actor_ShowEmote(ACTOR_MENARDI, 0x101, 60);
    Actor_RunRepeatedMotion(ACTOR_ALEX, 2);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 60);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_JASMINE, 0x102, 0);
    Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 40);
    Actor_FaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 20);
    Actor_SetAnimation(ACTOR_SATUROS, 3);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_SATUROS, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_MENARDI, 0x9999, 0x4ccc);
    Actor_WalkTo(ACTOR_MENARDI, 0x1db, 0x15c);
    Actor_WalkTo(ACTOR_SATUROS, 0x1eb, 0x15c);
    Actor_WaitForMove(ACTOR_MENARDI);
    Actor_WaitForMove(ACTOR_SATUROS);
    Actor_SetAnimation(ACTOR_MENARDI, 1);
    Actor_SetAnimation(ACTOR_SATUROS, 1);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 10);
    Actor_ShowEmote(ACTOR_MENARDI, 0x103, 0);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_ShowMessage(0x200b, 0);
    Actor_SetSpeed(ACTOR_MENARDI, 0x19999, 0xcccc);
    Actor_SetSpeed(ACTOR_JASMINE, 0x13333, 0x9999);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x152);
    record = Engine_ActorGet(11);
    ((struct FacingObject *)record)->facing_flags = (u8)(254 & ((struct FacingObject *)record)->facing_flags);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x15c);
    Actor_RunRepeatedMotion(ACTOR_JASMINE, 1);
    Actor_Jump(ACTOR_JASMINE, 4, 0);
    Actor_WalkToAndWait(ACTOR_JASMINE, 0x1cb, 0x13c);
    Actor_SetSpeed(ACTOR_JASMINE, 0x8000, 0x4000);
    Actor_ShowEmote(ACTOR_GARCIA, 0x103, 0);
    Actor_StartRepeatedMotion(ACTOR_GARCIA, 3);
    Event_ShowMessageAndWait(ACTOR_GARCIA, 0, 30);
    Actor_RunRepeatedMotion(ACTOR_MENARDI, 1);
    Event_Wait(20);
    ((struct FacingObject *)record)->facing_flags |= 1;
    Actor_SetSpeed(ACTOR_MENARDI, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1db, 0x14c);
    Actor_FaceDirection(ACTOR_MENARDI, 0xb000, 20);
    Event_ShowMessage(0x200b, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_StartRepeatedMotion(ACTOR_SATUROS, 2);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
    Actor_ShowEmote(ACTOR_MENARDI, 0x102, 20);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 20);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 4);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Actor_FaceDirection(ACTOR_MENARDI, 0xb000, 20);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    Actor_SetAnimationAndWait(ACTOR_GARCIA, 3);
    Event_Wait(20);
    Actor_WalkTo(ACTOR_JASMINE, 0x1b0, 0x13c);
    Actor_WalkToAndWait(ACTOR_GARCIA, 0x1a6, 0x137);
    Actor_SetAnimation(ACTOR_JASMINE, 1);
    Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 20);
    Actor_RunRepeatedMotion(ACTOR_SATUROS, 2);
    Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 20);
    Event_ShowMessageAndWait(0x100a, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 30);
    Actor_SetSpeed(ACTOR_SUKURETA, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 0x1eb, 0x128);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
    Actor_SetSpeed(ACTOR_SATUROS, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MENARDI, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_GARCIA, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_ALEX, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_SATUROS, 0x1d7, 0x134);
    Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 0);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x1c7, 0x134);
    Actor_FaceDirection(ACTOR_MENARDI, 0x3000, 0);
    Actor_WalkToAndWait(ACTOR_ALEX, 0x1e7, 0x134);
    Actor_FaceDirection(ACTOR_SATUROS, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 0);
    Actor_FaceDirection(ACTOR_ALEX, 0xb000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xd000, 0);
    Actor_FaceDirection(ACTOR_GARCIA, 0xd000, 0);
    Camera_SetSpeed(0x8000, 0x1000);
    Camera_MoveTo(0x1d80000, -1, 0x12c0000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    GameFlag_Set(0x246);
    Actor_SetSpeed(ACTOR_SATUROS, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_MENARDI, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_ALEX, 0x8000, 0x4000);
    Actor_SetAnimationAndWait(ACTOR_SATUROS, 3);
    FieldScene_RunScene381_02000e30(10);
    Actor_RunRepeatedMotion(ACTOR_SUKURETA, 2);
    FieldScene_RunScene381_02000e30(9);
    Actor_SetAnimationAndWait(ACTOR_MENARDI, 3);
    FieldScene_RunScene381_02000e30(11);
    Actor_FaceDirection(ACTOR_JASMINE, 0x9000, 40);
    Actor_StartRepeatedMotion(ACTOR_JASMINE, 2);
    Event_ShowMessageAndWait(0x2005, 0, 40);
    Actor_FaceDirection(ACTOR_GARCIA, 0x3000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 30);
    Actor_FaceDirection(ACTOR_GARCIA, 0xe000, 0);
    FieldScene_RunScene381_02000e30(5);
    FieldScene_RunScene381_02000e30(13);
    Actor_FaceDirection(ACTOR_ALEX, 0x7000, 40);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
    Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0, 30);
    FieldScene_RunScene381_02000e30(14);
    Scene_RunExtendedEffectPresentation();
    Party_RemoveOwnerRestored(5);
}

void FieldScene_RunScene381_02000e30(s32 a0)
{
    u8 *rec7;
    s32 recA;
    s32 rec2;
    u8 i;

    recA = Engine_ActorGet(8);
    *(s32 *)(recA + 24) = 0x10000;
    *(s32 *)(recA + 28) = 0x10000;
    Value3(Engine_ActorWalkToAndWait, a0, 0x1d7, 0x122);
    Actor_FaceDirection(a0, 0xc000, 0);
    Event_Wait(10);
    Actor_SetPosition(8, 0x1d70000, 0x1220000);
    rec7 = Value1(Engine_ActorGet, a0);
    rec2 = Engine_ActorGet(a0);
    Actor_SetSpriteFlags(rec2, 0);
    Actor_SetChildValue(a0, 0x100);
    rec7[85] = 0;
    Audio_PlayCue(201);
    i = 0;
    do {
        *(s32 *)(rec7 + 12) += 0x8000;
        Event_Wait(1);
        i++;
    } while (i != 60);
    Audio_PlayCue(190);
    i = 0;
    do {
        *(s32 *)(rec7 + 12) += 0x1999;
        *(s32 *)(rec7 + 24) -= 0x28f;
        *(s32 *)(rec7 + 28) -= 0x28f;
        *(s32 *)(recA + 24) -= 0x28f;
        *(s32 *)(recA + 28) -= 0x28f;
        Event_Wait(1);
        i++;
    } while (i != 90);
    Actor_SetPosition(a0, 0, 0);
    Actor_SetPosition(8, 0, 0);
}

void Resource381_NoOpCallbackA(void)
{
}

void Resource381_NoOpCallbackB(void)
{
}

s32 FieldScene_RunWhenWord225Is10(void)
{
    if (gGameState.entrance == 10) {
        FieldEffect_InitSparkles();
        Scene_SaturosTakesHostages();
    }
    return 0;
}

s32 OverlayObject_SetRecordAngleFromHeading(Ent *p)
{
    *(u16 *)(p->unk50 + 30) = p->unk6 + 0x4000;
    return 1;
}
