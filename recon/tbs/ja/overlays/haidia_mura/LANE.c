/* Draft: removing the Japanese discarded villager lookup shortens this
 * function by 28 bytes. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/STAGED_MOTION.H"
#include "TYPES.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "IWRAM_CALL.H"

extern u8 MsgHaidiaDoYouNeedToGo[];
extern u8 MsgHaidiaDontGoBeyondSukuretasCottage[];
extern u8 MsgHaidiaIFeltAnotherOne[];
extern u8 MsgHaidiaIToldGeraldItWas[];
extern u8 MsgHaidiaIllGetYouForMy[];
extern u8 MsgHaidiaIsJasmineBackYet[];
extern u8 MsgHaidiaItWontRainForSome[];
extern u8 MsgHaidiaNoTravelersSinceTheEruption[];
extern u8 MsgHaidiaSukuretaHasntComeBack[];
extern u8 MsgHaidiaSukuretaIsWaitingForUs[];
extern u8 MsgHaidiaTheGroundStillShakes[];
extern u8 MsgHaidiaYouCantBeRobin[];
extern u8 MsgHaidiaYouMakeMeSoMad[];
extern u8 MsgHaidiaYourGrandpaIsTheMayor[];
extern u8 MsgHaidiaPuppiesPlayingOver[];
extern u8 MsgHaidiaRrruffRrrruff[];
extern struct MapRenderWork *gMapWork;
s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Battle_WaitMode0();
void ObjectMotion_SetSpeedParameters();
void Engine_MessageShowCentered();
void Engine_MapRedraw();
void Object_SetModeById();
void Engine_EventEnd();
void Engine_EventSetMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorWalkToAndWait();
extern u8 MsgHaidiaNotSneakingUpMtAleph[];

extern u8 MsgHaidiaAsStubbornAsYourFather[];
extern u8 MsgHaidiaDevastatedWhenKyle[];
extern u8 MsgHaidiaGoodJob[];
extern u8 MsgHaidiaWorkingYourselvesBone[];

enum HouseActor {
    ACTOR_BOARD = 23,
    ACTOR_LAST_BOARD = 24
};

enum {
    ANIM_HAMMER = 11
};

void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode);

/*
 * An actor climbs between the yard and the ledge in front of the house at
 * x 392, playing one animation for each half of the climb.
 */
void HaidiaMura_RunWalkScene032B0(s32 actor, s32 animation, s32 next_animation, s32 grounded);
void HaidiaMura_RunWalkScene03380(s32 actor, s32 animation, s32 next_animation, s32 grounded);
void PaletteGlow_Update(s32 a, s32 b);
void FieldScene_RunLargeStagingSequence(void);
void HaidiaMura_OpenVillagerLane(void);
void Scene_RepairTheHouse(void);
void SceneState_Send210AndApplyRectAt40x84(void);
void BattleFx_SetQueuedSoundAndPlay(s32 value);
void SceneActor_SetFlagByteBySlotZeroPosition(void);
void FieldScene_RunScene373SequenceB(void);
s32 SceneActor_RunStep18WhenTargetSet();
extern u8 gHaidiaMuraActor22Actions[];

extern u8 MsgHaidiaAh[];
extern u8 MsgHaidiaEverProtectFamily[];
extern u8 MsgHaidiaHowHaveYouBeen[];
extern u8 MsgHaidiaIUsedToPlayHere[];
void Engine_ActorSetSpeed();
void Engine_ActorFaceDirection();
void Engine_ActorSetAnimation();
void Engine_ActorMoveToAndWait();
void Engine_EventWait();

struct Flags38 {
    u8 pad[38];
    u8 flags;
};

struct Flags85 {
    u8 pad[85];
    u8 flags;
};

extern u8 MsgHaidiaRepairCaption[];

s32 Runtime_ComputeFixedPointDistance(s32 *first_position, s32 *second_position);
u16 ArcTan2(s32 z, s32 x);

extern u8 MsgHaidiaDoorWontOpen[];

void SceneState_ApplyRectAndRunTwo(void)
{
    s32 e;
    s32 f;
    e = 22;
    f = 36;
    Map_CopyCellAttributes(17, 0, 3, 1, e, f);
    StagedActor_AdvancePair();
    HaidiaMura_OpenVillagerLane();
}
