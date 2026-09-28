#include "KORIMAKI.H"

s32 gKorimaKiSparkOrigin[3] __attribute__((section(".bss")));
s32 gKorimaKiSparkCount __attribute__((section(".bss")));
s32 gKorimaKiSparkSound __attribute__((section(".bss")));
s32 gKorimaKiTransitionStep __attribute__((section(".bss")));

void FieldScene_RunScene395_02000158(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x845) != 0) {
        KorimaKi_PlayGesture(10, 1);
        Event_SetMessage(MSG_NOW_HAVE_SUCH_POWER_AXE);
        Event_ShowMessage(8, 0);
        KorimaKi_PlayGesture(10, 0);
    } else {
        if (GameFlag_IsSet(0x844) != 0) {
            KorimaKi_PlayGesture(10, 1);
            Event_SetMessage(MSG_SILENCE_2);
            Event_ShowMessage(8, 0);
            Value2(KorimaKi_PlayGesture, 10, 0);
            record = PartyInventory_FindOwner(184);
            if (record == -1) {
                goto L_02000220;
            }
            {
                u16 *target = (u16 *)((u8 *)gEventWork + 0x172);
                s32 shown = 1;

                *target = shown;
            }
        } else {
            Event_SetMessage(MSG_SILENCE);
            Event_ShowMessage(8, 0);
            ColorBuffer_ApplyTarget(0x406218, 1);
            ColorBuffer_Interpolate(20);
            Task_Wait(40);
            Event_ShowMessageAndWait(0x200e, 0, 10);
            Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Event_ShowMessage(0x200e, 0);
            ColorBuffer_ApplyTarget(0x10000, 1);
            ColorBuffer_Interpolate(20);
            Task_Wait(40);
        }
    }
    L_02000220:;
    Event_End();
}

void PaletteScene_RunActorNineBranch(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x845) != 0) {
        Event_SetMessage(MSG_MUST_HORRIBLE_BEYOND_RIVER_AM);
    } else {
        Event_SetMessage(MSG_HEALING_WATERS_MERCURY_LIGHTHOUSE_MIGHT);
    }
    Event_ShowMessage(9, 0);
    Event_End();
}

void PaletteScene_RunActorEightBranch(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x845) != 0) {
        Event_SetMessage(MSG_KNOW_CANNOT_STOP_BUT_PLEASE);
    } else {
        Event_SetMessage(MSG_PEOPLE_KOLIMA_FORGIVE_ME);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void PaletteScene_RunFlaggedBranch(void)
{
    Event_Begin();
    Battle_ResetEffectCounter();
    if (GameFlag_IsSet(0x844) == 0) {
        RunEventScript01();
    } else {
        PaletteScene_RunActorTransitionSequence();
    }
    Event_End();
}

void RunEventScript01(void)
{

    u32 i;
    s32 rec8;

    rec8 = Actor_Get(ACTOR_PARTY_LEADER);
    Value3(Engine_ActorFaceDirection, 0, 0xc000, 0);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    Audio_PlayCue(17);
    gKorimaKiSparkSound = 1;
    Call2(Engine_TaskAddCallback, (s32)PaletteScene_SpawnEffect, 0xc80);
    Task_Wait(30);
    gKorimaKiSparkSound = 0;
    Camera_MoveTo(0x1480000, -1, 0xeb0000, 1);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
    *((u8 *)Engine_ActorGet(0) + 90) &= 254;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
    Audio_PlayCue(133);
    *(s32 *)(rec8 + 40) = 0x50000;
    *(s32 *)(rec8 + 72) = 0x4000;
    *(s32 *)(rec8 + 68) = 0xa000;
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x14f, 0x102);
    while (*(s32 *)(rec8 + 40) >= 0) {
        Task_Wait(1);
    }
    do {
        Task_Wait(1);
    } while (*(s32 *)(rec8 + 40) <= 0);
    Audio_PlayCue(161);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 19);
    Event_Wait(120);
    Call1(Engine_TaskRemoveCallback, (s32)PaletteScene_SpawnEffect);
    Task_Wait(40);
    *(s32 *)(rec8 + 68) = 0x4000;
    {
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        u8 flags = record[90] | 1;

        record[90] = flags;
    }
    Event_Wait(80);
    Event_SetMessage(MSG_CONTROL_TRETS_HEART_SHALL_NOT);
    Event_ShowMessageAndWait(0x200e, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    Event_ShowMessage(0x200e, 0);
    Audio_PlayCueFromEventWork();
    ColorBuffer_ApplyTarget(0x10000, 1);
    ColorBuffer_Interpolate(20);
    Task_Wait(40);
    {
        s32 shown = 0xc000;

        *(u16 *)(rec8 + 6) = shown;
    }
    *(s32 *)(rec8 + 72) = 0x10000;
    *(s32 *)(rec8 + 68) = 0x4000;
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(40);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Event_Wait(20);
}

/* The scene's actor transition sequence. Actor three takes part only when the
 * saved flag reports it enabled, and the arms that skip it bump the step
 * counter instead. */
void PaletteScene_RunActorTransitionSequence(void)
{
    s32 actorThreeEnabled;
    u8 *object;
    s32 *transitionState;
    s32 cycle;
    s32 sceneWorkSlot;
    s32 effectCallback;
    const s32 *finalActions;

    actorThreeEnabled = GameFlag_IsSet(3);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x148, 212);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
    Audio_PlayCue(17);
    Message_ShowCentered(MSG_WATER_HERMES_SEEPED_INTO_TRET, 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Actor_EnableActionCallback(ACTOR_GERALD, SceneAction_ActorOneEntry);
    Actor_EnableActionCallback(ACTOR_IVAN, SceneAction_ActorTwoEntry);
    if (actorThreeEnabled != 0) {
        Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
        object = Actor_Get(ACTOR_PARTY_LEADER);
        if (object != 0) {
            Actor_SetPosition(ACTOR_MIA, *(s32 *)(object + 8), *(s32 *)(object + 16));
        }
        Actor_EnableActionCallback(ACTOR_MIA, SceneAction_ActorThreeEntry);
    }
    Object_RefreshSelectorById(2);
    Event_Wait(40);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(32);
    Task_Wait(40);
    transitionState = &gKorimaKiTransitionStep;
    *transitionState = 0;
    Value2(Engine_TaskAddCallback, (s32)PaletteScene_AdvanceTransition, 0xc80);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0x1000000, -1, 0xfe0000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(246);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 20);
    Camera_MoveTo(0x19d0000, -1, 0x1050000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(246);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 20);
    Camera_MoveTo(0x1460000, -1, 0x1800000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(246);
    if (*transitionState != 24) {
        do {
            Task_Wait(1);
        } while (*transitionState != 24);
    }
    Value1(Engine_TaskRemoveCallback, (s32)PaletteScene_AdvanceTransition);
    Task_Wait(10);
    cycle = 0;
    do {
        KorimaPalette_Restore(0);
        ColorBuffer_Interpolate(6);
        Task_Wait(6);
        KorimaPalette_Restore(1);
        ColorBuffer_Interpolate(6);
        cycle = (cycle + 1);
        Task_Wait(6);
    } while ((u32)cycle <= 3);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(40);
    Task_Wait(80);
    Camera_MoveTo(0x1480000, 0x80000, 0xd40000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    KorimaKi_PlayGesture(10, 1);
    Event_Wait(40);
    Audio_PlayCue(7);
    Event_SetMessage(MSG_FEEL_GREAT_POWER_SPREADING_THROUGH);
    Event_ShowMessage(8, 0);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 20);
    KorimaKi_PlayGesture(10, 2);
    Event_Wait(20);
    KorimaKi_PlayGesture(10, 3);
    Event_Wait(40);
    KorimaKi_PlayGesture(10, 1);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 40);
    Camera_MoveTo(0xea0000, 0, 0xe80000, 1);
    Camera_WaitForMove();
    Event_Wait(40);
    KorimaKi_PlayGesture(11, 1);
    Event_Wait(40);
    KorimaKi_PlayGesture(11, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    KorimaKi_PlayGesture(11, 2);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 20);
    KorimaKi_PlayGesture(11, 3);
    Event_Wait(20);
    KorimaKi_PlayGesture(11, 2);
    Event_Wait(20);
    KorimaKi_PlayGesture(11, 3);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    KorimaKi_PlayGesture(10, 0);
    Event_Wait(20);
    Event_ShowMessage(0x8008, 0);
    KorimaKi_PlayGesture(10, 1);
    Event_Wait(20);
    Event_OpenMessage(0x8008, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_ShowMessage(0x4009, 0);
        Event_ShowMessage(0x8008, 0);
    } else {
        sceneWorkSlot = (u32)&gEventWork;
        *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 2;
        Actor_ShowEmote(ACTOR_MIA, 0x103, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 0);
        Actor_ShowEmote(ACTOR_IVAN, 0x103, 40);
        Actor_SetAnimation(ACTOR_GERALD, 4);
        Event_ShowMessage(ACTOR_GERALD, 0);
        if (actorThreeEnabled != 0) {
            Actor_RunRepeatedMotion(ACTOR_MIA, 2);
            Event_ShowMessage(ACTOR_MIA, 0);
        } else {
            *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 1;
        }
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_ShowMessage(ACTOR_IVAN, 0);
        Event_ShowMessage(0x4009, 0);
        Event_ShowMessage(0x8008, 0);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Camera_MoveTo(0x1480000, 0x80000, 0xd40000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    KorimaKi_PlayGesture(10, 0);
    Event_Wait(20);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(40);
    Event_Wait(60);
    gKorimaKiSparkCount = 0;
    gKorimaKiSparkOrigin[0] = 0x1480000;
    gKorimaKiSparkOrigin[1] = 0x300000;
    effectCallback = (s32)KorimaKi_SpawnOrbitSparks;
    gKorimaKiSparkOrigin[2] = 0xcd0000;
    Value2(Engine_TaskAddCallback, effectCallback, 0xc80);
    Event_Wait(100);
    Engine_TaskRemoveCallback(effectCallback);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(60);
    Event_Wait(100);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(20);
    Event_Wait(40);
    KorimaKi_PlayGesture(10, 1);
    Event_Wait(10);
    Event_SetMessage(MSG_SHOULD_DO_PEOPLE_KOLIMA_CURSED);
    Event_ShowMessage(0x8008, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Camera_MoveTo(0xea0000, 0, 0xe80000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Event_ShowMessage(0x4009, 0);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    } else {
        Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
    }
    Event_ShowMessage(ACTOR_GERALD, 0);
    KorimaKi_PlayGesture(10, 4);
    Event_Wait(20);
    Event_SetMessage(MSG_WAS_INDEED_ANGRY_PEOPLE_HAD);
    Event_ShowMessage(0x8008, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Event_ShowMessage(0x8008, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    KorimaKi_PlayGesture(10, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8008, 0, 20);
    KorimaKi_PlayGesture(11, 0);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    KorimaKi_PlayGesture(11, 3);
    Event_Wait(40);
    KorimaKi_PlayGesture(11, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    KorimaKi_PlayGesture(10, 2);
    Event_Wait(20);
    Event_ShowMessage(0x8008, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 80);
    KorimaKi_PlayGesture(11, 5);
    Event_Wait(60);
    KorimaKi_PlayGesture(11, 3);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    KorimaKi_PlayGesture(10, 5);
    Event_Wait(40);
    KorimaKi_PlayGesture(10, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 10);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
    Event_ShowMessage(0x8002, 0);
    KorimaKi_PlayGesture(11, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    KorimaKi_PlayGesture(10, 1);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    KorimaKi_PlayGesture(10, 2);
    Event_Wait(20);
    KorimaKi_PlayGesture(11, 3);
    Event_Wait(40);
    KorimaKi_PlayGesture(11, 0);
    Event_Wait(20);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(1);
    Task_Wait(1);
    ColorBuffer_ApplyTarget(0x406218, 1);
    ColorBuffer_Interpolate(40);
    Event_Wait(60);
    gKorimaKiSparkCount = 0;
    gKorimaKiSparkOrigin[0] = 0x880000;
    gKorimaKiSparkOrigin[1] = 0x140000;
    effectCallback = (s32)KorimaKi_SpawnOrbitSparks;
    gKorimaKiSparkOrigin[2] = 0x1020000;
    Value2(Engine_TaskAddCallback, effectCallback, 0xc80);
    Event_Wait(100);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 40);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 20);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    KorimaKi_PlayGesture(10, 4);
    Event_Wait(20);
    Event_ShowMessage(0x8008, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 20);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 120);
    Engine_TaskRemoveCallback(effectCallback);
    Event_Wait(60);
    KorimaPalette_Restore(0);
    ColorBuffer_Interpolate(40);
    KorimaKi_PlayGesture(10, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8008, 0, 20);
    KorimaKi_PlayGesture(11, 3);
    Event_ShowMessage(0x4009, 0);
    Event_ShowMessage(0x8008, 0);
    KorimaKi_PlayGesture(11, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    KorimaKi_PlayGesture(10, 1);
    Event_OpenMessage(0x8008, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
    }
    Event_Wait(10);
    KorimaKi_PlayGesture(10, 2);
    Event_Wait(20);
    KorimaKi_PlayGesture(11, 3);
    Event_Wait(40);
    KorimaKi_PlayGesture(10, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Audio_PlayCue(17);
    finalActions = SceneAction_GroupFinish;
    Actor_EnableActionCallback(ACTOR_GERALD, finalActions);
    if (actorThreeEnabled != 0) {
        Actor_EnableActionCallback(ACTOR_MIA, finalActions);
    }
    Call2(Object_SetActionCallbackAndRefreshById, 2, (s32)finalActions);
    KorimaKi_PlayGesture(10, 4);
    KorimaKi_PlayGesture(10, 4);
    Event_Wait(20);
    Event_SetMessage(MSG_OWE_GREAT_DEBT_HAVE_SAVED);
    Event_ShowMessage(0x8008, 0);
    KorimaKi_PlayGesture(11, 4);
    KorimaKi_PlayGesture(11, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    GameFlag_Set(0x845);
    Audio_PlayCue(1);
    PaletteScene_SetRecordValue(184, 185);
}
