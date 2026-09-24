#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

#define NULL ((void *)0)
#define CreateOverlayObject Func_02002b26
#define SetOverlayObjectMode Func_02002bb0
#define SetOverlayObjectSlot Func_02002c50
#define Scene_GetRecord_1(args...) Func_020042de(args)
#define Scene_GetRecord_3(args...) Func_02004398(args)
#define REC_ID 16
#define QUERY_FLAG 0x200

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "STAGED_ACTOR.H"

/* Shared 22-byte head leaf proved identical for this overlay family. */
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

typedef struct {
    s32 v[6];
} PlacementResult;

typedef struct {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
    u8 rest : 4;
} Handle;

extern u8 Data_02000240_t[][2];
extern s32 Data_0200b738;
extern u8 Value_00000073;
extern u8 Value_00000074;
extern u8 Value_00000077;
extern u8 Value_00000079;
extern u8 Value_0000007a;
extern u8 Data_0200aeac[];
extern u8 Data_0200aef4[];
extern u8 Data_0200af3c[];
extern u8 Data_0200af84[];
extern u8 Data_0200afcc[];
extern u8 Data_0200ae7c[];
extern u8 Data_0200b06c[];
extern u8 Data_0200b0e4[];
extern u8 Data_0200b174[];
extern u8 Data_0200b2dc[];
extern u8 Data_0200b264[];
extern u8 Data_0200b3a8[];
extern u8 Data_0200b438[];
extern u8 Data_0200b498[];
extern u8 Data_0200b51c[];
extern u8 Data_0200b618[];
extern u8 Data_0200b39c[];
extern u8 *Data_03001ee0;
extern volatile s32 Data_03001e40;

u8 *Func_020051b4(void);
s32 Func_02001cc4();
s32 Func_02001d54();
void Func_020014d4();
void Func_02001386();
s32 Func_02001630();
s32 Func_02001662();
void Func_02003d20();
void Func_02003d5e();
s32 Func_02003da0();
s32 Func_02003dea();
void Func_020022e0();
void Func_020022f4();
void Func_02002308();
void Func_02002322();
void Func_02002336();
void Func_0200234a();
s32 Func_020040d6();
void Func_02004106();
void Func_0200417a();
void Func_02004186();
s32 Func_020042de();
s32 Func_02004398();
void Func_020023ac();
s32 Func_02003244();
void Func_02003d8c();
s32 Func_02004b02();
s32 Func_02004b14();
s32 Func_02005160();
int Func_02002798(void);
void Func_020018e0(void);
s32 Func_02001caa(PlacementResult *out);
void Func_02001e56(PlacementResult out);
void Func_02004bd4();
s32 Func_020029e6();
u8 *Func_02005240();

/*
 * Staged actor height wait for overlay resource_3b3. The callee name refers
 * to its own call word rather than to a shared runtime address.
 */

/*
 * Steps a record down by a tenth of a unit per frame until it reaches the
 * clamp at 0x1999 -- resource_3b3.  The eighty-byte owner includes its
 * alignment halfword and four pool words: the clamp 0x1999, -0x1999,
 * -0xcccc and the loop bound 0x1998, which is one less than the clamp.
 * None of them is an address, and each is reached only through a
 * pc-relative load.
 */

/* Apply the actor's low four counter bits as its animation mode. */

/* Begin a scene, attempt the forward transition, and fall back to pushing the
 * obstructing actor when the transition cannot run.  Complete 28-byte owner
 * from the prologue at 0x02001528 through return/alignment at 0x02001543. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Record id used for the single record configured below in either branch. */

/* Flag/id value passed to the query call and to the two closing calls, and
 * written into the flag byte at +85 when the query call returned zero. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    s32 Func_02001268();

    return f();
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    s32 Func_02001268();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    s32 Func_02001268();

    f(a0, a1, a2);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    s32 Func_02001268();

    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_0200263c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_020023f6(s32 x, s32 y, s32 z, s32 kind,
                          s32 arg4, s32 arg5, u32 flags, void *descriptor);

void Func_020024a0(s32 x, s32 y, s32 z, s32 kind,
                          s32 arg4, s32 arg5, u32 flags, void *desc);

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

/*
 * Poll an overlay object until it settles, then reset it -- resource_3b3.
 */

/* Declared without a prototype; the call site passes one argument. */
void *OverlayObject_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Object_Create(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Actor_SetSpriteFlags(obj, 0);
        Object_SetPalette(obj, 14);
        Object_SetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}

/* The size test is unsigned, so out20 and out16 are u32. The two rec words
 * that travel on the stack are read into their own locals, which puts both
 * loads before both stores. The +0x23 flag set takes its mask from a local
 * assigned first, which fixes the register the mask lands in. */

/*
 * Apply a placement query and tag the object. Func_02001cc4 fills out20 and
 * out16, a 24-byte record of which only rec[2] and rec[4] are read back, and
 * two further out-params whose written values are never read -- only the
 * pointers matter. On success the object is tagged at +0x23 and one of two
 * near-identical finishing calls runs; on failure the object is untouched.
 */

/* The callees are declared at their in-overlay entry points. Declaring them at
 * their veneer addresses in ROM space would route every call through a
 * veneer. */

/*
 * Apply a placement query to an actor. Func_02001d54 is an out-param helper:
 * it fills out20 and out16, a 24-byte record of which only rec[2] and rec[4]
 * are read back, and out12 and out8 passed on the stack. Its field semantics
 * are not established. On success the values thread unchanged into the two
 * finishing calls in that shape; on failure the object is left untouched.
 */

/*
 * Wait at most sixty polls for the object's +12 to reach its +20, then clear
 * +0x28, set +0x3c, and mirror +20 back into +12. Engine_TaskWait(1) is taken
 * to be a one-frame wait, and the field offsets are named by position and
 * not verified.
 */
void OverlayObject_WaitUntilSettledAndReset(u8 *obj)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        Task_Wait(1);
        if (*(u32 *)(obj + 12) == *(u32 *)(obj + 20)) {
            break;
        }
        cnt--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
    *(u32 *)(obj + 12) = *(u32 *)(obj + 20);
}

/*
 * Polls for up to sixty ticks until the height at +12 falls to the target
 * at +20 or to limit, then clears +0x28 and parks +0x3c. The height is not
 * mirrored back from +20 afterwards. The owner at 0x02000da8 is 52 bytes
 * and carries no pool.
 */
void SceneActor_WaitHeightBelowLimit(u8 *obj, s32 limit)
{
    s32 cnt = 60;

    for (;;) {
        if (cnt == 0) {
            break;
        }
        Task_Wait(1);
        if (*(s32 *)(obj + 12) <= *(s32 *)(obj + 20)) {
            break;
        }
        if (*(s32 *)(obj + 12) <= limit) {
            break;
        }
        cnt--;
    }

    *(u32 *)(obj + 0x28) = 0;
    *(u32 *)(obj + 0x3c) = 0x80000000;
}

s32 SceneActor_ApplyPlacementQueryAndTag(u8 *no)
{
    u8 *obj = Actor_Get(no);
    u32 out20, out16;
    s32 out12, out8;
    s32 rec[6];
    s32 r2, r4;
    u8 mask;

    if (Func_02001cc4(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    r2 = rec[2];
    r4 = rec[4];
    Map_CopyCellAttributes(2, 2, out20, out16, r2, r4);

    Object_SetAnimation(obj, 4);
    mask = 2;
    obj[0x23] = obj[0x23] | mask;

    if (out20 > out16) {
        Map_CopyCellsTo(70, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    } else {
        Map_CopyCellsTo(68, 40, rec[2] + 32, rec[4] + 2, out20, out16);
    }

    return 1;
}

s32 SceneActor_ApplyPlacementQuery(u8 *no)
{
    u8 *obj = Actor_Get(no);
    s32 out20, out16, out12, out8;
    s32 rec[6];

    if (Func_02001d54(no, &out20, &out16, rec, &out12, &out8) == 0) {
        return 0;
    }

    {
        s32 x = out12 + rec[2];
        s32 z = out8 + rec[4];

        Map_CopyCellAttributes(x, z, out20, out16, rec[2], rec[4]);
        Func_020014d4(0, rec[2], rec[4], out20, out16, 255);
    }

    Object_SetAnimation(obj, 1);
    obj[0x23] &= 0xfd;

    return 1;
}

/*
 * Each Func_ name labels the call word of one call site rather than a
 * runtime address.  The first call is made before r0 is disturbed, so the
 * index is passed straight through instead of being materialised again.  The
 * two exits differ: the thirty-two frame cap returns without pinning, while
 * the clamp path pins the record to exactly 0x1999.
 */
void StagedActor_StepDownUntilClamp(s32 index)
{
    u8 *obj = Actor_Get(index);
    u32 cnt;

    obj[0x55] = 0;

    cnt = 0;
    for (;;) {
        if (cnt > 31) return;
        Task_Wait(1);
        *(s32 *)(obj + 28) += -0x1999;
        *(s32 *)(obj + 12) += -0xcccc;
        cnt++;
        if (*(s32 *)(obj + 28) <= 0x1998) {
            *(s32 *)(obj + 28) = 0x1999;
            return;
        }
    }
}

void FieldScene_RunPrimarySequence(void)
{
    s32 Func_02001268();

    s32 rec;
    s32 flag;
    s32 p6;
    s32 p5;
    s32 record;
    s32 v1;
    s32 v2;
    s32 v3;
    u8 *base;
    u8 slot16[40];

    rec = Actor_Get(0);
    flag = Data_03001e40 & 3;
    if (flag == 0) {
        base = slot16;
        *(s32 *)(base + 4) = 10;
        *(s32 *)(base + 8) = 0xb333;
        *(s32 *)(base + 12) = 0xb333;
        v1 = Random_Next();
        p6 = *(s32 *)(rec + 8) + ((((u32)((v1 << 4) + v1) >> 16) - 8) << 16);
        v2 = Random_Next();
        p5 = *(s32 *)(rec + 16) + ((((u32)((v2 << 4) + v2) >> 16) - 8) << 16);
        v3 = Random_Next();
        record = Math_Divide((((u32)((v3 << 2) + v3) >> 16) << 16) + 0x30000, 10);
        Call8(Func_02001386, p6, *(s32 *)(rec + 12), p5, 0, record, flag, 0x90001, (s32)base);
    }
}

s32 Func_02001268(void)
{
    s32 Func_02001268();

    u8 *rec;
    u8 *pflag;
    s32 saved;
    s32 mode;
    s32 *p;
    s32 buf[3];

    rec = Actor_Get(0);
    pflag = rec + 85;
    saved = *pflag;
    mode = (*(u16 *)(rec + 6) + 0x2000) & 0xc000;
    if (Data_02000240_t[249][0] != 0) {
        return 0;
    }
    p = buf;
    p[0] = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(s32 *)(rec + 12);
    p[2] = (*(s32 *)(rec + 16) & -0x100000) + 0x80000;
    Call3(Func_02003d20, 0x100000, mode, (s32)p);
    if (Value2(Func_02003da0, (s32)rec, (s32)p) == 1) {
        goto reject;
    }
    if (Value2(Func_02001630, (s32)p, (s32)rec) != 0) {
        goto reject;
    }
    p[0] = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    p[1] = *(s32 *)(rec + 12);
    p[2] = (*(s32 *)(rec + 16) & -0x100000) + 0x80000;
    Call3(Func_02003d5e, 0x200000, mode, (s32)p);
    if (Value2(Func_02001662, (s32)p, (s32)rec) != 0) {
        goto reject;
    }
    if (Value2(Func_02003dea, (s32)rec, (s32)p) != 0) {
        goto reject;
    }
    Event_Begin();
    Object_SetAnimation((s32)rec, 6);
    Task_Wait(6);
    Audio_PlayCue(152);
    Object_SetAnimation((s32)rec, 7);
    *(s32 *)(rec + 48) = 0x30000;
    *(s32 *)(rec + 52) = 0x20000;
    *(s32 *)(rec + 40) = 0x40000;
    *pflag &= 126;
    Actor_SetSpriteFlags((s32)rec, 0);
    Actor_MoveToAndWait(0, *(s16 *)((u8 *)p + 2), *(s16 *)((u8 *)p + 10));
    Object_SetAnimation((s32)rec, 6);
    Actor_SetSpriteFlags((s32)rec, 1);
    *pflag = saved;
    Event_End();
    return 1;
reject:
    return 0;
}

void Func_020013b0(union FieldObject *object)
{
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;

    object->effect.x += object->effect.velocity_x;
    object->effect.y += object->effect.velocity_y;
    object->effect.z += object->effect.velocity_z;
    velocity_x = object->effect.velocity_x;
    velocity_y = object->effect.velocity_y;
    velocity_z = object->effect.velocity_z;
    object->effect.velocity_x = velocity_x - Math_Divide(velocity_x, 10);
    object->effect.velocity_y = velocity_y - Math_Divide(velocity_y, 3);
    object->effect.velocity_z = velocity_z - Math_Divide(velocity_z, 10);
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    object->effect.sprite->rotation += object->effect.spin;
}

s32 SceneActor_ApplyCounterLowBitsAsMode(u8 *actor)
{
    Object_SetPalette(actor, *(u16 *)(actor + 100) & 15);
    return 0;
}

/* Contiguous unnamed leaf-owner run for resource_3b3. */
s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200aeac;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200aef4;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200af3c;
    }
    if (v == (s32)&Value_00000079) {
        return (s32)Data_0200af84;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200afcc;
    }
    return (s32)Data_0200ae7c;
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableb014(void)
{
    return (u8 *)0x0200b014;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200b06c;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200b0e4;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200b174;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200b2dc;
    }
    return (s32)Data_0200b264;
}

void FieldScene_RunTransitionOrFallback(void)
{
    Event_Begin();
    if (Func_02002798() == 0)
        Func_020018e0();
    Event_End();
}

/*
 * Scene state reset for overlay resource_3b3. The callee name refers to its
 * own call word rather than to a shared runtime address.
 */
void SceneState_ApplyPlacementResult(void)
{
    PlacementResult out;

    Event_Begin();
    if (Func_02001caa(&out) != 0)
        Func_02001e56(out);
    Event_End();
}

void CopyAndOffsetCoordinatePreset(void)
{
    u32 *destination;
    const u32 *source;
    u16 *coordinates;

    source = (const u32 *)0x03001ad4;
    destination = (u32 *)0x0200b72c;
    *destination++ = *source++;
    *destination++ = *source++;
    *destination = *source;
    coordinates = (u16 *)0x0200b72c;
    coordinates[1] += 0xc0;
    coordinates[3] += 0xc0;
    coordinates[5] += 0xc0;
}

void FieldScene_RunScene3b3SequenceA(void)
{
    s32 record;

    record = GameFlag_IsSet(0x200);
    if (record == 0) {
        Func_020022e0(10, 19, 16, 5, record, 10, 31);
        Func_020022f4(10, 51, 16, 5, 1, 10, 31);
        Func_02002308(42, 51, 16, 5, 2, 10, 31);
    } else {
        Func_02002322(10, 19, 16, 5, 0, 10, 31);
        Func_02002336(10, 83, 16, 5, 1, 10, 31);
        Func_0200234a(42, 83, 16, 5, 2, 10, 31);
    }
    Data_0200b738 = 0;
    Func_020040d6(0x20095cd, 0xc80);
    Task_Wait(1);
    Func_02004106(1, 0, 0x2009579);
    Audio_PlayCue(231);
    Data_0200b738 = 0;
    do {
        Task_Wait(1);
    } while (++Data_0200b738 <= 100);
    Audio_PlayCue(0x121);
    if (GameFlag_IsSet(0x200) == 0) {
        Map_CopyCellsTo(0, 32, 32, 0, 32, 32);
        Map_CopyCellsTo(32, 32, 64, 0, 32, 32);
    } else {
        Map_CopyCellsTo(0, 64, 32, 0, 32, 32);
        Map_CopyCellsTo(32, 64, 64, 0, 32, 32);
    }
    Task_Wait(1);
    Func_02004186(1, 0, 0);
    Task_Wait(1);
    Call1(Func_0200417a, 0x20095cd);
    Map_Redraw();
    Task_Wait(30);
}

/* Runs one of two near-identical setup sequences for record REC_ID and
 * records 9-15, chosen by the query call's return value; each sequence ends
 * with its own closing call carrying QUERY_FLAG. */
void FieldScene_RunFlaggedDisplayScene(void)
{
    u32 i;
    u8 *queried;
    u8 *record;

    Event_Begin();
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x1190000, -1, 0x1b00000, 1);
    Camera_WaitForMove();
    Message_ShowCentered(0x1528, 1);
    queried = GameFlag_IsSet(QUERY_FLAG);
    if (queried == 0) {
        Audio_PlayCue(232);
        Map_AnimateCells(0x200ada8, 84, 24);
        Event_Wait(30);
        Audio_PlayCue(240);
        Actor_SetSpritePriority(REC_ID, 1);
        /* Flag byte at +85: cleared, since queried is zero here. */
        *(u8 *)(Scene_GetRecord_1(REC_ID) + 85) = queried;
        record = Actor_Get(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Actor_SetPosition(REC_ID, 0x1100000, 0x1a00000);
        Actor_SetAnimation(REC_ID, 1);
        Map_AnimateCells(0x200adfc, 80, 24);
        Map_AnimateCells(0x200ae50, 80, 28);
        Map_CopyCellsTo(65, 40, 16, 27, 2, 4);
        FieldScene_RunScene3b3SequenceA();
        SceneActor_ApplyPlacementQueryAndTag(9);
        SceneActor_ApplyPlacementQueryAndTag(10);
        SceneActor_ApplyPlacementQueryAndTag(11);
        SceneActor_ApplyPlacementQueryAndTag(12);
        SceneActor_ApplyPlacementQueryAndTag(13);
        SceneActor_ApplyPlacementQueryAndTag(14);
        SceneActor_ApplyPlacementQueryAndTag(15);
        Map_CopyCellAttributes(24, 3, 1, 1, 24, 8);
        GameFlag_Set(QUERY_FLAG);
    } else {
        Audio_PlayCue(232);
        Map_AnimateCells(0x200add2, 84, 24);
        Event_Wait(30);
        Audio_PlayCue(230);
        /* Flag byte at +85: cleared unconditionally in this branch. */
        *(u8 *)(Scene_GetRecord_3(REC_ID) + 85) = 0;
        record = Actor_Get(REC_ID);
        *(s32 *)(record + 12) = -0x200000;
        Actor_SetPosition(REC_ID, 0x1100000, 0x1b40000);
        Actor_SetAnimation(REC_ID, 2);
        Map_CopyCellsTo(65, 45, 16, 27, 2, 4);
        Map_AnimateCells(0x200ae26, 80, 24);
        FieldScene_RunScene3b3SequenceA();
        SceneActor_ApplyPlacementQuery(9);
        SceneActor_ApplyPlacementQuery(10);
        SceneActor_ApplyPlacementQuery(11);
        SceneActor_ApplyPlacementQuery(12);
        SceneActor_ApplyPlacementQuery(13);
        SceneActor_ApplyPlacementQuery(14);
        SceneActor_ApplyPlacementQuery(15);
        Map_CopyCellAttributes(24, 4, 1, 1, 24, 8);
        GameFlag_Clear(QUERY_FLAG);
    }
    Event_End();
}

void FieldScene_RunScene3b3_02001fd4(void)
{
    Event_Begin();
    if (Value0(Func_02003244) == 0) {
        *(u8 *)(Func_02004b02(0) + 85) &= 254;
        *(u8 *)(Func_02004b14(0) + 35) &= 254;
        Func_020023ac();
        Func_02003d8c();
        {
            u8 bits = 1;
            u8 *flags = (u8 *)Actor_Get(0) + 85;
            u8 value = *flags;

            value |= bits;
            *flags = value;
            flags = (u8 *)Actor_Get(0) + 35;
            bits |= *flags;
            *flags = bits;
        }
    }
    Event_End();
}

/* Complete one-call wrapper through interworking return and alignment. */
void FieldScene_RunSingleStep(void)
{
    Func_02004bd4();
}

/* Complete scene/entity linker through return and its sole pool word. */
void SceneState_LinkActorZeroToWork24(void)
{
    u8 *obj = Actor_Get(0);
    *(u8 **)(Data_03001ee0 + 24) = obj;
    obj[98] = 1;
}

/*
 * Clears Data_03001ee0[+24] and one flag byte on the object returned by
 * Engine_ActorGet. The 28-byte owner at 0x0200209c includes its one pool
 * word, the Data_03001ee0 pointer.
 */
void SceneState_ClearWord24AndObjectByte62(void)
{
    u8 *obj = Actor_Get(0);

    *(s32 *)(Data_03001ee0 + 24) = 0;
    obj[0x62] = 0;
}

/*
 * Place a staged actor at object ten's grid cell -- resource_3b3.
 */

/*
 * The Func_ aliases name the call words encoded in the overlay image. The
 * declarations are old-style because the call sites vary in arity.
 */

/*
 * resource_3b3 @ 0x020020b8 (56 bytes including trailing alignment).
 *
 * Compares an actor with slot zero.  When it is farther right, bit 1 at +35
 * is cleared and then restored only if the actor is also above slot zero.
 * The function always returns zero.
 */
s32 SceneActor_UpdateBit1ByPositionToSlotZero(u8 *actor)
{
    u8 *ref = Actor_Get(0);

    if (*(s32 *)(actor + 16) > *(s32 *)(ref + 16)) {
        actor[35] = (u8)(actor[35] & 0xfd);
        if (*(s32 *)(actor + 12) < *(s32 *)(ref + 12))
            actor[35] = (u8)(actor[35] | 2);
    }

    return 0;
}

void FieldScene_RunScene3b3_020020f0(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value0(Engine_ActorGet);
    Event_Begin();
    *(s32 *)(rec7 + 108) = 0x200a0b9;
    Map_CopyCellAttributes(20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
    GameFlag_Set((a0 + 0x1f5));
    Value2(Engine_ActorEnableActionCallback, a0, 0x200ad64);
    Event_End();
}

/* Contiguous unnamed leaf-owner run for resource_3b3. */

/* Complete 12-byte actor-11 wrapper before 0x02002150. */
void FieldScene_RunActor11Step(void)
{
    FieldScene_RunScene3b3_020020f0(11);
}

/* Complete 12-byte actor-12 wrapper before 0x0200215c. */
void FieldScene_RunActor12Step(void)
{
    FieldScene_RunScene3b3_020020f0(12);
}

void FieldScene_RunScene3b3_0200215c(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    u8 *p6;

    rec7 = Value1(Engine_ActorGet, 0);
    record = Actor_Get(13);
    p6 = *(s32 *)0x03001f30;
    if ((*(s32 *)(record + 8) >> 20) == (*(s32 *)(rec7 + 8) >> 20)) {
        if ((*(s32 *)(record + 16) >> 20) != (*(s32 *)(rec7 + 16) >> 20)) {
            goto L_02002198;
        }
        GameFlag_Set(0x203);
        p6[53] = 1;
    } else {
        L_02002198:;
        GameFlag_Clear(0x203);
    }
}

void SceneEffect_SpawnRandomizedParticle(void)
{
    u8 descriptor[40];
    u8 *d;
    s32 spread;
    s32 secondary;
    u32 draw;
    u32 mask;

    if ((Data_03001e40 & 2) != 0)
        return;
    if ((Data_03001e40 & 7) == 0)
        Audio_PlayCue(136);

    d = descriptor;
    *(s32 *)(d + 4) = 10;
    *(s32 *)(d + 8) = 0x8000;
    *(s32 *)(d + 12) = 0x8000;
    *(s32 *)(d + 16) = 0x19999;
    *(s32 *)(d + 20) = 0x19999;
    draw = (u32)Random_Next();
    mask = 0x0ffff000;
    mask &= draw;
    *(u16 *)(d + 32) = (u16)mask;
    *(s32 *)(d + 36) = 0x020093b1;

    draw = (u32)Random_Next();
    spread = -((s32)((draw * 5) >> 16) * 0x10000 + 0x60000);
    spread /= 2;
    draw = (u32)Random_Next();
    secondary = -((s32)((draw * 5) >> 16) * 0x10000 + 0x50000);

    Func_020023f6(0x01440000, 0x00300000, 0x00e40000, spread,
                  secondary, 0, 0x014d0000, d);
}

s32 SceneEffect_SpawnRandomEffectEveryEightFrames(u8 *actor)
{
    u8 desc[40];
    u8 *p;
    u32 phase = (u32)Data_03001e40 & 7;
    s32 x;
    s32 y;
    s32 z;
    s32 scale;

    if (phase != 0)
        return 0;

    p = desc;
    *(s32 *)(p + 4) = 7;
    *(s32 *)(p + 8) = 0xb333;
    *(s32 *)(p + 12) = 0xb333;

    x = *(s32 *)(actor + 8) + (((s32)(((u32)Random_Next() * 17) >> 16) - 8) << 16);
    y = *(s32 *)(actor + 12) + ((s32)(((u32)Random_Next() * 17) >> 16) << 16);
    z = *(s32 *)(actor + 16) + (((s32)(((u32)Random_Next() * 17) >> 16) - 8) << 16);
    scale = Math_Divide((s32)(((u32)Random_Next() * 5) >> 16) * 0x10000 + 0x30000, 10);

    Func_020024a0(x, y, z, 0, scale, (s32)phase, 0x00090001, p);
    return 0;
}

/*
 * Fetch object ten, shift its +8 and +16 fixed-point fields down to grid
 * coordinates, and place there. The last two literal arguments go on the
 * stack.
 */
void StagedActor_PlaceAtObjectTenCell(void)
{
    u8 *obj = Actor_Get(10);
    s32 x;
    s32 z;

    Event_Begin();

    x = *(s32 *)(obj + 8) >> 20;
    z = *(s32 *)(obj + 16) >> 20;

    Func_020029e6(2, x, z, 1, 1, 0);
    Event_End();
}

s32 SceneData_SelectSecondaryTableBySceneId(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200b3a8;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200b438;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200b498;
    }
    if (v == (s32)&Value_00000079) {
        return (s32)Data_0200b51c;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200b618;
    }
    return (s32)Data_0200b39c;
}

void FieldScene_RunScene3b3_0200263c(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Func_02005160();
    if (GameFlag_IsSet((a0 + 0x1f5)) != 0) {
        Object_SetAnimation(rec7, 5);
        *(s32 *)(rec7 + 108) = 0x200a0b9;
        Map_CopyCellAttributes(20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
        Engine_ActorEnableActionCallback(a0, 0x200ad64);
    }
}

void OverlayObject_SetCallbackAndMode2(void)
{
    u8 *obj = Func_020051b4();
    u8 *base = obj;
    u8 zero = 0;

    obj += 0x22;
    *obj = 2;
    base[0x55] = zero;
    *(u32 *)(base + 0x6c) = 0x02009aa9;
}

void SceneActor_CheckActors8To11NearSlotZero(void)
{
    u8 *hero = Actor_Get(0);
    u32 selector = 8;
    u8 *actor;

loop:
    actor = Actor_Get(selector);

    if (*(s32 *)(hero + 12) / 0x10000 != *(s32 *)(actor + 12) / 0x10000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 16) > *(s32 *)(actor + 16) - 0x80000
        || *(s32 *)(hero + 16) <= *(s32 *)(actor + 16) - 0x180000)
        goto mark_and_continue;

    if (*(s32 *)(hero + 8) - 0x100000 > *(s32 *)(actor + 8)
        || *(s32 *)(actor + 8) >= *(s32 *)(hero + 8) + 0x100000)
        goto continue_loop;

    {
        Handle *handle = *(Handle **)(actor + 80);
        Actor_SetSpritePriority(0, handle->mode);
    }
    goto done;

mark_and_continue:
    {
        u8 *mark = Func_02005240(0) + 35;
        u8 bit = 1;
        bit |= *mark;
        *mark = bit;
    }

continue_loop:
    selector++;
    if (selector <= 11)
        goto loop;

done:
    return;
}
