#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum PartyEventsMessage {
    MSG_WHEN_HEARD_WERE_BACK_IVAN = 0x1b21,
    MSG_LORD_HAMMET_WILL_RELEASED_SOON = 0x1b83,
    MSG_HAS_LEGACY_LORD_HAMMETS_SILK = 0x1b88,
    MSG_ROBIN_SNEAKED_INTO_LUNPA_THATS = 0x1b91,
    MSG_ITS_IVAN_HIS_COMPANIONS_PERFECT = 0x2588
};

void Map_ClearLayerEntryFlag();
void Map_SetLayerEntryFlag();
u8 *Object_GetById();
void SceneChannel_ConfigureUniformAndHandoff(s32);
void SceneChannel_ConfigureUniformAndHandoff();
s32 Object_SetActionCallbackAndRefreshById();

/* Signed halfword table in RAM; index 225 selects the scene. */

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

    gEventWork->message += amount;
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

    gEventWork->message += amount;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The scene's message table, laid out after the code. */
extern u8 Placement_Messages[];

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}
