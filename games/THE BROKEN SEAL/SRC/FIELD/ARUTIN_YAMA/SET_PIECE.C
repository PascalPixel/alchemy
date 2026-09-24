#include "TYPES.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define CalculateFacingAngle Func_02003afa
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Value3(Engine_ActorWalkToAndWait, a0, a1, a2)
#define Audio_PlayCue_1(a0) Value1(Engine_AudioPlayCue, a0)
#define ObjectMotion_ResetAndSetPosition_2(a0, a1, a2) Value3(Engine_ActorSetDestination, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_1(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetVariantCallback_1(a0, a1) Value2(Engine_ActorStartRepeatedMotion, a0, a1)
#define ObjectMotion_CommitCurrentPositionAndActivate_1(a0) Value1(Engine_ActorWaitForMove, a0)
#define SharedWorkData_SetFirstAndSecondFields_1(a0, a1) Call2(Func_02006c72, a0, a1)
#define BattleEffect_ComputeWeightedResultAndDispatch_1(a0, a1) Call2(Func_02006c6a_a, a0, a1)
#define WORK_BYTE_22B (Data_02000240[0x22B])
#define Scene_GetRecord_1(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_2(a0) Value1(Engine_ActorGet, a0)
#define Scene_GetRecord_3(a0) Value1(Engine_ActorGet, a0)
#define ObjectMotion_EnableActionAndSetCallback_1(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_EnableActionAndSetCallback_2(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_1(a0, a1) Value2(Func_02006c9e, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_2(args...) Func_02006e7e_a(args)
#define ObjectMotion_EnableActionAndSetCallback_5(a0, a1) Value2(Engine_ActorEnableActionCallback, a0, a1)
#define ObjectMotion_MarkActiveAndSetActionCallback_3(args...) Func_02006f52(args)
#define SCENE_PHASE (*(s32 *)(work + 0x1c0))
#define SCENE_FIELD_1C8 (*(s32 *)(work + 0x1c8))

#include "FACING_OBJECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

enum {
    /* Message 0x182 + 202. */
    ITEM_FROST_JEWEL = 202
};

enum SetPieceMessage {
    MSG_ROBIN_FLIPPED_SWITCH = 0x1528,
    MSG_WE_DID_ROBIN_WE_BEAT = 0x190c,
    MSG_GUARDIAN_STATUES_WERE_CREATED_LONG = 0x1910
};

struct Frame {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern u8 Value_0000004d;
extern u8 Value_0000004f;
extern u8 Value_00001018;   /* The pooled constant 0x1018. */
extern u8 Value_0000004e;
extern u8 Value_00000050;
extern u8 Value_00000051;
extern u8 Value_00000052;
extern u8 Value_00000053;
extern u8 Value_00000054;
extern u8 Value_00000055;
extern u8 Value_00000056;
extern u8 Value_00000057;
extern u8 Data_0200c194[];
extern u8 Data_0200c20c[];
extern u8 Data_0200c26c[];
extern u8 Data_0200c314[];
extern u8 Data_0200c3ec[];
extern u8 Data_0200c464[];
extern u8 Data_0200c524[];
extern u8 Data_0200c59c[];
extern u8 Data_0200c644[];
extern u8 Data_0200c704[];
extern u8 Data_0200c77c[];
extern u8 Data_0200c164[];
extern u8 Data_0200c80c[];
extern u8 Data_0200c83c[];
extern u8 Data_0200c85c[];   /* Image offset 0x485c. */
extern u8 Data_0200c940[];
extern u8 Data_0200c9a0[];
extern u8 Data_0200ca00[];
extern u8 Data_0200ca60[];
extern u8 Data_0200caa8[];
extern u8 Data_0200cb68[];
extern u8 Data_0200cb98[];
extern u8 Data_0200cc40[];
extern u8 Data_0200ccd0[];
extern u8 Data_0200c928[];
extern unsigned char Value_00004ccc;
extern u8 Data_00000000[];
extern u8 Data_03001ebc[];
extern u8 Data_00001953[];
extern s16 Data_02000240_t[][1];
extern s32 Data_0200d240[3];
extern s32 Data_0200d23c;
extern s32 Data_0200d240[3];   /* image offset 0x5240 */
extern u8 *Func_02006652(s32 arg0, s32 arg1, s32 arg2, s32 arg3);  /* site veneer -> Func_080090c8 */
extern s32 Data_0200d1b0;   /* In-image writable data at image offset 0x51b0. */
extern u8 Data_0200c0e4[];
extern u8 Data_0200c12c[];

void Func_02003cd8(s32, s32);
void Func_02004042(s32, s32);
void Func_0200403a(s32, s32);
void Func_0200406e(s32, s32);
void Func_02004066(s32, s32);
s32 Func_02003afa(s32, s32);
void Func_0200409a(s32, s32);
void Func_02004092(s32, s32);
u8 *Func_02004812(s32);
u8 *Func_02004826(s32);
u8 *Func_02006fc6(s32 id);
u8 *Func_02006fd6(s32 id);
void Func_02004208();
void Func_02004236();
void Func_020042fc();
void Func_0200432a();
void Func_02004410();
void Func_0200443e();
void Func_02004844(void);
void Func_020048da();
void Func_02004976();
s32 Func_0200486e();
void Func_02004c4a();
void Func_02004c5e();
void Func_0200500a();
void Func_0200abe1();
s32 Func_02004ff6();
s32 Func_0200501a();
s32 Func_02005040_a();
void Func_02005d7c();
void Func_020050b4();
void Func_02005f1a_a();
void Func_02006212();
s32 Func_02006514();
s32 Func_0200665a();
s32 Func_0200668a();
s32 Func_020066ec();
void Func_0200696e();
s32 Func_02006a3c();
void Func_0200364c();
void Func_020037a0();
void Func_020069d4();
void Func_02006c6a_a();
void Func_02006c72();
void Func_0200479c(void);
s32 Func_02006c9e();
void Func_02006d34();
void Func_02006d54_a();
void Func_02006e7e_a();
void Func_02006f52();
void Func_0200affd();

/*
 * resource_3a4 owner at 0x02003460, 34 bytes with no literal pool: clear
 * bit 0 of the scene record's flags byte at +89, then place a marker at the
 * record's x and z at level 255. Both callees are reached through veneers.
 */

/* Byte at offset 0x22B of the shared work buffer. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_0200d238[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_0200d238[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_0200d238[];

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_0200d238[];

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_0200d238[];

    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_0200d238[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_0200d238[];

    f(a0, a1, a2, a3);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_0200d238[];

    return f(a0, a1);
}

static __inline__ s32 Value0(s32 (*f)())
{
    extern u8 Data_0200d238[];

    return f();
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_0200d238[];

    return f(a0, a1, a2, a3);
}

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
static __inline__ void Call0(void (*f)())
{
    extern u8 Data_0200d238[];

    f();
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_0200d238[];

    return f(a0, a1, a2);
}

/* Phase/status word at 0x1c0, and a companion word at 0x1c8, of the shared
 * scene work record. */

/* Remaining raw call sites, named from the engine function's own source. */

static __inline__ void Call1_02003028(void (*f)(), s32 a0)
{
    f(a0);
}

void Func_02006fbc();          /* Place a marker: kind, x, z, level. */

s32 Func_020068d2();           /* per-site veneer -> Func_0808a5e0 */

s32 Func_0200664c();           /* per-site veneer -> Func_080000d0 */

void Func_02006a32_a();          /* site veneer -> Func_0808a440 */

/*
 * Read the low three bits of the runtime word at 0x03001e40 and, on two of the
 * eight values, forward the incoming argument. The 40-byte owner includes its
 * alignment halfword and one pool word. The argument is never materialised in
 * the body, yet it is live at both call sites, so the parameter must stay. The
 * meaning of the selector is not established; the other values fall through
 * deliberately.
 */

/* Two call words that reach the same target. */

/*
 * Write 0x1018 into the halfword at scene workspace + 382. The 24-byte owner
 * includes its two-word literal pool holding 0x03001ebc and 0x00001018, both
 * read by pc-relative loads in the row. The displacement is built as 191 << 1
 * and is a plain displacement, never used as a value. 0x1018 is loaded from
 * the pool rather than built. No arguments and no return value.
 */
void SceneState_SetValue14Mode23(void)
{
    extern s32 Data_03001e40;

    Func_02003cd8(0xE, 0x17);
}

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        angle = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

s32 SceneActor_CopyActor8PositionWithFixedY(struct Frame *dst)
{
    struct Frame *src = Actor_Get(8);

    dst->f08 = src->f08;
    dst->f0c = 0xFFF40000;
    dst->f10 = src->f10;
    return 0;
}

/*
 * Per-frame integrator for one actor record in resource_3a4. Advances the
 * position pair at +8 and +12, advances +24 and +28 by one shared velocity,
 * damps that velocity, and returns 0.
 *
 * The damping subtracts +72 from the value of +40 already held in a register,
 * not from a fresh load; v28 and v2c carry those earlier reads and must stay
 * locals rather than become repeated loads.
 */
s32 OverlayObject_IntegrateAndDamp(u8 *p)
{
    s32 v28;
    s32 v2c;

    *(s32 *)(p + 8) = *(s32 *)(p + 8) + *(s32 *)(p + 36);

    v28 = *(s32 *)(p + 40);
    *(s32 *)(p + 12) = *(s32 *)(p + 12) + v28;

    v2c = *(s32 *)(p + 44);
    *(s32 *)(p + 24) = *(s32 *)(p + 24) + v2c;
    *(s32 *)(p + 28) = *(s32 *)(p + 28) + v2c;

    *(s32 *)(p + 40) = v28 - *(s32 *)(p + 72);

    return 0;
}

/*
 * Table getter for resource_3a4, published from the overlay's header as an
 * entry point.
 *
 * The eight-byte owner at 0x02000204 includes its one pool word at
 * 0x02000208; the load reads that word and returns it as an address,
 * without dereferencing it.
 */
s32 SceneData_SelectTableByWord224(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000004d) {
        return (s32)Data_0200c194;
    }
    if (v == (s32)&Value_0000004e) {
        return (s32)Data_0200c20c;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)Data_0200c26c;
    }
    if (v == (s32)&Value_00000050) {
        return (s32)Data_0200c314;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)Data_0200c3ec;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)Data_0200c464;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)Data_0200c524;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)Data_0200c59c;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200c644;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200c704;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)Data_0200c77c;
    }
    return (s32)Data_0200c164;
}

s32 SceneData_SelectTableC80cOrC83c(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200c80c;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200c83c;
    }
    return 0;
}

u8 *SceneData_GetTableC85c(void)
{
    return Data_0200c85c;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000004d) {
        return (s32)Data_0200c940;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)Data_0200c9a0;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)Data_0200ca00;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)Data_0200ca60;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)Data_0200caa8;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)Data_0200cb68;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200cb98;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200cc40;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)Data_0200ccd0;
    }
    return (s32)Data_0200c928;
}

void SceneState_SetWorkByte22bTo3(void)
{
    extern u8 Data_02000240[];
    extern s32 Data_03001e40;

    Data_02000240[0x22b] = 3;
    Func_02004042((s32)&Value_0000004d, 99);
    Func_0200403a(53, 2);
}

void SceneState_SetByte22bTo3(void)
{
    extern u8 Data_02000240[];
    extern s32 Data_03001e40;

    Data_02000240[0x22b] = 3;
    Func_0200406e((s32)&Value_0000004f, 99);
    Func_02004066(53, 2);
}

void SceneState_SetByte22bTo3AndSend51(void)
{
    extern u8 Data_02000240[];

    Data_02000240[0x22b] = 3;
    Func_0200409a((s32)&Value_00000051, 99);
    Func_02004092(53, 2);
}

void SceneActor_UpdateSlot10ByTileX(void)
{
    s32 *a = Actor_Get(10);

    if (a != 0) {
        s32 x = 24;
        s32 y = 26;
        s32 t;

        Map_CopyCellAttributes(x, 27, 2, 1, x, y);
        t = a[2] >> 20;
        if (t == 25) {
            Map_CopyCellAttributes(0, 0, 1, 1, t, y);
        } else {
            Map_CopyCellAttributes(0, 0, 1, 1, x, y);
        }
        Actor_SetSpriteFlags(a, 0);
        ((u8 *)a)[0x55] = 0;
        Map_Redraw();
        Task_Wait(1);
    }
}

/*
 * Presentation setup in resource_3a4: clear a record byte, adjust two
 * handle flag bits, run two presentation primitives, then stamp a fixed
 * rate into the record.
 */

/* Declared without prototypes -- call sites vary in argument shape. */

/*
 * resource_3a4: a published callback that sets the mode of actor record 8.
 */

/*
 * Presentation callback for resource_3a4, published rather than called
 * directly from this overlay.
 */
void ActorPresentation_SetCellAndLowerActorEight(void)
{
    extern u32 Data_03001e40;

    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(8);
    s0 = 9;
    s1 = 13;
    Map_CopyCellAttributes(7, 13, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneState_ApplyRectAndSetActor9Byte55(void)
{
    extern s32 Data_03001e40;

    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(9);
    s0 = 17;
    s1 = 13;
    Map_CopyCellAttributes(29, 1, 3, 1, s0, s1);
    if (p != 0) {
        ((u8 *)p)[0x55] = 2;
    }
    GameFlag_Set(0x201);
}

void SceneActor_RaiseSlot9StepA(void)
{
    s32 *p;
    s32 s0;

    p = Actor_Get(9);
    s0 = 26;
    Map_CopyCellAttributes(0, 0, 1, 1, s0, s0);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneActor_RaiseSlot9StepB(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(9);
    s0 = 25;
    s1 = 13;
    Map_CopyCellAttributes(23, 13, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneState_ApplyRectAndLowerActor9(void)
{
    extern s32 Data_03001e40;

    s32 *rec;
    s32 s0;
    s32 s1;

    rec = Actor_Get(9);
    s0 = 43;
    s1 = 41;
    Map_CopyCellAttributes(45, 41, 1, 1, s0, s1);
    if (rec != 0) {
        Actor_SetSpriteFlags(rec, 0);
        rec[3] += 0xffe00000;
        ((u8 *)rec)[0x23] = 2;
    }
    GameFlag_Set(0x200);
}

void SceneActor_RaiseSlot11AndSetFlag201(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(11);
    s0 = 17;
    s1 = 10;
    Map_CopyCellAttributes(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x201);
}

void SceneActor_AdjustSlot12AndSetFlag204(void)
{
    extern s32 Data_03001e40;

    s32 *p;
    s32 s0;
    s32 s1;

    p = Actor_Get(12);
    s0 = 26;
    s1 = 15;
    Map_CopyCellAttributes(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Actor_SetSpriteFlags(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    GameFlag_Set(0x204);
}

void SceneState_SetDispcntBit9ByThreshold(void)
{
    extern u16 Data_0200d238;

    volatile u16 *reg = (volatile u16 *)0x04000000;
    s16 v = *reg & 0xfdff;

    if ((u32)(Random_Next() * 100) >> 16 >= Data_0200d238) {
        s32 k = 0x200;

        v |= k;
    }
    {
        u32 t = (u16)v;

        *reg = t;
    }
}

void FieldScene_RunEarlySequence(void)
{
    extern u8 Data_0200d238[];

    s32 i;
    s32 record;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Audio_PlayCue(230);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    rec = p5 + 356;
    for (i = 0; i <= 23; i++) {
        *(s32 *)(rec + 12) -= 0x10000;
        Task_Wait(4);
        if (i == 8) {
            record = Value1(Engine_ActorGet, 8);
            *(s32 *)(record + 24) = 0x1999;
            record = Actor_Get(8);
            *(s32 *)(record + 28) = 0x1999;
            Actor_SetPosition(8, 0x980000, 0xd80000);
            Actor_EnableActionCallback(8, 0x200bd48);
        }
    }
    Func_02004208(1, 0, 0x20086a1);
    *(u16 *)Data_0200d238 = 0;
    do {
        Task_Wait(1);
        v = *(u16 *)Data_0200d238 + 1;
        *(u16 *)Data_0200d238 = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Task_Wait(1);
    Func_02004236(1, 0, 0);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(30);
    Map_CopyCellAttributes(0, 0, 1, 2, 3, 14);
    GameFlag_Set(0x8fd);
}

void Func_020007e8(void)
{
    extern u8 Data_0200d238[];

    s32 i;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Map_CopyCells(93, 41, 16, 4, 77, 28);
    Audio_PlayCue(230);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    rec = p5 + 356;
    for (i = 23; i >= 0; i--) {
        *(s32 *)(rec + 12) -= 0x10000;
        Task_Wait(4);
    }
    Func_020042fc(1, 0, 0x20086a1);
    *(u16 *)Data_0200d238 = 0;
    do {
        Task_Wait(1);
        v = *(u16 *)Data_0200d238 + 1;
        *(u16 *)Data_0200d238 = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Task_Wait(1);
    Func_0200432a(1, 0, 0);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(30);
    Map_CopyCells(77, 41, 16, 4, 77, 28);
    GameFlag_Set(0x8fe);
}

void Func_020008d4(void)
{
    extern u8 Data_0200d238[];

    s32 i;
    u8 *p8;
    u8 *rec;
    s32 v;

    p8 = *(u8 **)0x03001e70;
    Map_CopyCellsTo(113, 31, 103, 17, 1, 1);
    Map_CopyCellsTo(111, 32, 104, 18, 3, 2);
    Map_CopyCellsTo(64, 32, 103, 18, 1, 2);
    Audio_PlayCue(230);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    Event_Wait(10);
    rec = p8 + 356;
    for (i = 23; i >= 0; i--) {
        *(s32 *)(rec + 12) -= 0x10000;
        Task_Wait(4);
    }
    Func_02004410(1, 0, 0x20086a1);
    *(u16 *)Data_0200d238 = 0;
    do {
        Task_Wait(1);
        v = *(u16 *)Data_0200d238 + 1;
        *(u16 *)Data_0200d238 = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Task_Wait(1);
    Func_0200443e(1, 0, 0);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(30);
    Map_CopyCellsTo(103, 14, 103, 17, 4, 3);
    GameFlag_Set(0x907);
}

void FieldScene_RunScene3a4SequenceB(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x323) != 0) {
        Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
        GameFlag_Clear(0x323);
    } else {
        Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
        GameFlag_Set(0x323);
    }
}

void FieldScene_RunValue1528Scene(void)
{
    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_FLIPPED_SWITCH, 1);
    Audio_PlayCue(125);
    FieldScene_RunScene3a4SequenceB();
    Task_Wait(20);
    Func_0200479c();
    Event_End();
}

void FieldScene_RunScene3a4SequenceA(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x325) != 0) {
        Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
        GameFlag_Clear(0x325);
    } else {
        Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
        GameFlag_Set(0x325);
    }
}

void FieldScene_RunLine1528Sequence(void)
{
    extern u8 Data_0200d238[];

    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_FLIPPED_SWITCH, 1);
    Audio_PlayCue(125);
    FieldScene_RunScene3a4SequenceA();
    Task_Wait(20);
    Func_02004844();
    Event_End();
}

void FieldScene_RunScene3a4SequenceC(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    s32 record;

    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_FLIPPED_SWITCH, 1);
    Audio_PlayCue(125);
    if (GameFlag_IsSet(0x326) != 0) {
        Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
        GameFlag_Clear(0x326);
    } else {
        Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
        GameFlag_Set(0x326);
    }
    Task_Wait(20);
    Func_020048da();
    Event_End();
}

void FieldScene_RunScene3a4SequenceD(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    s32 record;

    Event_Begin();
    Message_ShowCentered(MSG_ROBIN_FLIPPED_SWITCH, 1);
    Audio_PlayCue(125);
    if (GameFlag_IsSet(0x327) != 0) {
        Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
        GameFlag_Clear(0x327);
    } else {
        Map_CopyCellAttributes(30, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
        GameFlag_Set(0x327);
    }
    Task_Wait(20);
    Func_02004976();
    Event_End();
}

void SceneActor_SetActor10Byte23To3(void)
{
    extern u32 Data_03001e40;

    Func_02004812(10)[0x23] = 3;
}

void SceneActor_SetActor10Byte23To1(void)
{
    extern u32 Data_03001e40;

    Func_02004826(10)[0x23] = 1;
}

void FieldScene_RunScene3a4_02000c9c(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    s32 record;

    Event_Begin();
    Map_CopyCellAttributes(24, 27, 2, 1, 24, 26);
    Audio_PlayCue(185);
    Actor_SetSpeed(10, 0x3333, 0x1999);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    *(u8 *)(Func_0200486e(10) + 90) &= 254;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 8);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 0x190, 0x1a8);
    Actor_SetDestination(10, 0x198, 0x1a8);
    Actor_WaitForMove(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    SceneActor_UpdateSlot10ByTileX();
    Event_End();
}

void FieldScene_RunSharedSetPiece(s32 a0)
{
    struct FieldActor *actor0;
    struct FieldActor *actor8;
    struct FieldActor *actor9;
    struct FieldActor *actor10;
    struct FieldActor *actor;

    actor0 = Actor_Get(ACTOR_PARTY_LEADER);
    actor8 = Actor_Get(8);
    actor9 = Actor_Get(9);
    actor10 = Actor_Get(10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Event_Wait(40);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x3100000, -1, 0x740000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 6);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x318, 140);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 100);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
    Audio_PlayCue(183);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Event_Wait(20);
    actor9->scale_x = 0x13333;
    actor9->scale_y = 0x13333;
    actor9->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    actor9->update = (void (*)(union FieldObject *))SceneActor_CopyActor8PositionWithFixedY;
    Actor_SetAnimation(8, 4);
    *(s32 *)((u8 *)actor8 + 68) = 0x8000;
    actor8->x.fixed = 0x3120000;
    actor8->y.fixed = 0x200000;
    actor8->z.fixed = 0x5a0000;
    actor8->scale_x = 0x20000;
    actor8->scale_y = 0x20000;
    Event_Wait(10);
    Audio_PlayCue(183);
    Work_SetValuesIfNonNegative(0x40000, 0x20000, 0x10000);
    Event_Wait(20);
    actor10->x.fixed += 0xe0000;
    actor10->y.fixed += -0x80000;
    actor10->sprite->rotation = 0xc000;
    Audio_PlayCue(107);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
    Audio_PlayCue(55);
    Work_SetValuesIfNonNegative(0x10000, 0x30000, 0x10000);
    Actor_SetSpritePriority(8, 0);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 0);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
    *(u16 *)((u8 *)actor0 + 100) = 0;
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 0x200bdec);
    if (GameFlag_IsSet(0x205) != 0) {
        Actor_SetPosition(ACTOR_GERALD, 0x36e0000, 0x2100000);
        actor = Actor_Get(ACTOR_GERALD);
        actor->facing = 0x5000;
    }
    Camera_SetSpeed(0x14000, 0x2800);
    Camera_MoveTo(0x3120000, -1, 0x22c0000, 1);
    Event_Wait(a0);
    Actor_SetSpritePriority(8, 1);
    Actor_SetSpeed(8, 0x195c2, 0xcae1);
    *(u16 *)((u8 *)actor8 + 100) = 0;
    Actor_EnableActionCallback(8, 0x200bd78);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)actor0 + 100) == 0);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0);
    do {
        Task_Wait(1);
    } while (*(s16 *)((u8 *)actor8 + 100) == 0);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    Actor_Get(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    actor9->x.fixed = 0x3120000;
    actor9->update = NULL;
    actor9->z.fixed = 0x26a0000;
    actor9->y.fixed = -0x400000;
    Actor_SetSpeed(8, 0x19999, 0xcccc);
    *(s32 *)((u8 *)actor8 + 68) = 0x1999;
    *(s32 *)((u8 *)actor8 + 72) = 0x3333;
    actor8->velocity_y = 0x40000;
    Actor_MoveToAndWait(8, 0x312, 0x25c);
    Actor_SetSpeed(8, 0x33333, 0x19999);
    Actor_SetDestination(8, 0x312, 0x284);
    Event_Wait(15);
    Work_SetValuesIfNonNegative(0x50000, 0x70000, 0x10000);
    Map_CopyCellsTo(25, 36, 43, 36, 11, 9);
    Map_CopyCellAttributes(25, 35, 10, 5, 43, 35);
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Func_02004c4a(Func_0200abe1, 0xc80);
    Event_Wait(80);
    Func_02004c5e(Func_0200abe1);
    Event_Wait(60);
    Audio_PlayCue(17);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(120);
    if (GameFlag_IsSet(0x205) != 0) {
        Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
        Actor_WalkTo(ACTOR_GERALD, 0x338, 0x22e);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x356, 0x248);
    if (GameFlag_IsSet(0x205) != 0) {
        Actor_SetAnimation(ACTOR_GERALD, 1);
        Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 40);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
    Camera_MoveTo(0x3140000, -0x400000, 0x2620000, 1);
    Camera_WaitForMove();
    Audio_PlayCue(148);
    Event_Wait(240);
    if (GameFlag_IsSet(0x205) != 0) {
        Camera_SetSpeed(0x40000, 0x8000);
        Camera_MoveTo(0x3560000, 0, 0x2480000, 1);
        Camera_WaitForMove();
        Actor_WalkToAndWait(ACTOR_GERALD, 0x348, 0x228);
        Actor_WalkToAndWait(ACTOR_GERALD, 0x356, 0x232);
        Actor_SetAnimation(ACTOR_GERALD, 2);
        actor = Actor_Get(ACTOR_PARTY_LEADER);
        if (actor != NULL) {
            Actor_SetDestination(ACTOR_GERALD, actor->x.part.pixel, actor->z.part.pixel);
        }
        Actor_WaitForMove(ACTOR_GERALD);
        Actor_SetPosition(ACTOR_GERALD, 0, 0);
    }
    Func_0200500a();
    GameFlag_Set(0x908);
}

void FieldScene_RunScene3a4_02001398(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    u8 *record;
    s32 base5_1953;

    Event_Begin();
    base5_1953 = (s32)Data_00001953;
    Value2(Engine_MessageShowCentered, base5_1953, 1);
    if (GameFlag_IsSet(0x908) != 0) {
    } else {
        if (GameFlag_IsSet(0xf14) != 0) {
        } else {
            GameFlag_Set(0x205);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x316, 140);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x30c, 140);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
            record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            if ((s32)record != 0) {
                Actor_SetPosition(ACTOR_GERALD, *(volatile s32 *)((s32)record + 8), *(volatile s32 *)((s32)record + 16));
            }
            Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
            Actor_WalkToAndWait(ACTOR_GERALD, 0x320, 140);
            Actor_FaceDirection(ACTOR_GERALD, 0xc000, 20);
            Event_SetMessage((base5_1953 + 1));
            Actor_SetAnimation(ACTOR_GERALD, 4);
            Event_Wait(20);
            Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
            Actor_Jump(ACTOR_GERALD, 6, 0);
            Actor_SetSpeed(ACTOR_GERALD, 0x19999, 0xcccc);
            *(u8 *)(Func_02004ff6(1) + 90) &= 254;
            Actor_WalkToAndWait(ACTOR_GERALD, 0x318, 110);
            Event_Wait(1);
            *(u8 *)(Func_0200501a(1) + 90) |= 1;
            Audio_PlayCue(161);
            Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
            *(u8 *)(Func_02005040_a(1) + 90) &= 254;
            Actor_WalkToAndWait(ACTOR_GERALD, 0x318, 120);
            Event_Wait(1);
            {
                u8 *record = Actor_Get(ACTOR_GERALD);
                u8 value = *(volatile u8 *)&record[90];

                record[90] = (u8)(value | 1);
            }
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            Event_Wait(80);
            Audio_PlayCue(141);
            Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Event_Wait(40);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
            Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
            Actor_FaceDirection(ACTOR_GERALD, 0, 20);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
            Actor_FaceDirection(ACTOR_GERALD, 0x8000, 40);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
            Actor_FaceDirection(ACTOR_GERALD, 0, 40);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
            Actor_FaceDirection(ACTOR_GERALD, 0xc000, 40);
            Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
            Actor_FaceDirection(ACTOR_GERALD, 0x4000, 20);
            Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
            Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
            Actor_SetSpeed(ACTOR_GERALD, 0x28000, 0x14000);
            Actor_SetAnimation(ACTOR_GERALD, 5);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x31c, 138);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x324, 140);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x324, 166);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x2fc, 166);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x2fc, 198);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x312, 198);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
            Actor_MoveToAndWait(ACTOR_GERALD, 0x312, 246);
            Actor_SetAnimation(ACTOR_GERALD, 1);
            Actor_SetPosition(ACTOR_GERALD, 0, 0);
            Event_Wait(40);
            FieldScene_RunSharedSetPiece(10);
        }
    }
    Event_End();
}

void FieldScene_RunScene3a4_02002310(void)
{
    extern struct GameState Data_02000240;

    if (GameFlag_IsSet(0x8fe) != 0) {
        *(u16 *)(*(u8 **)0x03001e70 + 20) &= ~0x200;
        Actor_SetPosition(9, 0, 0);
    } else {
        Func_02005d7c();
        if (GameFlag_IsSet(0x109) == 0 && Data_02000240.entrance == 99) {
            FieldScene_RunScene3a4SequenceF();
        } else {
            Map_CopyCellAttributes(38, 24, 1, 2, 37, 24);
            Map_CopyCellAttributes(44, 23, 1, 2, 45, 23);
            if (GameFlag_IsSet(0x8fe) == 0) {
                Actor_SetChildValue(9, 2);
                Actor_SetAnimation(9, 3);
                SceneState_StoreParamsAndInstallTask(0xee0000, 0, 0x1a20000, 0x8000);
            }
        }
    }
    if (GameFlag_IsSet(0x323) != 0) {
        Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
    } else {
        Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
        Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
    }
}

void FieldScene_RunScene3a4_02002428(void)
{
    extern u8 Data_0200d238[];

    extern u8 Data_02000240[];

    if (GameFlag_IsSet(0x8fe) != 0) {
        *(u16 *)(*(u8 **)0x03001e70 + 20) &= ~0x200;
    } else {
        Map_CopyCellAttributes(52, 42, 1, 1, 53, 42);
    }
    {
        s32 index = 225;
        if ((u32)((((u16 *)Data_02000240)[index] - 6) << 16) <= 0x10000) {
            GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
        }
    }
}

void FieldScene_RunScene3a4_02002490(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    u8 *record;

    if (GameFlag_IsSet(0x907) != 0) {
        *(u16 *)(*(u8 **)0x03001e70 + 20) &= ~0x200;
        Actor_SetPosition(10, 0, 0);
    } else {
        if (GameFlag_IsSet(0x109) == 0) {
            if (Data_02000240_t[225][0] == 99) {
                FieldScene_RunScene3a4SequenceE();
            }
        }
        Func_02005f1a_a();
        if (GameFlag_IsSet(0x907) == 0) {
            Actor_SetChildValue(10, 2);
            Actor_SetAnimation(10, 3);
            Call4(Func_020050b4, 0x2ec0000, 0x80000, 0x1180000, 0x8000);
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(9);
    if (GameFlag_IsSet(0x200) != 0) {
        Actor_SetAnimation(9, 5);
        Map_CopyCellAttributes(23, 13, 1, 1, 25, 13);
        {
            u8 *record = Actor_Get(9);
            u8 flags = record[35] | 2;

            record[35] = flags;
        }
    }
    if (GameFlag_IsSet(0x325) != 0) {
        Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
    } else {
        Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
        Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
    }
}

void FieldScene_RunScene3a4_020025c0(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    u8 *rec7;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        if (GameFlag_IsSet(0x109) == 0) {
            Actor_SetPosition(8, 0x1660000, 0x680000);
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(9);
    if (GameFlag_IsSet(0x200) != 0) {
        rec7 = Actor_Get(9);
        Actor_SetAnimation(9, 5);
        Map_CopyCellAttributes(45, 41, 1, 1, 43, 41);
        {
            u8 flags = rec7[35] | 2;

            rec7[35] = flags;
        }
    }
    if (GameFlag_IsSet(0x907) != 0) {
        *(u16 *)(*(u8 **)0x03001e70 + 20) &= ~0x200;
    }
    if (GameFlag_IsSet(0x326) != 0) {
        Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
    } else {
        Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
        Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
    }
}

void FieldScene_RunScene3a4_020026c0(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    u8 *record;

    record = Actor_Get(9);
    Actor_SetSpriteFlags((s32)record, 0);
    SceneActor_UpdateSlot10ByTileX();
    SceneActor_ClearCollisionFlagAndPlaceMarker(9);
    if (GameFlag_IsSet(0x200) != 0) {
        Actor_SetAnimation(9, 5);
        Map_CopyCellAttributes(0, 0, 1, 1, 26, 26);
        {
            u8 *record = Actor_Get(9);
            u8 flags = record[35] | 2;

            record[35] = flags;
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(11);
    if (GameFlag_IsSet(0x201) != 0) {
        Call2((void (*)())Engine_ActorSetAnimation, 11, 5);
        Map_CopyCellAttributes(1, 0, 1, 1, 17, 10);
        {
            u8 *record = Actor_Get(11);
            u8 flags = record[35] | 2;

            record[35] = flags;
        }
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(12);
    if (GameFlag_IsSet(0x204) != 0) {
        Actor_SetAnimation(12, 5);
        Map_CopyCellAttributes(1, 0, 1, 1, 26, 15);
        {
            u8 *record = Actor_Get(12);
            u8 flags = record[35] | 2;

            record[35] = flags;
        }
    }
    Call2(Func_02006212, 0x200b429, 0xc80);
    if (GameFlag_IsSet(0x327) != 0) {
        Call6((void (*)())Engine_MapCopyCellAttributes, 30, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
    } else {
        Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
        Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
    }
}

void FieldScene_RunScene3a4_02002934(void)
{
    extern u8 Data_0200d238[];

    s32 rec7;
    s32 record;
    s16 flag;

    rec7 = GameFlag_IsSet(0x909);
    if (rec7 != 0) {
        Actor_SetPosition(8, 0, 0);
        ((void (*)())Engine_ActorSetPosition)(9, 0, 0);
    } else {
        record = Actor_Get(8);
        Actor_SetSpriteFlags(record, 0);
        Actor_SetSpritePriority(9, 3);
        record = Actor_Get(9);
        Actor_SetSpriteFlags(record, 0);
        *(u8 *)(Func_02006514(9) + 89) = rec7;
    }
    flag = gGameState.entrance;
    if (flag == 1 || flag == 98) {
        if (GameFlag_IsSet(0x109) == 0) {
            rec7 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
            Event_Begin();
            *(s32 *)(rec7 + 12) = 0x100000;
            Event_End();
        }
    } else if (flag == 99) {
        if (GameFlag_IsSet(0x109) == 0) {
            RunEventScript01();
        }
    }
    /* unlifted: 0x020029bc..0x020029c2 (2) */
}

void FieldScene_RunScene3a4_020029dc(void)
{
    extern u8 Data_0200d238[];

    s32 record;

    record = Actor_Get(9);
    Actor_SetSpriteFlags(record, 0);
    if (gGameState.entrance == 2) {
        Actor_SetPosition(9, 0xb80000, 0x1480000);
    }
}

/*
 * Clear the record byte at +0x55, then rewrite the handle flags at +9 as
 * (flags & ~0x0c) | 0x04 -- the mask is built from the zero already in v,
 * not spelled as a constant. The rate address is held in a local and
 * stored to both +24 and +28. The 50-byte owner includes its one pool
 * word.
 */
void SceneActor_SetMode3AndRate4ccc(u8 *rec)
{
    extern u32 Data_03001e40;

    u8 *p = rec + 0x55;
    s32 v = 0;
    u8 *h;

    *p = v;
    h = *(u8 **)(rec + 80);
    v -= 13;
    v &= h[9];
    v |= 4;
    h[9] = (u8)v;
    Object_SetPalette(rec, 3);
    Actor_SetSpriteFlags(rec, 0);
    {
        s32 rate = (s32)&Value_00004ccc;

        *(s32 *)(rec + 24) = rate;
        *(s32 *)(rec + 28) = rate;
    }
}

void SceneEffect_SpawnObject222(void)
{
    extern s32 Data_03001e40;

    u8 *obj;

    if ((Data_03001e40 & 3) != 0) {
        return;
    }
    obj = Func_02006652(222, Data_0200d240[0], Data_0200d240[1], Data_0200d240[2]);
    if (obj == 0) {
        return;
    }
    {
        u16 *p = (u16 *)(obj + 100);
        u16 v = 30;
        *p = v;
    }
    {
        u16 *q = (u16 *)(obj + 102);
        u16 w = 1;
        *q = w;
    }
    *(s32 *)(obj + 104) = 20;
    SceneActor_SetMode3AndRate4ccc(obj);
    *(s32 *)(obj + 108) = 0x0200aa49;
    Object_SetAnimation(obj, 1);
}

void SceneState_StoreParamsAndInstallTask(s32 v0, s32 v1, s32 v2, s32 v3)
{
    Data_0200d240[0] = v0;
    Data_0200d240[1] = v1;
    Data_0200d240[2] = v2;
    Data_0200d23c = v3;

    Func_020068d2(170);
    Func_0200664c(0x0200ab59, 0xc80);
}

void FieldScene_RunScene3a4SequenceG(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    u8 *rec7;
    s32 record;
    s32 value;
    s32 base6_3001e40;

    base6_3001e40 = 0x3001e40;
    if (Value2(Func_0200665a, *(volatile s32 *)base6_3001e40, 3) == 0) {
        value = Value0(Engine_RandomNext);
        rec7 = Value4(Func_020066ec, 200, ((((u32)(((value << 1) + value) << 4) >> 16) << 16) + 0x2fd0000), -0x400000, 0x2600000);
        if ((s32)rec7 != 0) {
            if (Value2(Func_0200668a, *(volatile s32 *)base6_3001e40, 9) == 0) {
                {
                    s32 v2 = Random_Next();
                    if (((u32)(v2 << 1) >> 16) != 0) {
                        Audio_PlayCue(145);
                    } else {
                        Audio_PlayCue(144);
                    }
                }
            }
            rec7[85] = 0;
            {
                s32 v3 = Random_Next();
                s32 tmp2824 = (((u32)(v3 << 15) >> 16) + 0x4ccc);
                *(s32 *)(rec7 + 72) = 0x6666;
                *(s32 *)((s32)rec7 + 28) = tmp2824;
                *(s32 *)((s32)rec7 + 24) = tmp2824;
            }
            rec7[97] = 1;
            Actor_SetSpriteFlags((s32)rec7, 0);
            rec7[35] &= 254;
            {
                u8 *p80 = *(u8 **)(rec7 + 80);
                s32 mask9 = -13;
                p80[9] = (mask9 & p80[9]) | 4;
            }
            Object_SetAnimation((s32)rec7, 1);
            Object_SetScript((s32)rec7, 0x200c01c);
            value = Value0(Engine_RandomNext);
            *(s32 *)(rec7 + 36) = ((((u32)(((value << 1) + value) << 1) >> 16) - 3) << 16);
            *(s32 *)(rec7 + 40) = 0x80000;
            value = Value0(Engine_RandomNext);
            *(s32 *)(rec7 + 44) = (((u32)(((value << 1) + value) << 9) >> 16) + -0x300);
        }
    }
}

void SceneState_ForwardByRuntimeSelector(s32 arg)
{
    extern s32 Data_03001e40;

    s32 sel = Data_03001e40 & 7;

    if (sel == 0) {
        Object_SetPalette(arg, 2);
    } else if (sel == 2) {
        Object_SetPalette(arg, 0);
    }
}

void FieldScene_RunActorTenFourStepSequence(void)
{
    Psynergy_Begin(24, 1);
    Psynergy_SetTarget(10, 9);
    Psynergy_RaiseHands();
    Actor_SetChildValue(10, 2);
    Psynergy_PlayEffect(1);
    Actor_SetChildValue(10, 2);
    Psynergy_LowerHands();
    Actor_SetChildValue(10, 2);
    Func_02006a32_a();
    Audio_PlayCue(288);
    Actor_SetChildValue(10, 2);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
    Event_Begin();
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(8, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(20);
    Item_ShowFound(ITEM_FROST_JEWEL, 3);
    Party_GiveItem(ITEM_FROST_JEWEL, 0);
    *(u8 *)(Func_02006a3c() + 85) = 0;
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x640000, 0, 0xf90000, 1);
    Camera_WaitForMove();
    Event_Wait(20);
    FieldScene_RunEarlySequence();
    Camera_MoveTo(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16), 1);
    Camera_WaitForMove();
    Call1(Func_0200696e, 0x200cd6c);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Event_End();
}

void FieldScene_RunScene3a4SequenceF(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(8, 0x1480000, 0x1a80000);
    GameFlag_Set(0x323);
    FieldScene_RunScene3a4SequenceB();
    Map_Redraw();
    Task_Wait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Func_0200364c();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Event_End();
}

void FieldScene_RunScene3a4SequenceE(void)
{
    extern u8 Data_0200d238[];

    u32 i;
    s32 record;

    Event_Begin();
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(8, 0x1e80000, 0x8a0000);
    GameFlag_Set(0x325);
    FieldScene_RunScene3a4SequenceA();
    Map_Redraw();
    Task_Wait(1);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Func_020037a0();
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Event_End();
}

/*
 * Overlay resource_3a4. Per-frame tick that counts to sixty, fires one
 * audio cue and rewinds itself. The address of this routine is stored in
 * a record as a plain word, so it runs as a published callback.
 */

/* Plays a sound cue; the name is this site's own call word, not a runtime
 * address. */

/*
 * The owner spans the code, one alignment halfword and its one pool word,
 * thirty-six bytes in all. Engine_AudioPlayCue names the loader-relocated call
 * word for the cue call, not a runtime address. The limit of sixty reads
 * as one second of frames, but nothing here fixes a frame rate.
 */
void SceneAudio_PlayCue183EverySixtyTicks(void)
{
    Data_0200d1b0 = Data_0200d1b0 + 1;
    if (Data_0200d1b0 == 60) {
        Audio_PlayCue(183);
        Data_0200d1b0 = 0;
    }
}

/* Runs a fixed chain of 19 calls with literal arguments, sets byte 0x22B of
 * the shared work buffer to 3, then issues two more calls. */
void FieldScene_RunLateAuxiliarySequence(void)
{
    extern u8 Data_0200d238[];

    extern u8 Data_02000240[];

    Event_Begin();
    Camera_SetSpeed(39321, 4915);
    Camera_MoveTo(21495808, -1, 5701632, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 39321, 19660);
    ObjectMotion_SetPositionAndReset_1(0, 328, 116);
    Audio_PlayCue_1(148);
    Value2(Func_020069d4, 33599213, 3200); /* main:080000d0 */
    Work_SetValuesIfNonNegative(65536, 65536, 65536); /* main:080091f0 */
    Actor_SetSpeed(8, 6553, 3276);
    Actor_SetSpeed(9, 6553, 3276);
    Actor_SetAnimation(8, 2);
    Actor_SetDestination(8, 328, 104);
    ObjectMotion_ResetAndSetPosition_2(9, 328, 108);
    BattleRuntime_WaitIfModeZero_1(60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
    ObjectMotion_SetVariantCallback_1(0, 2);
    ObjectMotion_CommitCurrentPositionAndActivate_1(8);
    do {
        WORK_BYTE_22B = 3;
    } while (0);
    SharedWorkData_SetFirstAndSecondFields_1((s32)&Value_00000056, 99);
    BattleEffect_ComputeWeightedResultAndDispatch_1(53, 3);
}

/*
 * Bit 1 of the runtime status word at 0x03001e40 selects mode 7 or mode 6
 * for record 8. That bit's meaning is unverified: other callbacks here mask
 * different bits of the same word. Both branches reach the same veneer, and
 * the declarations carry no parameter list because the arguments are set up
 * in registers at the call site. The owner spans 44 bytes -- the body, one
 * alignment halfword and one literal pool word.
 */
void SceneActor_SetActor8ModeByCounterBit(void)
{
    extern u32 Data_03001e40;

    if (((Data_03001e40 >> 1) & 1) != 0) {
        Actor_SetChildValue(8, 7);
    } else {
        Actor_SetChildValue(8, 6);
    }
}

/* Record returned by Engine_ActorGet/26/3a: a pair of s32 fields at +8 and
 * +16 that get forwarded straight into the matching setup call. */
void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 addr_0200affd;
    s32 addr_0200c0e4;
    s32 addr_0200c12c;

    Event_Begin();
    Actor_SetPosition(8, 0x1480000, 0x580000);
    Actor_SetPosition(9, 0x1480000, 0x580000);
    Actor_SetAnimation(8, 0);
    work = *(u8 **)Data_03001ebc;
    SCENE_PHASE = 0x100;
    SCENE_FIELD_1C8 = 40;
    Event_OpenScreen();
    Event_WaitForScreen(); /* main:0808a370 */
    Event_Wait(20);
    record = Scene_GetRecord_1(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_2(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_GERALD, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_IVAN, 0x9999, 0x4ccc);
    Actor_SetSpeed(ACTOR_MIA, 0x9999, 0x4ccc);
    ObjectMotion_EnableActionAndSetCallback_1(1, 0x200c054);
    ObjectMotion_EnableActionAndSetCallback_2(2, 0x200c084);
    ObjectMotion_MarkActiveAndSetActionCallback_1(3, 0x200c0b4);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 40);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 10);
    Event_SetMessage(MSG_WE_DID_ROBIN_WE_BEAT);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
    Event_OpenMessage(ACTOR_IVAN, 0); /* main:0808a178 */
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    } else {
        Actor_SetAnimationAndWait(ACTOR_IVAN, 4);
        bump_step(1);
    }
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_SetMessage(MSG_GUARDIAN_STATUES_WERE_CREATED_LONG);
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 60);
    Actor_FaceDirection(ACTOR_GERALD, 0x2000, 40);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 40);
    Audio_PlayCue(190);
    Actor_SetChildValue(8, 7);
    Event_Wait(10);
    Audio_PlayCue(0x121);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 40);
    Audio_PlayCue(103);
    addr_0200affd = (s32)Func_0200affd;
    Call2(Func_02006d34, addr_0200affd, 0xc80); /* main:080000d0 */
    addr_0200c0e4 = (s32)Data_0200c0e4;
    Actor_EnableActionCallback(9, addr_0200c0e4);
    ObjectMotion_MarkActiveAndSetActionCallback_2(8, addr_0200c0e4);
    Func_02006d54_a(addr_0200affd); /* main:080000d8 */
    Event_Wait(60);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xa000, 20);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_GERALD, 0xe000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 40);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_ShowMessageAndWait(ACTOR_GERALD, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    Event_Wait(20);
    addr_0200c12c = (s32)Data_0200c12c;
    Actor_EnableActionCallback(ACTOR_GERALD, addr_0200c12c);
    ObjectMotion_EnableActionAndSetCallback_5(2, addr_0200c12c);
    ObjectMotion_MarkActiveAndSetActionCallback_3(3, addr_0200c12c);
    Event_Wait(20);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x204;
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 16;
    GameFlag_Set(0x909);
    Event_End();
}

void SceneState_SetWorkspaceHalfword382To1018(void)
{
    extern s32 Data_03001e40;

    *(u16 *)(*(u8 **)Data_03001ebc + (191 << 1)) = (int)&Value_00001018;
}

/*
 * Compare record 0's field at +12 against a signed threshold and set record
 * 12's mode accordingly; the taller branch also sets bit 1 of record 11's
 * byte at +35.  Both offsets are named by position only and their roles are
 * unverified; +35 is read-modify-written as a flags byte.  The threshold is
 * kept as the value 0x00300000 the code builds, in no assumed fixed-point
 * unit.
 */
void SceneActor_SetActor12ModeByActorZeroHeight(void)
{
    extern u32 Data_03001e40;

    if (*(s32 *)(Func_02006fc6(0) + 12) > 0x00300000) {
        {
            u8 *flag = Func_02006fd6(11) + 35;
            s32 bit = 2;

            bit |= *flag;
            *flag = bit;
        }
        Actor_SetSpritePriority(12, 3);
    } else {
        Actor_SetSpritePriority(12, 2);
    }
}

void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no)
{
    u8 *record;

    record = Actor_Get(no);
    record[89] &= 0xfe;

    Func_02006fbc(0, *(s32 *)(record + 8), *(s32 *)(record + 16), 255);
}

/*
 * Stamp a fixed value into the caller's record at +102, then play sound cue
 * 288. The owner at 0x02003724 in resource_3a4 is 20 bytes with no literal
 * pool.
 */
void SceneState_SetRecordWord102AndPlayCue288(u16 *record)
{
    record = (u16 *)((char *)record + 102);
    {
        s32 value = 0x21;

        *record = value;
    }
    Audio_PlayCue(288);
}
