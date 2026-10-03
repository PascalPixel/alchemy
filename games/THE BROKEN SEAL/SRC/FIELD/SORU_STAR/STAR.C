/* Object angle and the scene tables. */
#include "STAR.H"
#include "TYPES.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgSoruVenusStarBagged[];
extern u16 SoruStar_StarCells[];
extern struct EventWork *gEventWork;
s32 Scene_PresentItem();
void Event_SayThenWait();
void Engine_WorkSetValuesIfNonNegative();
void Engine_MapAnimateCells();
void Engine_EventBegin();
void Engine_EventWait();
void Engine_MapCopyCellAttributes();
void Engine_MapCopyCellsTo();
void Engine_TaskWait();
void Engine_MapRedraw();
void Engine_ColorBufferApplyTarget();
void Engine_ColorBufferInterpolate(s32 frames);
void Engine_CameraSetSpeed();
void Engine_AudioPlayCue();
void Engine_CameraWaitForMove();
void Engine_MapRenderWaitForValues();
void UiWork_PushValueSlotFar();
void Engine_CameraMoveTo();
void Engine_EventEnd();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void Engine_ActorJump();
void Engine_EventOpenScreen();
void Engine_ActorFaceDirection();
void Engine_EventSetMessage();

extern u8 MsgSoruPutMercuryStar[];

extern u8 MsgSoruJupiterStarBagged[];

extern u8 MsgSoruLooksLikeTheyveSpottedUs[];
extern u8 MsgSoruWhyDenyDont[];
struct ObjectRuntime;
void BattlePres_RunActionThenWaitIfModeZero();

/* Moves the event's current message on by amount. */
static __inline__ void SkipMessage(s32 amount)
{
    gEventWork->message += amount;
}

extern u8 MsgSoruMenardiDontYouWantThem[];

extern u8 MsgSoruDontHandOver[];
extern u8 MsgSoruGuessTakeElemental[];
extern u8 MsgSoruRightTake[];
extern u8 MsgSoruWontLetGo[];

extern u8 MsgSoruDontWantAnything[];
extern u8 MsgSoruDoubtHowFeel[];
extern u8 MsgSoruPermitRelieveElemental[];
extern u8 MsgSoruThankCooperation[];

/* FAKEMATCH: the flag byte's or-assign goes through this helper, which
 * keeps the reference's register for the byte. */
static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

s32 UpdateOverlayObjectAngle(struct OverlayObject *object)
{
    struct OverlayObject *linked_object = object->linked_object;
    if (linked_object != NULL) {
        s32 angle_delta;
        u16 angle;
        object->unknown_5a = object->unknown_5a & 0xFE;
        angle_delta = CalculateAngleFromCoordinateDelta(
            linked_object->coordinate_10 - object->coordinate_10,
            linked_object->coordinate_08 - object->coordinate_08);
        angle = object->angle;
        angle_delta -= angle;
        angle_delta <<= 16;
        angle_delta >>= 16;
        if (angle_delta != 0) {
            if (angle_delta > 0x1000) {
                angle_delta = 0x1000;
            }
            if (angle_delta < -0x1000) {
                angle_delta = -0x1000;
            }
            object->angle = angle + angle_delta;
        }
    }
    return 1;
}

s32 *SceneData_GetScriptTable(void)
{
    return Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

s32 *SceneData_GetActorTable(void)
{
    return Placement_Actors;
}

s32 *SceneData_GetEffectTable(void)
{
    return Placement_Effects;
}

/* The party bags the Venus Star while its chamber changes around them. */
void Scene_BagVenusStar(void)
{
    u32 i;
    s32 obj;
    s32 mes;

    Engine_EventBegin();
    Engine_AudioPlayCue(141);
    for (i = 0; i != 6; i++) {
        Engine_ColorBufferApplyTarget(0x403a52, 1);
        Engine_ColorBufferInterpolate(8);
        Engine_EventWait(8);
        Engine_ColorBufferApplyTarget(0x10000, 1);
        Engine_ColorBufferInterpolate(8);
        Engine_EventWait(8);
        if (i == 1) {
            Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        }
    }
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_EventWait(30);
    Engine_CameraSetSpeed(0x26666, 0x4ccc);
    Call4(Engine_CameraMoveTo, 0x2980000, -1, 0x1f10000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 96, 29);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 41, 29);
    Call6(Engine_MapCopyCellsTo, 87, 42, 41, 31, 1, 2);
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(0, 0, 0);
    Engine_CameraSetSpeed(0x66666, 0xcccc);
    Call4(Engine_CameraMoveTo, 0x1370000, -1, 0x1f10000, 1);
    Engine_CameraWaitForMove();
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 74, 29);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 19, 29);
    Engine_MapCopyCellsTo(87, 42, 19, 31, 1, 2);
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(0, 0, 0);
    Call4(Engine_CameraMoveTo, 0x2970000, -1, 0xc00000, 1);
    Engine_CameraWaitForMove();
    Call3(Engine_WorkSetValuesIfNonNegative, 0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 96, 10);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 41, 10);
    Engine_MapCopyCellsTo(87, 42, 41, 12, 1, 2);
    Engine_EventWait(40);
    *(s32 *)((u8 *)gEventWork + 0x1c0) = 0x202;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call4(Engine_CameraMoveTo, 0x2c60000, -1, 0x1da0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x10000, 0x10000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(0x121);
    Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Engine_EventWait(20);
    Engine_MapCopyCellsTo(0, 40, 43, 66, 3, 3);
    Engine_EventWait(20);
    obj = Scene_PresentItem(220, 0x2c80000, 0x100000, 0x1d00000);
    Engine_EventWait(40);
    UiWork_PushValueSlotFar(obj, 1);
    mes = (s32)MsgSoruVenusStarBagged;
    Engine_MessageShowCentered(mes, 1);
    Call3(Engine_ActorFaceDirection, 9, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 5, 0x2000, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call4(Engine_CameraMoveTo, 0x1ce0000, -1, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorJump(9, 4, 30);
    Engine_EventSetMessage(mes - 1);
    Event_SayThenWait(9, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x2c60000, -1, 0x1da0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(0x83c);
    Engine_EventEnd();
}

void Scene_BagMercuryStar(void)
{
    u32 i;
    s32 obj;
    s32 mes;

    Engine_EventBegin();
    Engine_AudioPlayCue(141);
    for (i = 0; i != 6; i++) {
        Engine_ColorBufferApplyTarget(0x404a4e, 1);
        Engine_ColorBufferInterpolate(8);
        Engine_EventWait(8);
        Engine_ColorBufferApplyTarget(0x10000, 1);
        Engine_ColorBufferInterpolate(8);
        Engine_EventWait(8);
        if (i == 1) {
            Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        }
    }
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Engine_EventWait(30);
    Engine_WorkSetValuesIfNonNegative(0, 0, 0);
    Engine_CameraSetSpeed(0x59999, 0xb333);
    Engine_CameraMoveTo(0x1d80000, -1, 0x620000, 1);
    Engine_CameraWaitForMove();
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 84, 4);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 29, 4);
    Engine_MapCopyCellsTo(87, 42, 29, 6, 1, 2);
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(0, 0, 0);
    Engine_CameraMoveTo(0x1570000, -1, 0x1710000, 1);
    Engine_CameraWaitForMove();
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 76, 21);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 21, 21);
    Engine_MapCopyCellsTo(87, 42, 21, 23, 1, 2);
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(0, 0, 0);
    Engine_CameraSetSpeed(0x33333, 0x6666);
    Engine_CameraMoveTo(0x1570000, -1, 0x1f10000, 1);
    Engine_CameraWaitForMove();
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 76, 29);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 21, 29);
    Engine_MapCopyCellsTo(87, 42, 21, 31, 1, 2);
    Engine_EventWait(40);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x2c80000, -1, 0x980000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_WorkSetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(0x121);
    Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Engine_EventWait(20);
    Engine_MapCopyCellsTo(0, 40, 43, 46, 3, 3);
    Engine_EventWait(20);
    obj = Scene_PresentItem(221, 0x2c80000, 0x100000, 0x900000);
    Engine_EventWait(40);
    UiWork_PushValueSlotFar(obj, 1);
    mes = (s32)MsgSoruPutMercuryStar;
    Engine_MessageShowCentered(mes, 1);
    Engine_ActorFaceDirection(ACTOR_SUKURETA, 0xe000, 0);
    Engine_ActorFaceDirection(ACTOR_JASMINE, 0xe000, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x1ce0000, -1, 0x15e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorJump(ACTOR_SUKURETA, 4, 30);
    Engine_EventSetMessage(mes - 2);
    Event_SayThenWait(9, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x2c80000, -1, 0x980000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_GameFlagSet(FLAG_MERCURY_STAR_BAGGED);
    Engine_EventEnd();
}

/* Handing over the stars, the Jupiter star and the paired actors. */
void Scene_HandOverStars(void)
{
    Engine_EventBegin();
    Scene_BagJupiterStar();
    FieldScene_StagePairedActors();
    FieldScene_RunElementalStarDemand();
    Scene_OfferGuarantee();
    Scene_UnmaskGarcia();
    Scene_AlexTakesStars();
    Engine_GameFlagSet(FLAG_STARS_GIVEN_TO_ALEX);
    Engine_EventEnd();
    SoruStar_LineUpFollowers();
}

void Scene_BagJupiterStar(void)
{
    u32 i;
    s32 obj;

    Engine_AudioPlayCue(141);
    for (i = 0; i != 6; i++) {
        Engine_ColorBufferApplyTarget(0x4049d2, 1);
        Engine_ColorBufferInterpolate(8);
        Engine_EventWait(8);
        Engine_ColorBufferApplyTarget(0x10000, 1);
        Engine_ColorBufferInterpolate(8);
        Engine_EventWait(8);
        if (i == 1) {
            Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        }
    }
    Engine_WorkSetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Engine_EventWait(30);
    Engine_CameraSetSpeed(0x26666, 0x4ccc);
    Engine_CameraMoveTo(0xa70000, -1, 0x2110000, 1);
    Engine_CameraWaitForMove();
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 65, 31);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 10, 31);
    Engine_MapCopyCellsTo(87, 42, 10, 33, 1, 2);
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(0, 0, 0);
    Engine_CameraSetSpeed(0x66666, 0xcccc);
    Engine_CameraMoveTo(0x1870000, -1, 0xb10000, 1);
    Engine_CameraWaitForMove();
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 79, 9);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 24, 9);
    Engine_MapCopyCellsTo(87, 42, 24, 11, 1, 2);
    Engine_EventWait(40);
    Engine_WorkSetValuesIfNonNegative(0, 0, 0);
    Engine_CameraSetSpeed(0x26666, 0x4ccc);
    Engine_CameraMoveTo(0x2470000, -1, 0xc10000, 1);
    Engine_CameraWaitForMove();
    Engine_WorkSetValuesIfNonNegative(0x10000, 0x20000, 0x10000);
    Engine_EventWait(20);
    Engine_AudioPlayCue(144);
    Engine_MapAnimateCells(SoruStar_StarCells, 91, 10);
    Engine_MapCopyCellAttributes(0, 0, 1, 1, 36, 10);
    Engine_MapCopyCellsTo(87, 42, 36, 12, 1, 2);
    Engine_EventWait(40);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0xe80000, -1, 0x1dd0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_WorkSetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_AudioPlayCue(0x121);
    Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Engine_EventWait(20);
    Engine_MapCopyCellsTo(0, 40, 13, 66, 3, 3);
    Engine_EventWait(20);
    obj = Scene_PresentItem(223, 0xe80000, 0x100000, 0x1d00000);
    Engine_EventWait(40);
    UiWork_PushValueSlotFar(obj, 1);
    Engine_MessageShowCentered((s32)MsgSoruJupiterStarBagged, 1);
}

/* Runs a sequence of position/scale/timing calls for actor pair 0 and 1,
 * copying a stored pair of 32-bit fields (offsets +8, +16) from actor 0's
 * record onto actor 1 partway through, then runs an analogous sequence for
 * actors 5, 9, 10 and 11. */
void FieldScene_StagePairedActors(void)
{
    u32 i;
    u8 *record;

    Engine_AudioPlayCue(17);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 231, 0x1ea);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xc000, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(180);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(80);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 246, 0x1df);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xe000, 10);
    /* Copy actor 0's stored fields at +8 and +16 onto actor 1, if a record
     * for actor 0 exists. */
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Engine_ActorSetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x101, 0x1eb);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xa000, 40);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x101, 80);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x109, 0x1c5);
    Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x11a, 0x1d5);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xe000, 40);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x100, 0);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
    Engine_ActorJump(ACTOR_GERALD, 6, 60);
    Engine_ActorSetPosition(ACTOR_JASMINE, 0x1db0000, 0x14c0000);
    Engine_ActorSetPosition(ACTOR_SUKURETA, 0x1eb0000, 0x14c0000);
    Engine_ActorSetPosition(ACTOR_MENARDI, 0x1cb0000, 0x15c0000);
    Engine_ActorSetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Engine_CameraSetSpeed(0x73333, 0xe666);
    Engine_CameraMoveTo(0x1e50000, -1, 0x1590000, 1);
    Engine_ActorFaceDirection(ACTOR_JASMINE, 0x6000, 0);
    Engine_ActorFaceDirection(ACTOR_SUKURETA, 0x5000, 0);
    Engine_ActorFaceDirection(ACTOR_MENARDI, 0x5000, 0);
    Engine_ActorFaceDirection(ACTOR_SATUROS, 0x5000, 0);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
}

/* The demand for the Elemental Stars. Its dialogue starts at MsgSoruLooksLikeTheyveSpottedUs
 * while the actors move into place; a nonzero answer to the prompt skips one
 * reply, and the dialogue then continues from MsgSoruWhyDenyDont. */
void FieldScene_RunElementalStarDemand(void)
{
    u8 *field_85;
    u8 *field_80_38;
    u8 *record12;
    u8 *record8;
    s32 cnt;
    s32 zero;

    Engine_AudioPlayCue(61);
    Engine_ActorSetAnimation(10, 4);
    Engine_EventSetMessage((s32)MsgSoruLooksLikeTheyveSpottedUs);
    Event_SayThenWait(10, 10);
    Engine_ActorSetAnimation(11, 4);
    Event_SayThenWait(11, 30);
    Call3(Engine_ActorShowEmote, 9, 0x102, 60);
    Engine_ActorJump(9, 4, 10);
    Engine_ActorJump(9, 6, 30);
    Event_SayThenWait(9, 10);
    Engine_ActorRunRepeatedMotion(10, 1);
    Call3(Engine_ActorFaceDirection, 10, 0xb000, 10);
    Event_SayThenWait(10, 20);
    Engine_ActorRunRepeatedMotion(11, 1);
    Call3(Engine_ActorFaceDirection, 11, 0xd000, 20);
    Event_SayThenWait(11, 30);
    Call3(Engine_ActorShowEmote, 9, 0x102, 60);
    Engine_ActorFaceDirection(5, 0, 0);
    Call3(Engine_ActorFaceDirection, 9, 0x7000, 80);
    Call3(Engine_ActorShowEmote, 5, 0x102, 40);
    Event_SayThenWait(5, 20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_ActorSetAnimation(9, 4);
    Event_SayThenWait(9, 10);
    record12 = (u8 *)Object_GetById(12);
    record8 = (u8 *)Object_GetById(8);
    field_80_38 = *(u8 **)(record12 + 80) + 38;
    zero = 0;
    *field_80_38 = zero;
    *(s32 *)(record12 + 24) = 0x1999;
    *(s32 *)(record12 + 28) = 0x1999;
    *(s32 *)(record8 + 24) = 0x1999;
    *(s32 *)(record8 + 28) = 0x1999;
    Call2(Engine_ActorSetChildValue, 12, 0x100);
    Call3(Engine_ActorSetPosition, 12, 0x1d70000, 0x1220000);
    field_85 = record12 + 85;
    *field_85 = zero;
    *(s32 *)(record12 + 12) = 0x280000;
    Engine_EventWait(1);
    Event_SayThenWait(12, 10);
    Call3(Engine_ActorShowEmote, 5, 0x100, 0);
    Call3(Engine_ActorShowEmote, 9, 0x100, 30);
    Call3(Engine_ActorFaceDirection, 5, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 9, 0xb000, 10);
    Engine_ActorFaceDirection(11, 0xd000, 0);
    Call3(Engine_ActorFaceDirection, 10, 0xb000, 0);
    Engine_CameraSetSpeed(0x20000, 0x4000);
    Engine_CameraMoveTo(0x1d70000, -1, 0x1350000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetPosition(8, 0x1d70000, 0x1220000);
    Engine_AudioPlayCue(190);
    Engine_ActorSetSpritePriority(12, 2);
    for (cnt = 0; cnt != 90; cnt++) {
        *(s32 *)(record12 + 12) += -0x1999;
        *(s32 *)(record12 + 24) += 0x28f;
        *(s32 *)(record12 + 28) += 0x28f;
        *(s32 *)(record8 + 24) += 0x28f;
        *(s32 *)(record8 + 28) += 0x28f;
        Engine_EventWait(1);
    }
    *field_85 = 5;
    Engine_EventWait(80);
    for (cnt = 0; cnt != 60; cnt++) {
        *(s32 *)(record12 + 12) += -0x8000;
        Engine_EventWait(1);
    }
    *field_85 = 3;
    Engine_EventWait(30);
    *field_80_38 = 1;
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetSpritePriority(12, 1);
    {
        u8 *flags = (u8 *)Object_GetById(12) + 35;
        cnt = 1;
        cnt |= *flags;
        *flags = cnt;
    }
    Engine_ActorSetChildValue(12, 0);
    Call3(Engine_ActorSetSpeed, 12, 0x8000, 0x4000);
    Engine_ActorWalkToAndWait(12, 0x1d7, 0x132);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(12, 2);
    Event_SayThenWait(0x400c, 20);
    Engine_ActorFaceEachOther(5, 9, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(10, 1);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(10, 4);
    Call3(Engine_ActorFaceDirection, 10, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 11, 0x5000, 10);
    Event_SayThenWait(10, 30);
    Engine_ActorSetAttachedEffect(12, 0x102);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(10);
    Event_SayThenWait(11, 30);
    Call3(Engine_ActorFaceDirection, 11, 0xd000, 30);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(20);
    Event_SayThenWait(11, 30);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 11, 0x5000, 40);
    Call3(Engine_ActorFaceDirection, 9, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 5, 0x6000, 20);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Call4(Engine_CameraMoveTo, 0x1080000, -1, 0x1cc0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Event_SayThenWait(10, 40);
    Engine_ActorStartRepeatedMotion(0, 3);
    Engine_ActorRunRepeatedMotion(1, 3);
    Engine_EventWait(80);
    Engine_EventOpenMessage(11, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        SkipMessage(1);
    }
    BattlePres_RunActionThenWaitIfModeZero(9, 0, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x1dd0000, -1, 0x14e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_ActorFaceDirection(10, 0xb000, 10);
    Engine_EventSetMessage((s32)MsgSoruWhyDenyDont);
    Event_SayThenWait(10, 20);
    Call3(Engine_ActorFaceDirection, 5, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 9, 0x3000, 10);
    Engine_ActorSetAnimation(9, 4);
    Event_SayThenWait(0x5009, 40);
    Engine_ActorRunRepeatedMotion(11, 1);
    Event_SayThenWait(11, 10);
    Engine_ActorStartRepeatedMotion(5, 2);
    Engine_ActorRunRepeatedMotion(9, 2);
}

/* Offering the guarantee. */
void Scene_OfferGuarantee(void)
{
    u32 i;
    s32 record;

    Engine_ActorFaceDirection(ACTOR_JASMINE, 0x6000, 0);
    Engine_ActorFaceDirection(ACTOR_SUKURETA, 0x5000, 0);
    Engine_ActorFaceDirection(ACTOR_SATUROS, 0x8000, 0);
    Engine_ActorFaceDirection(ACTOR_GARCIA_MASKED, 0x5000, 40);
    Engine_ActorFaceDirection(ACTOR_MENARDI, 0xd000, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
    Event_SayThenWait(11, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_GARCIA_MASKED, 2);
    Engine_ActorSetAttachedEffect(ACTOR_GARCIA_MASKED, 0x102);
    Engine_EventWait(60);
    Event_SayThenWait(12, 10);
    Engine_ActorFaceActor(ACTOR_SATUROS, ACTOR_GARCIA_MASKED, 0);
    Engine_ActorFaceActor(ACTOR_JASMINE, ACTOR_GARCIA_MASKED, 0);
    Engine_ActorFaceActor(ACTOR_SUKURETA, ACTOR_GARCIA_MASKED, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
    Engine_ActorFaceDirection(ACTOR_SATUROS, 0x8000, 10);
    Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
    Event_SayThenWait(10, 10);
    Engine_ActorFaceDirection(ACTOR_MENARDI, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_JASMINE, 0, 0);
    Engine_ActorFaceDirection(ACTOR_SUKURETA, 0x8000, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_JASMINE, 0xc000, 0);
    Engine_ActorFaceDirection(ACTOR_SUKURETA, 0xb000, 10);
    Engine_ActorFaceDirection(ACTOR_SATUROS, 0xb000, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
    Event_SayThenWait(10, 10);
    Engine_ActorShowEmote(ACTOR_JASMINE, 0x100, 20);
    Engine_ActorSetAttachedEffect(ACTOR_GARCIA_MASKED, 0x102);
    Engine_ActorRunRepeatedMotion(ACTOR_GARCIA_MASKED, 3);
    Engine_EventWait(40);
    Engine_ActorSetPosition(ACTOR_GERALD, 0x15a0000, 0x1b80000);
    Engine_TaskWait(1);
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xa000, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_ActorSetPosition(ACTOR_GERALD, 0x1180000, 0x1d60000);
    Engine_CameraMoveTo(0x1050000, -1, 0x1d20000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x3000, 10);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x102, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventAskYesNo(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventWait(10);
    Engine_EventAskYesNo(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgSoruMenardiDontYouWantThem);
    Engine_EventShowMessage(ACTOR_MENARDI, 0);
    Engine_ActorFaceDirection(ACTOR_MENARDI, 0xd000, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xe000, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x1dd0000, -1, 0x14e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_SUKURETA, 0x8000, 30);
    Event_SayThenWait(9, 20);
    Engine_ActorFaceDirection(ACTOR_JASMINE, 0, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(20);
    Event_SayThenWait(5, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_GARCIA_MASKED, 2);
    Engine_EventWait(80);
    Event_SayThenWait(12, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_ActorFaceDirection(ACTOR_JASMINE, 0xc000, 0);
    Engine_ActorFaceDirection(ACTOR_SUKURETA, 0xb000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GARCIA_MASKED, 3);
    Event_SayThenWait(12, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
    Engine_ActorFaceDirection(ACTOR_GARCIA_MASKED, 0xb000, 40);
}

void Scene_UnmaskGarcia(void)
{
    u8 *obj;
    s32 other;
    s32 tbl;
    s32 left;
    s32 cnt;
    s32 mes_a;
    s32 mes_b;

    Engine_AudioPlayCue(161);
    Engine_ActorRunRepeatedMotion(ACTOR_GARCIA_MASKED, 3);
    Engine_EventWait(40);
    other = Object_GetById(ACTOR_GARCIA_MASKED);
    if (other != 0) {
        Engine_ActorSetPosition(ACTOR_GARCIA, *(s32 *)(other + 8), *(s32 *)(other + 16));
    }
    Engine_ActorSetPosition(ACTOR_GARCIA_MASKED, 0, 0);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_GARCIA, 0x3000, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 3);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 3);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Event_SayThenWait(5, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_GARCIA, 3);
    Engine_EventWait(10);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 3);
    Engine_ActorFaceDirection(ACTOR_SUKURETA, 0x8000, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
    Event_SayThenWait(9, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Engine_EventWait(40);
    Engine_ActorFaceDirection(ACTOR_SUKURETA, 0xb000, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_GARCIA, 2);
    Event_SayThenWait(13, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 1);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_GARCIA, 3);
    Engine_EventWait(10);
    Event_SayThenWait(13, 40);
    Engine_ActorStartRepeatedMotion(ACTOR_SATUROS, 1);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
    Engine_EventWait(10);
    Event_SayThenWait(10, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
    Engine_ActorSetAnimation(ACTOR_MENARDI, 3);
    Event_SayThenWait(11, 80);
    Engine_ActorRunRepeatedMotion(ACTOR_GARCIA, 2);
    Event_SayThenWait(13, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(10);
    Event_SayThenWait(5, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_GARCIA, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
    Engine_EventWait(20);
    Event_SayThenWait(5, 80);
    Engine_ActorSetAnimationAndWait(ACTOR_GARCIA, 4);
    Event_SayThenWait(13, 80);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_EventWait(4);
    Event_SayThenWait(5, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
    Engine_ActorSetAnimation(ACTOR_SATUROS, 3);
    Event_SayThenWait(10, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
    Event_SayThenWait(11, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 1);
    Event_SayThenWait(10, 10);
    Engine_ActorFaceDirection(ACTOR_SUKURETA, 0x3000, 80);
    Engine_ActorShowEmote(ACTOR_SUKURETA, 0x105, 80);
    Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
    Engine_ActorFaceDirection(ACTOR_MENARDI, 0x5000, 40);
    Engine_ActorStartRepeatedMotion(ACTOR_MENARDI, 2);
    Event_SayThenWait(11, 20);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x1050000, -1, 0x1d20000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xb000, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 244, 0x1de);
    Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x104, 0x1ea);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x3000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xb000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventWait(20);
    cnt = 0;
    tbl = Owner_GetState(1) + 216;
    left = 14;
    do {
        u32 id = *(u16 *)(tbl)& 0x1ff;
        tbl += 2;
        if (id == 220 || id == 221 || id == 223)
            cnt++;
        left--;
    } while (left >= 0);
    Engine_EventOpenMessage(ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        mes_a = (s32)MsgSoruGuessTakeElemental;
        Engine_EventSetMessage(mes_a);
        Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
        Engine_EventWait(10);
        if (cnt <= 2) {
            Event_SayThenWait(1, 30);
            Engine_ActorWalkToAndWait(ACTOR_GERALD, 252, 0x1e6);
            Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
            Engine_EventWait(10);
            UiText_ShowCenteredMessage((mes_a + 1), 1, 0);
        } else {
            Engine_EventSetMessage((s32)MsgSoruRightTake);
            Event_SayThenWait(ACTOR_GERALD, 30);
        }
    } else {
        if (cnt <= 2) {
            mes_b = (s32)MsgSoruDontHandOver;
            Engine_EventSetMessage(mes_b);
            Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
            Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
            Event_SayThenWait(1, 10);
            Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
            Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
            Engine_ActorSetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
            obj = Object_GetById(ACTOR_PARTY_LEADER);
            obj[90] &= 254;
            Engine_ActorWalkToAndWait(ACTOR_GERALD, 244, 0x1de);
            Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
            Engine_ActorSetDestination(ACTOR_PARTY_LEADER, 218, 0x1d7);
            Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
            UiText_ShowCenteredMessage((mes_b + 1), 1, 0);
            Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0, 30);
            {
                /* FAKEMATCH: a result temporary, not a compound or-assign: the
                 * reference merges the byte into the mask's register, which the
                 * two-address ORR does only when the result is its own object. */
                u8 flags = obj[90] | 1;

                obj[90] = flags;
            }
        } else {
            Engine_EventSetMessage((s32)MsgSoruWontLetGo);
            Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
            Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
            Event_SayThenWait(1, 10);
            Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
            Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0xe000, 30);
        }
    }
    Engine_CameraSetSpeed(0x8000, 0x1000);
    Engine_CameraFollowActor(ACTOR_GERALD, 1);
    Engine_CameraWaitForMove();
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x8000, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    obj = Object_GetById(ACTOR_GERALD);
    obj[90] &= 254;
    Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x108, 0x1e2);
    {
        /* FAKEMATCH: a result temporary, not a compound or-assign: the
         * reference merges the byte into the mask's register, which the
         * two-address ORR does only when the result is its own object. */
        u8 flags = obj[90] | 1;

        obj[90] = flags;
    }
    Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x116, 0x1e0);
    *(s32 *)(obj + 48) = 0x30000;
    *(s32 *)(obj + 52) = 0x20000;
    Engine_AudioPlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Engine_ActorSetAnimation(ACTOR_GERALD, 7);
    Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x138, 0x1d6);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_EventWait(30);
    Engine_AudioPlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Engine_ActorSetAnimation(ACTOR_GERALD, 7);
    Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x156, 0x1d6);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_EventWait(30);
    Engine_AudioPlayCue(153);
    *(s32 *)(obj + 40) = 0x60000;
    Engine_ActorSetAnimation(ACTOR_GERALD, 7);
    Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x178, 0x1d6);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
}

void Scene_AlexTakesStars(void)
{
    u32 i;
    u8 *rec;
    s32 record;
    s32 none;

    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0x3000, 0);
    record = Object_GetById(ACTOR_ALEX);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetChildValue(ACTOR_ALEX, 15);
    Engine_ActorSetPosition(ACTOR_ALEX, 0x1880000, 0x1c60000);
    SoruStar_RiseActorFourteenSparks();
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xd000, 10);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x100, 40);
    Engine_ActorFaceDirection(ACTOR_ALEX, 0x5000, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgSoruPermitRelieveElemental);
    Engine_EventShowMessage(ACTOR_ALEX, 0);
    Engine_ActorSetPosition(ACTOR_SATUROS, 0x1d50000, 0x15c0000);
    Engine_EventWait(20);
    Event_SayThenWait(0x200a, 10);
    Event_SayThenWait(0x200a, 40);
    Engine_ActorSetPosition(ACTOR_SATUROS, 0x1fb0000, 0x15c0000);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_EventWait(40);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x185, 0x1d4);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xd000, 60);
    Event_SayThenWait(1, 20);
    UiText_ShowCenteredMessage((s32)MsgSoruPermitRelieveElemental + 4, 1, 10);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    rec = Object_GetById(ACTOR_GERALD);
    rec[90] &= 254;
    /* FAKEMATCH: the zero is parked here, well before its one store, which
     * keeps it in the register the reference holds it in. */
    none = 0;
    Engine_ActorWalkToAndWait(ACTOR_GERALD, 0x178, 0x1d6);
    Engine_EventWait(30);
    SetFlagBits(&rec[90], 1);
    Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 4);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgSoruPermitRelieveElemental + 5);
    Event_SayThenWait(14, 20);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x101, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
    Event_SayThenWait(14, 20);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x102, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_ALEX, 0xc000, 20);
    Engine_ActorSetChildValue(ACTOR_ALEX, 0x100);
    record = Object_GetById(ACTOR_ALEX);
    Engine_ActorSetSpriteFlags(record, 0);
    rec = Object_GetById(ACTOR_ALEX);
    rec[85] = none;
    Engine_AudioPlayCue(220);
    for (i = 0; i != 30; i++) {
        *(s32 *)(rec + 12) += 0x10000;
        Engine_EventWait(1);
    }
    rec[85] = 5;
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Event_SayThenWait(1, 10);
    Engine_ActorShowEmote(ACTOR_ALEX, 0x101, 60);
    Engine_ActorFaceDirection(ACTOR_ALEX, 0x5000, 10);
    Event_SayThenWait(1, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_ALEX, 1);
    Event_SayThenWait(14, 20);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x103, 20);
    Event_SayThenWait(1, 30);
    Engine_ActorShowEmote(ACTOR_ALEX, 0x105, 80);
    Engine_ActorFaceDirection(ACTOR_ALEX, 0xd000, 40);
    Engine_ActorFaceDirection(ACTOR_SATUROS, 0x5000, 10);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x1dd0000, -1, 0x14e0000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
    Event_SayThenWait(10, 10);
    Engine_EventOpenMessage(ACTOR_MENARDI, 0);
    Engine_CameraSetSpeed(0x66666, 0xcccc);
    Engine_CameraMoveTo(0x1760000, -1, 0x1d60000, 1);
    Engine_CameraWaitForMove();
    Engine_ActorFaceDirection(ACTOR_ALEX, 0x5000, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xe000, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    if (Engine_EventChooseYesNo(1, 0) != 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(14, 4);
        Engine_EventSetMessage((s32)MsgSoruDontWantAnything);
        Engine_EventOpenMessage(ACTOR_ALEX, 0);
        if (Engine_EventChooseYesNo(1, 0) == 0) {
            do {
                Engine_EventWait(20);
                Engine_ActorSetAnimationAndWait(14, 4);
                Engine_EventWait(10);
                Engine_EventSetMessage((s32)MsgSoruDoubtHowFeel);
                Engine_EventOpenMessage(ACTOR_ALEX, 0);
            } while (Engine_EventChooseYesNo(1, 0) == 0);
        }
    }
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgSoruThankCooperation);
    Event_SayThenWait(14, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_ALEX, 3);
    Engine_EventWait(10);
    Event_SayThenWait(14, 30);
    rec[85] = 0;
    Engine_ActorSetSpeed(ACTOR_ALEX, 0x26666, 0x13333);
    Call4(Object_SetMoveTarget, (s32)rec, 0x1cc0000, 0, 0x1680000);
    Engine_ActorWaitForMove(ACTOR_ALEX);
    Engine_ActorSetChildValue(ACTOR_ALEX, 0);
    record = Object_GetById(ACTOR_ALEX);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_EventWait(30);
    Engine_CameraFollowActor(ACTOR_GERALD, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorShowEmote(ACTOR_GERALD, 0x103, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 3);
    Engine_EventWait(20);
    rec = Object_GetById(ACTOR_GERALD);
    SetFlagBits(&rec[90], 1);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    Engine_AudioPlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Engine_ActorSetAnimation(ACTOR_GERALD, 7);
    Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x156, 0x1d6);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_EventWait(30);
    Engine_AudioPlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Engine_ActorSetAnimation(ACTOR_GERALD, 7);
    Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x138, 0x1d6);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_EventWait(30);
    Engine_AudioPlayCue(153);
    *(s32 *)(rec + 40) = 0x60000;
    Engine_ActorSetAnimation(ACTOR_GERALD, 7);
    Engine_ActorMoveToAndWait(ACTOR_GERALD, 0x116, 0x1e0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_EventWait(30);
    Engine_CameraSetSpeed(0x8000, 0x1000);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    {
        s32 slot = Object_GetById(ACTOR_PARTY_LEADER);

        if (slot != 0) {
            Engine_ActorSetDestination(ACTOR_GERALD, *(s16 *)(slot + 10), *(s16 *)(slot + 18));
        }
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_ActorSetPosition(ACTOR_GERALD, 0, 0);
    PartyInventory_Discard(220);
    PartyInventory_Discard(221);
    PartyInventory_Discard(223);
}

extern const s32 SoruStar_AngleScript[];

/* An actor that turns with the leader, as this scene sets it up. */
struct StarFollower {
    u8 unknown_00[12];
    s32 y;
    u8 unknown_10[8];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[0x35];
    u8 motion_flags;
    u8 unknown_56[4];
    u8 flags;
    u8 unknown_5b[13];
    struct FieldActor *leader;
};

/* Stands the party and the two thieves around the star's pedestal, each
 * turning with the leader. */
void SoruStar_LineUpFollowers(void)
{
    struct FieldActor *leader;
    struct StarFollower *actor;
    struct StarFollower *menardi;

    leader = Object_GetById(0);
    Engine_EventBegin();
    Engine_ActorEnableActionCallback(5, (const u8 *)1);
    Engine_ActorEnableActionCallback(9, (const u8 *)1);
    Engine_ActorEnableActionCallback(11, (const u8 *)1);
    Engine_ActorEnableActionCallback(10, (const u8 *)1);
    Engine_ActorEnableActionCallback(14, (const u8 *)1);
    Engine_ActorEnableActionCallback(13, (const u8 *)1);
    Call3(Engine_ActorSetPosition, 5, 0x1db0000, 0x14c0000);
    Call3(Engine_ActorSetPosition, 9, 0x1eb0000, 0x14c0000);
    Call3(Engine_ActorSetPosition, 11, 0x1cb0000, 0x15c0000);
    Call3(Engine_ActorSetPosition, 10, 0x1fb0000, 0x15c0000);
    Call3(Engine_ActorSetPosition, 14, 0x1cc0000, 0x1680000);
    Call3(Engine_ActorSetPosition, 13, 0x1d70000, 0x1320000);
    actor = (struct StarFollower *)Object_GetById(5);
    actor->leader = leader;
    actor->flags |= 1;
    Engine_ObjectSetScript((struct FieldActor *)actor, SoruStar_AngleScript);
    actor = (struct StarFollower *)Object_GetById(9);
    actor->leader = leader;
    actor->flags |= 1;
    Engine_ObjectSetScript((struct FieldActor *)actor, SoruStar_AngleScript);
    actor = (struct StarFollower *)Object_GetById(11);
    actor->leader = leader;
    actor->flags |= 1;
    Engine_ObjectSetScript((struct FieldActor *)actor, SoruStar_AngleScript);
    actor = (struct StarFollower *)Object_GetById(10);
    actor->leader = leader;
    actor->flags |= 1;
    Engine_ObjectSetScript((struct FieldActor *)actor, SoruStar_AngleScript);
    menardi = (struct StarFollower *)Object_GetById(14);
    menardi->leader = leader;
    menardi->flags |= 1;
    menardi->scale_x = 0x10000;
    menardi->scale_y = 0x10000;
    menardi->motion_flags = Object_GetById(11)->motion_flags;
    menardi->y = 0;
    Engine_ObjectSetScript((struct FieldActor *)menardi, SoruStar_AngleScript);
    actor = (struct StarFollower *)Object_GetById(13);
    actor->leader = leader;
    actor->flags |= 1;
    Engine_ObjectSetScript((struct FieldActor *)actor, SoruStar_AngleScript);
    Engine_EventEnd();
}
