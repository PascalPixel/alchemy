#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareSceneObject        Func_02000048
#define SceneDialogue_RunActor13FlaggedLine      Func_02000cc4
#define SetSceneActor     SceneActor_RunActorCommandWithFlag91

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct SceneActor {
    u8 reserved_00[6];
    s16 temporary_state;
    u8 reserved_08[92];
    u16 presentation_flags;
};

struct SceneActor_02000cfc {
    u8 reserved_00[100];
    u16 presentation_flags;
};

struct SceneActor_02000d78 { u8 reserved_00[100]; u16 presentation_flags; };

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor_02000fb4 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor_02001010 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor_0200113c {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct OverlayEffectMotion {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[28];
    s32 horizontal_rate;
    s32 vertical_rate;
    s32 shadow_x;
    s32 shadow_y;
    s32 shadow_z;
    u8 pad44[32];
    s16 mode;
};

extern u8 *Data_03001e8c[];
extern u8 Data_000012c0[];

u8 *Func_02001dc6(int);
void Func_02001f28(void);
u8 *Func_02001dde(int);
void Func_02001e26(int, int, int, int);
void Func_02001e0a(int);
s32 Func_02001e10(int);
void Func_02001d64();
void Func_02001dcc(int, int, int);
void Func_02001e20(int, int, int);
void Func_02001e74(int, int, int);
void Func_02001f10(int, int, int);
void Func_02001f62(s32, s32, s32);
void Func_02001fd2(s32, s32, s32);
void Func_0200201c();
void Func_02002060(int, int, int);
s32 Func_02002322(void);
void Func_020021e4(s32, s32, s32);
void Func_02002242(s32, s32, s32);
void Func_02002294(int, int, int);
void Func_020022e8(int, int, int);
void Func_02002344(s32, s32, s32);
s32 Func_0200259e(s32);
struct SceneActor *Func_020026e2(s32);
struct SceneActor *Func_0200273e(s32);
struct SceneActor *Func_0200275e(s32);
struct SceneActor *Func_020027a0(s32);
void Func_02002c6e();
typedef s32(*IwramSqrt02001638)(s32);

/*
 * Every Func_ symbol above names a loader-relocated call word, not a runtime
 * address. Declarations are old-style because an overlay import's arity is
 * not fixed per name, and each call site names the veneer it reaches rather
 * than the import behind it -- one import reached from several sites carries
 * several names, and naming the import instead makes fresh veneers.
 * SceneActor_CheckFacingAndRange is void because its result is discarded. SceneActor_CheckFacingAndRange
 * returns s32 so that testing it emits no narrowing shift.
 */

/*
 * The wrapper helpers below pass their constants straight into the argument
 * registers. A direct call precomputes an expensive constant into a value the
 * compiler then shares with later uses in the same block.
 */
void SceneActor_RunActorCommandWithFlag91(s32 x);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

/*
 * A value-returning call sets r0 last of its arguments, so the callee must be
 * spelled as returning a value even where the result is unused.
 */
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

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Actor_SetSpeed(actor, horizontal, vertical);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    void Actor_SetPosition();

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    void Actor_SetPosition();

    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    void Actor_SetPosition();

    f(a0, a1, a2, a3);
}

void SceneEffect_SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_PrepareSceneObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = Object_Create(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        Actor_SetSpriteFlags(obj, 0);
        Object_SetPalette(obj, 0xE);
        Object_SetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *result = Object_Create(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        u8 *object = *(u8 **)(result + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object[9] = mask;
        result[0x55] = 0;
        result[0x59] = 8;
        Actor_SetSpriteFlags(result, 0);
        Object_SetPalette(result, 15);
        result[0x23] = (result[0x23] & 0xfe) | 2;
        return result;
    }
    return NULL;
}

/*
 * Per-frame callback for one actor record, installed into field +0x6c of
 * actors 14 and 15 by the overlay initialiser. Always returns 0. The pointer
 * table at 0x03001e8c holds the scene record at entry 0 and the overlay
 * workspace at entry 12; entry 12 is the same pointer the rest of this
 * overlay loads as Data_03001ebc, and is modelled here as one global rather
 * than two.
 */
s32 SceneActor_UpdateProximityToLeader(u8 *self)
{
    u8 **globals = (u8 **)0x03001e8c;
    u8 *scene = globals[0];
    u8 *workspace = globals[12];        /* == *(u8 **)0x03001ebc */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    /*
     * Bit 0 of the actor's own flag halfword selects which partner to test.
     * This must stay two calls rather than one call on a conditional
     * expression: the conditional form folds to arithmetic on the flag.
     */
    if ((*flags & 1) != 0) {
        partner = Actor_Get(15);
    } else {
        partner = Actor_Get(14);
    }
    if (SceneActor_CheckFacingAndRange(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Actor_Get(0);

    /*
     * Widen the range when the scene counter at workspace + 376 is already
     * running, or when the scene byte at scene + 0x0ea4 is set.
     */
    if (*(s16 *)(workspace + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    SceneActor_CheckFacingAndRange(self, player, range, force);
    return 0;
}

s32 ActorPresentation_UpdateEntityFromLeader(u8 *entity)
{
    u8 *base = Data_03001e8c[0];
    u8 *workspace = Data_03001e8c[12];
    s32 flag = 0;
    s32 selector = 18;
    u8 *leader;

    if (*(s32 *)(entity + 56) == (s32)0x80000000)
        return 0;

    leader = Actor_Get(0);
    if (*(s16 *)(workspace + 376) != 0 || base[0x0ea4] != 0) {
        selector = 26;
        flag = 1;
    }
    SceneActor_CheckFacingAndRange(entity, leader, selector, flag);
    return 0;
}

void *SceneData_GetScriptTable(void)
{
    return (void *)0x02009f5c;
}

int SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetMessageTable(void)
{
    return (void *)0x0200a094;
}

void *SceneData_SelectActorTableByFlag855(void)
{
    if (GameFlag_IsSet(0x855) != 0)
        return (void *)0x0200a27c;
    return (void *)0x0200a0cc;
}

int OverlayObject_GetObject2Byte280(void)
{
    return Func_02001dc6(2)[280];
}

/*
 * The flagged path passes the result of its final call back to the caller, so
 * this is spelled as a tail call and the return type is s32. The
 * fall-through path returns nothing.
 */
s32 OverlayObject_RunObject2WhenFlagged(void)
{
    Func_02001f28();
    if ((*(u32 *)(Func_02001dde(2) + 248) & 1) != 0) {
        Func_02001e26(2, 0, 0, 0);
        Audio_PlayCue(126);
        Func_02001e0a(0);
        return Func_02001e10(2);
    }
}

#include "TYPES.H"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void Func_02001f50();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

/* The scene step counter at 0x1d8 of the shared scene work record. */

void FieldScene_RunScene382_020004a0(void)
{
    s32 record;
    struct EventWork *p5;

    p5 = gEventWork;
    if (GameFlag_IsSet(0x855) != 0 || GameFlag_IsSet(0x856) == 0) {
        Event_RequestExit(p5->touched_trigger - 19);
        return;
    }
    Event_Begin();
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetPosition(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(2, 0xcccc, 0x6666);
    if (p5->touched_trigger == 20) {
        Actor_WalkToAndWait(2, 0x190, 0x1c0);
    } else {
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(0xe00000, -1, 0xa20000, 1);
        Actor_WalkToAndWait(2, 224, 162);
        Camera_WaitForMove();
    }
    Actor_FaceEachOther(0, 2, 0);
    Event_Wait(20);
    Event_SetMessage(0x1327);
    Event_ShowMessageAndWait(0x9002, 0, 20);
    Actor_SetAnimationAndWait(0, 3);
    if (Value0(OverlayObject_GetObject2Byte280)!= 0) {
        Event_SetMessage(0x132a);
        Event_ShowMessage(2, 0);
        OverlayObject_RunObject2WhenFlagged();
        Task_Wait(20);
    }
    Func_02001f50(2);
    Event_RequestExit(p5->touched_trigger - 19);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void SceneState_SetFlags947And29dc(void)
{
    Event_Begin();
    Message_ShowCentered(0x947, 1);
    Message_ShowCentered(0x29dc, 1);
    Event_End();
}

void *SceneData_SelectTableA414ByFlag855(void)
{
    if (GameFlag_IsSet(0x855) != 0)
        return (void *)0x0200a630;
    return (void *)0x0200a414;
}

void SceneDialogue_RunActor9LineAndAdvance(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(0x1223);
    Func_02001d64(9, 0, 2);
    Event_OpenMessage(9, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Event_End();
}

void ActorPresentation_RunActorThirteenSceneSetup(void)
{
    u8 *workspace;

    Event_Begin();
    Event_SetMessage(0x1229);
    Actor_SetAnimation(13, 1);
    Func_02001dcc(13, 0, 2);
    Event_OpenMessage(13, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Event_ShowMessage(13, 0);
    Event_End();
}

void ActorPresentation_RunActorSeventeenSceneSetup(void)
{
    int Event_ChooseYesNo(int, int);

    u8 *workspace;

    Event_Begin();
    Event_SetMessage(0x122f);
    Func_02001e20(17, 0, 2);
    Event_OpenMessage(17, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Event_ShowMessage(17, 0);
    Event_End();
}

void ActorPresentation_RunActorEighteenSceneSetup(void)
{
    u8 *workspace;

    Event_Begin();
    Event_SetMessage(0x1232);
    Func_02001e74(18, 0, 2);
    Event_OpenMessage(18, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        workspace = *(u8 **)0x03001ebc;
        ++*(u16 *)(workspace + 472);
    }
    Event_ShowMessage(18, 0);
    Event_End();
}

void SceneDialogue_RunActor11Line(void) { Engine_EventBegin(); Engine_EventSetMessage(0x1227); SceneActor_RunActorStep(11); Engine_EventEnd(); }

void SceneDialogue_RunActor16Line(void) { Engine_EventBegin(); Engine_EventSetMessage(0x122e); SceneActor_RunActorStep(16); Engine_EventEnd(); }

void SceneDialogue_RunActor19Line(void)
{
    void Event_ShowMessage(int, int);

    Event_Begin(); Event_SetMessage(0x1235); Actor_SetAnimation(19, 0);
    Func_02001f10(19, 0, 2); Event_ShowMessage(19, 0); Event_End();
}

void ActorPresentation_RunActorFourteenDialogue(void)
{
    void Actor_SetAnimation(s32, s32);

    struct SceneActor *actor = Actor_Get(14);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Event_Begin();
    Engine_EventSetMessage(0x122c);
    Actor_SetAnimation(14, 0);
    Func_02001f62(14, 0, 2);
    SceneActor_ApplyActorZeroThenWait(14, 10);
    actor->temporary_state = saved;
    Task_Wait(1);
    Event_End();
    actor->presentation_flags &= 1;
}

void ActorPresentation_RunActorFifteenDialogue(void)
{
    struct SceneActor *actor = Actor_Get(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Event_Begin();
    Engine_EventSetMessage(0x122d);
    Actor_SetAnimation(15, 0);
    Func_02001fd2(15, 0, 2);
    SceneActor_ApplyActorZeroThenWait(15, 10);
    actor->temporary_state = saved;
    Task_Wait(1);
    Event_End();
    actor->presentation_flags &= 1;
}

void FieldScene_RunActor21Sequence(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)Data_000012c0);
    Func_0200201c(21, 0, 2);
    Actor_ShowEmote(21, 0x103, 0);
    Event_Wait(30);
    Event_OpenMessage(21, 0);
    Event_End();
}

void SceneActor_RunActorStep(int actor)
{
    void Event_ShowMessage(int, int);

    Event_Begin(); Actor_SetAnimation(actor, 1); Func_02002060(actor, 0, 2);
    Event_ShowMessage(actor, 0); Event_End();
}

void SceneActor_RunActorCommandWithFlag91(s32 x)
{
    void Event_ShowMessage(s32, s32);

    u8 *flag = (u8 *)Func_02002322() + 91;
    s32 zero = 0;

    *flag = 1;
    Event_Begin();
    Actor_SetAnimation(x, 1);
    Event_Wait(2);
    Event_ShowMessage(x, 0);
    Event_End();
    *flag = zero;
}

void ActorPresentation_RunActorEightSceneSetup(void)
{
    void Func_020020cc_a(int, int, int);

    u8 *workspace;
    Event_Begin(); Event_SetMessage(0x1330); Func_020020cc_a(8, 0, 2); Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Event_ShowMessage(8, 0); Event_End();
}

void SceneDialogue_RunActor11SecondLine(void) { Engine_EventBegin(); Engine_EventSetMessage(0x1335); SceneActor_RunActorStep(11); Engine_EventEnd(); }

void SceneDialogue_RunActor12LineAndAdvance(void)
{
    u8 *workspace;
    Event_Begin(); Event_SetMessage(0x1336);
    if (GameFlag_IsSet(2) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    SceneActor_RunActorStep(12); Event_End();
}

void SceneDialogue_RunActor13Line(void) { Engine_EventBegin(); Engine_EventSetMessage(0x1338); SceneActor_RunActorStep(13); Engine_EventEnd(); }

void ActorPresentation_RunActorFourteenDialogueAndAdvanceStory(void)
{
    void Task_Wait(s32);

    struct SceneActor *actor = Actor_Get(14);
    u16 *flags = &actor->presentation_flags;
    s16 saved = actor->temporary_state;
    /* tmp keeps the flag result live in a register; do not fold it away. */
    s32 tmp;

    *flags = (tmp = *flags | 2);
    Event_Begin();
    Engine_EventSetMessage(0x1339);
    if (GameFlag_IsSet(2) != 0)
        ++gEventWork->message;
    Actor_SetAnimation(14, 0);
    Func_020021e4(14, 0, 2);
    SceneActor_ApplyActorZeroThenWait(14, 10);
    actor->temporary_state = saved;
    Task_Wait(1);
    Event_End();
    *flags &= 1;
}

void ActorPresentation_RunActorFifteenFollowupDialogue(void)
{
    struct SceneActor *actor = Actor_Get(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Event_Begin();
    Engine_EventSetMessage(0x133b);
    Actor_SetAnimation(15, 0);
    Func_02002242(15, 0, 2);
    SceneActor_ApplyActorZeroThenWait(15, 10);
    actor->temporary_state = saved;
    Task_Wait(1);
    Event_End();
    actor->presentation_flags &= 1;
}

void ActorPresentation_RunActorSixteenSceneSetup(void)
{
    u8 *workspace;
    Event_Begin(); Event_SetMessage(0x133c); Actor_SetAnimation(16, 1); Func_02002294(16, 0, 2); Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Event_ShowMessage(16, 0); Event_End();
}

void ActorPresentation_RunActorEighteenFollowupSceneSetup(void)
{
    void Event_ShowMessage(int, int);

    u8 *workspace;
    Event_Begin(); Event_SetMessage(0x133f); Func_020022e8(18, 0, 2); Event_OpenMessage(18, 0);
    if (Event_ChooseYesNo(0, 0) != 0) { workspace = *(u8 **)0x03001ebc; ++*(u16 *)(workspace + 472); }
    Event_ShowMessage(18, 0); Event_End();
}

void ActorPresentation_RunActorNineteenDialogueAndSetSceneState(void)
{
    extern u8 *Data_03001ebc;
    void Actor_SetAnimation(s32, s32);
    void Actor_SetAnimation(s32, s32);

    Event_Begin();
    Event_SetMessage(0x1342);
    Actor_SetAnimation(19, 0);
    Func_02002344(19, 0, 2);
    Event_ShowMessage(19, 0);
    Actor_SetAnimation(19, 1);
    if (Func_0200259e(231) != -1 && GameFlag_IsSet(0x858) == 0) {
        u16 *p = (u16 *)(Data_03001ebc + 370);
        u16 value = 1;

        *p = value;
    }
    Event_End();
}

void SceneDialogue_RunActor20Line(void)
{
    void Func_020023a4_a(int, int, int);

    Event_Begin(); Event_SetMessage(0x137f); Func_020023a4_a(20, 0, 2); Actor_SetAnimationAndWait(20, 3);
    Event_Wait(20); Event_ShowMessage(20, 0); Event_End();
}

void SceneDialogue_RunActor11FlaggedLine(void)
{
    int GameFlag_IsSet(int);

    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) Event_SetMessage(0x1239); else Event_SetMessage(0x1346);
    SceneActor_RunActorCommandWithFlag91(11); Event_End();
}

void SceneDialogue_RunActor13FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(0x123b);
    } else {
        Event_SetMessage(0x1348);
    }
    SetSceneActor(13);
    Event_End();
}

void ActorPresentation_RunActorFourteenFlaggedDialogue(void)
{
    Func_020026e2(14)->presentation_flags |= 2;
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(0x123c);
    } else {
        Event_SetMessage(0x1349);
        if (GameFlag_IsSet(2) != 0)
            ++gEventWork->message;
    }
    SceneActor_RunActorCommandWithFlag91(14);
    Event_End();
    Func_0200273e(14)->presentation_flags &= 1;
}

void ActorPresentation_RunActorFifteenScriptBranch(void)
{
    void Event_SetMessage(s32);

    Func_0200275e(15)->presentation_flags |= 2;
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0)
        Event_SetMessage(0x123d);
    else
        Event_SetMessage(0x134b);
    SceneActor_RunActorCommandWithFlag91(15);
    Event_End();
    Func_020027a0(15)->presentation_flags &= 1;
}

void ActorPresentation_RunActorSixteenScriptBranch(void)
{
    void Event_SetMessage(int);

    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) Event_SetMessage(0x123e); else Event_SetMessage(0x134c);
    SceneActor_RunActorCommandWithFlag91(16); Event_End();
}

void ActorPresentation_RunActorNineteenScriptBranch(void)
{
    void Event_Wait(int);

    u8 *actor = Actor_Get(19); actor[91] = 1; Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage(0x1241); Actor_SetAnimation(19, 0); Event_Wait(2);
    } else if (GameFlag_IsSet(0x858) != 0) {
        Event_SetMessage(0x13ab);
    } else {
        Event_SetMessage(0x134e);
    }
    Event_ShowMessage(19, 0); Event_End(); actor[91] = 0;
}

void SceneDialogue_RunActor21Line(void) { Engine_EventBegin(); Engine_EventSetMessage(0x12c1); SceneActor_RunActorCommandWithFlag91(21); Engine_EventEnd(); }

void SceneState_Apply200ThenPlace55_26(void)
{
    GameFlag_Set(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Map_CopyCellAttributes(55, 26, 4, 2, v1, v2);
    }
}

void SceneState_Apply200ThenPlace23_23(void)
{
    GameFlag_Clear(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Map_CopyCellAttributes(23, 23, 4, 2, v1, v2);
    }
}

void SceneActor_PlaceActor0AndSetSceneDelay(s32 x, s32 y, s32 continuation)
{
    SetScale(0, 0x8000, 0x4000);
    Actor_WalkTo(0, x, y);
    gEventWork->transition_frames = 16;
    Event_RequestExit(continuation);
}

void FieldScene_SetupScene5At408_320(void)
{
    Audio_PlayCue(158); Map_AnimateCells((void *)0x0200a828, 56, 19); SceneActor_PlaceActor0AndSetSceneDelay(408, 320, 5);
}

void FieldScene_SetupScene6At312_304(void)
{ Engine_AudioPlayCue(158); Engine_MapAnimateCells((void *)0x0200a83e, 50, 18); SceneActor_PlaceActor0AndSetSceneDelay(312, 304, 6); }

void FieldScene_SetupScene7At216_288(void)
{ Engine_AudioPlayCue(158); Engine_MapAnimateCells((void *)0x0200a854, 44, 17); SceneActor_PlaceActor0AndSetSceneDelay(216, 288, 7); }

void ActorPresentation_SetupActorZeroForSceneEightAt376_224(void)
{
    struct SceneActor_02000fb4 *actor = Actor_Get(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells((const void *)0x0200a86a, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceActor0AndSetSceneDelay(376, 224, 8);
}

void ActorPresentation_SetupActorZeroForSceneNineAt296_176(void)
{

    struct SceneActor_02001010 *actor = Actor_Get(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells((const void *)0x0200a880, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceActor0AndSetSceneDelay(296, 176, 9);
}

void FieldScene_SetupScene10At120_144(void)
{ Engine_AudioPlayCue(158); Engine_MapAnimateCells((void *)0x0200a896, 38, 6); SceneActor_PlaceActor0AndSetSceneDelay(120, 144, 10); }

void ActorPresentation_SetupActorZeroForSceneTwelveAt72_160(void)
{

    struct SceneActor_0200113c *actor = Actor_Get(0);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells((const void *)0x0200a8ac, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceActor0AndSetSceneDelay(72, 160, 12);
}

void FieldScene_SetupScene13At152_264(void) { Engine_AudioPlayCue(123); SceneActor_PlaceActor0AndSetSceneDelay(152, 264, 13); }

void ActorPresentation_MoveActorToPositionAndWait(int actor, int x, int z, int field40)
{
    void Task_Wait(int);
    void Actor_SetPosition(int, int, int);

    u8 *record = Actor_Get(actor); int frames;
    Actor_SetSpeed(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Actor_SetSpriteFlags(record, 0);
    Actor_MoveToAndWait(actor, x, z); Actor_SetPosition(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Task_Wait(1); if (*(s16 *)(record + 42) == 0) break; }
    Actor_SetSpriteFlags(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void FieldScene_RunActor23SequenceOnceByFlag867(void)
{
    void Event_Wait();

    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(100);
    Event_Wait(40);
    if (GameFlag_IsSet(0x867) == 0) {
        Actor_SetAttachedEffect(23, 0x102);
        Actor_Jump(23, 4, 0);
        Event_Wait(12);
        Actor_Jump(23, 4, 0);
        Event_Wait(20);
        Call4(ActorPresentation_MoveActorToPositionAndWait, 23, 0x188, 104, 0x70000);
        Event_Wait(20);
        Actor_WalkToAndWait(23, 0x198, 104);
        Actor_WalkToAndWait(23, 0x198, 120);
        GameFlag_Set(0x867);
    }
    Event_End();
}

void FieldScene_RunActor19MotionSequence(void)
{
    u32 i;
    s32 record;

    Func_02002c6e(231);
    Event_Begin();
    Event_Wait(10);
    Actor_RunRepeatedMotion(19, 2);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_WalkToAndWait(19, 216, 0x198);
    Event_Wait(10);
    Actor_FaceDirection(19, 0x4000, 20);
    Actor_Jump(19, 6, 0);
    Event_Wait(30);
    Actor_Jump(19, 6, 0);
    Event_Wait(30);
    Actor_Jump(19, 6, 0);
    Event_Wait(30);
    Actor_WalkToAndWait(19, 216, 0x188);
    Event_Wait(10);
    Actor_FaceDirection(19, 0x4000, 20);
    GameFlag_Set(0x858);
    Event_End();
}

s32 SceneActor_GetPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return ((IwramSqrt02001638) 0x030001D8)(dxsq + dysq + dzsq);
}

void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay)
{
    Event_ShowMessage(actor, 0);
    Event_Wait(delay);
}

void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay)
{
    Actor_FaceEachOther(actor, cue, 0);
    Event_Wait(delay);
}

/*
 * Per-frame step for a projectile. Advance x by its rate and mirror it into
 * the shadow copy, then either follow the vertical rate or fall at a fixed
 * rate depending on the mode word, and finally decay both rates.
 */
void SceneEffect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect)
{
    s32 horizontal_rate;
    s32 vertical_rate;

    effect->x += effect->horizontal_rate;
    effect->shadow_x = effect->x;

    if (effect->mode != 0) {
        effect->y += effect->vertical_rate;
        effect->shadow_y = effect->y;
    } else {
        effect->z += effect->vertical_rate;
        effect->shadow_z = effect->z;
        effect->y += 1024;
        effect->shadow_y = effect->y;
    }

    horizontal_rate = effect->horizontal_rate;
    effect->horizontal_rate = horizontal_rate - Math_Divide(horizontal_rate, 28);
    vertical_rate = effect->vertical_rate;
    effect->vertical_rate = vertical_rate - Math_Divide(vertical_rate, 28);
}
