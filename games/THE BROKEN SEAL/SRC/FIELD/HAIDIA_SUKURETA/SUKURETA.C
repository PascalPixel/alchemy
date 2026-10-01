#include "SUKURETA.H"
#include "CALL.H"
#include "TYPES.H"

extern u8 MsgHaidiaFineIfTheyDontSee[];
extern u8 MsgHaidiaGeraldIllTakeOverIf[];
extern u8 MsgHaidiaGeraldYouCanHandleThe[];
extern u8 MsgHaidiaHearAwfulGrowls[];
extern u8 MsgHaidiaIllClimbTheFenceSomeday[];
extern u8 MsgHaidiaJasmineOurSecret[];
extern u8 MsgHaidiaJasmineTheyMightBeThieves[];
extern u8 MsgHaidiaSukuretaIWaitedYearsFor[];
extern u8 MsgHaidiaSukuretaOhRobin[];
extern u8 MsgHaidiaSukuretaWeOnlyCheckMt[];
extern u8 MsgHaidiaSukuretaWeOnlyCheckThe[];
extern u8 MsgHaidiaYouCannotEnterMtAleph[];

extern u8 MsgHaidiaSukuretaOurBestBet[];

extern u8 Sukureta_StrangerActions[];
void Object_SetTargetAndCallback();
void Audio_PlayCueFromEventWork(void);
extern u8 MsgHaidiaSaturosGo[];
extern u8 MsgHaidiaTheyKnowLittleOfThe[];
extern u8 MsgHaidiaYoureTheOnesSneakingAround[];

extern u8 MsgHaidiaWho[];
s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_ActorSetSpeed();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_ActorSetPosition();
void Engine_ActorWalkTo();
void Engine_ActorWalkToAndWait();
void Engine_ActorSetAnimation();
void Engine_ActorFaceDirection();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_ActorRunRepeatedMotion();
void Engine_EventWait();
void Engine_EventShowMessageAndWait();
void Engine_ActorJump();
void Engine_ActorShowEmote();
void Engine_ActorSetAnimationAndWait();
void Engine_CameraFollowActor();
void Engine_ActorSetDestination();
void Engine_ActorWaitForMove();
void Engine_EventEnd();

extern u8 Sukureta_Actor11Actions[];
void Engine_GameFlagClear();
s32 Effect_SoundAndFlash();
void BattleFx_StartTwelveFrameBlend();
void Engine_ActorEnableActionCallback();
void Engine_MapCopyCellAttributes();
void Scene_LeaveForMtAleph();
struct GameState;
extern struct GameState gGameState;

extern u8 MsgHaidiaRockslideDestroyedFence[];
extern u8 MsgHaidiaSaveYourselves[];
extern u8 Sukureta_Actor11RockslideActions[];
void Engine_ActorStop();
s32 Engine_EventOpenMessage();
s32 Engine_EventChooseYesNo();
void Engine_ActorFaceActor();
void Object_SetActionCallbackAndRefreshById();

extern u8 MsgHaidiaMemoriesOfThisCottage[];

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 ang;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        ang = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(ang - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

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

/* Picks one of three tables by story progress, testing flags 0x87a then
 * 0x834. */
u8 *SceneData_SelectEffectTable(void)
{
    if (GameFlag_IsSet(0x87A) != 0) {
        return Placement_Effects87a;
    }
    if (GameFlag_IsSet(0x834) != 0) {
        return Placement_Effects834;
    }
    return Placement_Effects;
}

void Scene_RunActorTwelveDialogue(void)
{
    s32 base;

    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Engine_EventSetMessage((s32)MsgHaidiaIllClimbTheFenceSomeday);
        Event_ShowMessage(12, 0);
    } else {
        base = (s32)MsgHaidiaHearAwfulGrowls;
        Engine_EventSetMessage(base);
        Actor_FaceActor(12, ACTOR_PARTY_LEADER, 10);
        Engine_ActorRunRepeatedMotion(12, 2);
        Engine_EventWait(6);
        Event_OpenMessage(12, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventSetMessage(base + 1);
        } else {
            Engine_EventSetMessage(base + 2);
        }
        Engine_ActorStartRepeatedMotion(12, 3);
        Event_ShowMessage(12, 0);
        Actor_FaceDirection(12, 49152, 10);
    }
    Engine_EventEnd();
}

void Scene_PlanSanctumVisit(void)
{
    u8 *record;
    s32 x, y;
    s32 sneak;

    if (GameFlag_IsSet(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
        Engine_EventBegin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        Engine_EventSetMessage((s32)MsgHaidiaSukuretaOhRobin);
        Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 0x108);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_SUKURETA, 20);
        Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        record = (u8 *)Object_GetById(0);
        x = *(s16 *)(record + 10);
        y = *(s16 *)(record + 18);
        Actor_SetPosition(ACTOR_JASMINE, x << 16, y << 16);
        Actor_SetPosition(ACTOR_GERALD, x << 16, y << 16);
        Actor_SetSpeed(ACTOR_JASMINE, 0x8000, 0x4000);
        Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
        Actor_WalkTo(ACTOR_JASMINE, 248, 0x108);
        Actor_WalkToAndWait(ACTOR_GERALD, 216, 0x108);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
        Engine_ActorSetAnimation(ACTOR_JASMINE, 1);
        Engine_ActorSetAnimation(ACTOR_GERALD, 1);
        Engine_EventWait(4);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
        Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
        Engine_EventWait(10);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 20);
        Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
        Engine_EventWait(10);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        Actor_FaceDirection(ACTOR_GERALD, 0x3000, 40);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 10);
        Engine_ActorSetAnimation(ACTOR_SUKURETA, 3);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 8);
        Actor_FaceDirection(ACTOR_JASMINE, 0x3000, 20);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
        Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
        Engine_EventWait(10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
        Actor_ShowEmote(ACTOR_JASMINE, 0x101, 60);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 20);
        Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        Actor_ShowEmote(ACTOR_SUKURETA, 0x102, 60);
        Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        Actor_FaceDirection(ACTOR_GERALD, 0, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 20);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 60);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 0);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 20);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 40);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 40);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 20);
        Event_OpenMessage(ACTOR_SUKURETA, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventSetMessage((s32)MsgHaidiaYouCannotEnterMtAleph);
        } else {
            Engine_EventSetMessage((s32)MsgHaidiaSukuretaIWaitedYearsFor);
        }
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
        Engine_EventWait(10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        Engine_EventSetMessage((s32)MsgHaidiaJasmineTheyMightBeThieves);
        Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 10);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 6);
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 30);
        Engine_ActorJump(ACTOR_GERALD, 4, 30);
        Actor_FaceDirection(ACTOR_GERALD, 0, 10);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 6);
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 10);
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 0);
        Actor_FaceActor(ACTOR_SUKURETA, ACTOR_GERALD, 10);
        Actor_FaceActor(ACTOR_SUKURETA, ACTOR_JASMINE, 10);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimation(ACTOR_GERALD, 3);
        Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
        Engine_EventWait(10);
        Engine_ActorSetAnimation(ACTOR_JASMINE, 1);
        Engine_ActorSetAnimation(ACTOR_GERALD, 1);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 16);
        Actor_SetAttachedEffect(ACTOR_SUKURETA, 0x102);
        Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 3);
        Engine_EventWait(10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
        Actor_ShowEmote(ACTOR_JASMINE, 0x100, 40);
        Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
        Engine_EventWait(10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
        Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 1);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 10);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 6);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
        Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
        Engine_EventWait(6);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 0);
        Engine_ActorJump(ACTOR_GERALD, 2, 0);
        Engine_ActorJump(ACTOR_JASMINE, 2, 10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 6);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x5000, 10);
        Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
        Engine_EventWait(16);
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JASMINE, 40);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
        Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 6);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x105, 0);
        Actor_ShowEmote(ACTOR_JASMINE, 0x105, 80);
        Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 4);
        Event_OpenMessage(ACTOR_SUKURETA, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventSetMessage((s32)MsgHaidiaSukuretaWeOnlyCheckThe);
        } else {
            Engine_EventSetMessage((s32)MsgHaidiaSukuretaWeOnlyCheckMt);
        }
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 20);
        sneak = (s32)MsgHaidiaFineIfTheyDontSee;
        Engine_EventSetMessage(sneak);
        Actor_FaceDirection(ACTOR_GERALD, 0, 10);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
        Event_OpenMessage(ACTOR_GERALD, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventSetMessage((sneak + 1));
        } else {
            Engine_EventSetMessage((sneak + 2));
        }
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 6);
        Engine_EventSetMessage((s32)MsgHaidiaJasmineOurSecret);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 10);
        Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 1);
        Event_OpenMessage(ACTOR_JASMINE, 0);
        Engine_EventWait(4);
        if (Engine_EventChooseYesNo(0, 0) == 1) {
            Engine_ActorJump(ACTOR_JASMINE, 2, 20);
            Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 10);
        } else {
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
            Engine_ActorSetAnimation(ACTOR_GERALD, 3);
            Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
            Engine_EventWait(8);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
            bump_step(1);
        }
        Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
        Engine_EventWait(10);
        Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 10);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimation(ACTOR_GERALD, 3);
        Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
        Engine_EventWait(10);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
        Event_OpenMessage(ACTOR_SUKURETA, 0);
        Engine_EventWait(4);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventSetMessage((s32)MsgHaidiaGeraldYouCanHandleThe);
        } else {
            Engine_EventSetMessage((s32)MsgHaidiaGeraldIllTakeOverIf);
        }
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
        Actor_FaceDirection(ACTOR_GERALD, 0, 10);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 6);
        Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 10);
        Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 4);
        Event_ShowMessageAndWait(ACTOR_JASMINE, 0, 6);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
        Actor_FaceDirection(ACTOR_GERALD, 0x8000, 10);
        Actor_ShowEmote(ACTOR_GERALD, 0x103, 30);
        Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
        Actor_ShowEmote(ACTOR_SUKURETA, 0x100, 40);
        Engine_ActorJump(ACTOR_SUKURETA, 4, 40);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
        Actor_FaceDirection(ACTOR_JASMINE, 0xb000, 20);
        Actor_FaceDirection(ACTOR_SUKURETA, 0xb000, 10);
        Audio_PlayCue(158);
        Map_AnimateCells(Sukureta_GateCells, 43, 8);
        Actor_SetSpeed(ACTOR_SUKURETA, 0x10000, 0x8000);
        Actor_WalkToAndWait(ACTOR_SUKURETA, 232, 218);
        Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
        Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
        Actor_ShowEmote(ACTOR_JASMINE, 0x101, 60);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
        Engine_EventCloseScreen();
        Engine_EventWaitForScreen();
        Engine_EventRequestExit(13);
        Engine_EventEnd();
    }
}

void Scene_LeaveForMtAleph(void)
{
    s32 record;
    s32 facing;

    Engine_EventBegin();
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Actor_SetPosition(ACTOR_GERALD, 0xd80000, 0x1080000);
    Actor_SetPosition(ACTOR_JASMINE, 0xf80000, 0x1080000);
    record = (s32)Object_GetById(1);
    facing = 0xc000;
    *(u16 *)(record + 6) = facing;
    record = (s32)Object_GetById(5);
    *(u16 *)(record + 6) = facing;
    Map_AnimateCells(Sukureta_GateCells, 43, 8);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Actor_SetSpeed(ACTOR_SUKURETA, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_SUKURETA, 0xe60000, 0xdc0000);
    Actor_WalkToAndWait(ACTOR_SUKURETA, 230, 232);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SUKURETA, 3);
    Engine_EventSetMessage((s32)MsgHaidiaSukuretaOurBestBet);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_SUKURETA, 2);
    Actor_FaceDirection(ACTOR_SUKURETA, 0x3000, 10);
    Event_ShowMessageAndWait(ACTOR_SUKURETA, 0, 10);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x8000, 10);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_JASMINE, 0xcccc, 0x6666);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    record = ((s32 (*)())Object_GetById)(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(ACTOR_JASMINE, 2);
    record = ((s32 (*)())Object_GetById)(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 2);
    record = ((s32 (*)())Object_GetById)(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_SUKURETA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Engine_ActorWaitForMove(ACTOR_SUKURETA);
    Actor_SetPosition(ACTOR_SUKURETA, 0, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 1);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 1);
    Engine_ActorSetAnimation(ACTOR_SUKURETA, 1);
    Actor_SetPosition(ACTOR_SATUROS, 0, 0);
    Actor_SetPosition(ACTOR_MENARDI, 0, 0);
    GameFlag_Set(FLAG_SANCTUM_VISIT_PLANNED);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    ColorBuffer_ApplySource(0x10000, 0);
    GameFlag_Set(0x242);
    Engine_EventEnd();
}

void FieldScene_SetupWithDescriptorA0ACWhenFlag242Clear(void)
{
    if (GameFlag_IsSet(0x242) == 0) {
        Audio_PlayCue(0x9E);
        Map_AnimateCells(Sukureta_GateCells, 0x2B, 8);
    }
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0xE5, 0xD9);
    Engine_EventRequestExit(3);
}

void Scene_OverhearSaturosAndMenardi(void)
{
    u8 *record;
    s32 x, z;
    s32 evt;
    s32 evt2;

    if (GameFlag_IsSet(FLAG_MET_SATUROS_AND_MENARDI) != 0) {
        return;
    }

    Engine_EventBegin();
    Audio_PlayCue(17);
    GameFlag_Set(FLAG_MET_SATUROS_AND_MENARDI);

    evt = (s32)MsgHaidiaTheyKnowLittleOfThe;
    Engine_EventSetMessage(evt);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);

    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 30);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x188, 0x148);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);

    record = (u8 *)Object_GetById(0);
    x = *(s16 *)(record + 10);
    z = *(s16 *)(record + 18);
    Actor_SetPosition(ACTOR_JASMINE, x << 16, z << 16);
    Actor_SetPosition(ACTOR_GERALD, x << 16, z << 16);

    Actor_SetSpeed(ACTOR_JASMINE, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_WalkTo(ACTOR_JASMINE, 0x178, 0x148);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x198, 0x148);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
    Engine_ActorSetAnimation(ACTOR_JASMINE, 0);
    Engine_ActorSetAnimation(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x101, 30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 0);
    Camera_SetSpeed(0x60000, 0xc000);
    Camera_MoveTo(0xd70000, -1, 0x1590000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Audio_PlayCue(61);

    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
    Engine_ActorSetAnimation(ACTOR_SATUROS, 4);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 20);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 10);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceDirection(ACTOR_MENARDI, 0x4000, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 3);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
    Event_ShowMessageAndWait(ACTOR_MENARDI, 0, 6);
    Actor_ShowEmote(ACTOR_SATUROS, 0x100, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 10);
    Actor_ShowEmote(ACTOR_MENARDI, 0x101, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0, 60);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0, 40);
    Actor_WalkToAndWait(ACTOR_SATUROS, 232, 0x168);
    Actor_FaceDirection(ACTOR_SATUROS, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
    Engine_EventWait(10);
    Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x168);
    Actor_WalkToAndWait(ACTOR_MENARDI, 216, 0x168);
    Actor_WalkTo(ACTOR_MENARDI, 0x178, 0x168);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 0);
    Camera_MoveTo(0x1890000, -1, 0x1530000, 1);
    Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x168);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x178, 0x168);
    Engine_ActorSetAnimation(ACTOR_SATUROS, 0);
    Engine_ActorSetAnimation(ACTOR_MENARDI, 0);
    Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_ShowEmote(ACTOR_GERALD, 258, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 1);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 4);
    Event_OpenMessage(0x100f, 0);

    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventSetMessage(evt + 10);
    } else {
        Engine_EventSetMessage(evt + 11);
    }

    Event_ShowMessageAndWait(0x100f, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);

    evt2 = (s32)MsgHaidiaYoureTheOnesSneakingAround;
    Engine_EventSetMessage(evt2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 20);
    Engine_ActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    Actor_FaceDirection(ACTOR_SATUROS, 0xd000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 20);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 60);
    Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 1);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_MENARDI, 3);
    Event_ShowMessageAndWait(0x100f, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Engine_ActorSetAnimationAndWait(ACTOR_JASMINE, 3);
    Event_ShowMessageAndWait(0x1005, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    Actor_FaceDirection(ACTOR_SATUROS, 0xa000, 20);
    Event_OpenMessage(ACTOR_SATUROS, 0);

    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventSetMessage(evt2 + 5);
    } else {
        Engine_EventSetMessage(evt2 + 6);
    }

    Actor_FaceDirection(ACTOR_JASMINE, 0, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0x8000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Actor_FaceActor(ACTOR_SATUROS, ACTOR_GERALD, 30);
    Actor_FaceActor(ACTOR_SATUROS, ACTOR_JASMINE, 30);
    Actor_ShowEmote(ACTOR_SATUROS, 0x105, 80);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
    Engine_EventSetMessage((s32)MsgHaidiaSaturosGo);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 6);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_JASMINE, 0x100, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_JASMINE, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(ACTOR_JASMINE, 2);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_FaceDirection(ACTOR_JASMINE, 0x4000, 10);
    Event_ShowMessageAndWait(0x1005, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
    Actor_FaceDirection(ACTOR_MENARDI, 0, 10);
    Event_ShowMessageAndWait(0x100f, 0, 10);
    Actor_FaceDirection(ACTOR_SATUROS, 0x8000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_SATUROS, 4);
    Event_ShowMessageAndWait(ACTOR_SATUROS, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_MENARDI, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_SATUROS, 0xb000, 0);
    Actor_FaceDirection(ACTOR_MENARDI, 0xd000, 20);
    Actor_SetSpeed(ACTOR_MENARDI, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_SATUROS, 0x8000, 0x4000);

    record = (u8 *)Object_GetById(14);
    *(record + 90) &= 0xfe;
    record = (u8 *)Object_GetById(15);
    *(record + 90) &= 0xfe;

    Actor_WalkTo(ACTOR_SATUROS, 0x188, 0x178);
    Actor_WalkToAndWait(ACTOR_MENARDI, 0x178, 0x178);
    Engine_EventWait(6);

    record = (u8 *)Object_GetById(14);
    *(record + 90) |= 1;
    record = (u8 *)Object_GetById(15);
    {
        /* FAKEMATCH: a result temporary, not the compound or-assign the
         * first occurrence above uses: the reference merges the byte into
         * the mask's register, which the two-address ORR does only when
         * the result is its own object. */
        u8 merged = (u8)(*(record + 90) | 1);

        *(record + 90) = merged;
    }

    Engine_ActorSetAnimation(ACTOR_SATUROS, 0);
    Engine_ActorSetAnimation(ACTOR_MENARDI, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 1, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
    Audio_PlayCue(17);

    Engine_ActorSetAnimation(ACTOR_GERALD, 2);
    record = (u8 *)Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);

    Engine_ActorSetAnimation(ACTOR_JASMINE, 2);
    record = (u8 *)Object_GetById(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_JASMINE, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Engine_ActorWaitForMove(ACTOR_JASMINE);
    Actor_SetPosition(ACTOR_JASMINE, 0, 0);
    Call3(Object_SetTargetAndCallback, 14, 0x10000, (s32)Sukureta_StrangerActions);
    Call3(Object_SetTargetAndCallback, 15, 0x10000, (s32)Sukureta_StrangerActions);
    Audio_PlayCueFromEventWork();
    Engine_EventEnd();
}

void HaidiaSukureta_RunArrivalScene(void)
{
    u32 i;
    s32 record;

    if (Value1(Engine_GameFlagIsSet, 0x800) != 0) {
    } else {
        Engine_EventBegin();
        Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
        Engine_EventSetMessage((s32)MsgHaidiaWho);
        Engine_EventShowMessage(13, 0);
        record = (s32)Object_GetById(0);
        {
            s32 x = *(s16 *)(record + 10);
            s32 y = *(s16 *)(record + 18);

            Engine_ActorSetPosition(5, x << 16, y << 16);
            Engine_ActorSetPosition(1, x << 16, y << 16);
        }
        Call3(Engine_ActorSetSpeed, 5, 0xcccc, 0x6666);
        Call3(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
        Call3(Engine_ActorWalkTo, 5, 0x128, 0x148);
        Call3(Engine_ActorWalkToAndWait, 1, 0x118, 0x148);
        Engine_ActorSetAnimation(0, 0);
        Engine_ActorSetAnimation(5, 0);
        Engine_ActorSetAnimation(1, 0);
        Call3(Engine_ActorFaceDirection, 5, 0xb000, 0);
        Call3(Engine_ActorFaceDirection, 1, 0xb000, 0);
        Call3(Engine_ActorFaceDirection, 0, 0xb000, 0);
        Engine_CameraMoveTo(0xe80000, -1, 0xf00000, 1);
        Engine_CameraWaitForMove();
        Call3(Engine_ActorFaceDirection, 13, 0x8000, 20);
        Engine_ActorRunRepeatedMotion(13, 2);
        Engine_EventShowMessage(13, 0);
        Call3(Engine_ActorSetSpeed, 13, 0x3333, 0x1999);
        Engine_ActorWalkToAndWait(13, 216, 232);
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(13, 1);
        Engine_EventWait(10);
        Engine_EventShowMessageAndWait(13, 0, 40);
        Engine_ActorJump(13, 2, 10);
        Engine_ActorFaceDirection(13, 0, 10);
        Engine_EventShowMessageAndWait(13, 0, 10);
        Engine_ActorWalkToAndWait(13, 248, 232);
        Engine_EventWait(40);
        Engine_ActorRunRepeatedMotion(13, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(13, 0);
        Call3(Engine_ActorShowEmote, 13, 0x101, 60);
        Engine_ActorRunRepeatedMotion(13, 1);
        Call3(Engine_ActorFaceDirection, 13, 0x8000, 20);
        Engine_EventShowMessageAndWait(13, 0, 2);
        Engine_ActorWalkToAndWait(13, 232, 232);
        Engine_EventWait(2);
        Call3(Engine_ActorFaceDirection, 13, 0x4000, 4);
        Engine_ActorRunRepeatedMotion(13, 2);
        Engine_EventWait(4);
        Engine_EventShowMessageAndWait(13, 0, 2);
        Engine_ActorSetAnimationAndWait(13, 4);
        Engine_EventWait(20);
        Engine_CameraFollowActor(0, 1);
        Engine_CameraWaitForMove();
        Engine_ActorSetAnimation(1, 2);
        record = (s32)Object_GetById(0);
        if (record != 0) {
            Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(1);
        Engine_ActorSetPosition(1, 0, 0);
        Engine_ActorSetAnimation(5, 2);
        record = (s32)Object_GetById(0);
        if (record != 0) {
            Engine_ActorSetDestination(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Engine_ActorWaitForMove(5);
        Engine_ActorSetPosition(5, 0, 0);
        Engine_GameFlagSet(0x800);
        Engine_EventEnd();
    }
}

s32 HaidiaSukureta_RestoreEntryState(void)
{
    u32 i;
    u8 *record;
    s32 v5;
    s32 stranger_actions;
    s16 *room;

    {
        /* FAKEMATCH: indexing through a variable keeps the game state base in a
         * register and adds the entrance offset, as the reference does. */
        s32 k = 225;

        room = (s16 *)&gGameState + k;
    }
    if (*room == 5 || *room == 6) {
        Engine_GameFlagClear(0x12f);
    }
    if (Value1(Engine_GameFlagIsSet, 0x109) != 0) {
        Engine_GameFlagClear(0x242);
    }
    if (Engine_GameFlagIsSet(0x834) != 0) {
        Effect_SoundAndFlash();
        BattleFx_StartTwelveFrameBlend();
        Engine_EventBegin();
        Engine_ActorSetPosition(12, 0, 0);
        Engine_ActorSetPosition(13, 0, 0);
        Engine_ActorSetPosition(14, 0, 0);
        Engine_ActorSetPosition(15, 0, 0);
        Engine_ActorSetPosition(5, 0, 0);
        {
            u8 *record = (u8 *)Object_GetById(8);
            u8 value = *(volatile u8 *)&record[89];
        
            record[89] = (u8)(value | 8);
        }
        Call3(Engine_ActorSetPosition, 11, 0x530000, 0x1090000);
        Engine_ActorWalkToAndWait(11, 83, 0x111);
        Engine_ActorSetAnimation(11, 5);
        record = (u8 *)Object_GetById(11);
        {
            s32 shown = 12;
        
            *(u16 *)((s32)record + 32) = shown;
        }
        Engine_ActorEnableActionCallback(11, (s32)Sukureta_Actor11Actions);
        if (Engine_GameFlagIsSet(0x839) != 0) {
            Engine_ActorSetPosition(11, 0, 0);
        }
        v5 = 21;
        Engine_EventEnd();
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 14, v5);
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 15, v5);
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 23, 19);
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 24, 19);
        v5 = 20;
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 23, 20);
        Engine_MapCopyCellAttributes(9, 24, 1, 1, 24, 20);
        goto L_0200171e;
    }
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x109) == 0) {
        if (*room == 10) {
            Scene_LeaveForMtAleph();
        }
    }
    if (Engine_GameFlagIsSet(0x801) != 0) {
        Engine_ActorSetPosition(13, 0, 0);
        Engine_ActorSetPosition(14, 0, 0);
        Engine_ActorSetPosition(15, 0, 0);
    } else {
        if (Engine_GameFlagIsSet(0x808) != 0) {
            Call3(Engine_ActorSetPosition, 14, 0x1880000, 0x1780000);
            Call3(Engine_ActorSetPosition, 15, 0x1780000, 0x1780000);
            stranger_actions = (s32)Sukureta_StrangerActions;
            Call3(Object_SetTargetAndCallback, 14, 0x10000, stranger_actions);
            Call3(Object_SetTargetAndCallback, 15, 0x10000, stranger_actions);
        }
    }
    if (Engine_GameFlagIsSet(0x87a) != 0) {
        Call3(Engine_ActorSetPosition, 16, 0x840000, 0x1080000);
    }
    Engine_EventEnd();
    L_0200171e:;
    return 0;
}

void HaidiaSukureta_RunActorSequence(void)
{
    s32 record;
    s32 base;

    if (Value1(Engine_GameFlagIsSet, 0x839) == 0) {
        if (Value1(Engine_GameFlagIsSet, 0x82f) != 0) {
            Engine_EventBegin();
            Engine_ActorRunRepeatedMotion(11, 2);
            Engine_EventSetMessage((s32)MsgHaidiaSaveYourselves);
            Engine_EventShowMessage(11, 0);
            Engine_EventEnd();
        } else {
            Engine_EventBegin();
            Engine_ActorStop(11);
            Engine_ActorRunRepeatedMotion(11, 1);
            base = (s32)MsgHaidiaRockslideDestroyedFence;
            Engine_EventSetMessage(base);
            Engine_EventShowMessageAndWait(11, 0, 20);
            Call3(Engine_ActorShowEmote, 0, 0x100, 30);
            Engine_CameraMoveTo(0x620000, -1, 0x11b0000, 1);
            Call3(Engine_ActorWalkToAndWait, 0, 94, 0x125);
            Call3(Engine_ActorFaceDirection, 0, 0xa000, 0);
            record = (s32)Object_GetById(0);
            if (record != 0) {
                Engine_ActorSetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
            }
            Call3(Engine_ActorWalkToAndWait, 1, 110, 0x117);
            Call3(Engine_ActorFaceDirection, 1, 0xa000, 40);
            Engine_ActorRunRepeatedMotion(11, 2);
            Engine_EventWait(40);
            Engine_EventOpenMessage(11, 0);
            if (Engine_EventChooseYesNo(0, 0) == 0) {
                Engine_ActorRunRepeatedMotion(11, 2);
                Engine_EventWait(20);
                Engine_EventSetMessage((base + 2));
                Engine_EventShowMessage(11, 0);
                Engine_GameFlagSet(0x82f);
            } else {
                Engine_ActorRunRepeatedMotion(11, 2);
                Engine_EventWait(20);
                Engine_EventSetMessage((base + 3));
                Engine_EventShowMessageAndWait(11, 0, 40);
                Engine_ActorFaceActor(11, 0, 0);
                Engine_ActorSetAnimation(11, 1);
                Engine_ActorJump(11, 4, 40);
                Engine_ActorSetAnimation(11, 6);
                Call3(Engine_ActorShowEmote, 11, 0x101, 40);
                Engine_EventShowMessageAndWait(11, 0, 10);
                Engine_ActorSetAnimation(11, 1);
                Engine_EventWait(10);
                Engine_ActorSetAnimationAndWait(11, 3);
                Engine_EventShowMessageAndWait(11, 0, 10);
                Engine_ActorSetAnimationAndWait(11, 3);
                Call3(Object_SetTargetAndCallback, 0, 0x1000b, (s32)Sukureta_StrangerActions);
                Call3(Object_SetTargetAndCallback, 1, 0x1000b, (s32)Sukureta_StrangerActions);
                Object_SetActionCallbackAndRefreshById(11, (s32)Sukureta_Actor11RockslideActions);
                Engine_ActorStop(0);
                Engine_ActorStop(1);
                Call3(Engine_ActorFaceDirection, 0, 0x4000, 0);
                Call3(Engine_ActorFaceDirection, 1, 0x4000, 60);
                Call3(Engine_ActorShowEmote, 0, 0x105, 0);
                Engine_ActorShowEmote(1, 0x105, 120);
                Engine_GameFlagSet(0x839);
            }
            Engine_ActorSetAnimation(1, 2);
            record = (s32)Object_GetById(0);
            if (record != 0) {
                Engine_ActorSetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Engine_ActorWaitForMove(1);
            Engine_ActorSetPosition(1, 0, 0);
            Engine_EventEnd();
        }
    }
}

void SceneState_SetWorkAndFlag87d(void)
{
    u8 *work;

    Engine_EventBegin();

    work = *(u8 **)&gEventWork;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    GameFlag_Set(0x87D);
    BattleFx_SetWeightedResult(12, 0);
    GameFlag_Set(0x900);   /* 144 << 4 */
    Engine_EventEnd();
}

void SceneState_SetWorkAndFlag87e(void)
{
    u8 *work;

    Engine_EventBegin();

    work = *(u8 **)&gEventWork;
    *(u32 *)(work + 448) = 512;
    *(u32 *)(work + 456) = 64;

    GameFlag_Set(0x87E);
    BattleFx_SetWeightedResult(12, 1);
    GameFlag_Set(0x900);   /* 144 << 4 */
    Engine_EventEnd();
}

void SceneDialogue_RunActorSixteenDialogue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgHaidiaMemoriesOfThisCottage);
    Event_AskYesNo(16, 0);
    Engine_EventEnd();
}
