#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum ActorPresentationMessage {
    MSG_CAME_BACK_PLEASE_DRAUGHT_QUICKLY = 0x2183,
    MSG_WOULD_HAVE_REVEALED_MYSELF_SOONER = 0x218a,
    MSG_MUST_TRAVELED_INDEED_IF_DIDNT = 0x218b,
    MSG_HOW_DID_BECOME_PARALYZED_WHILE = 0x218c,
    MSG_THATS_SUKURETA_SAID_ALCHEMY_COULD = 0x21a4,
    MSG_FORGOT_SUKURETA_SAID_ALCHEMY_COULD = 0x21a5,
    MSG_WHAT_WAS_THAT = 0x21a6,
    MSG_BABI = 0x21a8,
    MSG_SO_WELL_DO_IT = 0x21ce,
    MSG_DONT_WANT_EITHER_DO = 0x21cf,
    MSG_EITHER_WAY_WERE_STUCK_HERE = 0x21d0,
    MSG_GREAT_ITS_DECIDED_JUST_HAVE = 0x21d7,
    MSG_DONT_LET_ME_DOWN_LIKE = 0x21d9
};


#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)

s32 Engine_PartyRemoveItem(s32 item);
void Engine_EventResetEffectCounter(void);
void Engine_ActorWalkToAndWait(s32 actor, s32 x, s32 z);
void Engine_ActorStepOutFromLeader(s32 actor, s32 dx, s32 dz, s32 facing);
void Engine_ActorJump(s32 actor, s32 height, s32 frames);
void Engine_ActorFollow(s32 actor, s32 leader);
void Engine_ActorStartAction(s32 actor);
void Engine_AudioPlaySceneCue(void);

extern const u8 gAltmillerActionA[];
extern const u8 gAltmillerActionB[];
extern const u8 gAltmillerActionC[];
extern const u8 gAltmillerActionD[];

static inline s32 Party_RemoveItem(s32 item)
{
    return Engine_PartyRemoveItem(item);
}

static inline void Event_ResetEffectCounter(void)
{
    Engine_EventResetEffectCounter();
}

static inline void Actor_StepOutFromLeader(s32 actor, s32 dx, s32 dz, s32 facing)
{
    Engine_ActorStepOutFromLeader(actor, dx, dz, facing);
}

static inline void Actor_Follow(s32 actor, s32 leader)
{
    Engine_ActorFollow(actor, leader);
}

static inline void Actor_StartAction(s32 actor)
{
    Engine_ActorStartAction(actor);
}

static inline void Audio_PlaySceneCue(void)
{
    Engine_AudioPlaySceneCue();
}

void FieldScene_RunExtendedActorPresentation(void)
{
    u8 *display;
    struct FieldActor *actor;
    struct FieldActor *object;
    s32 i;
    s32 eva;
    s32 evb;

    GameFlag_Set(0x962);
    Party_RemoveItem(237);
    Event_Begin();
    Event_ResetEffectCounter();
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_SetMessage(MSG_CAME_BACK_PLEASE_DRAUGHT_QUICKLY);
    Event_ShowMessage(8, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 160);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(50);
    Event_Wait(10);
    Camera_MoveTo(PIXELS(264), -1, PIXELS(200), 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 208);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_StepOutFromLeader(ACTOR_GERALD, -16, 16, FACING_NORTH);
    Actor_StepOutFromLeader(ACTOR_MIA, 0, 16, FACING_NORTH);
    Actor_StepOutFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
    Actor_WaitForMove(ACTOR_GERALD);
    Event_Wait(20);
    Event_Wait(10);
    Actor_ShowEmote(8, EMOTE_IN_FRONT | 8, 40);
    Event_ShowMessage(8, 0);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Event_Wait(20);
    actor = Actor_Get(8);
    Event_Wait(30);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(30);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(60);
    Audio_PlayCue(17);
    actor->sprite->rotation = 0;
    Actor_Jump(8, 10, 70);
    for (i = 0; i < 30; i++) {
        actor->sprite->priority = 0;
        Event_Wait(2);
        actor->sprite->priority = 2;
        Event_Wait(2);
    }
    Event_Wait(40);
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
    Event_Wait(20);
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
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_STAND);
    Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
    Task_Wait(1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 5, 40);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT, 40);
    Actor_Jump(ACTOR_GERALD, 4, 13);
    Actor_Jump(ACTOR_GERALD, 4, 30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_GERALD, 30);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 5, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 30);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_SetAnimationAndWait(9, ANIM_SHAKE_HEAD);
    Event_Wait(30);
    Event_OpenMessage(9, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(9, ANIM_NOD);
        Event_Wait(30);
        Event_SetMessage(MSG_WOULD_HAVE_REVEALED_MYSELF_SOONER);
        Event_ShowMessage(9, 0);
    } else {
        Event_SetMessage(MSG_MUST_TRAVELED_INDEED_IF_DIDNT);
        Event_ShowMessage(9, 0);
    }
    Event_SetMessage(MSG_HOW_DID_BECOME_PARALYZED_WHILE);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(9, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_WalkByAndWait(ACTOR_IVAN, 0, -48);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    Event_Wait(40);
    Psynergy_Begin(ABILITY_MIND_READ, 1);
    Psynergy_SetTarget(2, 9);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Event_Wait(150);
    Psynergy_PlayEffect(2);
    Psynergy_LowerHands();
    Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(40);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 2, 40);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_Jump(9, 4, 13);
    Actor_Jump(9, 4, 30);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 50);
    Actor_FaceDirection(9, FACING_SOUTH, 0);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(20);
    Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(9, 0);
    Event_Wait(50);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_WalkByAndWait(ACTOR_IVAN, 0, 48);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Event_Wait(50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(9, ANIM_SHAKE_HEAD);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(30);
    Actor_SetAnimationAndWait(9, ANIM_NOD);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(9, ANIM_SHAKE_HEAD);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT, 80);
    Event_Wait(20);
    Actor_FaceDirection(9, FACING_NORTH, 0);
    Event_Wait(50);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 1, 40);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_SetMessage(MSG_THATS_SUKURETA_SAID_ALCHEMY_COULD);
        Event_ShowMessage(ACTOR_GERALD, 0);
    } else {
        Event_SetMessage(MSG_FORGOT_SUKURETA_SAID_ALCHEMY_COULD);
        Event_ShowMessage(ACTOR_GERALD, 0);
    }
    Event_SetMessage(MSG_WHAT_WAS_THAT);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(9, FACING_SOUTH, 0);
    Event_Wait(10);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 40);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
    Event_Wait(30);
    Event_OpenMessage(9, 0);
    Event_SetMessage(MSG_BABI);
    Event_ChooseYesNo(0, 0);
    display = *(u8 **)0x03001ecc;
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
        Task_Wait(7);
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
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Event_Wait(10);
    Event_Wait(10);
    Camera_FollowActor(10, 1);
    Camera_WaitForMove();
    Event_Wait(70);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(11, 0);
    Actor_Follow(ACTOR_PARTY_LEADER, 10);
    Actor_Follow(ACTOR_GERALD, 10);
    Actor_Follow(ACTOR_MIA, 10);
    Actor_Follow(ACTOR_IVAN, 10);
    Actor_SetSpeed(10, 0x16666, 0xb333);
    Actor_SetSpeed(11, 0x16666, 0xb333);
    Actor_EnableActionCallback(10, gAltmillerActionA);
    Event_Wait(3);
    Actor_WalkByAndWait(11, -16, 0);
    Actor_EnableActionCallback(11, gAltmillerActionA);
    Actor_StartAction(10);
    Actor_WalkByAndWait(10, 0, -16);
    Actor_StartAction(11);
    Actor_FaceDirection(10, FACING_EAST, 0);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_GERALD);
    Actor_Stop(ACTOR_MIA);
    Actor_Stop(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Camera_MoveTo(PIXELS(248), -1, PIXELS(216), 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(50);
    Event_Wait(10);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Event_Wait(10);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(10, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(9, 0);
    Actor_WalkByAndWait(11, 16, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Actor_WalkByAndWait(10, 16, 0);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_Jump(11, 4, 13);
    Actor_Jump(11, 4, 30);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(10, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
    Event_Wait(10);
    Actor_FaceDirection(11, FACING_SOUTHEAST, 0);
    Event_Wait(70);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Event_Wait(10);
    Actor_FaceDirection(10, FACING_EAST, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | 1, 40);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
    Event_Wait(20);
    Actor_FaceDirection(9, FACING_WEST, 0);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_FaceEachOther(10, 11, 70);
    Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(11, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(10, ANIM_NOD);
    Event_Wait(30);
    Actor_SetAnimationAndWait(11, ANIM_NOD);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(11, 2);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | 8, 40);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_FaceDirection(10, FACING_EAST, 0);
    Event_Wait(10);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_SetAnimation(10, ANIM_NOD);
    Actor_SetAnimationAndWait(11, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(9, ANIM_NOD);
    Event_Wait(40);
    Actor_Follow(ACTOR_PARTY_LEADER, 9);
    Actor_Follow(ACTOR_GERALD, 9);
    Actor_Follow(ACTOR_MIA, 9);
    Actor_Follow(ACTOR_IVAN, 9);
    Actor_SetSpeed(9, 0x10000, 0x8000);
    Actor_WalkByAndWait(9, -16, 0);
    Actor_WalkBy(10, 0, 48);
    Actor_WalkBy(11, 0, 48);
    Actor_WalkByAndWait(9, -16, 0);
    Actor_WalkByAndWait(9, 0, 32);
    Actor_SetAnimation(10, ANIM_STAND);
    Actor_SetAnimation(11, ANIM_STAND);
    Actor_Stop(ACTOR_PARTY_LEADER);
    Actor_Stop(ACTOR_GERALD);
    Actor_Stop(ACTOR_MIA);
    Actor_Stop(ACTOR_IVAN);
    Event_Wait(10);
    Actor_ShowEmote(9, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(10, FACING_NORTH, 0);
    Actor_FaceDirection(11, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 30);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_GERALD, 9, 30);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_Wait(20);
    Actor_FaceDirection(9, FACING_SOUTH, 0);
    Event_Wait(70);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_Wait(20);
    Actor_FaceDirection(9, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(9, ANIM_NOD);
    Event_Wait(30);
    Actor_EnableActionCallback(11, gAltmillerActionB);
    Actor_EnableActionCallback(10, gAltmillerActionC);
    Actor_EnableActionCallback(9, gAltmillerActionD);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Actor_StartAction(9);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    Event_Wait(20);
    Camera_MoveTo(PIXELS(248), -1, PIXELS(248), 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_FaceDirection(9, FACING_NORTH, 0);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Actor_FaceDirection(10, FACING_WEST, 0);
    Event_Wait(10);
    Actor_FaceDirection(11, FACING_WEST, 0);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(11, ANIM_SHAKE_HEAD);
    Event_Wait(30);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    Actor_ShowEmote(9, EMOTE_IN_FRONT | 7, 40);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Event_Wait(20);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_ShowEmote(10, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(11, EMOTE_IN_FRONT | 2, 40);
    Event_Wait(30);
    Actor_FaceActor(10, 11, 60);
    Actor_FaceDirection(10, FACING_WEST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(10, ANIM_NOD);
    Event_Wait(40);
    Actor_FaceDirection(9, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(9, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(9, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(9, ANIM_NOD);
    Event_Wait(30);
    Actor_FaceDirection(9, FACING_EAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(9, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimation(10, ANIM_NOD);
    Actor_SetAnimationAndWait(11, ANIM_NOD);
    Event_Wait(40);
    Actor_FaceDirection(10, FACING_EAST, 0);
    Event_Wait(20);
    Actor_FaceDirection(11, FACING_EAST, 0);
    Event_Wait(40);
    Actor_WalkBy(9, 32, 0);
    Actor_WalkBy(11, 0, 64);
    Actor_WalkByAndWait(10, 16, 0);
    Actor_WalkBy(10, 0, 64);
    Actor_WaitForMove(9);
    Actor_WalkByAndWait(9, 0, 64);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Event_Wait(20);
    for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
        {
            s32 control = 0x3f42;

            REG_BLDCNT = control;
        }
        REG_BLDALPHA = (evb << 8) | eva;
        Task_Wait(7);
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
    Task_Wait(1);
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
        Camera_MoveToActor(0, 1);
        Camera_WaitForMove();
    } while (0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Event_OpenMessage(ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(20);
        Event_SetMessage(MSG_SO_WELL_DO_IT);
        Event_ShowMessage(ACTOR_GERALD, 0);
    } else {
        Event_Wait(10);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
        Event_Wait(20);
        Event_SetMessage(MSG_DONT_WANT_EITHER_DO);
        Event_ShowMessage(ACTOR_GERALD, 0);
    }
    Event_SetMessage(MSG_EITHER_WAY_WERE_STUCK_HERE);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_MIA, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_NOD);
        Event_Wait(30);
        Event_SetMessage(MSG_GREAT_ITS_DECIDED_JUST_HAVE);
        Event_ShowMessage(ACTOR_GERALD, 0);
        Event_Wait(10);
        Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
        Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        Event_Wait(30);
        Event_ShowMessage(ACTOR_IVAN, 0);
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
        Actor_SetAnimation(ACTOR_GERALD, ANIM_NOD);
        Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
        Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
        Event_Wait(30);
    } else {
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_GERALD, ANIM_SHAKE_HEAD);
        Event_Wait(30);
        Event_SetMessage(MSG_DONT_LET_ME_DOWN_LIKE);
        Event_ShowMessage(ACTOR_GERALD, 0);
        Event_Wait(10);
        Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
        Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        Event_Wait(30);
        Event_ShowMessage(ACTOR_IVAN, 0);
        Event_Wait(10);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
        Event_Wait(20);
        Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        Event_Wait(30);
        Actor_ShowEmote(ACTOR_GERALD, EMOTE_IN_FRONT | 2, 70);
    }
    Audio_PlayCue(17);
    Actor_SetSpeed(ACTOR_GERALD, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
    Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
    Actor_SetAnimation(ACTOR_GERALD, ANIM_WALK);
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != NULL) {
        Actor_SetDestination(ACTOR_GERALD, object->x.part.pixel, object->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != NULL) {
        Actor_SetDestination(ACTOR_IVAN, object->x.part.pixel, object->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
    object = Actor_Get(ACTOR_PARTY_LEADER);
    if (object != NULL) {
        Actor_SetDestination(ACTOR_MIA, object->x.part.pixel, object->z.part.pixel);
    }
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Audio_PlaySceneCue();
    Event_End();
}
