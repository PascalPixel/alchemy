/*
 * Overlay resource_386: actor placement, per-actor dialogue lines and the
 * scene initialiser that installs the per-frame task.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"

#define WORKSPACE (*(u8 **)0x03001ebc)

#include "RESOURCE_386_STATE.H"

enum PlacementMessage {
    MSG_HAVE_LOT_LEFTOVER_BONES_FROM = 0x137c,
    MSG_GEE_ALWAYS_GET_HUNGRY_WHEN = 0x1382,
    MSG_WOW_HAVE_MANY_THINGS_ARENT = 0x1384,
    MSG_WANT_MORE_BONES = 0x1385,
    MSG_THOUGHT_SAW_CAT_GET_UP = 0x1cc9,
    MSG_DODONPAS_HENCHMEN_NEVER_ATTACKED_US = 0x1ccd,
    MSG_ITS_NEAR_TIME_FOR_COLOSSO = 0x1cd0,
    MSG_ROBIN_YOUVE_COME_BACK_VAULT = 0x1cd4,
    MSG_THOSE_GUYS_REALLY_WANTED_REVENGE = 0x1cda,
    MSG_THERES_RIVER_FIRE_ON_OTHER = 0x1ce2,
    MSG_THEY_SAY_VOLCANO_ON_MT = 0x1ce4,
    MSG_WE_HAVENT_BEEN_GETTING_MANY = 0x1ce6,
    MSG_MMMM_NOTHING_DO_NOTHING_DO = 0x1cec,
    MSG_DID_SOME_COOKING_NOW_IVE = 0x1cee,
    MSG_HE_REALLY_LIKES_BONES_WONDER = 0x1cf4
};


extern s16 Data_02000240[];

void Func_020006d4(u8 *);
void Func_020006da(s32);
s32 Func_02000898();
s32 Func_0200096c(void);
u8 *Func_020009ea(s32);
u8 *Func_02000a32(s32);
u8 *Func_02000a7a(s32);
u8 *Func_02000ac2(s32);
u8  *Func_02000b80();
u8  *Func_02000b8c();
u8  *Func_02000b94();
void Func_02000b34();
u8 *Func_02000b76(s32);
u8 *Func_02000b98(s32, s32);

/* Each Func_ name is a loader-relocated call word, not a runtime address. */

/*
 * Call sites spelled through these wrappers pass their constants straight
 * into the argument registers. A direct call instead precomputes a costly
 * constant into a temporary that is then shared with later uses in the same
 * block. A value-returning call sets r0 last of its arguments.
 */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value0(s32 (*f)())
{
    void Actor_FaceEachOther();
    void Event_ShowMessage();

    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Actor_FaceEachOther();
    void Event_ShowMessage();

    return f(a0);
}

void SceneState_CheckPositionWindow(void)
{
    s32 v1;
    s32 v0;

    v0 = ((struct Resource386FirstView *)Resource386_GetFirstView(0))->sample_08;
    v1 = (s32)((struct Resource386SecondView *)Resource386_GetSecondView(0))->sample_10 >> 0x14;
    if (((u32)((v0 >> 0x14) - 0x22) <= 1U) && (v1 > 0x28) && (v1 <= 0x2A)) {
        Resource386_OnWindowMatch(0x250);
        return;
    }
    Resource386_OnWindowMiss(0x250);
}

/*
 * The eight-byte owner includes its one pool word, which holds the address
 * returned here. The word is loaded and returned, never dereferenced.
 */
u8 *SceneData_GetScriptTable(void)
{
    return (u8 *)0x020086dc;
}

/* Table slot with no data: reads nothing and returns zero. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetMessageTable(void)
{
    return (u8 *)0x020087cc;
}

u8 *SceneData_InitAndGetTable87f4(void)
{
    u8 *slot = (u8 *)0x020087F4;

    Func_020006d4(slot);
    return slot;
}

void SceneActor_RunActorStep(s32 arg0)
{
    Func_020006da(arg0);
    Actor_SetAnimation(arg0, 1);
    Event_ShowMessage(arg0, 0);
    Event_End();
}

void SceneDialogue_RunActor9Line(void)
{
    Event_SetMessage(MSG_THOUGHT_SAW_CAT_GET_UP);
    Actor_FaceEachOther(9, 0, 2);
    SceneActor_RunActorStep(9);
}

void SceneDialogue_RunActor11Line(void)
{
    Event_SetMessage(MSG_DODONPAS_HENCHMEN_NEVER_ATTACKED_US);
    Actor_FaceEachOther(11, 0, 2);
    SceneActor_RunActorStep(11);
}

void SceneDialogue_RunActor12Line(void)
{
    Event_SetMessage(MSG_ITS_NEAR_TIME_FOR_COLOSSO);
    Actor_FaceEachOther(12, 0, 2);
    SceneActor_RunActorStep(12);
}

void FieldScene_RunActor16Sequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_ROBIN_YOUVE_COME_BACK_VAULT);
    Actor_FaceEachOther(16, 0, 2);
    Actor_SetAnimation(16, 1);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 20);
    Actor_ShowEmote(16, 0x102, 60);
    Event_ShowMessageAndWait(16, 0, 30);
    Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessageAndWait(16, 0, 20);
    GameFlag_Set(0x300);
    GameFlag_Set(0x868);
    Event_End();
}

void SceneDialogue_RunActor16Line(void)
{
    void Actor_FaceEachOther(s32, s32, s32);

    Event_SetMessage(MSG_THOSE_GUYS_REALLY_WANTED_REVENGE);
    Actor_FaceEachOther(16, 0, 2);
    SceneActor_RunActorStep(16);
}

void SceneDialogue_RunActor23Line(void)
{
    void Actor_FaceEachOther(s32, s32, s32);

    Event_SetMessage(MSG_DID_SOME_COOKING_NOW_IVE);
    Actor_FaceEachOther(23, 0, 2);
    SceneActor_RunActorStep(23);
}

void FieldScene_RunActor18FlaggedSequence(void)
{
    void Event_Wait();
    void Actor_FaceEachOther();
    void Event_ShowMessage();

    u32 i;
    s32 record;

    Event_Begin();
    Actor_FaceEachOther(18, 0, 0);
    if (GameFlag_IsSet(0x85b) == 0) {
        Event_SetMessage(MSG_HAVE_LOT_LEFTOVER_BONES_FROM);
        Event_OpenMessage(18, 0);
    } else {
        Event_SetMessage(MSG_WANT_MORE_BONES);
        Event_OpenMessage(18, 0);
    }
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_ShowMessage(18, 0);
        Event_Wait(20);
        Actor_RunRepeatedMotion(18, 2);
        Event_Wait(20);
        if (Value0(Func_02000898) == 0) {
            Actor_SetAnimationAndWait(18, 4);
            Event_Wait(20);
            Event_SetMessage(MSG_WOW_HAVE_MANY_THINGS_ARENT);
            Event_ShowMessage(18, 0);
            goto L_020002d4;
        }
        Item_ShowFound(231, 3);
        Party_GiveItem(231, 0);
        GameFlag_Set(0x85b);
    } else {
        bump_step(1);
        Event_Wait(20);
        Actor_SetAnimationAndWait(18, 3);
        Event_Wait(20);
        Event_ShowMessage(18, 0);
    }
    L_020002d4:;
    Actor_FaceDirection(18, 0x4000, 0);
    Event_End();
}

void SceneActor_RunActor16StepWithFlag91(void)
{
    u8 *Func_02000976_a(s32);
    u8 *Func_02000986(s32);

    u8 *slot;
    u8 clear = 0;

    Event_Begin();
    Actor_RunRepeatedMotion(16, 1);
    Event_End();
    slot = Func_02000976_a(16) + 91;
    *slot = 1;
    FieldScene_RunActor16Sequence();
    slot = Func_02000986(16) + 91;
    *slot = clear;
    Actor_EnableActionCallback(16, 2);
}

void FieldScene_RunActor18ConditionalCue(void)
{
    void Event_Wait(s32);
    void Actor_SetAnimationAndWait(s32, s32);

    Event_Begin();

    if (Func_0200096c() == 0) {
        Actor_SetAnimationAndWait(18, 4);
        Event_Wait(20);
        Event_SetMessage(MSG_WOW_HAVE_MANY_THINGS_ARENT);
        Event_ShowMessage(18, 0);
    } else {
        Item_ShowFound(0xE7, 3);
        Party_GiveItem(0xE7, 0);
    }

    Event_End();
}

void FieldScene_RunActor19StepByPlace(void)
{
    void Event_Begin(void);

    u32 place;

    place = *(u16 *)(Func_020009ea(0) + 6);
    Event_Begin();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(4, 19);
    } else {
        Event_SetMessage(MSG_THERES_RIVER_FIRE_ON_OTHER);
        Event_ShowMessage(19, 0);
    }

    Event_End();
}

void FieldScene_RunActor20StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Func_02000a32(0) + 6);
    Event_Begin();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(5, 20);
    } else {
        Event_SetMessage(MSG_THEY_SAY_VOLCANO_ON_MT);
        Event_ShowMessage(20, 0);
    }

    Event_End();
}

void FieldScene_RunActor21StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Func_02000a7a(0) + 6);
    Event_Begin();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Shop_Open(6, 21);
    } else {
        Event_SetMessage(MSG_WE_HAVENT_BEEN_GETTING_MANY);
        Event_ShowMessage(21, 0);
    }

    Event_End();
}

void FieldScene_RunActor22StepByPlace(void)
{
    u32 place;

    place = *(u16 *)(Func_02000ac2(0) + 6);
    Event_Begin();

    if (place + 0xFFFF5FFF <= 0x3FFE) {
        Inn_Open(1, 22);
    } else {
        Event_SetMessage(MSG_MMMM_NOTHING_DO_NOTHING_DO);
        Event_ShowMessage(22, 0);
    }

    Event_End();
}

void SceneDialogue_RunActor18FlaggedLine(void)
{
    s32 GameFlag_IsSet(s32);

    Event_Begin();
    if (GameFlag_IsSet(0x85B) == 0) {
        Event_SetMessage(MSG_GEE_ALWAYS_GET_HUNGRY_WHEN);
    } else {
        Event_SetMessage(MSG_HE_REALLY_LIKES_BONES_WONDER);
    }
    Event_ShowMessage(18, 0);
    Event_End();
}

/* The eight-byte owner includes the pool word holding this address. */
u8 *SceneData_GetEffectTable(void)
{
    s32 GameFlag_IsSet(s32);

    return (u8 *)0x0200898c;
}

/*
 * Overlay entry point: selects the scene from the global block and runs the
 * matching setup. It returns a constant zero status.
 *
 * Data_02000240 is the cross-overlay RAM global block rather than an in-image
 * address, and the signed halfword read out of it selects the scene.
 */
s32 FieldScene_InitSceneStateByStep(void)
{
    void Func_02000abe_a();

    s32 scene;
    s32 zero;

    *(s32 *)(WORKSPACE + 448) = 521;
    scene = gGameState.entrance;

    if (scene == 5) {
        s32 fifth = 4;
        s32 sixth = 3;

        /*
         * The fifth and sixth arguments go on the stack. The two locals
         * are what put them there, so they must stay locals.
         */
        Map_CopyCellsTo(0, 120, 8, 67, fifth, sixth);
        zero = 0;
        Func_02000b80(8)[0x55] = zero;
        *(s32 *)(Func_02000b8c(8) + 12) = zero;
        *(s32 *)(Func_02000b94(8) + 20) = zero;
    } else if (scene == 7 || scene == 11) {
        /* Built by shifts: 142 << 18, 128 << 13, 168 << 18. */
        Func_02000abe_a(0xe7, 0x02380000, 0x00100000, 0x02a00000);
        /*
         * 0x02008031 is Func_02000030 plus the Thumb bit, a task callback
         * rather than data; 200 << 4 is the period.
         */
        Func_02000b34(0x02008031, 0xc80);
    }

    return 0;
}

/*
 * Prepare object 22 for display. Call sites set three further registers that
 * this function does not read.
 */
void OverlayObject_InitObject22(s32 a)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = Func_02000b76(22);
    if (o != 0) {
        q = *(u8 **)(o + 0x50);
        p = q + 38;
        *p = z;
        p += 1;
        *p = z;
        m = 33;
        m = -m;
        q[5] &= m;
        q[9] &= 15;
        o[0x55] = z;
        o[0x5c] = 1;
        v = Func_02000b98(17, 0x608);
        Item_LoadIcon(a);
        v += 0x400;
        Vram_Load(q[28], 0x80, v);
        Heap_Release(17);
    }
}
