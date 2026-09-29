#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKareiHasLegacyLordHammetsSilk[];
extern u8 MsgKareiLordHammetWillReleasedSoon[];
extern u8 MsgKareiRobinSneakedIntoLunpaThats[];


void Map_ClearLayerEntryFlag();
void Map_SetLayerEntryFlag();
u8 *Object_GetById();
void SceneChannel_ConfigureUniformAndHandoff(s32);
void SceneChannel_ConfigureUniformAndHandoff();
s32 Object_SetActionCallbackAndRefreshById();

/* Signed halfword table in RAM; index 225 selects the scene. */

/*
 * Each Func_ symbol names the pre-relocation call word the image holds, not
 * a runtime address; imports are named by the main-image address in the
 * trailing word of the overlay veneer. Old-style declarations are required
 * here, because the arity varies from site to site.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight
 * into the argument registers, while a direct call precomputes a costly
 * constant into a local that later uses in the block share. A call that
 * returns a value sets r0 last of its arguments.
 */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    void Map_ClearLayerEntryFlag();

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    void Map_ClearLayerEntryFlag();

    u8 *work = *(u8 **)&gEventWork;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureFourth(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformSecond(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformThird(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ void ConfigureUniformFourth(s32 channel, s32 value, s32 zero)
{
    Actor_FaceDirection(channel, value, zero);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{

    *(u16 *)(*(u8 **)&gEventWork + 0x1d8) += amount;
}

void SceneDialogue_RunActor13Message1b83(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKareiLordHammetWillReleasedSoon);
    Event_AskYesNo(13, 0);
    Event_End();
}

void SceneDialogue_RunActor16Message1b88(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKareiHasLegacyLordHammetsSilk);
    Event_AskYesNo(16, 0);
    Event_End();
}

void FieldScene_RunActorEightTurnDialogue(void)
{
    void Event_End(void);

    Event_Begin();
    Actor_ShowEmote(8, 0x100, 0x3C);
    Event_SetMessage((s32)MsgKareiRobinSneakedIntoLunpaThats);
    Event_ShowMessageAndWait(8, 0, 0xA);
    Actor_RunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 0xA);
    Actor_SetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 0xA);
    Actor_SetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 0xA);
    GameFlag_Set(0x913);
    Event_End();
}

void FieldScene_RunScene3aa_02000184(void)
{
    void Map_ClearLayerEntryFlag();

    u32 i;
    s32 record;
    struct EventWork *p5;

    p5 = gEventWork;
    Event_Begin();
    Event_Wait(10);
    if (p5->touched_trigger == 4) {
        Audio_PlayCue(188);
    } else {
        Audio_PlayCue(158);
    }
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    if (p5->touched_trigger == 4) {
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
    Event_Wait(16);
    Event_RequestExit(p5->touched_trigger);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Event_End();
}
