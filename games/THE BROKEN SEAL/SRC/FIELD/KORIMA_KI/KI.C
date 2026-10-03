#include "RESOURCE.H"
#include "KORIMAKI.H"
#include "TYPES.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"

extern u8 MsgKorimaWho[];
extern u8 MsgKorimaForestKolimaAlive[];
extern u8 MsgKorimaLeaveBeforeForest[];
void Engine_EventBegin();
void KorimaKi_PlayGesture(s32 actor, s32 gesture);
s32 Engine_GameFlagIsSet();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventShowMessageAndWait();
void Engine_EventWait();
void Engine_ActorSetAnimationAndWait();
void Engine_EventEnd();

extern u8 MsgKorimaControlTretsHeartShallNot[];
extern u8 MsgKorimaFeelGreatPowerSpreadingThrough[];
extern u8 MsgKorimaHealingWatersMercuryLighthouseMight[];
extern u8 MsgKorimaKnowCannotStopButPlease[];
extern u8 MsgKorimaMustHorribleBeyondRiverAm[];
extern u8 MsgKorimaNowHaveSuchPowerAxe[];
extern u8 MsgKorimaOweGreatDebtHaveSaved[];
extern u8 MsgKorimaPeopleKolimaForgiveMe[];
extern u8 MsgKorimaShouldDoPeopleKolimaCursed[];
extern u8 MsgKorimaSilence[];
extern u8 MsgKorimaSilence2[];
extern u8 MsgKorimaWasIndeedAngryPeopleHad[];
extern u8 MsgKorimaWaterHermesSeepedIntoTret[];
s32 gKorimaKiSparkOrigin[3] __attribute__((section(".bss")));
s32 gKorimaKiSparkCount __attribute__((section(".bss")));
s32 gKorimaKiSparkSound __attribute__((section(".bss")));
s32 gKorimaKiTransitionStep __attribute__((section(".bss")));

void PaletteScene_AdjustPaletteWindow(s32 step);
s32 Object_ReplaceResourceEntry(struct FieldSprite *sprite, s32 previous);

struct Spark {
    u8 unknown_00[0x64];
    u16 phase;
    u16 angle;
};

/* The tree's scene tables, which the main image asks for through the
 * overlay's entry veneers. */
u8 *KorimaKi_GetEntrances(void)
{
    return gKorimaKiEntrances;
}

u8 *KorimaKi_GetRegions(void)
{
    return gKorimaKiRegions;
}

u8 *KorimaKi_GetExits(void)
{
    return gKorimaKiExits;
}

u8 *KorimaKi_GetPlacements(void)
{
    return gKorimaKiPlacements;
}

void PaletteScene_Initialize(void)
{
    void *scene;

    scene = *(void **)&gEventWork;
    Engine_EventBegin();
    Engine_ActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 0);
    Engine_EventRequestExit(FIELD_AT_OFFSET(scene, s16 *, 0x16C));
    Engine_EventEnd();
}

u8 *KorimaKi_GetEvents(void)
{
    return gKorimaKiEvents;
}

void KorimaKi_RunMessageScene(void)
{
    Engine_EventBegin();
    KorimaKi_PlayGesture(11, 1);
    if (Engine_GameFlagIsSet(0x845) != 0) {
        Engine_EventSetMessage((s32)MsgKorimaForestKolimaAlive);
        Engine_EventShowMessage(9, 0);
    } else if (Value1(Engine_GameFlagIsSet, 0x84c) != 0) {
        Engine_EventSetMessage((s32)MsgKorimaLeaveBeforeForest);
        Engine_EventShowMessage(9, 0);
    } else {
        Engine_EventSetMessage((s32)MsgKorimaWho);
        Engine_EventShowMessageAndWait(9, 0, 20);
        KorimaKi_PlayGesture(11, 0);
        Engine_EventWait(60);
        KorimaKi_PlayGesture(11, 1);
        Engine_EventShowMessageAndWait(9, 0, 10);
        Engine_ActorSetAnimationAndWait(0, 3);
        Engine_EventWait(40);
        Engine_EventShowMessage(9, 0);
        KorimaKi_PlayGesture(11, 0);
        Engine_EventWait(80);
        Engine_EventShowMessageAndWait(9, 0, 20);
        KorimaKi_PlayGesture(11, 1);
        Engine_EventShowMessageAndWait(9, 0, 20);
        Engine_GameFlagSet(0x84c);
    }
    KorimaKi_PlayGesture(11, 0);
    Engine_EventEnd();
}

void FieldScene_RunScene395_02000158(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    if (GameFlag_IsSet(0x845) != 0) {
        KorimaKi_PlayGesture(10, 1);
        Engine_EventSetMessage((s32)MsgKorimaNowHaveSuchPowerAxe);
        Event_ShowMessage(8, 0);
        KorimaKi_PlayGesture(10, 0);
    } else {
        if (GameFlag_IsSet(0x844) != 0) {
            KorimaKi_PlayGesture(10, 1);
            Engine_EventSetMessage((s32)MsgKorimaSilence2);
            Event_ShowMessage(8, 0);
            KorimaKi_PlayGesture(10, 0);
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
            Engine_EventSetMessage((s32)MsgKorimaSilence);
            Event_ShowMessage(8, 0);
            ColorBuffer_ApplyTarget(0x406218, 1);
            Engine_ColorBufferInterpolate(20);
            Engine_TaskWait(40);
            Event_ShowMessageAndWait(0x200e, 0, 10);
            Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Event_ShowMessage(0x200e, 0);
            ColorBuffer_ApplyTarget(0x10000, 1);
            Engine_ColorBufferInterpolate(20);
            Engine_TaskWait(40);
        }
    }
    L_02000220:;
    Engine_EventEnd();
}

void PaletteScene_RunActorNineBranch(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x845) != 0) {
        Engine_EventSetMessage((s32)MsgKorimaMustHorribleBeyondRiverAm);
    } else {
        Engine_EventSetMessage((s32)MsgKorimaHealingWatersMercuryLighthouseMight);
    }
    Engine_EventShowMessage(9, 0);
    Engine_EventEnd();
}

void PaletteScene_RunActorEightBranch(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x845) != 0) {
        Engine_EventSetMessage((s32)MsgKorimaKnowCannotStopButPlease);
    } else {
        Engine_EventSetMessage((s32)MsgKorimaPeopleKolimaForgiveMe);
    }
    Engine_EventShowMessage(8, 0);
    Engine_EventEnd();
}

void PaletteScene_RunFlaggedBranch(void)
{
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    if (Engine_GameFlagIsSet(0x844) == 0) {
        RunEventScript01();
    } else {
        PaletteScene_RunActorTransitionSequence();
    }
    Engine_EventEnd();
}

void RunEventScript01(void)
{

    u32 i;
    s32 rec8;

    rec8 = Actor_Get(ACTOR_PARTY_LEADER);
    Engine_ActorFaceDirection(0, 0xc000, 0);
    ColorBuffer_ApplyTarget(0x406218, 1);
    Engine_ColorBufferInterpolate(20);
    Engine_TaskWait(40);
    Audio_PlayCue(17);
    gKorimaKiSparkSound = 1;
    Engine_TaskAddCallback((s32)PaletteScene_SpawnEffect, 0xc80);
    Engine_TaskWait(30);
    gKorimaKiSparkSound = 0;
    Camera_MoveTo(0x1480000, -1, 0xeb0000, 1);
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
    *((u8 *)Object_GetById(0) + 90) &= 254;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
    Audio_PlayCue(133);
    *(s32 *)(rec8 + 40) = 0x50000;
    *(s32 *)(rec8 + 72) = 0x4000;
    *(s32 *)(rec8 + 68) = 0xa000;
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x14f, 0x102);
    while (*(s32 *)(rec8 + 40) >= 0) {
        Engine_TaskWait(1);
    }
    do {
        Engine_TaskWait(1);
    } while (*(s32 *)(rec8 + 40) <= 0);
    Audio_PlayCue(161);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
    Engine_EventWait(120);
    Engine_TaskRemoveCallback((s32)PaletteScene_SpawnEffect);
    Engine_TaskWait(40);
    *(s32 *)(rec8 + 68) = 0x4000;
    {
        u8 *record = Actor_Get(ACTOR_PARTY_LEADER);
        u8 flags = record[90] | 1;

        record[90] = flags;
    }
    Engine_EventWait(80);
    Engine_EventSetMessage((s32)MsgKorimaControlTretsHeartShallNot);
    Event_ShowMessageAndWait(0x200e, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Event_ShowMessage(0x200e, 0);
    Audio_PlayCueFromEventWork();
    ColorBuffer_ApplyTarget(0x10000, 1);
    Engine_ColorBufferInterpolate(20);
    Engine_TaskWait(40);
    {
        s32 shown = 0xc000;

        *(u16 *)(rec8 + 6) = shown;
    }
    *(s32 *)(rec8 + 72) = 0x10000;
    *(s32 *)(rec8 + 68) = 0x4000;
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(40);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(20);
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
    Engine_MessageShowCentered((s32)MsgKorimaWaterHermesSeepedIntoTret, 1);
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
    Engine_ActorEnableActionCallback(ACTOR_GERALD, SceneAction_ActorOneEntry);
    Engine_ActorEnableActionCallback(ACTOR_IVAN, SceneAction_ActorTwoEntry);
    if (actorThreeEnabled != 0) {
        Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
        object = Actor_Get(ACTOR_PARTY_LEADER);
        if (object != 0) {
            Actor_SetPosition(ACTOR_MIA, *(s32 *)(object + 8), *(s32 *)(object + 16));
        }
        Engine_ActorEnableActionCallback(ACTOR_MIA, SceneAction_ActorThreeEntry);
    }
    Object_RefreshSelectorById(2);
    Engine_EventWait(40);
    KorimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(32);
    Engine_TaskWait(40);
    transitionState = &gKorimaKiTransitionStep;
    *transitionState = 0;
    Engine_TaskAddCallback((s32)PaletteScene_AdvanceTransition, 0xc80);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 20);
    Camera_SetSpeed(0x33333, 0x6666);
    Camera_MoveTo(0x1000000, -1, 0xfe0000, 1);
    Engine_CameraWaitForMove();
    Audio_PlayCue(246);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_IVAN, 0x2000, 20);
    Camera_MoveTo(0x19d0000, -1, 0x1050000, 1);
    Engine_CameraWaitForMove();
    Audio_PlayCue(246);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 20);
    Camera_MoveTo(0x1460000, -1, 0x1800000, 1);
    Engine_CameraWaitForMove();
    Audio_PlayCue(246);
    if (*transitionState != 24) {
        do {
            Engine_TaskWait(1);
        } while (*transitionState != 24);
    }
    Engine_TaskRemoveCallback((s32)PaletteScene_AdvanceTransition);
    Engine_TaskWait(10);
    cycle = 0;
    do {
        KorimaPalette_Restore(0);
        Engine_ColorBufferInterpolate(6);
        Engine_TaskWait(6);
        KorimaPalette_Restore(1);
        Engine_ColorBufferInterpolate(6);
        cycle = (cycle + 1);
        Engine_TaskWait(6);
    } while ((u32)cycle <= 3);
    KorimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(40);
    Engine_TaskWait(80);
    Camera_MoveTo(0x1480000, 0x80000, 0xd40000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    KorimaKi_PlayGesture(10, 1);
    Engine_EventWait(40);
    Audio_PlayCue(7);
    Engine_EventSetMessage((s32)MsgKorimaFeelGreatPowerSpreadingThrough);
    Event_ShowMessage(8, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_MIA, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 20);
    KorimaKi_PlayGesture(10, 2);
    Engine_EventWait(20);
    KorimaKi_PlayGesture(10, 3);
    Engine_EventWait(40);
    KorimaKi_PlayGesture(10, 1);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x105, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 40);
    Camera_MoveTo(0xea0000, 0, 0xe80000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    KorimaKi_PlayGesture(11, 1);
    Engine_EventWait(40);
    KorimaKi_PlayGesture(11, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    KorimaKi_PlayGesture(11, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 20);
    KorimaKi_PlayGesture(11, 3);
    Engine_EventWait(20);
    KorimaKi_PlayGesture(11, 2);
    Engine_EventWait(20);
    KorimaKi_PlayGesture(11, 3);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    KorimaKi_PlayGesture(10, 0);
    Engine_EventWait(20);
    Event_ShowMessage(0x8008, 0);
    KorimaKi_PlayGesture(10, 1);
    Engine_EventWait(20);
    Event_OpenMessage(0x8008, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Event_ShowMessage(0x4009, 0);
        Event_ShowMessage(0x8008, 0);
    } else {
        sceneWorkSlot = (u32)&gEventWork;
        *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 2;
        Actor_ShowEmote(ACTOR_MIA, 0x103, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 0);
        Actor_ShowEmote(ACTOR_IVAN, 0x103, 40);
        Engine_ActorSetAnimation(ACTOR_GERALD, 4);
        Event_ShowMessage(ACTOR_GERALD, 0);
        if (actorThreeEnabled != 0) {
            Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
            Event_ShowMessage(ACTOR_MIA, 0);
        } else {
            *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 1;
        }
        Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
        Event_ShowMessage(ACTOR_IVAN, 0);
        Event_ShowMessage(0x4009, 0);
        Event_ShowMessage(0x8008, 0);
    }
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_MIA, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Camera_MoveTo(0x1480000, 0x80000, 0xd40000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    KorimaKi_PlayGesture(10, 0);
    Engine_EventWait(20);
    KorimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    ColorBuffer_ApplyTarget(0x406218, 1);
    Engine_ColorBufferInterpolate(40);
    Engine_EventWait(60);
    gKorimaKiSparkCount = 0;
    gKorimaKiSparkOrigin[0] = 0x1480000;
    gKorimaKiSparkOrigin[1] = 0x300000;
    effectCallback = (s32)KorimaKi_SpawnOrbitSparks;
    gKorimaKiSparkOrigin[2] = 0xcd0000;
    Engine_TaskAddCallback(effectCallback, 0xc80);
    Engine_EventWait(100);
    Engine_TaskRemoveCallback(effectCallback);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(100);
    KorimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(20);
    Engine_EventWait(40);
    KorimaKi_PlayGesture(10, 1);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgKorimaShouldDoPeopleKolimaCursed);
    Event_ShowMessage(0x8008, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_MIA, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Camera_MoveTo(0xea0000, 0, 0xe80000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Event_ShowMessage(0x4009, 0);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    } else {
        Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
    }
    Event_ShowMessage(ACTOR_GERALD, 0);
    KorimaKi_PlayGesture(10, 4);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgKorimaWasIndeedAngryPeopleHad);
    Event_ShowMessage(0x8008, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Event_ShowMessage(0x8008, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_MIA, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    KorimaKi_PlayGesture(10, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x8008, 0, 20);
    KorimaKi_PlayGesture(11, 0);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    KorimaKi_PlayGesture(11, 3);
    Engine_EventWait(40);
    KorimaKi_PlayGesture(11, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    KorimaKi_PlayGesture(10, 2);
    Engine_EventWait(20);
    Event_ShowMessage(0x8008, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x102, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 80);
    KorimaKi_PlayGesture(11, 5);
    Engine_EventWait(60);
    KorimaKi_PlayGesture(11, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    KorimaKi_PlayGesture(10, 5);
    Engine_EventWait(40);
    KorimaKi_PlayGesture(10, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4008, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 10);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 20);
    Event_ShowMessage(0x8002, 0);
    KorimaKi_PlayGesture(11, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4009, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    KorimaKi_PlayGesture(10, 1);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    KorimaKi_PlayGesture(10, 2);
    Engine_EventWait(20);
    KorimaKi_PlayGesture(11, 3);
    Engine_EventWait(40);
    KorimaKi_PlayGesture(11, 0);
    Engine_EventWait(20);
    KorimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    ColorBuffer_ApplyTarget(0x406218, 1);
    Engine_ColorBufferInterpolate(40);
    Engine_EventWait(60);
    gKorimaKiSparkCount = 0;
    gKorimaKiSparkOrigin[0] = 0x880000;
    gKorimaKiSparkOrigin[1] = 0x140000;
    effectCallback = (s32)KorimaKi_SpawnOrbitSparks;
    gKorimaKiSparkOrigin[2] = 0x1020000;
    Engine_TaskAddCallback(effectCallback, 0xc80);
    Engine_EventWait(100);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 40);
    Engine_ActorStartRepeatedMotion(ACTOR_IVAN, 1);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 20);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    KorimaKi_PlayGesture(10, 4);
    Engine_EventWait(20);
    Event_ShowMessage(0x8008, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 60);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 10);
    Event_ShowMessageAndWait(0x8002, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 20);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_MIA, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x6000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0x6000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x6000, 120);
    Engine_TaskRemoveCallback(effectCallback);
    Engine_EventWait(60);
    KorimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(40);
    KorimaKi_PlayGesture(10, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x8008, 0, 20);
    KorimaKi_PlayGesture(11, 3);
    Event_ShowMessage(0x4009, 0);
    Event_ShowMessage(0x8008, 0);
    KorimaKi_PlayGesture(11, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_MIA, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    KorimaKi_PlayGesture(10, 1);
    Event_OpenMessage(0x8008, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
    }
    Engine_EventWait(10);
    KorimaKi_PlayGesture(10, 2);
    Engine_EventWait(20);
    KorimaKi_PlayGesture(11, 3);
    Engine_EventWait(40);
    KorimaKi_PlayGesture(10, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x8008, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_MIA, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Audio_PlayCue(17);
    finalActions = SceneAction_GroupFinish;
    Engine_ActorEnableActionCallback(ACTOR_GERALD, finalActions);
    if (actorThreeEnabled != 0) {
        Engine_ActorEnableActionCallback(ACTOR_MIA, finalActions);
    }
    Object_SetActionCallbackAndRefreshById(2, (s32)finalActions);
    KorimaKi_PlayGesture(10, 4);
    KorimaKi_PlayGesture(10, 4);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgKorimaOweGreatDebtHaveSaved);
    Event_ShowMessage(0x8008, 0);
    KorimaKi_PlayGesture(11, 4);
    KorimaKi_PlayGesture(11, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(0x4009, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    GameFlag_Set(0x845);
    Audio_PlayCue(1);
    PaletteScene_SetRecordValue(184, 185);
}

/* Korima's tree: the overlay's first entry stages the tree actors and sets
 * Retreat to return the party to the Korima bridge. */

/* Stage the tree actors: sizes, sprite flags and priorities, heights and collision. */
s32 KorimaKi_PrepareActors(void)
{
    struct FieldActor *first;
    struct FieldActor *third;
    struct FieldActor *second;
    u8 zero;

    first = Object_GetById(10);
    third = Object_GetById(14);
    second = Object_GetById(11);
    Engine_TaskWait(1);
    Engine_ActorSetChildValue(14, 15);
    gEventWork->start_transition = 0x204;
    gGameState.retreat_scene = (s32)&SceneId_KorimaHashi;
    gGameState.retreat_entrance = 4;
    /* FAKEMATCH: the heights are cleared through a u8 local zero, which the
     * compiler loads from the pool. */
    zero = 0;
    if (!Engine_GameFlagIsSet(0x845))
        PaletteScene_AdjustPaletteWindow(3);
    Object_GetById(8)->radius = 6;
    Object_GetById(9)->radius = 6;
    Object_GetById(12)->radius = 6;
    Object_GetById(13)->radius = 6;
    Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    Engine_ActorSetSpritePriority(8, 2);
    Engine_ActorSetSpritePriority(14, 2);
    Engine_ActorSetSpritePriority(9, 2);
    first->motion_flags = zero;
    first->y.fixed = 0x1c0000;
    second->motion_flags = zero;
    second->y.fixed = 0x1c0000;
    third->motion_flags = zero;
    third->y.fixed = 0x1c0000;
    Engine_ActorSetAnimation(9, 3);
    Engine_ActorSetAnimation(8, 3);
    Object_GetById(8)->collision_flags |= 8;
    Object_GetById(9)->collision_flags |= 8;
    Object_GetById(10)->collision_flags |= 8;
    Object_GetById(11)->collision_flags |= 8;
    Object_GetById(14)->collision_flags |= 8;
    return 0;
}

s32 PaletteScene_AdvanceEffectFrame(struct PaletteEffectFrame *frame)
{
    frame->progress += 0x1EB8;
    if (frame->limit == 0x80000000) {
        if (frame->second_limit == frame->limit) {
            if (frame->third_limit == frame->second_limit) {
                Engine_ObjectDispatchRelease(frame);
            }
        }
    }
    return 1;
}

void PaletteScene_SpawnEffect(void)
{

    struct PaletteEffect *effect;
    struct EffectSprite *sprite;
    s32 phase;
    s32 effect_flags;
    s32 sprite_flags;
    s32 spawn_x = 0x01460000;
    s32 spawn_y = 0x00200000;
    s32 spawn_z = 0x00c00000;
    s32 target_x = 0x01460000;
    s32 target_z = 0x00f00000;

    phase = gFrameCount & 3;
    if (phase != 0) return;
    if (gKorimaKiSparkSound != 0) Engine_AudioPlayCue(200);
    effect = (struct PaletteEffect *)Engine_ObjectCreate(26, spawn_x, spawn_y, spawn_z);
    if (effect == 0) return;
    sprite = effect->sprite;
    sprite->state = phase;
    effect_flags = 0xfe;
    effect_flags &= effect->flags;
    effect->flags = effect_flags;
    sprite_flags = ~12;
    sprite_flags &= sprite->flags;
    sprite_flags |= 4;
    sprite->flags = sprite_flags;
    effect->progress = 0x1999;
    effect->rate_x = 0x40000;
    effect->rate_y = 0x40000;
    effect->mode = phase;
    Object_SetMode(effect, 2);
    Engine_ObjectSetPosition((struct FieldActor *)effect, target_x, 0, target_z);
    Engine_ObjectSetScript(effect, gKorimaKiEffectScript);
}

/* Steps the shared transition counter, firing at 0 and at 20 and wrapping at
 * 30. */
void PaletteScene_AdvanceTransition(void)
{
    s32 step = gKorimaKiTransitionStep;

    if (step == 0) {
        KorimaPalette_Restore(0);
        Engine_ColorBufferInterpolate(20);
    } else if (step == 20) {
        KorimaPalette_Restore(1);
        Engine_ColorBufferInterpolate(8);
    }
    step = gKorimaKiTransitionStep + 1;
    gKorimaKiTransitionStep = step;
    if (step == 30) {
        gKorimaKiTransitionStep = 0;
    }
}

/* Play a numbered gesture: actor 10 selects actor 8's twelve, any other actor 9's six; then wait twelve frames. */
void KorimaKi_PlayGesture(s32 actor, s32 gesture)
{
    if (actor == 10) {
        switch (gesture) {
        case 0:
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 3);
            break;
        case 1:
            Engine_ActorSetAnimation(8, 1);
            break;
        case 2:
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 5);
            break;
        case 3:
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 4);
            break;
        case 4:
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 3);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 3);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 1);
            break;
        case 5:
            Engine_ActorSetAnimation(8, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 2);
            break;
        case 6:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 8);
            break;
        case 8:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 9);
            break;
        case 9:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 10);
            break;
        case 10:
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 8);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 6);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 8);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(8, 6);
            break;
        case 7:
        case 11:
            Engine_ActorSetAnimation(8, 6);
            break;
        }
    } else {
        switch (gesture) {
        case 0:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 3);
            break;
        case 1:
            Engine_ActorSetAnimation(9, 1);
            break;
        case 2:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 5);
            break;
        case 3:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 4);
            break;
        case 4:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 3);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 3);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 1);
            break;
        case 5:
            Engine_ActorSetAnimation(9, 1);
            Engine_TaskWait(6);
            Engine_ActorSetAnimation(9, 2);
            break;
        }
    }
    Engine_TaskWait(12);
}

void PaletteScene_AdvanceOrbit(struct OrbitingPaletteEffect *effect)
{
    s32 position[3];
    s32 step = effect->step;
    s32 heading;

    if (step <= 119) {
        position[0] = effect->anchor_x;
        position[1] = effect->anchor_y;
        position[2] = effect->anchor_z;
        heading = effect->heading;
        Vector_AddPolarOffset(step << 16, step * 768 + heading, position);
        effect->x = position[0];
        effect->y = position[1];
        effect->z = position[2];
        effect->angle_x += 0x147;
        effect->angle_y += 0x147;
        effect->step++;
    } else {
        Resource_ResetEntry(effect->owner[0x1c]);
        Engine_ObjectDispatchRelease(effect);
    }
}

/* Every ten frames up to forty, ring the centre with six orbiting sparks; the
 * frame counter wraps after 120. */
void KorimaKi_SpawnOrbitSparks(void)
{
    struct FieldActor *spark;
    s32 previous;
    u32 i;

    previous = 0;
    switch (gKorimaKiSparkCount) {
    case 0:
    case 10:
    case 20:
    case 30:
    case 40:
        Engine_AudioPlayCue(220);
        for (i = 0; i <= 5; i++) {
            spark = Engine_ObjectCreate(0x11d, gKorimaKiSparkOrigin[0], gKorimaKiSparkOrigin[1], gKorimaKiSparkOrigin[2]);
            if (spark != 0) {
                previous = Object_ReplaceResourceEntry(spark->sprite, previous);
                spark->motion_flags = 0;
                spark->sprite->priority = 1;
                Engine_ActorSetSpriteFlags(spark, 0);
                Object_SetMode(spark, 1);
                ((struct Spark *)spark)->phase = 0;
                ((struct Spark *)spark)->angle = (i * 60 << 16) / 360;
                spark->target_x = gKorimaKiSparkOrigin[0];
                spark->target_y = gKorimaKiSparkOrigin[1];
                spark->target_z = gKorimaKiSparkOrigin[2];
                spark->speed = 0x19999;
                spark->update = (void (*)(union FieldObject *))PaletteScene_AdvanceOrbit;
            }
        }
        break;
    }
    if (++gKorimaKiSparkCount > 120) {
        gKorimaKiSparkCount = 0;
    }
}

/* Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names. */
void PaletteScene_SetRecordValue(s32 key, s32 value)
{
    s32 slot = PartyInventory_FindOwner(key);

    if (slot != -1) {
        s32 index = Inventory_Find(slot, key);

        if (index != -1) {
            Owner_GetState(slot)->values[index] = value;
        }
    }
}

/* Applies the adjustment to palette RAM, skipping two protected windows. */
void PaletteScene_AdjustPaletteWindow(s32 adjustment)
{
    volatile u16 *palette = (volatile u16 *)0x05000000;
    u32 phase;
    u32 next_phase;
    KorimaPalette_SaveFirst();
    phase = 0;
    do {
        u32 index = phase >> 16;
        u32 second_window;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            second_window = (index + 0xff3f) << 16;
            if (second_window > 0x70000)
                palette[index] = PaletteScene_AdjustColor(palette[index], adjustment);
        }
        next_phase = phase + 0x10000;
        phase = next_phase;
    } while (next_phase <= 0x00df0000);
    KorimaPalette_Capture(); KorimaPalette_SaveSecond(); Engine_ColorBufferApplyTarget(0x10000, 0);
}

/*
 * Applies the asymmetric RGB555 colour adjustment: red rises, green and blue
 * fall. Control jumps over a mask literal inside the span and rejoins before
 * the common return, so the literal belongs to this owner.
 */
u16 PaletteScene_AdjustColor(u16 color, s32 adjustment)
{
    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Math_Divide(
        red,
        (s32)((u32)adjustment << 2)
    ));
    green = (s16)(green - Math_Divide(green, adjustment));
    blue = (s16)(blue - Math_Divide(blue, adjustment));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
