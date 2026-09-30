/* Handing over the stars, the Jupiter star and the paired actors. */
#include "STAR.H"
extern u8 MsgSoruJupiterStarBagged[];

void Scene_HandOverStars(void)
{
    Event_Begin();
    Scene_BagJupiterStar();
    FieldScene_StagePairedActors();
    FieldScene_RunElementalStarDemand();
    Scene_OfferGuarantee();
    Scene_UnmaskGarcia();
    Scene_AlexTakesStars();
    GameFlag_Set(FLAG_STARS_GIVEN_TO_ALEX);
    Event_End();
    Func_0200227c();
}

void Scene_BagJupiterStar(void)
{
    u32 i;
    s32 obj;

    Audio_PlayCue(141);
    for (i = 0; i != 6; i++) {
        ColorBuffer_ApplyTarget(0x4049d2, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(8);
        Event_Wait(8);
        if (i == 1) {
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        }
    }
    Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Event_Wait(30);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0xa70000, -1, 0x2110000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 65, 31);
    Map_CopyCellAttributes(0, 0, 1, 1, 10, 31);
    Map_CopyCellsTo(87, 42, 10, 33, 1, 2);
    Event_Wait(40);
    Work_SetValuesIfNonNegative(0, 0, 0);
    Camera_SetSpeed(0x66666, 0xcccc);
    Camera_MoveTo(0x1870000, -1, 0xb10000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 79, 9);
    Map_CopyCellAttributes(0, 0, 1, 1, 24, 9);
    Map_CopyCellsTo(87, 42, 24, 11, 1, 2);
    Event_Wait(40);
    Work_SetValuesIfNonNegative(0, 0, 0);
    Camera_SetSpeed(0x26666, 0x4ccc);
    Camera_MoveTo(0x2470000, -1, 0xc10000, 1);
    Camera_WaitForMove();
    Work_SetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 91, 10);
    Map_CopyCellAttributes(0, 0, 1, 1, 36, 10);
    Map_CopyCellsTo(87, 42, 36, 12, 1, 2);
    Event_Wait(40);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Event_CloseScreen();
    Event_WaitForScreen();
    Camera_MoveTo(0xe80000, -1, 0x1dd0000, 0);
    Map_Redraw();
    Task_Wait(1);
    Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Event_Wait(20);
    Map_CopyCellsTo(0, 40, 13, 66, 3, 3);
    Event_Wait(20);
    obj = Scene_PresentItem(223, 0xe80000, 0x100000, 0x1d00000);
    Event_Wait(40);
    UiWork_PushValueSlotFar(obj, 1);
    Message_ShowCentered((s32)MsgSoruJupiterStarBagged, 1);
}

/* Runs a sequence of position/scale/timing calls for actor pair 0 and 1,
 * copying a stored pair of 32-bit fields (offsets +8, +16) from actor 0's
 * record onto actor 1 partway through, then runs an analogous sequence for
 * actors 5, 9, 10 and 11. */
void FieldScene_StagePairedActors(void)
{
    u32 i;
    u8 *record;

    Audio_PlayCue(17);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 231, 0x1ea);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 30);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(180);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(80);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 246, 0x1df);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 10);
    /* Copy actor 0's stored fields at +8 and +16 onto actor 1, if a record
     * for actor 0 exists. */
    record = Actor_Get(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x101, 0x1eb);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xa000, 40);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 80);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x109, 0x1c5);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x11a, 0x1d5);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
    Actor_Jump(ACTOR_GERALD, 6, 60);
    Actor_SetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Actor_SetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Actor_SetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Actor_SetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Camera_SetSpeed(0x73333, 0xe666);
    Camera_MoveTo(0x1e50000, -1, 0x1590000, 1);
    Actor_FaceDirection(ACTOR_JASMINE, 0x6000, 0);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0x5000, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0x5000, 0);
    Camera_WaitForMove();
    Event_Wait(40);
}
