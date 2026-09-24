#include "TYPES.H"
#include "FIELD_EVENT.H"

#define OverlayObject_IntegrateVelocities Func_02000ab0
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_000000b0
#define SecondaryRuntimeSelector Value_000000af
#define TertiaryRuntimeSelector Value_000000ae
#define PrimaryOverlayData Data_0200b270
#define SecondaryOverlayData Data_0200b330
#define TertiaryOverlayData Data_0200b4f8
#define DefaultOverlayData Data_0200b558
#define PrimaryOverlayData_02000fdc Data_0200b694
#define SecondaryOverlayData_02000fdc Data_0200b754
#define TertiaryOverlayData_02000fdc Data_0200b784
#define DefaultOverlayData_02000fdc Data_0200b88c
#define PrimaryOverlayData_020027a0 Data_0200b91c
#define SecondaryOverlayData_020027a0 Data_0200b9e8
#define TertiaryOverlayData_020027a0 Data_0200bac0
#define DefaultOverlayData_020027a0 Data_0200bc28

#include "FIELD_EFFECT.H"
#define OverlayObject_IntegrateVelocities Effect_Move
#include "STAGED_ACTOR.H"
#include "SPAWN_CONFIGURED_EFFECT.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

struct LevelCheckRecord {
    u8 unknown_00[12];
    s32 y;                          /* +0x0c */
};

struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

/*
 * Per-frame integrator for an effect record -- resource_3c5. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

/* Every field is named by offset; the layout is not verified. */
struct MotionEffect {
    u8 pad00[8];
    s32 position[3];
    u8 pad14[4];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flatla3;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity[3];
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Obj {
    u8 pad00[6];
    u16 f06;
};

extern struct LevelCheckRecord *Func_02005552();   /* record by slot index */
extern struct LevelCheckRecord *Func_02004d68();   /* occupant one square ahead */
extern void Func_020027f2();                       /* push the object ahead */
extern u8 Value_000000b0;
extern u8 Data_0200b5b8[];

u8 *Func_020037ae(s32 kind, s32 x, s32 y, s32 z);
u8 *Func_02003806(s32 kind, s32 x, s32 y, s32 z);
void Func_02003c00();
void Func_020052a6();
s32 *Func_02005678();
s32 *Func_020056ce();
void Func_0200190c();
void Func_02003b90();
s32 Func_02003bca();
s32 Func_02003bd6();
s32 Func_02003be2();
s32 Func_02003bec();
s32 Func_02003c0a();
s32 Func_02003c1c();
s32 Func_02003cc0();
s32 Func_02003ccc();
void Func_02001c0a();
s32 Func_02003d24();
s32 Func_02003d30();
s32 Func_02003d3c();
s32 Func_02003d46();
s32 Func_02003d50();
s32 Func_02003d70();
s32 Func_02003d7c();
void Func_02003d86();
void Func_020022e4();
void Func_02001ba4();
void Func_0200230c();
s32 Func_02001bcc();
void Func_02001bdc();
void Func_02002344();
void Func_0200235c();
void Func_02004046();
void Func_02005400_a();
void Func_020053ea();
void Func_02004afa();
void Func_020011c0();
void Func_02002130();
void Func_0200402c();
s32 *Func_02003f62();
s32 *Func_02003f6a();
s32 *Func_02003f78();
void Func_0200223c();
void Func_0200408e();
void Func_020041b4();
void Func_020041fe();
void Func_0200420e();
void Func_0200421c();
void Func_0200422a();
s32 Func_020048c2();
s32 Func_020048f2();
s32 Func_02004922();
void Func_02004394_a();
void Func_02004a86();
void Func_02004acc();
void Func_02004adc();
void Func_02004aea();
s32 Func_020051ce();
s32 Func_020051fe();
s32 Func_0200522e();
void Func_0200526c();
void Func_02005282();
s32 *Func_02005510();
struct Obj *Func_0200558e();
s32 Func_02004da4();
s32 Func_020056ba();
s32 Func_020056be();
void Func_02004e90();

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* Copy the low two mode bits into the object's owner record. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Contiguous unnamed leaf-owner run for resource_3c5. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call4_02000eac(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_020024d0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value1_02002548(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/*
 * Configure this overlay's scene parameters, enable the configuration, and
 * apply the final magnitude.  The complete owner is the saved-link function
 * at 0x020027f4-0x02002820; its stack frame holds arguments five and six for
 * the first call.
 */
static __inline__ void ResetSceneParameters(s32 a, s32 b, s32 c, s32 mode)
{
    Camera_MoveTo(a, b, c, mode);
}

/* Scene calls are bound to loader-runtime addresses by the translation unit. */

/* The translation unit binds scene calls at the loader-runtime base. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02001238(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_02001238(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2_02001238(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3_02001238(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4_02001238(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02001238(s32 amount)
{
    gEventWork->message += amount;
}

void Func_020009dc(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

/*
 * Spawns an overlay object and puts it into its configured state --
 * resource_3c5.
 */

/*
 * The four callees are declared at the in-overlay entry points that are
 * called directly, not at their veneer addresses.
 */

/*
 * Spawn an object, clear the low bits of its owner record, and issue command
 * 15.  The three callees are declared at their in-overlay entry points, not at
 * veneers.  Each read-modify-write takes its mask from a local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.  The +0x23 mask stays byte-wide as
 * 0xfe rather than ~1, which would widen it to a negated 2.
 */
u8 *OverlayObject_SpawnConfiguredObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Func_020037ae(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        /*
         * The mask is a local initialised first and ANDed with the loaded
         * byte afterwards; folding the clear into one expression swaps the
         * two registers.
         */
        u32 masked = ~12;
        masked = masked & owner[9];
        owner[9] = (u8)masked;
        object[85] = 0;
        object[89] = 8;
        Actor_SetSpriteFlags(object, 0);
        Object_SetPalette(object, 14);
        Object_SetBlendMode(object, 1);
        result = object;
    } else {
        result = 0;
    }
    return result;
}

u8 *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *result;
    u8 *object = Func_02003806(kind, x, y, z);

    if (object != 0) {
        u8 *owner = *(u8 **)(object + 80);
        u32 mask = ~12;
        u32 low;
        mask = mask & owner[9];
        owner[9] = (u8)(mask | 4);
        object[85] = 0;
        object[89] = 8;
        Actor_SetSpriteFlags(object, 0);
        Object_SetPalette(object, 15);
        low = 0xfe;
        low = low & object[35];
        object[35] = (u8)(low | 2);
        result = object;
    } else {
        result = 0;
    }
    return result;
}

void SceneState_SetValue8Mode66(void)
{
    Func_02003c00(8, 66);
}

void OverlayObject_WaitUntilIdle(s32 *obj)
{
    s32 i = 60;

    while (i != 0) {
        Task_Wait(1);
        i--;
        if (obj[10] == 0) {
            break;
        }
    }
}

/*
 * A loader-relocated call word rather than a runtime address; it turns a value
 * and a duration into a per-frame step. Declared without a prototype, and the
 * call site passes two arguments.
 */

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void Func_02000cf0(struct MotionEffect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity[0];
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity[1];
    } while (0);
    velocity_z = effect->velocity[2];
    effect->position[2] += velocity_z;

    effect->velocity[0] = velocity_x - Math_Divide(velocity_x, 18);
    effect->velocity[2] = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    struct FieldActor *leader;
    s32 zero;
    struct EffectOptions options;
    s32 vec[3];

    Func_02003b90(a0);
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Task_Wait(1);
    *(s32 *)(Func_02003bca(0) + 12) = 0x820000;
    *(s32 *)(Func_02003bd6(0) + 72) = 0x8000;
    zero = 0;
    *(s32 *)(Func_02003be2(0) + 68) = zero;
    *(u8 *)(Func_02003bec(0) + 85) = zero;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(30);
    Audio_PlayCue(204);
    *(u8 *)(Func_02003c0a(0) + 85) = 3;
    Event_Wait(24);
    leader = (struct FieldActor *)Value1(Func_02003c1c, 0);
    options.palette = 7;
    options.update = (void (*)(union FieldObject *))Func_02000cf0;
    options.start_scale_x = 0xcccc;
    options.start_scale_y = 0xcccc;
    for (i = 0; i < 17; i++) {
        vec[0] = Math_Cos(i << 12);
        vec[1] = 0;
        vec[2] = Math_Sin(i << 12);
        vec[0] += vec[0] / 2;
        Func_0200190c(leader->x.fixed, leader->y.fixed, leader->z.fixed, vec[0], vec[1], vec[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1, &options);
    }
    Audio_PlayCue(188);
    Actor_SetAttachedEffect(0, 0x101);
    Actor_SetAnimation(0, 22);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    MapRender_WaitForValues();
    Actor_SetAttachedEffect(0, 0x100);
    *(s32 *)(Func_02003cc0(0) + 72) = 0x10000;
    *(s32 *)(Func_02003ccc(0) + 68) = 0x4000;
    Event_End();
}

void FieldScene_RunScene3c5SequenceA(s32 a0)
{
    u32 i;
    s32 record;
    s32 v5;

    Event_Begin();
    Camera_MoveTo(-1, -1, -1, 0);
    Map_Redraw();
    Task_Wait(1);
    record = Func_02003d24(0);
    *(s32 *)(record + 12) = 0x820000;
    record = Func_02003d30(0);
    *(s32 *)(record + 72) = 0x4000;
    v5 = 0;
    record = Func_02003d3c(0);
    *(s32 *)(record + 68) = v5;
    *(u8 *)(Func_02003d46(0) + 85) = v5;
    record = Func_02003d50(0);
    Actor_SetSpriteFlags(record, 0);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(10);
    Audio_PlayCue(204);
    *(u8 *)(Func_02003d70(0) + 85) = 3;
    record = Func_02003d7c(0);
    *(s32 *)(record + 40) = -0x50000;
    Func_02003d86(0);
    Func_02001c0a();
    Actor_SetChildValue(0, 15);
    Event_RequestExit(a0);
    Event_End();
}

/* Contiguous unnamed leaf-owner run for resource_3c5. */

/* Return this overlay's state block. */
s32 SceneData_SelectByRuntimeSelector(void)
{
    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}

s32 SceneData_SelectTableB5b8ByState(void)
{
    if (gGameState.scene == (s32)&Value_000000b0) {
        return (s32)Data_0200b5b8;
    }
    return 0;
}

u8 *SceneData_GetTableb5f8(void) { return (u8 *)0x0200b5f8; }

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_02000fdc[];
    extern u8 SecondaryOverlayData_02000fdc[];
    extern u8 TertiaryOverlayData_02000fdc[];
    extern u8 DefaultOverlayData_02000fdc[];

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_02000fdc;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_02000fdc;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_02000fdc;
    }
    return (s32)DefaultOverlayData_02000fdc;
}

void FieldScene_RunFourCallSequence(void)
{
    Event_Begin();
    Func_020011c0();
    Func_02002130();
    Event_End();
}

void SceneState_BranchOnActorEightOrNineTile(void)
{
    s32 *p = Func_02003f62(9);

    if ((Func_02003f6a(0)[2] >> 20) <= 12) {
        p = Func_02003f78(8);
        if ((p[2] >> 20) == 6) {
            if ((p[4] >> 20) == 20) {
                Func_0200223c();
                return;
            }
        }
    } else {
        if ((p[2] >> 20) == 18) {
            if ((p[4] >> 20) == 20) {
                Func_0200223c();
                return;
            }
        }
    }
    Func_0200408e();
}

/* resource_3c5 owner at 0x02001158, 42 bytes. */
void ResetSceneParametersAndFinishSetup(void)
{
    ResetSceneParameters(-1, -1, -1, 0);
    Func_0200402c(0, 6, 0);
    Event_CloseScreen();
    Event_WaitForScreen();
}

void FieldScene_RunStep11(void)
{
    Event_Begin();
    Func_020022e4();
    Event_RequestExit(11);
    Event_End();
}

void FieldScene_RunStep12WithPosition(void)
{
    Event_Begin();
    Func_02001ba4(0x1d00000, 0, 0x1220000, 223);
    Func_0200230c();
    Event_RequestExit(12);
    Event_End();
}

void FieldScene_RunStep13WithTwoPositions(void)
{
    Event_Begin();
    Func_02001bcc(0x8f0000, 0, 0x1220000, 223);
    Func_02001bdc(0x790000, 0, 0x11e0000, 253);
    Func_02002344();
    Event_RequestExit(13);
    Event_End();
}

void FieldScene_RunStep15(void)
{
    Event_Begin();
    Func_0200235c();
    Event_RequestExit(15);
    Event_End();
}

void FieldScene_RunStepWithValue2693(void)
{
    Event_Begin();
    Actor_SetAnimation(0, 1);
    Message_ShowCentered(0x2693, 1);
    Event_End();
}

void FieldScene_RunBranchingActorSequence(void)
{
    s32 record;

    GameFlag_Set(0x988);
    GameFlag_Set(0x98a);
    Event_Begin();
    Func_020041b4();
    Event_SetMessage(0x2702);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_WalkToAndWait(0, 0x128, 0x160);
    Actor_FaceDirection(0, 0xc000, 0);
    Event_Wait(10);
    Call4_02001238(Func_020041fe, 10, 16, 0, 0xc000);
    Call4_02001238(Func_0200420e, 1, -8, 16, 0xc000);
    Call4_02001238(Func_0200421c, 2, 8, 16, 0xc000);
    Call4_02001238(Func_0200422a, 3, 24, 16, 0xc000);
    Actor_WaitForMove(3);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x1180000, -1, 0x1200000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_SetAnimationAndWait(11, 3);
    Event_Wait(30);
    Event_ShowMessage(11, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0xc000, 0);
    Actor_FaceDirection(11, 0xc000, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(13, 2);
    Actor_RunRepeatedMotion(12, 2);
    Event_Wait(40);
    Actor_StartRepeatedMotion(13, 2);
    Actor_RunRepeatedMotion(12, 2);
    Event_Wait(40);
    Actor_StartRepeatedMotion(13, 2);
    Actor_RunRepeatedMotion(12, 2);
    Event_Wait(40);
    Actor_ShowEmote(12, 0x102, 50);
    Event_ShowMessage(12, 0);
    Event_Wait(10);
    Actor_FaceDirection(13, 0, 0);
    Event_Wait(25);
    Actor_RunRepeatedMotion(13, 2);
    Event_Wait(20);
    Event_ShowMessage(13, 0);
    Event_Wait(10);
    Actor_FaceDirection(12, 0x8000, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(13, 4);
    Event_Wait(20);
    Event_ShowMessage(13, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(11, 4);
    Event_Wait(20);
    Event_ShowMessage(11, 0);
    Event_Wait(20);
    Actor_ShowEmote(13, 0x107, 40);
    Event_Wait(10);
    Actor_FaceDirection(13, 0x4000, 0);
    Event_Wait(20);
    Event_ShowMessage(13, 0);
    Event_Wait(20);
    Actor_ShowEmote(13, 0x101, 75);
    Actor_ShowEmote(14, 0x101, 60);
    Actor_FaceDirection(12, 0x4000, 0);
    Event_Wait(20);
    Actor_FaceDirection(11, 0x4000, 0);
    Event_Wait(20);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Camera_MoveTo(0x1180000, -1, 0x1400000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(30);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(10, 4);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(20);
    Actor_ShowEmote(14, 0x105, 60);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(3, 4);
    Event_Wait(20);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(1, 2);
    Event_Wait(20);
    Event_OpenMessage(1, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(30);
        Actor_FaceDirection(10, 0x8000, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(10, 3);
        Event_Wait(30);
        Event_ShowMessage(10, 0);
        bump_step_02001238(1);
    } else {
        Event_Wait(30);
        Actor_FaceDirection(10, 0x8000, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(10, 4);
        Event_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(10, 0);
    }
    Event_Wait(10);
    Actor_ShowEmote(14, 0x101, 60);
    Actor_SetSpeed(14, 0x10000, 0x8000);
    Actor_WalkByAndWait(14, 0, 16);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceDirection(10, 0xc000, 0);
    Event_Wait(35);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_ShowEmote(3, 0x102, 40);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(1, 3);
    Event_Wait(30);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_ShowEmote(14, 0x100, 40);
    Event_OpenMessage(14, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(30);
        Actor_SetAnimationAndWait(14, 4);
        ((void (*)())Engine_EventWait)(20);
        Event_ShowMessage(14, 0);
        bump_step_02001238(1);
    } else {
        Event_Wait(30);
        ((void (*)())Engine_ActorSetAnimationAndWait)(14, 4);
        Event_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(14, 0);
    }
    Event_Wait(10);
    Actor_ShowEmote(10, 0x102, 50);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(13, 2);
    Event_Wait(20);
    Actor_SetSpeed(13, 0x14ccc, 0xa666);
    Actor_WalkByAndWait(13, 0, 16);
    Event_Wait(20);
    Event_ShowMessage(13, 0);
    Event_Wait(10);
    Actor_SetSpeed(12, 0x14ccc, 0xa666);
    Actor_WalkByAndWait(12, 0, 16);
    Event_Wait(20);
    Actor_ShowEmote(12, 0x107, 50);
    Event_ShowMessage(12, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(10, 0x102, 60);
    Actor_FaceDirection(10, 0x8000, 0);
    Event_Wait(25);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Event_OpenMessage(10, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(30);
        Actor_SetAnimationAndWait(10, 3);
        ((void (*)())Engine_EventWait)(30);
        Event_ShowMessage(10, 0);
        bump_step_02001238(1);
    } else {
        Event_Wait(30);
        Actor_SetAnimationAndWait(10, 4);
        Event_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(10, 0);
    }
    Event_Wait(10);
    Actor_FaceDirection(10, 0xc000, 0);
    Event_Wait(35);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Actor_FaceDirection(14, 0xb000, 0);
    Event_Wait(40);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceEachOther(12, 13, 50);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(30);
    Actor_FaceDirection(12, 0x4000, 0);
    Actor_FaceDirection(13, 0x4000, 0);
    Event_Wait(20);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(30);
    Actor_SetSpeed(12, 0x10000, 0x8000);
    Actor_SetSpeed(13, 0x10000, 0x8000);
    Actor_WalkBy(12, 32, 0);
    Actor_WalkByAndWait(13, 32, 0);
    Actor_WalkBy(12, 0, 16);
    Actor_WalkByAndWait(13, 16, 0);
    Actor_WalkTo(13, 0x158, 0x138);
    Actor_WalkToAndWait(12, 0x158, 0x150);
    Actor_SetAnimation(13, 1);
    Actor_FaceDirection(12, 0x8000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0x4000, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetSpeed(14, 0x10000, 0x8000);
    Actor_WalkToAndWait(14, 0x148, 0x138);
    Actor_FaceDirection(14, 0x8000, 0);
    Event_Wait(30);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_WalkToAndWait(11, 0x148, 0x148);
    Actor_FaceDirection(11, 0x8000, 0);
    Event_Wait(20);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetSpeed(3, 0x13333, 0x9999);
    Actor_SetAnimation(1, 2);
    record = Value1_02001238(Func_020048c2, 0);
    if (record != 0) {
        Actor_SetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetAnimation(2, 2);
    record = Value1_02001238(Func_020048f2, 0);
    if (record != 0) {
        Actor_SetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    Actor_SetAnimation(3, 2);
    record = Value1_02001238(Func_02004922, 0);
    if (record != 0) {
        Actor_SetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(3);
    Actor_SetPosition(3, 0, 0);
    Event_Wait(10);
    Event_End();
}

void FieldScene_RunActorEventSequence(void)
{
    s32 record;

    GameFlag_Set(0x989);
    Event_Begin();
    Func_02004a86();
    Event_SetMessage(0x272f);
    Actor_SetSpeed(0, 0x10000, 0x8000);
    Actor_WalkToAndWait(0, 0x128, 0x138);
    Actor_FaceDirection(0, 0, 0);
    Event_Wait(10);
    Func_02004acc(1, 0, 16, 0);
    Call4_02001238(Func_02004adc, 2, -16, -8, 0);
    Call4_02001238(Func_02004aea, 3, -16, 24, 0);
    Actor_WaitForMove(3);
    Event_Wait(20);
    Camera_SetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x1180000, -1, 0x1480000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Event_Wait(10);
    Actor_FaceDirection(10, 0xb000, 0);
    Event_Wait(10);
    Actor_ShowEmote(10, 0x100, 40);
    Event_ShowMessage(10, 0);
    Actor_Jump(10, 4, 13);
    Actor_Jump(10, 4, 30);
    Event_Wait(10);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_ShowEmote(12, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 40);
    Event_Wait(10);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_ShowEmote(13, 0x102, 40);
    Event_ShowMessage(13, 0);
    Event_Wait(20);
    Actor_ShowEmote(12, 0x101, 50);
    Actor_FaceDirection(12, 0x8000, 0);
    Event_Wait(25);
    Event_ShowMessage(12, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0x8000, 0);
    Actor_FaceDirection(11, 0x8000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Event_Wait(30);
    Event_Wait(10);
    Actor_RunRepeatedMotion(3, 2);
    Event_Wait(20);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_ShowEmote(1, 0x102, 40);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(10, 4);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(25);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Actor_SetSpeed(10, 0x10000, 0x8000);
    Actor_WalkByAndWait(10, 0, -40);
    Actor_FaceDirection(10, 0, 0);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceDirection(13, 0x4000, 0);
    Actor_FaceDirection(12, 0xc000, 0);
    Event_Wait(30);
    Actor_SetAnimation(12, 3);
    Actor_SetAnimationAndWait(13, 3);
    Event_Wait(30);
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_FaceDirection(12, 0x8000, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(30);
    Camera_FollowActor(10, 1);
    Actor_WalkByAndWait(10, 0, -32);
    Func_02004394_a();
    GameFlag_Clear(0x301);
    Camera_MoveTo(-1, -1, -1, 0);
    Event_Begin();
    Actor_ShowEmote(10, 0x102, 40);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(1, 0xc000, 0);
    Actor_FaceDirection(2, 0xc000, 0);
    Actor_FaceDirection(3, 0xc000, 0);
    Event_Wait(30);
    Actor_ShowEmote(11, 0x100, 0);
    Actor_ShowEmote(12, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 70);
    Camera_MoveTo(0x1180000, -1, 0x1380000, 1);
    Camera_WaitForMove();
    Event_Wait(10);
    Actor_ShowEmote(12, 0x102, 40);
    Event_ShowMessage(12, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(13, 4);
    Event_Wait(20);
    Event_ShowMessage(13, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(2, 2);
    Event_Wait(30);
    Actor_FaceDirection(2, 0x2000, 0);
    Event_Wait(20);
    Event_ShowMessage(2, 0);
    Event_Wait(10);
    Actor_FaceDirection(10, 0x4000, 0);
    Event_Wait(30);
    Actor_FaceDirection(3, 0xe000, 0);
    Event_Wait(30);
    Actor_ShowEmote(3, 0x102, 40);
    Actor_FaceDirection(0, 0x3000, 0);
    Event_Wait(20);
    Event_ShowMessage(3, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(20);
    Event_ShowMessage(1, 0);
    Event_Wait(20);
    Actor_ShowEmote(14, 0x103, 50);
    Actor_FaceDirection(14, 0x8000, 0);
    Event_Wait(20);
    Event_OpenMessage(14, 0);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(11, 0x8000, 0);
    Actor_FaceDirection(12, 0x8000, 0);
    Actor_FaceDirection(13, 0x8000, 0);
    Actor_FaceDirection(2, 0x1000, 0);
    Actor_FaceDirection(3, 0xe000, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(30);
        Actor_RunRepeatedMotion(14, 2);
        Event_Wait(20);
        Event_ShowMessage(14, 0);
        bump_step_02001238(1);
    } else {
        Event_Wait(30);
        Actor_RunRepeatedMotion(14, 2);
        Event_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(14, 0);
    }
    Event_Wait(10);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Actor_WalkByAndWait(10, 0, 16);
    Actor_FaceDirection(10, 0x2000, 0);
    Event_Wait(20);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, 0xa000, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(30);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_OpenMessage(14, 0);
    Event_Wait(40);
    Actor_FaceDirection(10, 0x5000, 0);
    Event_Wait(20);
    Actor_ShowEmote(10, 0x101, 60);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(30);
        Actor_FaceDirection(10, 0x2000, 0);
        Event_Wait(20);
        Actor_SetAnimationAndWait(10, 3);
        Event_Wait(30);
        Event_ShowMessage(10, 0);
        bump_step_02001238(1);
    } else {
        Event_Wait(30);
        Actor_FaceDirection(10, 0x2000, 0);
        Event_Wait(20);
        Actor_SetAnimationAndWait(10, 4);
        Event_Wait(20);
        bump_step_02001238(1);
        Event_ShowMessage(10, 0);
    }
    Event_Wait(10);
    Actor_ShowEmote(14, 0x102, 50);
    Event_ShowMessage(14, 0);
    Event_Wait(15);
    Actor_SetAnimationAndWait(10, 3);
    Event_Wait(30);
    Event_Wait(20);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(40);
    Actor_FaceDirection(14, 0x8000, 0);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_FaceDirection(10, 0x5000, 0);
    Event_Wait(25);
    Actor_RunRepeatedMotion(10, 2);
    Event_Wait(20);
    Actor_WalkToAndWait(10, 0x138, 0x138);
    Actor_FaceDirection(0, 0, 0);
    Actor_FaceDirection(10, 0x8000, 0);
    Event_Wait(25);
    Event_ShowMessage(10, 0);
    Event_Wait(10);
    Actor_FaceEachOther(0, 1, 0);
    Actor_FaceEachOther(3, 2, 0);
    Event_Wait(30);
    Actor_SetAnimation(0, 3);
    Actor_SetAnimation(1, 3);
    Actor_SetAnimation(3, 3);
    Actor_SetAnimationAndWait(2, 3);
    Event_Wait(30);
    Actor_SetSpeed(1, 0x13333, 0x9999);
    Actor_SetSpeed(2, 0x13333, 0x9999);
    Actor_SetSpeed(3, 0x13333, 0x9999);
    Actor_SetAnimation(1, 2);
    record = Value1_02001238(Func_020051ce, 0);
    if (record != 0) {
        Actor_SetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(1);
    Actor_SetPosition(1, 0, 0);
    Actor_SetAnimation(2, 2);
    record = Value1_02001238(Func_020051fe, 0);
    if (record != 0) {
        Actor_SetDestination(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(2);
    Actor_SetPosition(2, 0, 0);
    Actor_SetAnimation(3, 2);
    record = Value1_02001238(Func_0200522e, 0);
    if (record != 0) {
        Actor_SetDestination(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(3);
    Actor_SetPosition(3, 0, 0);
    Event_Wait(10);
    Actor_FaceDirection(0, 0xc000, 0);
    Actor_FaceDirection(10, 0xb000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_FaceDirection(11, 0xb000, 0);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_FaceDirection(13, 0xb000, 0);
    Event_Wait(30);
    Event_End();
}

void ActorPresentation_SetSceneCellByFlag985(void)
{
    if (GameFlag_IsSet(0x985) == 0) {
        s32 k5 = 17, k6 = 78;

        Func_0200526c(36, 78, 1, 2, k5, k6);
    } else {
        s32 k5 = 17, k6 = 78;

        Func_02005282(34, 78, 1, 2, k5, k6);
    }
}

void SceneState_ApplyRectAt32x78(void)
{
    {
        s32 k5 = 17, k6 = 78;

        Func_020052a6(32, 78, 1, 2, k5, k6);
    }
}

void FieldScene_RunScene3c5_020024d0(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x985) == 0) {
        GameFlag_Set(0x985);
        Audio_PlayCue(157);
        Event_Begin();
        Actor_SetDestination(8, 0x118, 240);
        Actor_SetDestination(9, 0x148, 240);
        Actor_WaitForMove(8);
        Actor_WaitForMove(9);
        Map_CopyCellAttributes(81, 14, 4, 1, 17, 14);
        Event_End();
        if (GameFlag_IsSet(0x989) == 0) {
            Func_02004046();
        }
    }
}

void FieldScene_RunScene3c5_02002548(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x985) != 0) {
        GameFlag_Clear(0x985);
        Audio_PlayCue(157);
        Event_Begin();
        Actor_SetDestination(8, 0x128, 240);
        Actor_SetDestination(9, 0x138, 240);
        Actor_WaitForMove(8);
        Actor_WaitForMove(9);
        Map_CopyCellAttributes(0, 14, 4, 1, 17, 14);
        Event_End();
        if (GameFlag_IsSet(0x301) != 0) {
            GameFlag_Clear(0x301);
        } else {
            GameFlag_Set(0x301);
        }
    }
}

void FieldScene_RunFlag985DialogueBranch(void)
{
    extern u8 *Data_03001ebc;

    u8 *base = Data_03001ebc;
    s16 *h;

    Event_Begin();
    h = (s16 *)(base + 0xcb8);
    if (h[0] != 0) {
        if (GameFlag_IsSet(0x985) == 0) {
            s32 k5 = 17, k6 = 78;

            Message_ShowCentered(0x1528, 1);
            Audio_PlayCue(155);
            Func_020053ea(35, 78, 1, 2, k5, k6);
            Event_Wait(10);
            Func_02005400_a(34, 78, 1, 2, k5, k6);
            Event_Wait(10);
            Func_02004afa();
        }
    } else {
        Event_SetMessage(0x2756);
        Event_ShowMessage(-1, 0);
    }
    Event_End();
}

void SceneState_SetRuntimeByte34(void)
{
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x34) = 1;
}

void ActorPresentation_PlaceActorTwelveAtTile20And12(void)
{
    s32 *p = Func_02005510(12);
    s32 a = p[2] >> 20;

    if (a == 20) {
        s32 b = p[4] >> 20;

        if (b == 12) {
            ((u8 *)p)[85] = 2;
            p[5] = 0x300000;
            ((u8 *)p)[35] = 2;
            {
                s32 k5 = a, k6 = b;

                Map_CopyCellAttributes(38, 12, 1, 1, k5, k6);
            }
        }
    }
}

void SceneActor_PushObjectAheadIfLevel(void)
{
    struct LevelCheckRecord *p = Func_02005552(0);
    struct LevelCheckRecord *q = Func_02004d68(p);
    s32 diff;

    if (q == 0) {
        return;
    }

    diff = q->y - p->y;

    if (diff >= 0) {
        /* Written with an empty arm on purpose: the reference branches away on
         * the *return* condition (`bge`), and spelling this as a plain
         * `if (diff >= 0x80000) return;` inverts it to `blt`. Arm order
         * decides the branch sense; no flag moves it. */
        if (diff < 0x80000) {
        } else {
            return;
        }
    } else if (p->y - q->y >= 0x80000) {
        return;
    }

    Func_020027f2();
}

void SceneActor_RunSlotZeroFacingCheck(void)
{
    extern u8 SceneStateBytes[];

    struct Obj *p = Func_0200558e(0);
    s32 x = Func_02004da4();
    s32 m = (p->f06 + 0x2000) & 0xc000;
    s32 r = -1;

    if (SceneStateBytes[498] == 1 || x == 0) {
        if (m == 0xc000) {
            r = Func_020056ba();
        }
        if (m == 0x4000) {
            r = Func_020056be();
        }
    }
    if (r != 0) {
        if (SceneStateBytes[498] != 1) {
            Func_02004e90();
        }
    }
}

s32 SceneData_SelectTableB91cByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_020027a0[];
    extern u8 SecondaryOverlayData_020027a0[];
    extern u8 TertiaryOverlayData_020027a0[];
    extern u8 DefaultOverlayData_020027a0[];

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_020027a0;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_020027a0;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_020027a0;
    }
    return (s32)DefaultOverlayData_020027a0;
}

void SceneState_ConfigureRegion82_7AndApply768(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 a = 18;
    s32 b = 7;

    Map_CopyCellAttributes(82, 7, 1, 2, a, b);
    Task_Wait(1);
    GameFlag_Set(768);
}

void SceneState_ApplyRectsAtActors8And9(void)
{
    s32 *p = Func_02005678(8);

    Actor_SetSpritePriority(8, 1);
    Actor_SetSpritePriority(9, 1);
    {
        s32 k5 = 5, k6 = 19;

        Map_CopyCellAttributes(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = 17, k6 = 19;

        Map_CopyCellAttributes(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = p[2] >> 20, k6 = p[4] >> 20;

        Map_CopyCellAttributes(3, 3, 1, 1, k5, k6);
    }
    {
        s32 *q = Func_020056ce(9);
        s32 k5 = q[2] >> 20, k6 = q[4] >> 20;

        Map_CopyCellAttributes(3, 3, 1, 1, k5, k6);
    }
}
