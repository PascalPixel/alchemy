#include "TYPES.H"
#include "FIELD_EVENT.H"

#define WORKSPACE (*(u8 **) 0x03001EBC)

#include "RESOURCE_38C.H"

enum MessageBranchMessage {
    MSG_WHEN_SPRING_COMES_WANT_GO = 0x13d9,
    MSG_THERE_WAS_ABLE_HEALER_IN = 0x13e3,
    MSG_HAVE_EVER_HEARD_OCEAN = 0x13e5,
    MSG_MUST_USED_TYPES_DANGER_BEING = 0x13e9,
    MSG_CURSE_ON_KOLIMA_SCARY_DEVELOPMENT = 0x13eb,
    MSG_IM_REALLY_WORRIED_ABOUT_KOLIMA = 0x13ed,
    MSG_GRRR = 0x13f0,
    MSG_DO_BELIEVE_TREE_SPIRIT_CAN = 0x13f6,
    MSG_EVEN_FROZEN_IMIL_MUST_FEEL = 0x146f,
    MSG_DO_KNOW_IF_HOLY_TREE = 0x16e1,
    MSG_DO_KNOW_SILK = 0x16ec,
    MSG_OUR_WEAPONS_BEST_CAN_FIND = 0x16f5,
    MSG_FOR_SOME_REASON_OCEAN_FILLS = 0x16f7,
    MSG_TURNED_OUT_WARRIORS_HIRED_BY = 0x16f9,
    MSG_WASNT_CURSE_IN_KOLIMA_HORRIFYING = 0x16fb,
    MSG_TWO_SPECIALS_ONE_DINNER_ONE = 0x16ff,
    MSG_IF_WANT_MEAL_SPEAK_WAITRESS = 0x1702,
    MSG_REALLY_THINK_HEAD_CHEF_HAS = 0x1703,
    MSG_LETS_SEE_SERVE_THEM_WATER = 0x1705,
    MSG_WAS_SOME_MEAL_DONT_JUST = 0x170a
};


void Func_02000574(s32);
u8 *Func_0200059a(s32);
u8 *Func_020005fa(s32);
u8 *Func_02000832(s32);

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned table address 0x02008598.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned table address 0x02008688.
 */

/*
 * The eight-byte owner at 0x0200011c includes its one pool word, which holds
 * the returned table address 0x020088f0.
 */

/*
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, not a runtime address.
 */

/*
 * Call sites spelled through these wrappers pass their constants straight into
 * the argument registers; a direct call precomputes a costly constant into a
 * pseudo shared with later uses in the block.  A value-returning call also
 * sets r0 last of its arguments.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */

/* The workspace pointer this overlay reaches through. */
static __inline__ void Call1(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{

    f(a0, a1, a2);
}

static __inline__ void bump_step(s32 amount)
{

    gEventWork->message += amount;
}

static __inline__ void SetScale(s32 actor, s32 scale, s32 duration)
{
    Actor_FaceDirection(actor, scale, duration);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Actor_FaceDirection();

    f(a0, a1);
}

static __inline__ void SetScale_020009d8(s32 actor, s32 scale, s32 duration)
{

    Actor_FaceDirection(actor, scale, duration);
}

u8 *SceneData_GetPrimaryTable(void)
{
    return (u8 *)RESOURCE38C_PRIMARY_TABLE_ADDRESS;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetSecondaryTable(void)
{
    return (u8 *)RESOURCE38C_SECONDARY_TABLE_ADDRESS;
}

s32 SceneData_PrepareTable86b0(void)
{
    Func_02000574(0x020086B0);
    return 0x020086B0;
}

void FieldScene_RunActor16MessageBranch(void)
{

    u32 dir;

    dir = *(u16 *)(Func_0200059a(0) + 6);
    Event_Begin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(7, 16);
    } else {
        if (GameFlag_IsSet(0x845) == 0) {
            Event_SetMessage(MSG_THERE_WAS_ABLE_HEALER_IN);
        } else {
            Event_SetMessage(MSG_OUR_WEAPONS_BEST_CAN_FIND);
        }
        Event_ShowMessage(16, 0);
    }

    Event_End();
}

void FieldScene_RunActor18MessageBranch(void)
{

    u32 dir;

    dir = *(u16 *)(Func_020005fa(0) + 6);
    Event_Begin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(9, 18);
    } else {
        if (GameFlag_IsSet(0x845) == 0) {
            Event_SetMessage(MSG_MUST_USED_TYPES_DANGER_BEING);
        } else {
            Event_SetMessage(MSG_TURNED_OUT_WARRIORS_HIRED_BY);
        }
        Event_ShowMessage(18, 0);
    }

    Event_End();
}

u8 *SceneData_GetTertiaryTable(void)
{
    return (u8 *)RESOURCE38C_TERTIARY_TABLE_ADDRESS;
}

void FieldScene_RunActor17MessageBranch(void)
{
    s32 Func_02000662_a();

    u32 dir;

    dir = *(u16 *)(Value1(Func_02000662_a, 0) + 6);
    Event_Begin();
    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(8, 17);
    } else {
        if (GameFlag_IsSet(0x845) == 0) {
            Event_SetMessage(MSG_HAVE_EVER_HEARD_OCEAN);
            Actor_FaceActor(17, 0, 0);
            Event_Wait(10);
            Event_AskYesNo(17, 0);
            Actor_FaceDirection(17, 0x3000, 10);
        } else {
            Event_SetMessage(MSG_FOR_SOME_REASON_OCEAN_FILLS);
            Event_ShowMessage(17, 0);
        }
    }
    Event_End();
}

void FieldScene_ConfigureActor21Scene(void)
{
    Event_Begin();
    Event_SetMessage(MSG_IM_REALLY_WORRIED_ABOUT_KOLIMA);
    Actor_FaceActor(21, 0, 0);
    Event_ShowMessage(21, 0);
    SetScale(21, 0xc000, 10);
    Event_End();
}

void FieldScene_RunActor24Sequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_GRRR);
    Event_ShowMessageAndWait(24, 0, 20);
    Actor_FaceActor(24, 0, 0);
    Event_Wait(10);
    Event_OpenMessage(24, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(24, 0);
    Actor_FaceDirection(24, 0x4000, 10);
    Event_End();
}

void FieldScene_RunActor27Sequence(void)
{
    void Actor_FaceDirection();

    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_DO_BELIEVE_TREE_SPIRIT_CAN);
    Actor_FaceActor(27, 0, 0);
    Event_Wait(10);
    Event_OpenMessage(27, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(27, 0);
    Actor_FaceDirection(27, 0x4000, 10);
    Event_End();
}

void FieldScene_RunActor8Message(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DO_KNOW_IF_HOLY_TREE);
    Event_AskYesNo(8, 0);
    Event_End();
}

void FieldScene_RunActor13Message(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DO_KNOW_SILK);
    Event_AskYesNo(13, 0);
    Event_End();
}

void FieldScene_RunActor19MessageBranch(void)
{
    s32 GameFlag_IsSet(s32);

    u32 dir;

    dir = *(u16 *)(Func_02000832(0) + 6);
    Event_Begin();

    if (dir + 0xFFFF5FFF <= 0x3FFE) {
        Inn_Open(2, 19);
    } else if (GameFlag_IsSet(0x845) != 0) {
        Event_SetMessage(MSG_WASNT_CURSE_IN_KOLIMA_HORRIFYING);
        Event_AskYesNo(19, 0);
    } else {
        Event_SetMessage(MSG_CURSE_ON_KOLIMA_SCARY_DEVELOPMENT);
        Event_ShowMessage(19, 0);
    }

    Event_End();
}

void FieldScene_RunActor21SequenceOnFlag300(void)
{
    void Actor_FaceDirection();

    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x300) == 0) {
        Event_SetMessage(MSG_TWO_SPECIALS_ONE_DINNER_ONE);
        Event_ShowMessage(21, 0);
        Actor_FaceDirection(21, 0x8000, 20);
        Event_ShowMessage(21, 0);
        Actor_StartRepeatedMotion(22, 2);
        Actor_SetAttachedEffect(22, 0x102);
        Event_Wait(60);
        Event_ShowMessage(22, 0);
        Event_Wait(10);
        GameFlag_Set(0x300);
    }
    Actor_FaceActor(21, 0, 0);
    Event_SetMessage(MSG_IF_WANT_MEAL_SPEAK_WAITRESS);
    Event_ShowMessage(21, 0);
    Actor_FaceDirection(21, 0xc000, 10);
    Event_End();
}

void FieldScene_ConfigureActor22Scene(void)
{
    void Actor_FaceActor(s32, s32, s32);

    Event_Begin();
    Event_SetMessage(MSG_REALLY_THINK_HEAD_CHEF_HAS);
    Event_ShowMessage(0x16, 0);
    Actor_FaceActor(0x16, 0, 0);
    Event_ShowMessage(0x16, 0);
    Actor_FaceDirection(0x16, 0, 0xA);
    Event_End();
}

void FieldScene_ConfigureActor23Scene(void)
{

    Event_Begin();
    Event_SetMessage(MSG_LETS_SEE_SERVE_THEM_WATER);
    Event_ShowMessage(23, 0);
    Actor_FaceActor(23, 0, 0);
    Event_ShowMessage(23, 0);
    SetScale_020009d8(23, 0xc000, 10);
    Event_End();
}

void FieldScene_RunActor27Message(void)
{
    void Event_ShowMessage(s32, s32);

    Event_Begin();
    Event_SetMessage(MSG_WAS_SOME_MEAL_DONT_JUST);
    Event_AskYesNo(27, 0);
    Event_End();
}

void FieldScene_RunActor10MessageBranch(void)
{
    s32 GameFlag_IsSet(s32);
    void Event_End(void);
    void Event_ShowMessage(s32, s32);

    Event_Begin();
    if (GameFlag_IsSet(3) != 0) {
        Event_SetMessage(MSG_EVEN_FROZEN_IMIL_MUST_FEEL);
    } else {
        Event_SetMessage(MSG_WHEN_SPRING_COMES_WANT_GO);
    }
    Event_ShowMessage(10, 0);
    Event_End();
}

s32 FieldScene_SetupActor27OnEntry(void)
{
    u8 *Func_02000a14_a(s32);

    u8 *actor;
    u8 *record;
    s32 bits;

    *(s32 *)(WORKSPACE + 448) = 521;
    actor = Func_02000a14_a(27);
    /*
     * The stored zero is also the mask's starting value: -13 is built by
     * subtracting from the register the strb already set to zero, not by
     * materializing 0xf3 or negating 13.
     */
    actor[0x23] = bits = 0;
    record = *(u8 **)(actor + 0x50);
    bits -= 13;
    bits &= record[9];
    bits |= 8;
    record[9] = bits;
    return 0;
}
