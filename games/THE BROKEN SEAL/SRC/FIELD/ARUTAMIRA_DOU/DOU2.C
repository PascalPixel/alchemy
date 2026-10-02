#include "TYPES.H"
#include "EDITION.H"
#include "ABILITY_IDS.H"
#include "FIELD_EVENT.H"
#include "OBJECT_DISPATCH.H"
#include "FIELD_SCENE.H"
#include "ARUTAMIRA.H"
#include "CALL.H"
#include "SCENE_IDS.H"
#include "DMA.H"

extern u8 MsgArutamiraBabi[];
extern u8 MsgArutamiraCameBackPleaseDraughtQuickly[];
extern u8 MsgArutamiraDontLetMeDownLike[];
extern u8 MsgArutamiraDontWantEitherDo[];
extern u8 MsgArutamiraEitherWayWereStuckHere[];
extern u8 MsgArutamiraForgotSukuretaSaidAlchemyCould[];
extern u8 MsgArutamiraGreatItsDecidedJustHave[];
extern u8 MsgArutamiraHowDidBecomeParalyzedWhile[];
extern u8 MsgArutamiraMustTraveledIndeedIfDidnt[];
extern u8 MsgArutamiraSoWellDoIt[];
extern u8 MsgArutamiraThatsSukuretaSaidAlchemyCould[];
extern u8 MsgArutamiraWhatWasThat[];
extern u8 MsgArutamiraWouldHaveRevealedMyselfSooner[];
extern u8 Data_03001ecc[];
#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)
s32 Engine_PartyRemoveItem(s32 item);
void Battle_ResetEffectCounter(void);
void Engine_ActorWalkToAndWait(s32 actor, s32 x, s32 z);
void Motion_LaunchFromFocusedObject(s32 actor, s32 dx, s32 dz, s32 facing);
void Engine_ActorJump(s32 actor, s32 height, s32 frames);
void Engine_ActorFollow(s32 actor, s32 leader);
void Engine_ActorStartAction(s32 actor);
void Audio_PlayCueFromEventWork(void);
extern const u8 gAltmillerActionA[];
extern const u8 gAltmillerActionB[];
extern const u8 gAltmillerActionC[];
extern const u8 gAltmillerActionD[];

static inline void Actor_StepOutFromLeader(s32 actor, s32 dx, s32 dz, s32 facing)
{
    /* FAKEMATCH: a direct call changes FieldScene_RunExtendedActorPresentation from bl Engine_ActorFaceDirection to mov r5, #192 (2402/2397 assembly lines). */
    Motion_LaunchFromFocusedObject(actor, dx, dz, facing);
}

extern u8 MsgArutamiraDontTrustAnyone[];
extern u8 MsgArutamiraKiddingHaventActually[];
extern u8 MsgArutamiraSaidCouldntMove[];
extern u8 MsgArutamiraSee[];

extern u8 MsgArutamiraForgetOrderRock[];

extern u8 MsgArutamiraWait[];

extern const struct SceneEvent gArutamiraDouEvents2[];
extern const struct SceneEvent gArutamiraDouEvents3[];
extern const struct SceneEvent gArutamiraDouEvents4[];
extern const struct SceneEvent gArutamiraDouEvents5[];
extern const struct SceneEvent gArutamiraDouEvents6[];
extern const struct SceneEvent gArutamiraDouEventsOther[];

void SceneActor_PlaceFiveActorsInRow(s32 spacing);
void SceneEffect_SetupBlendByFlag201(void);

extern u8 gFrameTick[];

void BattleFx_InitializeSlots(void);
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);
void Shop_RestoreSceneTiles(s32 mode);
void Shop_InitEffect(void);
void Camera_WorldToScreen(s32 *pos);
void EffectSlot_Initialize(u8 *object, s32 type, s32 x, s32 z);
void EffectSlot_SetCallback(u8 *object, void (*update)());
void EffectSlot_SetObjectMode(u8 *object, s32 mode);
s32 Math_DivideUnsigned(s32 value, s32 divisor);
void OverlayObject_UpdateThreeStateMotion();

/* The overlay object records (72 bytes each), from +88. */
extern u8 *gEffectWork;

void FieldScene_RunExtendedActorPresentation(void)
{
    u8 *display;
    struct FieldActor *actor;
    struct FieldActor *object;
    s32 i;
    s32 eva;
    s32 evb;

    GameFlag_Set(0x962);
    Engine_PartyRemoveItem(237);
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgArutamiraCameBackPleaseDraughtQuickly);
    Event_ShowMessage(8, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 160);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Engine_EventWait(50);
    Engine_EventWait(10);
    Camera_MoveTo(PIXELS(264), -1, PIXELS(200), 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 208);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_StepOutFromLeader(ACTOR_GERALD, -16, 16, FACING_NORTH);
    Actor_StepOutFromLeader(ACTOR_MIA, 0, 16, FACING_NORTH);
    Actor_StepOutFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | 8, 40);
    Event_ShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Engine_EventWait(20);
    actor = Actor_Get(8);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(60);
    Audio_PlayCue(17);
    actor->sprite->rotation = 0;
    Engine_ActorJump(8, 10, 70);
    for (i = 0; i < 30; i++) {
        actor->sprite->priority = 0;
        Engine_EventWait(2);
        actor->sprite->priority = 2;
        Engine_EventWait(2);
    }
    Engine_EventWait(40);
    {
        s32 x;
        s32 z;

        x = Actor_Get(8)->x.part.pixel;
        z = Actor_Get(8)->z.part.pixel;
        Actor_SetPosition(8, 0, 0);
        Actor_SetPosition(9, PIXELS(x), PIXELS(z));
    }
    actor->sprite->flags = 0;
    Audio_PlayCue(30);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_WalkByAndWait(9, 32, 32);
    Actor_FaceDirection(9, FACING_SOUTH, 0);
    Engine_EventWait(20);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a &= ~1;
    Actor_Get(ACTOR_GERALD)->unknown_5a &= ~1;
    Actor_Get(ACTOR_MIA)->unknown_5a &= ~1;
    Actor_Get(ACTOR_IVAN)->unknown_5a &= ~1;
    Actor_WalkBy(ACTOR_PARTY_LEADER, 0, 16);
    Actor_WalkBy(ACTOR_GERALD, 0, 16);
    Actor_WalkBy(ACTOR_MIA, 0, 16);
    Actor_WalkByAndWait(ACTOR_IVAN, 0, 16);
    Actor_Get(ACTOR_PARTY_LEADER)->unknown_5a |= 1;
    Actor_Get(ACTOR_GERALD)->unknown_5a |= 1;
    Actor_Get(ACTOR_MIA)->unknown_5a |= 1;
    Actor_Get(ACTOR_IVAN)->unknown_5a |= 1;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_STAND);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
    Engine_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
    Engine_TaskWait(1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_GERALD, 2);
    Engine_ActorStartRepeatedMotion(ACTOR_MIA, 2);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 5, 40);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT, 40);
    Engine_ActorJump(ACTOR_GERALD, 4, 13);
    Engine_ActorJump(ACTOR_GERALD, 4, 30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_GERALD, 30);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 5, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 30);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_ActorSetAnimationAndWait(9, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Event_OpenMessage(9, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
        Engine_EventWait(30);
        Engine_EventSetMessage((s32)MsgArutamiraWouldHaveRevealedMyselfSooner);
        Event_ShowMessage(9, 0);
    } else {
        Engine_EventSetMessage((s32)MsgArutamiraMustTraveledIndeedIfDidnt);
        Event_ShowMessage(9, 0);
    }
    Engine_EventSetMessage((s32)MsgArutamiraHowDidBecomeParalyzedWhile);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_WalkByAndWait(ACTOR_IVAN, 0, -48);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    Engine_EventWait(40);
    Engine_PsynergyBegin(ABILITY_MIND_READ, 1);
    Engine_PsynergySetTarget(2, 9);
    Engine_PsynergyRaiseHands();
    Engine_PsynergyPlayEffect(1);
    Engine_EventWait(150);
    Engine_PsynergyPlayEffect(2);
    Engine_PsynergyLowerHands();
    Engine_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_IVAN, 2);
    Engine_EventWait(40);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 2, 40);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorJump(9, 4, 13);
    Engine_ActorJump(9, 4, 30);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 50);
    Actor_FaceDirection(9, FACING_SOUTH, 0);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(9, 0);
    Engine_EventWait(50);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_WalkByAndWait(ACTOR_IVAN, 0, 48);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Engine_EventWait(50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(9, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(9, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT, 80);
    Engine_EventWait(20);
    Actor_FaceDirection(9, FACING_NORTH, 0);
    Engine_EventWait(50);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 1, 40);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventSetMessage((s32)MsgArutamiraThatsSukuretaSaidAlchemyCould);
        Event_ShowMessage(ACTOR_GERALD, 0);
    } else {
        Engine_EventSetMessage((s32)MsgArutamiraForgotSukuretaSaidAlchemyCould);
        Event_ShowMessage(ACTOR_GERALD, 0);
    }
    Engine_EventSetMessage((s32)MsgArutamiraWhatWasThat);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(9, FACING_SOUTH, 0);
    Engine_EventWait(10);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 40);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Engine_EventWait(30);
    Event_OpenMessage(9, 0);
    Engine_EventSetMessage((s32)MsgArutamiraBabi);
    Engine_EventChooseYesNo(0, 0);
    display = *(u8 **)Data_03001ecc;
    {
        u16 *slot = (u16 *)(display + 0x52a);
        s32 value = 32;

        *slot = value;
    }
    for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
        {
            s32 control = 0x3f42;

            REG_BLDCNT = control;
        }
        REG_BLDALPHA = (eva << 8) | evb;
        Engine_TaskWait(7);
    }
    {
        u16 *slot = (u16 *)(display + 0x536);
        s32 value = 0x3f3f;

        *slot = value;
    }
    Actor_SetPosition(10, PIXELS(280), PIXELS(304));
    Actor_SetPosition(11, PIXELS(296), PIXELS(304));
    Event_ShowMessage(10, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_EventWait(10);
    Engine_EventWait(10);
    Engine_CameraFollowActor(10, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(70);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(11, 0);
    Engine_ActorFollow(ACTOR_PARTY_LEADER, 10);
    Engine_ActorFollow(ACTOR_GERALD, 10);
    Engine_ActorFollow(ACTOR_MIA, 10);
    Engine_ActorFollow(ACTOR_IVAN, 10);
    Actor_SetSpeed(10, 0x16666, 0xb333);
    Actor_SetSpeed(11, 0x16666, 0xb333);
    Engine_ActorEnableActionCallback(10, gAltmillerActionA);
    Engine_EventWait(3);
    Actor_WalkByAndWait(11, -16, 0);
    Engine_ActorEnableActionCallback(11, gAltmillerActionA);
    Engine_ActorStartAction(10);
    Actor_WalkByAndWait(10, 0, -16);
    Engine_ActorStartAction(11);
    Actor_FaceDirection(10, FACING_EAST, 0);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(ACTOR_GERALD);
    Engine_ActorStop(ACTOR_MIA);
    Engine_ActorStop(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Camera_MoveTo(PIXELS(248), -1, PIXELS(216), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(50);
    Engine_EventWait(10);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Engine_EventWait(10);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(10, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(9, 0);
    Actor_WalkByAndWait(11, 16, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Actor_WalkByAndWait(10, 16, 0);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Engine_ActorJump(11, 4, 13);
    Engine_ActorJump(11, 4, 30);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(10, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(11, FACING_SOUTHEAST, 0);
    Engine_EventWait(70);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(10, FACING_EAST, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
    Engine_EventWait(20);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(10, 11, 70);
    Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(11, FACING_SOUTHEAST, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(10, ANIM_NOD);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(11, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | 8, 40);
    Event_ShowMessage(11, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(10, FACING_EAST, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(10, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(11, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventWait(40);
    Engine_ActorFollow(ACTOR_PARTY_LEADER, 9);
    Engine_ActorFollow(ACTOR_GERALD, 9);
    Engine_ActorFollow(ACTOR_MIA, 9);
    Engine_ActorFollow(ACTOR_IVAN, 9);
    Actor_SetSpeed(9, 0x10000, 0x8000);
    Actor_WalkByAndWait(9, -16, 0);
    Actor_WalkBy(10, 0, 48);
    Actor_WalkBy(11, 0, 48);
    Actor_WalkByAndWait(9, -16, 0);
    Actor_WalkByAndWait(9, 0, 32);
    Engine_ActorSetAnimation(10, ANIM_STAND);
    Engine_ActorSetAnimation(11, ANIM_STAND);
    Engine_ActorStop(ACTOR_PARTY_LEADER);
    Engine_ActorStop(ACTOR_GERALD);
    Engine_ActorStop(ACTOR_MIA);
    Engine_ActorStop(ACTOR_IVAN);
    Engine_EventWait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(10, FACING_NORTH, 0);
    Actor_FaceDirection(11, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 30);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_GERALD, 9, 30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(9, 2);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(9, FACING_SOUTH, 0);
    Engine_EventWait(70);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(9, FACING_SOUTH, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventWait(30);
    Engine_ActorEnableActionCallback(11, gAltmillerActionB);
    Engine_ActorEnableActionCallback(10, gAltmillerActionC);
    Engine_ActorEnableActionCallback(9, gAltmillerActionD);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Engine_ActorStartAction(9);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Engine_EventWait(20);
    Camera_MoveTo(PIXELS(248), -1, PIXELS(248), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    Actor_FaceDirection(9, FACING_NORTH, 0);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(10, FACING_WEST, 0);
    Engine_EventWait(10);
    Actor_FaceDirection(11, FACING_WEST, 0);
    Engine_EventWait(20);
    Event_ShowMessage(10, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(11, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Event_ShowMessage(11, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 7, 40);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Engine_EventWait(20);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(10, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | 2, 40);
    Engine_EventWait(30);
    Actor_FaceActor(10, 11, 60);
    Actor_FaceDirection(10, FACING_WEST, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(10, ANIM_NOD);
    Engine_EventWait(40);
    Actor_FaceDirection(9, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(9, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventWait(30);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(10, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(11, ANIM_NOD);
    Engine_EventWait(40);
    Actor_FaceDirection(10, FACING_EAST, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Engine_EventWait(40);
    Actor_WalkBy(9, 32, 0);
    Actor_WalkBy(11, 0, 64);
    Actor_WalkByAndWait(10, 16, 0);
    Actor_WalkBy(10, 0, 64);
    Engine_ActorWaitForMove(9);
    Actor_WalkByAndWait(9, 0, 64);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Engine_EventWait(20);
    for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
        {
            s32 control = 0x3f42;

            REG_BLDCNT = control;
        }
        REG_BLDALPHA = (evb << 8) | eva;
        Engine_TaskWait(7);
    }
    {
        u16 *slot = (u16 *)(display + 0x52a);
        s32 value = 5;

        *slot = value;
    }
    {
        u16 *slot = (u16 *)(display + 0x536);
        s32 value = 31;

        *slot = value;
    }
    Engine_TaskWait(1);
    {
        s32 value;

        value = 0x3f42;
        REG_BLDCNT = value;
        value = 0xc04;
        REG_BLDALPHA = value;
    }
    /* FAKEMATCH: the reference loads r0 before r1 for this Camera_MoveToActor
     * call; ordinary C loads them the other way (2 halfwords). The do/while
     * block is a scheduling construct, not recovered source. */
    do {
        Engine_CameraMoveToActor(0, 1);
        Engine_CameraWaitForMove();
    } while (0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgArutamiraSoWellDoIt);
        Event_ShowMessage(ACTOR_GERALD, 0);
    } else {
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(ACTOR_GERALD, 2);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgArutamiraDontWantEitherDo);
        Event_ShowMessage(ACTOR_GERALD, 0);
    }
    Engine_EventSetMessage((s32)MsgArutamiraEitherWayWereStuckHere);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_MIA, 2);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
        Engine_EventWait(30);
        Engine_EventSetMessage((s32)MsgArutamiraGreatItsDecidedJustHave);
        Event_ShowMessage(ACTOR_GERALD, 0);
        Engine_EventWait(10);
        Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
        Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        Engine_EventWait(30);
        Event_ShowMessage(ACTOR_IVAN, 0);
        Engine_EventWait(20);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
        Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_NOD);
        Engine_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
        Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
        Engine_EventWait(30);
    } else {
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(ACTOR_GERALD, ANIM_SHAKE_HEAD);
        Engine_EventWait(30);
        Engine_EventSetMessage((s32)MsgArutamiraDontLetMeDownLike);
        Event_ShowMessage(ACTOR_GERALD, 0);
        Engine_EventWait(10);
        Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
        Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        Engine_EventWait(30);
        Event_ShowMessage(ACTOR_IVAN, 0);
        Engine_EventWait(10);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        Engine_EventWait(30);
        Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 70);
    }
    Audio_PlayCue(17);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Engine_ActorSetAnimation(ACTOR_GERALD, ANIM_WALK);
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != NULL) {
        Actor_SetDestination(ACTOR_GERALD, object->x.part.pixel, object->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Engine_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != NULL) {
        Actor_SetDestination(ACTOR_IVAN, object->x.part.pixel, object->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Engine_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != NULL) {
        Actor_SetDestination(ACTOR_MIA, object->x.part.pixel, object->z.part.pixel);
    }
    Engine_ActorWaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Audio_PlayCueFromEventWork();
    Engine_EventEnd();
}

/*
 * Every call site is written out separately and repeated calls must not be
 * folded: the sequence of distinct call words is what reproduces the
 * reference. The three record lookups near the end are null checked before
 * their stored coordinates are forwarded.
 */
void FieldScene_RunBranchingCutsceneSequence(void)
{
    u8 *record;
    s32 line;
    GameFlag_Set(0x960);
    Engine_AudioPlayCue(24);
    Engine_EventBegin();
    Battle_ResetEffectCounter(); /* main:0808a460 */
    Engine_EventSetMessage((s32)MsgArutamiraSee);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Camera_MoveTo(0xf80000, -1, 0xb80000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 192);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Call4(Motion_LaunchFromFocusedObject, 1, -16, 16, 0xc000);
    Call4(Motion_LaunchFromFocusedObject, 3, 0, 16, 0xc000);
    Call4(Motion_LaunchFromFocusedObject, 2, 16, 16, 0xc000);
    Engine_ActorWaitForMove(1);
    Engine_EventWait(20);
    Call3(Engine_ActorShowEmote, 2, 0x102, 0);
    Engine_EventWait(40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x100, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Actor_SetAttachedEffect(8, 0x102); /* main:0808a1f0 */
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x100, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x105, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 30);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Engine_ActorFaceDirection(1, 0xc000, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x102, 40);
    Event_ShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(3, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(8, 0x106, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Engine_ActorShowEmote(2, 0x101, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 40);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(3, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x102, 40);
    Event_OpenMessage(8, 0); /* main:0808a178 */
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventSetMessage((s32)MsgArutamiraKiddingHaventActually);
        Engine_EventWait(20);
        Actor_ShowEmote(ACTOR_GERALD, 0x100, 40);
        Actor_SetSpeed(ACTOR_GERALD, 0x20000, 0x10000);
        Call3(Engine_ActorWalkByAndWait, 1, 0, -16);
        Engine_EventWait(10);
        Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 30);
        Engine_EventShowMessage(1, 0);
    } else {
        Engine_EventSetMessage((s32)MsgArutamiraDontTrustAnyone);
        Engine_EventWait(10);
        Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
        Call3(Engine_ActorWalkByAndWait, 1, 0, -16);
        Engine_EventWait(10);
        Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 30);
        Engine_ActorSetAnimationAndWait(ACTOR_GERALD, 3);
        Engine_EventWait(20);
        Engine_EventShowMessage(1, 0);
    }

    line = (s32)MsgArutamiraSaidCouldntMove;
    Engine_EventSetMessage(line);
    Actor_ShowEmote(ACTOR_IVAN, 0x103, 40);
    Actor_SetSpeed(ACTOR_IVAN, 0x20000, 0x10000);
    Call3(Engine_ActorWalkByAndWait, 2, 0, -16);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_MIA, 0);
    Engine_ActorFaceActor(2, 3, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Engine_ActorWalkByAndWait(1, 0, 16);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(1, 4);
    Engine_EventWait(30);
    Engine_ActorFaceActor(1, 0, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(30);
    Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 30);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_MIA, 3);
    Engine_EventWait(30);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Engine_ActorWalkByAndWait(2, 0, 16);
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    line += 7;
    Engine_EventSetMessage(line);
    Engine_EventWait(30);
    Actor_ShowEmote(8, 0x100, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x108, 40);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 3);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 40);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Engine_ActorFaceDirection(2, 0xc000, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(30);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(2, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(8, 0x108, 50);
    Engine_EventShowMessage(8, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 40);
    Engine_EventShowMessage(1, 0);
    Engine_EventWait(10);
    Actor_ShowEmote(8, 0x102, 40);
    Event_ShowMessage(8, 0);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(3, 4);
    Engine_EventWait(20);
    Engine_EventShowMessage(3, 0);
    Engine_EventWait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 40);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(30);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_MIA, 0);
    Engine_ActorFaceActor(2, 3, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_GERALD, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_IVAN, 3);
    Engine_EventWait(30);
    Engine_AudioPlayCue(17);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Engine_ActorSetAnimation(1, 2);
    /* If the id-1 record lookup succeeds, forward its stored coordinates. */
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(1, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    Engine_ActorWaitForMove(1);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Engine_ActorSetAnimation(2, 2);
    /* If the id-2 record lookup succeeds, forward its stored coordinates. */
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(2, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    Engine_ActorWaitForMove(2);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Engine_ActorSetAnimation(3, 2);
    /* If the id-3 record lookup succeeds, forward its stored coordinates. */
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(3, *(s16 *)(record + RECORD_COORD_X_OFFSET), *(s16 *)(record + RECORD_COORD_Y_OFFSET));
    }
    Engine_ActorWaitForMove(3);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Audio_PlayCueFromEventWork();
    Engine_EventEnd();
}

/* Actor 8 asks whether the party forgot the order of the rock colours; each
 * answer is a line after the question, yes first. */
void ArutamiraDou_AskAboutRockOrder(void)
{
    s32 question;

    Engine_EventBegin();
    question = (s32)MsgArutamiraForgetOrderRock;
    Engine_EventSetMessage(question);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_EventSetMessage(question + 1);
        Event_ShowMessage(8, 0);
    } else {
        Engine_EventWait(20);
        Engine_EventSetMessage(question + 2);
        Event_ShowMessage(8, 0);
    }
    Engine_EventEnd();
}

void FieldScene_RunFlagGatedActorEightDialogue(void)
{
    if (GameFlag_IsSet(0x960) == 0)
        return;
    if (GameFlag_IsSet(0x962) != 0)
        return;

    GameFlag_Set(0x961);
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutamiraWait);
    Event_ShowMessage(8, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Engine_EventWait(30);
    Event_ShowMessage(8, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_EventEnd();
}

/* What the scene answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ArutamiraDou2) {
        return gArutamiraDouEvents2;
    }
    if (scene == (s32)&SceneId_ArutamiraDou3) {
        return gArutamiraDouEvents3;
    }
    if (scene == (s32)&SceneId_ArutamiraDou4) {
        return gArutamiraDouEvents4;
    }
    if (scene == (s32)&SceneId_ArutamiraDou5) {
        return gArutamiraDouEvents5;
    }
    if (scene == (s32)&SceneId_ArutamiraDou6) {
        return gArutamiraDouEvents6;
    }
    return gArutamiraDouEventsOther;
}

void ArutamiraDou_MatchLeaderPriority(struct FieldActor *actor)
{
    if (actor != NULL) {
        actor->priority_flags = 0;
        actor->sprite->priority = Object_GetById(0)->sprite->priority;
    }
}

/* A pointer to the words the entry clears when the party arrives by entrance
   52 of the cave's last area. */

/* Altmiller Cave entry: number the entrance by area, then per area restore the lifts, the cells and the row of five actors from the story flags; open with the blend or the plain transition. */
s32 ArutamiraDou_ApplyEntryState(void)
{
    s8 *state;
    struct FieldActor *actor;
    volatile s32 zero;
    s32 i;

    if (gGameState.entrance == 0) {
        if (gGameState.scene == (s32)&SceneId_ArutamiraDou2) {
            gGameState.entrance = 10;
        }
        if (gGameState.scene == (s32)&SceneId_ArutamiraDou3) {
            gGameState.entrance = 20;
        }
        if (gGameState.scene == (s32)&SceneId_ArutamiraDou4) {
            gGameState.entrance = 30;
        }
        if (gGameState.scene == (s32)&SceneId_ArutamiraDou5) {
            gGameState.entrance = 40;
        }
        if (gGameState.scene == (s32)&SceneId_ArutamiraDou6) {
            gGameState.entrance = 50;
        }
    }
    Engine_GameFlagSet(0x200);
    Call1(Engine_GameFlagClear, 0x201);
    if (gGameState.scene == (s32)&SceneId_ArutamiraDou1) {
        if (gGameState.entrance == 1) {
            if (!Engine_GameFlagIsSet(0x109)) {
                *(u8 *)((u32)gSceneState + 4) = 0;
            }
            Engine_GameFlagSet(0x201);
        }
        if (gGameState.entrance == 2) {
            if (!Engine_GameFlagIsSet(0x109)) {
                *(u8 *)((u32)gSceneState + 4) = 5;
            }
            Engine_GameFlagSet(0x201);
        }
    }
    if (gGameState.scene == (s32)&SceneId_ArutamiraDou2) {
        if (Engine_GameFlagIsSet(0x962)) {
            Engine_ActorSetPosition(8, 0, 0);
        } else {
            actor = Object_GetById(8);
            ((s8 *)actor->sprite)[9] = (((s8 *)actor->sprite)[9] & ~0xc) | 4;
            ((u8 *)actor->sprite)[38] = 2;
            actor->sprite->rotation = 0x4000;
        }
    }
    if (gGameState.scene == (s32)&SceneId_ArutamiraDou4) {
        Engine_GameFlagClear(0x200);
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        if (Engine_GameFlagIsSet(0x211)) {
            Engine_ActorSetAnimation(11, 5);
            Call6(Engine_MapCopyCellAttributes, 76, 16, 1, 1, 73, 17);
        } else {
            Object_GetById(11)->priority_flags |= 2;
        }
        Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
#if EDITION_INTERNATIONAL
        if (Engine_GameFlagIsSet(0x212)) {
            Call6(Engine_MapCopyCellAttributes, 30, 20, 1, 1, 32, 20);
        }
#endif
    }
    if (gGameState.scene == (s32)&SceneId_ArutamiraDou6) {
        Engine_GameFlagClear(0x200);
        FieldScene_RedrawActorFootprint(8);
        FieldScene_RedrawActorFootprint(9);
        FieldScene_RedrawActorFootprint(10);
        /* FAKEMATCH: the three callbacks are stored as s32, so the entrance read
         * after them may alias them and stays below the last store. */
        *(s32 *)&Object_GetById(8)->update = (s32)ArutamiraDou_MatchLeaderPriority;
        *(s32 *)&Object_GetById(9)->update = (s32)ArutamiraDou_MatchLeaderPriority;
        *(s32 *)&Object_GetById(10)->update = (s32)ArutamiraDou_MatchLeaderPriority;
        if (gGameState.entrance == 52) {
            zero = 0;
            Dma_Set(&zero, ((void *)ArutamiraDou_ClearTarget), 0x85000003, (volatile u32 *)0x040000d4);
            if (!Engine_GameFlagIsSet(0x109)) {
                u8 *race = gSceneState;

                race[0] = 0;
                race[1] = 0;
                race[2] = 4;
            }
        }
        state = (s8 *)&gSceneState[1];
        if (state[0] == 99) {
            Call6((void (*)())Engine_MapCopyCellsTo, 41, 55, 3, 2, 30, 55);
            Call6(Engine_MapCopyCellAttributes, 42, 8, 1, 1, 31, 8);
        }
        if (state[0] == 2) {
            SceneActor_PlaceFiveActorsInRow((state[1] << 16) / 5 + 0x4000);
        }
        for (i = 0; i < 5; i++) {
            actor = Object_GetById(i + 11);
            actor->motion_flags = 0;
            actor->collision_flags = 0;
            actor->scale_x = 0x10000;
            actor->scale_y = 0x10000;
            Engine_ActorSetSpriteFlags(Object_GetById(i + 11), 0);
            Engine_ActorSetAnimation(i + 11, i + 1);
        }
        Engine_ActorSetChildValue(11, 1);
        Engine_ActorSetChildValue(12, 4);
        Engine_ActorSetChildValue(13, 11);
        Engine_ActorSetChildValue(14, 2);
        Engine_ActorSetChildValue(15, 3);
        Engine_ActorSetChildValue(16, 6);
        Engine_ActorSetChildValue(17, 6);
        Engine_ActorSetChildValue(18, 6);
        Engine_ActorSetChildValue(19, 6);
        Engine_ActorSetChildValue(20, 6);
        Object_GetById(16)->sprite->priority = 3;
        Object_GetById(20)->sprite->priority = 3;
        Object_GetById(16)->priority_flags = 2;
        Object_GetById(20)->priority_flags = 2;
        Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(20), 0);
    }
    if (Engine_GameFlagIsSet(0x200)) {
        SceneEffect_SetupBlendByFlag201();
    } else {
        gEventWork->start_transition = 0x204;
        gEventWork->transition_frames = 24;
    }
    return 0;
}

void OverlayObject_UpdateThreeStateMotion(void *obj)
{
    s32 position[3];
    s32 x;
    s32 z;
    u8 *p;
    s32 state;

    p = (u8 *)obj + 0x40;
    state = *(s8 *)p;
    if (state == 0) {
        z = FIELD(obj, s32, 0x18);
        x = FIELD(obj, s32, 0x14);
        FIELD(obj, s32, 8) = z;
        position[2] = z;
        FIELD(obj, s32, 4) = x;
        position[0] = x;
        Vector_AddPolarOffset(0x780000, Random_Next(), position);
        FIELD(obj, s32, 0xC) = position[0];
        FIELD(obj, s32, 0x10) = position[2];
        FIELD(obj, s32, 0x24) = 0x50000;
        FIELD(obj, s32, 0x20) = 0x50000;
        FIELD(obj, u8, 0x42) = state;
        (*p)++;
        if ((*(s32 *)gFrameTick & 3) == 0)
            Audio_PlayCue(0x86);
    } else if (state == 1) {
        if (BattleFx_HasReachedTarget(obj) == 0) {
            s32 value = *p;
            value--;
            *p = value;
        }
    } else if (state == 2) {
        if (BattleFx_HasReachedTarget(obj) == 0)
            BattleFx_ClearOwnedSlot(obj);
    }
}

/* Releases 24 objects (type 284) one per frame from a point by the cave
 * wall, each with a random frame and speed, opens the wall's cells, then
 * marks every object still active once the screen has faded. */
void ArutamiraDou_ReleaseWallBurst(void)
{
    u8 *work;
    s32 pos[3];
    s32 *p;
    u8 *object;
    s32 n;

    BattleFx_InitializeSlots();
    work = gEffectWork;
    Shop_RestoreSceneTiles(0x202108);
    p = pos;
    p[0] = 0x1f80000;
    p[1] = 0x180000;
    p[2] = 0x900000;
    Camera_WorldToScreen(p);
    object = work + 88;
    for (n = 23; n >= 0; n--) {
        s32 speed;

        EffectSlot_Initialize(object, 284, p[0], p[2]);
        EffectSlot_SetCallback(object, OverlayObject_UpdateThreeStateMotion);
        EffectSlot_SetObjectMode(object, 7);
        ObjectGroup_SetChildValueUnlessFifteen((void *)*(s32 *)object, (u32)(Engine_RandomNext() * 7) >> 16);
        speed = Math_DivideUnsigned(Engine_RandomNext(), 3) + 0x18000;
        *(s32 *)(object + 44) = speed;
        *(s32 *)(object + 40) = speed;
        Engine_TaskWait(1);
        object += 72;
    }
    Engine_TaskWait(80);
    Call6((void (*)())Engine_MapCopyCellsTo, 41, 55, 3, 2, 30, 55);
    Call6((void (*)())Engine_MapCopyCellAttributes, 42, 8, 1, 1, 31, 8);
    Engine_TaskWait(50);
    Call3((void (*)())Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
    Engine_TaskWait(30);
    {
        s32 mode = 2;
        u8 *state = work + 152;

        for (n = 23; n >= 0; n--) {
            if (((s8 *)state)[5] != 0) {
                *state = mode;
            }
            state += 72;
        }
    }
    Engine_MapWaitWorkValuesBelow256();
    Shop_InitEffect();
    BattleFx_ClearActiveSlotsAndScheduleUpdates();
}
