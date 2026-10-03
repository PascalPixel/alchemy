#include "HEYA.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"

extern u8 MsgRariberoTakenHostageFaran[];
extern u8 MsgRariberoWentOceanLook[];
void Shop_Run(s32, s32);

extern u8 MsgRariberoWantedSendShips[];
extern u8 MsgRariberoWarriorRightPlease[];
s32 Engine_GameFlagIsSet(s32);
void Engine_EventSetMessage(s32);
void Engine_EventShowMessage(s32, s32);

extern u8 MsgRariberoGuessPeopleAngara[];
extern u8 MsgRariberoImpossibleGoOcean[];

extern u8 MsgRariberoBabiIsntBuilding[];
extern u8 MsgRariberoBabiLighthouseStruck[];
void Shop_ConfirmAct(s32);

extern u8 MsgRariberoDidntKnowTime[];
extern u8 MsgRariberoWantStayLet[];
void Inn_CheckIn(s32, s32);

extern u8 MsgRariberoImSorry[];
extern const u8 gRariberoPoseAction[];
void Engine_ResetSceneEffectCounter(void);
void Engine_ActorSetAnimationAndWait(s32 actor, s32 mode);
void Engine_ActorRunRepeatedMotion(s32 actor, s32 mode);
void Engine_ActorWalkByAndWait(s32 actor, s32 x, s32 z);
void Engine_ActorFaceDirection(s32 actor, s32 angle, s32 value);

extern u8 MsgRariberoPleaseWaitForMeOutside[];

extern u8 MsgRariberoDoNotWorryAboutSheba[];
extern u8 MsgRariberoHowDidSearchForSheba[];

extern u8 MsgRariberoTellOthers[];

enum {
    ENTRANCE_SANCTUM_RETURN = 12,
    ENTRANCE_HOUSE_AFTER_REPORT = 21,
    ENTRANCE_FROM_AERIE = 90,
    ENTRANCE_HOUSE_REPORT = 99
};

enum {
    ACTOR_HOUSE_REPORT_SECOND = 11,
    ACTOR_HOUSE_REPORT_FIRST = 12,
    ACTOR_HOUSE_RESIDENT = 13,
    ACTOR_SANCTUM_SECOND = 18,
    ACTOR_SANCTUM_THIRD = 19,
    ACTOR_SANCTUM_FIRST = 20
};

enum {
    FLAG_AERIE_EVENTS_DONE = 0x9a7
};

s32 SceneActor_SetActor14Pose258(void)
{
    Engine_ActorSetAttachedEffect(14, 258);
    return 0;
}

u8 *RariberoHeya_GetEntrances(void)
{
    return gRariberoEntrances;
}

/* The sanctum has regions of its own; the house takes the town's. */
u8 *RariberoHeya_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_RariberoHeya1) {
        return gRariberoSanctumRegions;
    }
    return gRariberoRegions;
}

u8 *RariberoHeya_GetExits(void)
{
    return gRariberoExits;
}

/* The house places its actors anew once the aerie's events are done. */
u8 *RariberoHeya_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_RariberoHeya2) {
        if (Engine_GameFlagIsSet(0x9A7) != 0) {
            return gRariberoHeyaPlacements9a7;
        }
        return gRariberoHeyaPlacements;
    }
    return gRariberoPlacements;
}

/* The Lalivero shopkeeper: spoken to across the counter she opens shop 32;
 * otherwise she talks about the kidnapping, or about the rough sea once flag
 * 0x9a7 is set. The kidnapping question is loaded once and its two answers
 * are the lines after it. */
void RariberoHeya_RunItemShop(s32 keeper)
{
    struct FieldActor *leader = Object_GetById(0);

    /* FAKEMATCH: the halfword cast of the masked facing keeps the
     * reference's compare. */
    if ((u16)((leader->facing + 0x2000) & 0xc000) == 0xc000) {
        Shop_Run(32, keeper);
    } else if (Engine_GameFlagIsSet(0x9a7)) {
        Engine_EventSetMessage((s32)MsgRariberoWentOceanLook);
        Engine_EventShowMessage(keeper, 0);
    } else {
        s32 message = (s32)MsgRariberoTakenHostageFaran;

        Engine_EventSetMessage(message);
        Engine_EventOpenMessage(keeper, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(10);
            Engine_EventSetMessage(message + 1);
        } else {
            Engine_EventSetMessage(message + 2);
        }
        Engine_EventShowMessage(keeper, 0);
    }
}

/*
 * Raribero house: a face-toward branch. If the party faces the door (north)
 * the door line plays; otherwise a flag picks one of the two default lines.
 */
void Dialogue_HandleFacingBranch(s32 no)
{
    u16 party_facing = (((u16 *)Object_GetById(0))[3] + 0x2000) & ~0x3fff;
    if (party_facing == 0xc000) {
        Shop_Run(33, no);
    } else if (Engine_GameFlagIsSet(0x9a7)) {
        Engine_EventSetMessage((s32)MsgRariberoWantedSendShips);
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRariberoWarriorRightPlease);
        Engine_EventShowMessage(no, 0);
    }
}

/*
 * Raribero house: choose which dialogue branch to play. If the party faces
 * the door (north) the alternate line for this door runs; otherwise a flag
 * decides between the two default lines.
 */
void Dialogue_HandleAlternateFacingBranch(s32 no)
{
    u16 party_facing = (((u16 *)Object_GetById(0))[3] + 0x2000) & ~0x3fff;
    if (party_facing == 0xc000) {
        Shop_Run(34, no);
    } else if (Engine_GameFlagIsSet(0x9a7)) {
        Engine_EventSetMessage((s32)MsgRariberoImpossibleGoOcean);
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRariberoGuessPeopleAngara);
        Engine_EventShowMessage(no, 0);
    }
}

/*
 * Raribero house: a face-toward action. If the party faces the door (north)
 * the door line plays; otherwise a flag picks one of the two default lines.
 */
void Dialogue_HandleFacingAction(s32 no)
{
    u16 party_facing = (((u16 *)Object_GetById(0))[3] + 0x2000) & ~0x3fff;
    if (party_facing == 0xc000) {
        Shop_ConfirmAct(no);
    } else if (Engine_GameFlagIsSet(0x9a7)) {
        Engine_EventSetMessage((s32)MsgRariberoBabiLighthouseStruck);
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRariberoBabiIsntBuilding);
        Engine_EventShowMessage(no, 0);
    }
}

/*
 * Raribero house: a face-toward cue branch. If the party faces the door
 * (north) the cue line plays; otherwise a flag picks one of the two default
 * lines.
 */
void Dialogue_HandleFacingCueBranch(s32 no)
{
    u16 party_facing = (((u16 *)Object_GetById(0))[3] + 0x2000) & ~0x3fff;
    if (party_facing == 0xc000) {
        Inn_CheckIn(11, no);
    } else if (Engine_GameFlagIsSet(0x9a7)) {
        Engine_EventSetMessage((s32)MsgRariberoDidntKnowTime);
        Engine_EventShowMessage(no, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRariberoWantStayLet);
        Engine_EventShowMessage(no, 0);
    }
}

void RariberoScene_PlayPoseSequence(void)
{
    s16 facing;

    facing = (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
    Engine_GameFlagSet(0x300);
    Engine_EventBegin();
    Engine_ResetSceneEffectCounter();
    Engine_EventSetMessage((s32)MsgRariberoImSorry);
    Engine_EventWait(50);
    Engine_ActorShowEmote(14, 0x102, 50);
    Engine_ActorFaceActor(14, ACTOR_PARTY_LEADER, 20);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(14, 2);
    Engine_EventWait(30);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(14, 3);
    Engine_EventWait(20);
    Engine_EventShowMessage(14, 0);
    if ((u16)facing == 0x8000) {
        Engine_ActorWalkByAndWait(0, 0, 16);
        Engine_ActorFaceDirection(0, 0xc000, 0);
        Engine_EventWait(20);
    }
    Engine_ActorEnableActionCallback(14, gRariberoPoseAction);
    Engine_EventEnd();
}

void FieldScene_RunSequenceA(void)
{

    Engine_GameFlagSet(0x9BC);
    Engine_EventBegin();
    Engine_ResetSceneEffectCounter();
    Engine_EventWait(0xA);
    Engine_CameraMoveTo(0x780000, -1, 0x600000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(0x1E);
    Engine_EventSetMessage((s32)MsgRariberoPleaseWaitForMeOutside);
    Engine_EventShowMessage(0xC, 0);
    Engine_EventWait(0xA);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 0xC, 0);
    Engine_EventWait(0x1E);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(0x1E);
    Engine_EventEnd();
}

void FieldScene_RunThreeCallSequence(void)
{
    void Event_ShowMessage(s32, s32);

    Engine_GameFlagSet(0x9BC);
    Engine_EventSetMessage((s32)MsgRariberoPleaseWaitForMeOutside);
    Engine_EventShowMessage(0xC, 0);
}

void SceneState_ForwardWord16cAndApply7b(void)
{
    u8 *work = (u8 *)gEventWork;
    s16 *p = (s16 *)(work + 0x16C);

    Engine_EventRequestExit(*p);
    Engine_AudioPlayCue(0x7B);
}

/* Both interiors answer differently once the aerie's events are done. */
u8 *RariberoHeya_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_RariberoHeya2) {
        if (Engine_GameFlagIsSet(0x9a7) != 0) {
            return gRariberoHeyaEvents9a7;
        }
        return gRariberoHeyaEvents;
    }
    if (Engine_GameFlagIsSet(0x9a7) != 0) {
        return gRariberoEvents9a7;
    }
    return gRariberoEvents;
}

void FieldScene_RunPrimaryScript(void)
{
    void Engine_EventBegin();

    Engine_EventBegin();
    Owner_RefreshActiveRatios(1);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 6291456, 12058624);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
    Engine_ActorFaceDirection(11, 0, 0);
    Call3(Engine_ActorFaceDirection, 12, 32768, 0);
    Engine_EventSetMessage((s32)MsgRariberoHowDidSearchForSheba);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(10);
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 65536, 32768);
    Engine_ActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    Engine_ActorWalkToAndWait(0, 104, 136);
    Engine_EventWait(10);
    Call4(Motion_LaunchFromFocusedObject, 1, -16, 16, 49152);
    Call4(Motion_LaunchFromFocusedObject, 3, 0, 24, 49152);
    Value4(Motion_LaunchFromFocusedObject, 2, 16, 16, 49152);
    Engine_ActorWaitForMove(1);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(11, 16384, 0);
    Engine_ActorFaceDirection(12, 16384, 0);
    Engine_EventWait(30);
    Engine_ActorShowEmote(11, 261, 70);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(20);
    Engine_ActorShowEmote(12, 258, 40);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(12, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Engine_ActorShowEmote(11, 261, 50);
    Engine_EventShowMessage(11, 0);
    if (Engine_GameFlagIsSet(2495) == 0) {
        FieldScene_RunSecondaryScript();
    } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 50);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 261, 60);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(1, 49152, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(20);
    Engine_ActorShowEmote(12, 257, 40);
    Engine_EventOpenMessage(12, 0);
    Engine_EventChooseYesNo(0, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(12, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
    Engine_EventOpenMessage(8194, 0);
    }
    Engine_EventSetMessage((s32)MsgRariberoDoNotWorryAboutSheba);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 16384, 0);
    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 20);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 49152, 0);
    Engine_ActorFaceDirection(2, 49152, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(11, 0);
    (*(u16 *)(*(u8 **)&gEventWork + 0x1d8))++;
    } else {
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 49152, 0);
    Engine_ActorFaceDirection(2, 49152, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_EventWait(20);
    (*(u16 *)(*(u8 **)&gEventWork + 0x1d8))++;
    Engine_EventShowMessage(11, 0);
    }
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(3, 2, 0);
    Engine_EventWait(40);
    Engine_EventWait(10);
    Engine_ActorShowEmote(11, 258, 40);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 49152, 0);
    Engine_ActorFaceDirection(ACTOR_IVAN, 49152, 0);
    Call3(Engine_ActorFaceDirection, 3, 49152, 0);
    Engine_EventWait(50);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorShowEmote(ACTOR_MIA, 256, 40);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(11, 0, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(12, 2);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 12, 32768, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 11, 16384, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorShowEmote(12, 258, 50);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(20);
    Engine_ActorShowEmote(11, 257, 65);
    Engine_ActorFaceDirection(11, 0, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(20);
    Engine_ActorShowEmote(12, 256, 40);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(20);
    Call3(Engine_ActorFaceDirection, 12, 16384, 0);
    Engine_EventWait(40);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 11, 16384, 0);
    Engine_EventWait(50);
    Engine_ActorShowEmote(11, 262, 60);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_MIA, 2);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(11, 0, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(12, 2);
    Engine_EventWait(30);
    Call3(Engine_ActorFaceDirection, 12, 32768, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(10);
    Engine_ActorShowEmote(11, 257, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 1, 57344, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(1, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 12, 16384, 0);
    Engine_EventWait(30);
    Engine_ActorShowEmote(12, 261, 60);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 11, 16384, 0);
    Engine_EventWait(40);
    Engine_ActorShowEmote(11, 258, 40);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(2, 32768, 0);
    Engine_EventWait(30);
    Engine_ActorShowEmote(ACTOR_IVAN, 263, 60);
    Engine_ActorFaceDirection(2, 49152, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(2, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_MIA, 2);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_MIA, 3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 16384, 0);
    Engine_ActorFaceDirection(ACTOR_GERALD, 57344, 0);
    Engine_ActorFaceDirection(2, 40960, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Engine_ActorSetSpeed(ACTOR_GERALD, 78643, 39321);
    Engine_ActorSetSpeed(ACTOR_MIA, 78643, 39321);
    Engine_ActorSetSpeed(ACTOR_IVAN, 78643, 39321);
    Engine_ActorSetAnimation(1, 2);
    {
        u8 *rec = Object_GetById(ACTOR_PARTY_LEADER);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Engine_ActorSetDestination(1, y, x);
        }
    }
    Engine_ActorWaitForMove(1);
    Engine_ActorSetPosition(ACTOR_GERALD, 0, 0);
    Engine_ActorSetAnimation(3, 2);
    {
        u8 *rec = Object_GetById(ACTOR_PARTY_LEADER);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Engine_ActorSetDestination(3, y, x);
        }
    }
    Engine_ActorWaitForMove(3);
    Engine_ActorSetPosition(ACTOR_MIA, 0, 0);
    Engine_ActorSetAnimation(2, 2);
    {
        u8 *rec = Object_GetById(ACTOR_PARTY_LEADER);
        if (rec != 0) {
            s16 y = *(s16 *)(rec + 10);
            s16 x = *(s16 *)(rec + 18);
            Engine_ActorSetDestination(2, y, x);
        }
    }
    Engine_ActorWaitForMove(2);
    Engine_ActorSetPosition(2, 0, 0);
    Engine_EventWait(10);
    Engine_EventEnd();
}

/*
 * "But tell me, what of the others?" Actor 11 asks, and the party and actor
 * 12 answer in turn, each beat an action on one actor and then a wait; the
 * last line stays open for the question that follows.
 */
void FieldScene_RunSecondaryScript(void)
{
    Engine_EventSetMessage((s32)MsgRariberoTellOthers);
    Engine_EventWait(20);

    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(11, 0);
    Engine_EventWait(10);

    Engine_ActorFaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 50);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_GERALD, 0xc000, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);

    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);

    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);

    Engine_ActorRunRepeatedMotion(12, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(12, 0);
    Engine_EventWait(20);

    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Engine_EventWait(25);

    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(30);
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(30);

    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);

    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);

    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);

    Engine_ActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
    Engine_EventOpenMessage(0x2002, 0);
}

/* The action table actor 14 takes while flag 0x300 is set. */

/*
 * Opens both Lalivero interiors. Arriving from the aerie by entrance 90 sets
 * flag 0x9a7. In the sanctum three actors take collision flag 4 and sprite
 * priority 2, and returning by entrance 12 records the sanctum as the scene
 * to come back to. In the house the resident is set up the same way, actor
 * 14 takes its action table while flag 0x300 is set, and arriving by
 * entrance 99 plays the report before the entrance becomes 21.
 */
s32 Scene_Initialize(void)
{
    s32 scene;
    s16 entrance;
    struct FieldActor *actor;

    if (gGameState.entrance == ENTRANCE_FROM_AERIE) {
        Engine_GameFlagSet(FLAG_AERIE_EVENTS_DONE);
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    scene = gGameState.scene;
    if (scene == (s32)&SceneId_RariberoHeya1) {
        actor = Object_GetById(ACTOR_SANCTUM_FIRST);
        actor->priority_flags = 0;
        actor->collision_flags |= 4;
        actor->sprite->priority = 2;
        actor = Object_GetById(ACTOR_SANCTUM_SECOND);
        actor->priority_flags = 0;
        actor->collision_flags |= 4;
        actor->sprite->priority = 2;
        actor = Object_GetById(ACTOR_SANCTUM_THIRD);
        actor->collision_flags |= 4;
        actor->priority_flags = 0;
        actor->sprite->priority = 2;
        Engine_ActorSetAnimation(15, 6);
        entrance = gGameState.entrance;
        if (entrance == ENTRANCE_SANCTUM_RETURN) {
            gGameState.saved_scene = scene;
            gGameState.saved_entrance = entrance;
        }
    }
    if (gGameState.scene == (s32)&SceneId_RariberoHeya2) {
        actor = Object_GetById(ACTOR_HOUSE_RESIDENT);
        actor->collision_flags |= 4;
        actor->priority_flags = 0;
        actor->sprite->priority = 2;
        if (Engine_GameFlagIsSet(0x300) != 0) {
            Engine_ActorEnableActionCallback(14, gRariberoPoseAction);
        }
        if (gGameState.entrance == ENTRANCE_HOUSE_REPORT) {
            FieldScene_RunPrimaryScript();
            Engine_ActorSetActionCallback(Object_GetById(ACTOR_HOUSE_REPORT_FIRST), 6);
            Engine_ActorSetActionCallback(Object_GetById(ACTOR_HOUSE_REPORT_SECOND), 6);
            gGameState.entrance = ENTRANCE_HOUSE_AFTER_REPORT;
        }
    }
    return 0;
}
