#include "TYPES.H"

#define OverlayObject_IntegrateVelocities Func_02000ab0
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000044
#define SecondaryRuntimeSelector Value_00000045
#define TertiaryRuntimeSelector Value_00000046
#define PrimaryOverlayData Data_0200b0f4
#define SecondaryOverlayData Data_0200b1e4
#define TertiaryOverlayData Data_0200b334
#define DefaultOverlayData Data_0200b4b4
#define PrimaryOverlayData_02000f40 Data_0200b6a0
#define SecondaryOverlayData_02000f40 Data_0200b790
#define TertiaryOverlayData_02000f40 Data_0200b8b0
#define DefaultOverlayData_02000f40 Data_0200ba30
#define PrimaryOverlayData_020024ac Data_0200ba48
#define SecondaryOverlayData_020024ac Data_0200bb20
#define TertiaryOverlayData_020024ac Data_0200bc1c
#define DefaultOverlayData_020024ac Data_0200bd54
#define GetOrbitingSceneObject Func_020059d4
#define NormalizeOrbitingSceneObject Func_020059a6
#define IsGameFlagSet Func_020059ec
#define AllocateEffectTransfer Func_0200599c
#define LoadEffectResource Engine_ItemLoadIcon
#define ConfigurePaletteTransfer Func_020059c4
#define ReleaseEffectTransfer Engine_HeapRelease
#define UpdateOrbitingSceneObject Value_0200aba5

#include "FIELD_EFFECT.H"
#define OverlayObject_IntegrateVelocities Effect_Move
#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"


struct Owner {
    u8 unk0[9];
    u8 unk9_0 : 2;
    u8 mode : 2;
    u8 unk9_4 : 4;
};

/* The probe record is passed on by value: four words in r0-r3 and the last two
 * copied to the outgoing stack area by an `ldmia`/`stmia` pair. */
struct Resource39fProbe {
    s32 word[6];
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;


extern u8 Value_0200aba5;

u8 *Func_0200373a(s32 kind, s32 x, s32 y, s32 z);
void Func_02003788(u8 *obj, s32 mode);
void Func_02003860(u8 *obj, s32 mode);
void Func_020037a0(u8 *obj, s32 mode);
u8 *Func_02003792(s32 kind, s32 x, s32 y, s32 z);
void Func_020037e4(u8 *obj, s32 mode);
void Func_020038bc(u8 *obj, s32 mode);
void Func_02003ba4(s32 a, s32 b);
void Func_02003ae2(s32 a, s32 b);
void Func_02003b4c(s32 a, s32 b);
s32 *Func_020042f4(s32);
s32 *Func_020042ec(s32);
s32 *Func_02004366(s32);
s32 Func_02001796(s32, s32, s32, s32, s32, s32);
s32 Func_020017aa(s32, s32, s32, s32, s32, s32);
s32 Func_020017bc(s32, s32, s32, s32, s32, s32);
s32 Func_020017ce(s32, s32, s32, s32, s32, s32);
s32 Func_020017e0(s32, s32, s32, s32, s32, s32);
s32 *Func_02003a98_a(s32 a);
s32 Func_02003a40(s32 *p, s16 *q);
void Func_02003a20(s32 *p, s32 b);
void Func_02003a34(s32 *p, s32 b);
void Func_02003a8e(s32 *p, s32 b);
s32 Func_02003b14(s32 a, s32 b, s32 c);
void Func_02003a6c(s32 *p, s32 b);
void Func_02003aac(s32 *p, s32 b);
s32 *Func_02003e40(s32 a);
s32 Func_02001d82(s32 *actor);
u8 *Func_020040ec(s32);
void Func_020021e8(s32, s32);
void Func_020020e2(s32, s32, s32, s32);
void Func_02001e58(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02002142(s32, s32, s32, s32);
void Func_02002160(s32, s32, s32, s32);
void Func_0200217e(s32, s32, s32, s32);
void Func_02004198(s32);
u8 *Func_02004644(s32);
void Func_0200273e(s32, s32);
void Func_02002638(s32, s32, s32, s32);
void Func_020023b4(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_020026a2(s32, s32, s32, s32);
void Func_020026c0(s32, s32, s32, s32);
void Func_020026de(s32, s32, s32, s32);
void Func_020026fc(s32, s32, s32, s32);
void Func_0200472e(s32);
u8 *Func_02004890(s32);
u8 *Func_020048a0(s32);
u8 *Func_020048b0(s32);
u8 *Func_020048ba(s32);
s32 Func_020027d8(s32 *);
void Func_02003b36();
void Func_02003b5e();
s32 Func_02003b62();
void Func_02003bb8();
u8 *Func_02003f14(s32);
void Func_02001c7a(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_0200200e(s32, s32);
void Func_02001f04(s32, s32, s32, s32);
void Func_02003fb6_c(s32);
void Func_02001f62(s32, s32, s32, s32);
void Func_02001f7e(s32, s32, s32, s32);
void Func_02001f9a(s32, s32, s32, s32);
u8 *Func_02004008();
void Func_02002102();
void Func_02001ffc();
void Func_02001d72();
u8 *Func_02004082_a();
u8 *Func_0200408c();
void Func_02002070();
void Func_02004080();
void Func_02004184();
void Func_020041c8_a(s32 a);
void Func_020041c8_b(s32 a);
s32 Func_02004222(s32 a, s32 b, s32 c);
s32 Func_020018d6(struct Resource39fProbe *);
void Func_02001a82(struct Resource39fProbe);
void Func_02004246_a(s32);
u8 *Func_0200429c(s32);
s32 Func_02001752(s32, s32, s32, s32, s32, s32);
u8 *Func_020044b4(s32);
void Func_020025ae(s32, s32);
void Func_020024a6(s32, s32, s32, s32);
void Func_02002222(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_0200250a(s32, s32, s32, s32);
void Func_02002528(s32, s32, s32, s32);
void Func_0200254a(s32, s32, s32, s32);
void Func_02002568(s32, s32, s32, s32);
void Func_0200458e(s32);
void Func_020025c4();
void Func_02002658();
void Func_020026cc();
void Func_020045b0_a();
void Func_020045f4();
s32 Func_0200460a();
s32 Func_02001eb0();
void Func_02001ec2();
void Func_02001ed2();
s32 Func_02001ee2();
s32 Func_02001ef4();
void Func_02001f30();
void Func_02002834();
void Func_02002abc();
void Func_02002b32();
void Func_02002bc4();
s32 Func_02004a02();
s32 Func_02004a0a();
void Func_02004a6c();
s32 Func_02004a72();
s32 Func_02004a80();
s32 Func_02004ac8();
void Func_02004b42();
u8 *Func_02004b44();
void Func_02004b70();
void Func_02004c3c();
s32 Func_02004b44_a();
s32 Func_02004b44_b();
u8 *Func_02004ba4(s32);
void Func_02002c9e(s32, s32);
void Func_02002b96(s32, s32, s32, s32);
void Func_02002912(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02002bfc(s32, s32, s32, s32);
void Func_02002c1c(s32, s32, s32, s32);
void Func_02002c38(s32, s32, s32, s32);
void Func_02004c6c_b(s32);
u8 *Func_02004cb4(s32);
void Func_02002db0(s32, s32);
void Func_02002ca8(s32, s32, s32, s32);
void Func_02002a24(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02002d0a(s32, s32, s32, s32);
void Func_02002d2c(s32, s32, s32, s32);
void Func_02002d4a(s32, s32, s32, s32);
void Func_02004d7c(s32);
void Func_02002dc4();
void Func_02002e52();
void Func_02002eb8();
void Func_02004daa();
void Func_02004dee();
s32 Func_02004e04();
u8 *Func_02004e40(s32);
void Func_02002f3c(s32, s32);
void Func_02002e38(s32, s32, s32, s32);
void Func_02002bb4(s32, s32, s32, s32, s32, s32, s32, s32);
void Func_02002e9a(s32, s32, s32, s32);
void Func_02002ebc(s32, s32, s32, s32);
void Func_02002edc_a(s32, s32, s32, s32);
void Func_02002efa(s32, s32, s32, s32);
void Func_02004f2c(s32);
void Func_02002cf0();
void Func_02002f74();
void Func_02002fce();
void Func_02002ff0();
void Func_0200300e();
void Func_0200307c();
s32 Func_02004f74();
void Func_02005032();
void Func_02005040();
void Func_02005048();
s32 Func_02003d34(s32 no, s32 val);
void Func_02001ce8(s32 no);
s32 *Func_02003e00(s32 a);
void Func_02001d38(s32 *p);
s32 *Func_02004950();
s32 *Func_02004958();
s32 Func_02001dfa();
s32 Func_02001e0e();
s32 Func_02001e20();
s32 Func_02001e32();
s32 Func_02001e44();
s32 *Func_020049ca();
OrbitingSceneObject *Func_020059d4(void);
void Func_020059a6(OrbitingSceneObject *, s32);
s32 Func_020059ec(s32);
u8 *Func_0200599c(s32, s32);
s32 Func_020059c4(u8, s32, u8 *);

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

/* 0x02003fb6 serves the two-argument presentation select and, twice later,
 * one-argument imports; the `_b` alias carries the two-argument one. */

/* An EWRAM global, not overlay data: the module is linked above this address,
 * so it cannot be in-image. */

/* Returns a slot record.  Its fields are named by position and not verified:
 * +8/+12/+16 read as the position and +10/+18 as signed halfwords. */

/* Four register arguments plus four stack words. */

/* 0x02004246 serves two imports: the one-argument post at site 5 and the
 * six-argument message at site 12. */

/* 0x02004d98 serves two imports: the two-argument setter at site 7 and the
 * three-argument one at site 14. */

/* 0x02004f28 serves two imports: the two-argument setter at site 7 and the
 * three-argument step at site 14. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call3_02001818(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];


    u8 *Func_02004b4e();

    return f(a0);
}

static __inline__ void Call3_02001c34(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];


    u8 *Func_02004b4e();

    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];


    u8 *Func_02004b4e();

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    extern u8 Data_03001ebc[];


    u8 *Func_02004b4e();

    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

static __inline__ void Call3_02001d04(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];


    s32 Func_02004b4e();

    f(a0, a1, a2);
}

static __inline__ void Call3_02002004(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call3_020021b0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}


void Func_020009dc(u8 *object, s32 mode)
{
    struct Owner *owner = *(struct Owner **)(object + 80);
    owner->mode = mode;
}

/*
 * Spawns an overlay object and puts it into its configured state --
 * resource_39f.
 */

/*
 * The four callees are declared at the in-overlay entry points that are
 * called directly, not at their veneer addresses.
 */

/*
 * Spawn an object, clear the low bits of its owner record, and leave it in
 * mode 15.  The three callees are declared at their in-overlay entry points,
 * not at veneers.  Each read-modify-write takes its mask from a local that is
 * initialised first and ANDed with the loaded byte afterwards, which is what
 * puts the mask in r3 and the byte in r2.  The +0x23 mask stays byte-wide as
 * 0xfe rather than ~1, which would widen it to a negated 2.
 */
u8 *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_0200373a(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        /*
         * The mask is a local initialised first and ANDed with the loaded
         * byte afterwards; folding the clear into one expression swaps the
         * two registers.
         */
        u32 mask = ~12;
        mask = mask & rec[9];
        rec[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Func_02003788(obj, 0);
        Func_02003860(obj, 14);
        Func_020037a0(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

u8 *OverlayObject_SpawnConfiguredWithMode15(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_02003792(kind, x, y, z);

    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        u32 mask = ~12;
        u32 m;
        mask = mask & rec[9];
        rec[9] = (u8)(mask | 4);
        obj[85] = 0;
        obj[89] = 8;
        Func_020037e4(obj, 0);
        Func_020038bc(obj, 15);
        m = 0xfe;
        m = m & obj[35];
        obj[35] = (u8)(m | 2);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

/* Three sites of the record accessor, so three names. */

/* Five sites of one import. */
void SceneState_SetValue18Mode2(void)
{
    Func_02003b4c(18, 2);
}

/* Two veneers each serve two imports: 0x020046fa is the three-argument setter
 * at site 8 and the one-argument wait at site 21, and 0x02004730 is the
 * three-argument step at site 14 and the one-argument wait at site 27. */

/* Four sites of the record accessor, so four names. */

/* 0x02004918 serves two imports: the two-argument mode select and the
 * zero-argument bracket close. */
s32 SceneActor_TryRunSlotZeroMoveStep(s16 *arg)
{
    s32 *p = Func_02003a98_a(0);
    u8 *f = (u8 *)p + 0x55;
    s32 saved = *f;

    s32 r = Func_02003a40(p, arg);

    if (r == 0) {
        s32 m;

        Engine_EventBegin();
        Func_02003a20(p, 6);
        Engine_TaskWait(6);
        Engine_AudioPlayCue(152);
        Func_02003a34(p, 7);
        p[12] = 0x30000;
        p[13] = 0x20000;
        p[10] = 0x40000;
        m = 0x7e;
        m &= *f;
        *f = m;
        Func_02003a8e(p, 0);
        Func_02003b14(0, arg[1], arg[5]);
        Func_02003a6c(p, 6);
        Func_02003aac(p, 1);
        *f = saved;
        Engine_EventEnd();
        return 1;
    }
    return 0;
}

s32 OverlayObject_ApplyField100(s32 a)
{
    Func_02003ba4(a, *(s16 *)(a + 100));
    return 0;
}

s32 OverlayObject_ApplyZero(s32 a)
{
    Func_02003ae2(a, 0);
    return 0;
}

void FieldScene_RunScene39f_02000d90(s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    s32 rec7;

    rec7 = Func_02003b62();
    Engine_ActorSetSpritePriority(a0, 1);
    Call3(Engine_ActorSetSpeed, a0, 0x30000, 0x18000);
    Engine_AudioPlayCue(152);
    *(s32 *)(rec7 + 40) = a3;
    *(s32 *)(rec7 + 72) = 0x8000;
    *(s32 *)(rec7 + 68) = 0;
    Func_02003b36(rec7, 0);
    Func_02003bb8(a0, a1, a2);
    Engine_ActorSetPosition(a0, a1 << 16, a2 << 16);
    Func_02003b5e(rec7, 1);
    *(s32 *)(rec7 + 72) = 0x10000;
}

void FieldScene_RunSixCallSetupSequence(s32 no, s32 val)
{
    s32 v0 = 0x20000;
    s32 v1 = 0x4000;

    Engine_CameraSetSpeed(v0, v1);
    Func_02003d34(no, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Func_02001ce8(no);
    Engine_ActorSetChildValue(no, val);
}

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

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableB5bc(void)
{
    return (u8 *)0x0200b5bc;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_02000f40[];
    extern u8 SecondaryOverlayData_02000f40[];
    extern u8 TertiaryOverlayData_02000f40[];
    extern u8 DefaultOverlayData_02000f40[];

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_02000f40;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_02000f40;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_02000f40;
    }
    return (s32)DefaultOverlayData_02000f40;
}

/* The five tile-painting calls take (layer, x, z, width, height, value) and
 * all reach the same routine, but each keeps its own call word: the encoding
 * is per site, so they must not be collapsed onto one alias. */
void SceneActor_PassOffsetPointOfActorZero(void)
{
    s32 v[3];
    s32 *p = Func_02003e00(0);

    v[0] = (p[2] & 0xfff00000) + 0x80000;
    v[1] = p[3];
    v[2] = (p[4] & 0xfff00000) + 0xffe80000;
    Func_02001d38(v);
}

void SceneActor_BobActorZeroWhenAheadClear(void)
{
    s32 pos[3];
    s32 *actor = Func_02003e40(0);
    u8 *fp = (u8 *)actor + 0x55;
    s32 saved = *fp;

    pos[0] = (actor[2] & 0xfff00000) + 0x80000;
    pos[1] = actor[3];
    pos[2] = (actor[4] & 0xfff00000) + 0x280000;
    if (Func_02001d82(pos)!= 0) {
        Engine_EventBegin();
        *fp = 0;
        Engine_ActorSetAnimation(9, 7);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        Engine_TaskWait(2);
        actor[3] += -0x10000;
        actor[5] += -0x10000;
        Engine_TaskWait(10);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        Engine_TaskWait(4);
        actor[3] += 0x10000;
        actor[5] += 0x10000;
        *fp = saved;
        Engine_EventEnd();
    }
}

void FieldScene_RunScriptedSteps0And17E6(void)
{
    extern u8 Data_03001ebc[];

    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 1);
    Engine_MessageShowCentered(6118, 1);
    Engine_EventEnd();
}

void FieldScene_RunActor10WaypointSequence(void)
{
    extern u8 Data_03001ebc[];

    u8 *slot;

    slot = Func_02003f14(10);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    Func_0200200e(10, 1);
    Func_02001f04(10, 88, 120, 0x60000);        /* 192 << 11 */

    Func_02001c7a(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(10, 1);
    Engine_ActorFaceEachOther(10, 0, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_ActorSetAttachedEffect(10, 258);                     /* 129 << 1 */
    Engine_EventWait(60);

    /* Three waypoints, each at height 0x30000 (192 << 10). */
    Func_02001f62(10, 88, 152, 0x30000);
    Engine_ActorFaceActor(0, 10, 0);
    Engine_EventWait(10);

    Func_02001f7e(10, 120, 192, 0x30000);
    Engine_ActorFaceActor(0, 10, 0);
    Engine_EventWait(10);

    Func_02001f9a(10, 120, 240, 0x30000);
    Engine_ActorFaceActor(0, 10, 0);
    Engine_EventWait(10);

    Func_02003fb6_c(768);                       /* 192 << 2 */
    Engine_ActorSetPosition(13, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

/*
 * A full cutscene beat for slot 11: opens the slot, places it at (408, 456),
 * publishes an eight-argument piece, runs the presentation, then re-places the
 * slot on the party's current heading readings and sets the engine byte at
 * Data_02000240 + 0x22b to 3.  The 228-byte owner includes an alignment
 * halfword and its four pool words.
 */
void FieldScene_RunActorElevenPresentationBeat(void)
{
    extern u8 Data_03001ebc[];

    extern s16 Data_02000240[];

    u8 *slot;
    s32 offset;

    slot = Func_02004008(11);

    /* Reads the record left in r0 by the call above; it must not be respelled
     * as a fresh fetch. */
    Engine_EventBegin();

    Func_02002102(11, 0);
    Func_02001ffc(11, 408, 456, 0x60000);   /* 204 << 1, 228 << 1, 192 << 11 */

    Func_02001d72(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x180000,   /* 192 << 13 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(11, 1);
    Engine_ActorFaceEachOther(11, 0, 0);
    Engine_EventWait(30);
    Engine_ActorStartRepeatedMotion(11, 2);
    Engine_ActorShowEmote(11, 0x103, 0);
    Engine_AudioPlayCue(147);
    Engine_EventWait(60);

    /* Two signed halfwords of slot 0, each read after its own fetch of the
     * record. */
    Func_02002070(11,
                  *(s16 *)(Func_02004082_a(0) + 10),
                  *(s16 *)(Func_0200408c(0) + 18),
                  0x40000);                          /* 128 << 11 */

    Engine_EventWait(10);
    Func_02004080(0x301);
    Engine_ActorSetPosition(14, 0, 0);

    offset = 0x22b;
    ((u8 *)Data_02000240)[offset] = 3;

    Func_02004184(53, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void SceneActor_RunActorTwelveThreeWaypointMotion(void)
{
    u8 *slot;

    slot = Func_020040ec(12);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    Func_020021e8(12, 1);
    Func_020020e2(12, 536, 344, 0x70000);       /* 134 << 2, 172 << 1, 224 << 11 */

    Func_02001e58(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x100000,   /* 128 << 13 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(12, 1);
    Engine_ActorFaceEachOther(12, 0, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorSetAttachedEffect(12, 258);                     /* 129 << 1 */
    Engine_EventWait(60);

    /* Three waypoints, each at height 0x30000 (192 << 10); the X literals are
     * 146 << 2, 158 << 2 and 170 << 2 and the Z is the same 172 << 1. */
    Func_02002142(12, 584, 344, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Engine_EventWait(6);

    Func_02002160(12, 632, 344, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Engine_EventWait(6);

    Func_0200217e(12, 680, 344, 0x30000);
    Engine_ActorFaceActor(0, 12, 0);
    Engine_EventWait(6);

    Func_02004198(0x302);
    Engine_ActorSetPosition(15, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunStepFD4WithActor181(s32 a)
{
    extern u8 Data_03001ebc[];

    Func_020041c8_a(a);
    Func_02004222(16, 0, 0);
    Func_020041c8_b(4052);
    Engine_ItemShowFound(181, 3);
    Engine_PartyGiveItem(181, 0);
    Engine_EventEnd();
}

void FieldScene_RunProbedActorEightOrTenScene(void)
{
    extern u8 Data_03001ebc[];

    struct Resource39fProbe probe;
    s32 fifth;
    s32 sixth;
    s32 height;
    s32 value;

    /* No argument register is written before this branch. */
    Engine_EventBegin();

    if (Func_020018d6(&probe) != 0) {
        Func_02001a82(probe);

        if (probe.word[1] == 8 && (probe.word[4] >> 20) == 23) {
            fifth = 35;
            sixth = 68;
            Engine_MapCopyCellAttributes(35, 67, 4, 1, fifth, sixth);
        } else if (probe.word[1] == 10 && (probe.word[2] >> 20) == 35) {
            /* Written here, not at the call: the reference keeps it in a
             * callee-saved register across the whole sequence. */
            value = 0;
            Func_02004246_a(0x311);
            Engine_ActorSetAnimation(10, 3);
            Engine_ActorSetDestinationOffset(10, -16, 6);
            Engine_EventWait(30);
            Engine_ActorSetAnimation(10, 8);
            Engine_AudioPlayCue(240);

            Func_0200429c(10)[35] = 2;

            fifth = 34;
            sixth = 30;
            Engine_MapCopyCellAttributes(44, 30, 2, 4, fifth, sixth);
            height = 4;
            Func_02001752(2, 35, 30, 1, height, value);
        }
    }

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void SceneState_ApplyCrossRectsAroundActor11(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Engine_EventBegin();

    /* Both coordinates are 16.16 fixed point reduced to whole tiles with
     * `asrs #20`, i.e. 16 fractional bits plus a 16-unit tile pitch. */
    x = Func_020042ec(11)[2] >> 20;
    z = Func_020042f4(11)[4] >> 20;

    Func_02001796(2, x, z, 1, 1, 0xff);
    Func_020017aa(2, x + 1, z, 1, 1, 0);
    Func_020017bc(2, x - 1, z, 1, 1, 0);
    Func_020017ce(2, x, z + 1, 1, 1, 0);
    Func_020017e0(2, x, z - 1, 1, 1, 0);

    if (x == 36 && z == 24) {
        u8 *p = (u8 *)Func_02004366(11);

        p[85] = 0;
        *(s32 *)(p + 20) = (s32)0xfffe0000;
        *(s32 *)(p + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunActorThirteenPresentationBeat(void)
{
    extern u8 Data_03001ebc[];

    u8 *slot;

    slot = Func_020044b4(13);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    Func_020025ae(13, 1);
    Func_020024a6(13, 456, 104, 0x70000);       /* 228 << 1, 224 << 11 */
    Engine_EventWait(10);

    Func_02002222(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(13, 1);
    Engine_ActorFaceEachOther(13, 0, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(13, 2);
    Engine_ActorSetAttachedEffect(13, 258);                     /* 129 << 1 */
    Engine_EventWait(60);

    Func_0200250a(13, 472, 136, 0x30000);       /* 236 << 1, 192 << 10 */
    Engine_ActorFaceActor(0, 13, 0);
    Engine_EventWait(6);

    Func_02002528(13, 504, 136, 0x33333);       /* 252 << 1, pooled height */
    Engine_ActorFaceActor(0, 13, 0);
    Engine_EventWait(6);

    Func_0200254a(13, 552, 136, 0x38000);       /* 138 << 2, 224 << 10 */
    Engine_ActorFaceActor(0, 13, 0);
    Engine_EventWait(6);

    Func_02002568(13, 584, 136, 0x38000);       /* 146 << 2 */
    Engine_ActorFaceActor(0, 13, 0);
    Engine_EventWait(6);

    Engine_ActorSetPosition(13, 0, 0);
    Func_0200458e(772);                         /* 193 << 2 */
    Engine_ActorSetPosition(16, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunScene39f_02001818(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Engine_EventBegin();
    Func_020026cc(14, 1);
    Call4(Func_020025c4, 14, 0x1a8, 0x1e0, 0x79999);
    Engine_EventWait(2);
    Func_02002658(14);
    Engine_ActorSetChildValue(14, 15);
    record = Func_0200460a(14);
    Func_020045b0_a(record, 0);
    Engine_EventWait(30);
    Call1(Func_020045f4, 0x305);
    Call3_02001818(Engine_ActorSetPosition, 17, 0x1a80000, 0x1e00000);
    Engine_EventEnd();
}

void SceneActor_RunActorFourteenFourWaypointMotion(void)
{
    u8 *slot;

    slot = Func_02004644(14);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    Func_0200273e(14, 1);
    Func_02002638(14, 392, 504, 0x60000);       /* 196 << 1, 252 << 1, 192 << 11 */
    Engine_EventWait(10);

    Func_020023b4(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(14, 1);
    Engine_ActorFaceEachOther(14, 0, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(14, 2);
    Engine_ActorSetAttachedEffect(14, 258);                     /* 129 << 1 */
    Engine_EventWait(60);

    /* Four waypoints; Z is 132 << 2 and the height 192 << 10 throughout. */
    Func_020026a2(14, 360, 528, 0x30000);
    Engine_ActorFaceActor(0, 14, 0);
    Engine_EventWait(6);

    Func_020026c0(14, 328, 528, 0x30000);
    Engine_ActorFaceActor(0, 14, 0);
    Engine_EventWait(6);

    Func_020026de(14, 288, 528, 0x30000);
    Engine_ActorFaceActor(0, 14, 0);
    Engine_EventWait(6);

    Func_020026fc(14, 256, 528, 0x30000);
    Engine_ActorFaceActor(0, 14, 0);
    Engine_EventWait(6);

    Engine_CameraFollowActor(0, 1);
    Engine_ActorSetPosition(14, 0, 0);
    Engine_EventWait(30);
    Func_0200472e(0x306);
    Engine_ActorSetPosition(17, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void SceneActor_BobActorZeroWhenTargetClear(void)
{
    u8 *record;
    u8 *mode;
    u8 saved;
    s32 target[3];

    record = Func_02004890(0);
    mode = record + 85;
    saved = *mode;

    target[0] = *(s32 *)(Func_020048a0(0) + 8) + (s32)0xffe00000;
    target[1] = *(s32 *)(Func_020048b0(0) + 12);
    target[2] = *(s32 *)(Func_020048ba(0) + 16);

    if (Func_020027d8(target)!= 0) {
        /* r0 still holds the nonzero result of the test above. */
        Engine_EventBegin();

        *mode = 0;
        Engine_ActorSetAnimation(11, 7);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Engine_TaskWait(2);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Engine_TaskWait(10);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;
        Engine_TaskWait(4);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;

        *mode = saved;
        Engine_EventEnd();
    }
}

/*
 * Parking step for actor slot 13 in resource_39f. It marks the actor's own
 * tile 0xff, clears the four orthogonally adjacent tiles, and once the actor
 * stands on tile (45, 6) clears the record's mode byte and writes -2.0 in
 * 16.16 into the words at +12 and +20.
 *
 * The 176-byte owner at 0x02001b84 runs past its code to include an alignment
 * halfword and the pool word 0xfffe0000 at 0x02001c30.
 */
void SceneActor_MarkActorThirteenTileAndPark(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Engine_EventBegin();

    x = Func_02004950(13)[2] >> 20;
    z = Func_02004958(13)[4] >> 20;

    Func_02001dfa(2, x, z, 1, 1, 0xff);
    Func_02001e0e(2, x + 1, z, 1, 1, 0);
    Func_02001e20(2, x - 1, z, 1, 1, 0);
    Func_02001e32(2, x, z + 1, 1, 1, 0);
    Func_02001e44(2, x, z - 1, 1, 1, 0);

    if (x == 45 && z == 6) {
        u8 *record = (u8 *)Func_020049ca(13);

        record[85] = 0;
        *(s32 *)(record + 20) = (s32)0xfffe0000;
        *(s32 *)(record + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    extern u8 Data_03001ebc[];


    u8 *Func_02004b4e();

    s32 x;
    s32 y;
    u8 *record;

    Engine_EventBegin();
    record = Value1(Func_02004a02, 14);
    x = *(s32 *)(record + 8);
    record = Value1(Func_02004a0a, 14);
    y = *(s32 *)(record + 16);
    x >>= 20;
    y >>= 20;
    Func_02001eb0(2, x, y, 1, 1, 255);
    Call6(Func_02001ec2, 2, x + 1, y, 1, 1, 0);
    Call6(Func_02001ed2, 2, x - 1, y, 1, 1, 0);
    Func_02001ee2(2, x, y + 1, 1, 1, 0);
    Func_02001ef4(2, x, y - 1, 1, 1, 0);
    record = Value1(Func_02004a72, 14);
    if ((*(s32 *)(record + 16) >> 20) == 27) {
        record = Value1(Func_02004a80, 14);
        record[85] = 0;
        *(s32 *)(record + 20) = -0x20000;
        *(s32 *)(record + 12) = -0x20000;
        Call1(Func_02004a6c, 0x214);
        Call6(Func_02001f30, 2, 43, 23, 1, 1, 255);
    }
    Engine_EventEnd();
}

/*
 * Actor presentation beat for overlay resource_39f.  The twin at 0x02001d04
 * is the same beat for slot 15.
 */
void FieldScene_RunScene39fSequenceA(void)
{
    extern u8 Data_03001ebc[];


    s32 Func_02004b4e();

    s32 rec7;
    s32 big;
    s32 first;
    s32 shown;
    s32 second;
    s32 base3_2000240;

    rec7 = Func_02004ac8(15);
    big = 0x80000;
    Engine_EventBegin();
    Func_02002bc4(15, 0);
    Call4(Func_02002abc, 15, 0x1d8, 104, big);
    Engine_EventWait(10);
    Func_02002834(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + big), 0, 0, 0, 1, 0);
    Engine_CameraFollowActor(15, 1);
    Engine_ActorFaceEachOther(15, 0, 0);
    Engine_EventWait(30);
    Engine_ActorStartRepeatedMotion(15, 2);
    Call3_02001d04(Engine_ActorShowEmote, 15, 0x103, 0);
    Engine_AudioPlayCue(147);
    Engine_EventWait(60);
    first = Func_02004b44_a(0);
    shown = *(s16 *)(first + 10);
    second = Func_02004b4e(0);
    Call4(Func_02002b32, 15, shown, *(s16 *)(second + 18), 0x60000);
    ((s64 (*)())Func_02004b44_b)(10);
    Call1(Func_02004b42, 0x307);
    base3_2000240 = (s32)Data_02000240;
    *(u8 *)((base3_2000240 + 0x22b)) = 3;
    Func_02004c3c(53, 0);
    Engine_EventEnd();
}

void FieldScene_RunSlot16WaypointSequence(void)
{
    extern u8 Data_03001ebc[];

    u8 *slot;

    slot = Func_02004ba4(16);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    Func_02002c9e(16, 1);
    Func_02002b96(16, 456, 152, 0x60000);       /* 228 << 1, 192 << 11 */
    Engine_EventWait(10);

    Func_02002912(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(16, 1);
    Engine_ActorFaceEachOther(16, 0, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(16, 2);
    Engine_ActorSetAttachedEffect(16, 258);                     /* 129 << 1 */
    Engine_EventWait(60);

    /* Three waypoints at height 0x30000 (192 << 10). */
    Func_02002bfc(16, 448, 192, 0x30000);       /* 224 << 1 */
    Engine_ActorFaceActor(0, 16, 0);
    Engine_EventWait(6);

    Func_02002c1c(16, 424, 208, 0x30000);       /* 212 << 1 */
    Engine_ActorFaceActor(0, 16, 0);
    Engine_EventWait(6);

    Func_02002c38(16, 424, 224, 0x30000);
    Engine_ActorFaceActor(0, 16, 0);
    Engine_EventWait(6);

    Engine_CameraFollowActor(0, 1);
    Engine_ActorSetPosition(16, 0, 0);
    Engine_EventWait(30);
    Func_02004c6c_b(776);                         /* 194 << 2 */
    Engine_ActorSetPosition(20, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunActor17CameraSequence(void)
{
    extern u8 Data_03001ebc[];

    u8 *slot;

    slot = Func_02004cb4(17);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    Func_02002db0(17, 1);
    Func_02002ca8(17, 392, 104, 0x60000);       /* 196 << 1, 192 << 11 */
    Engine_EventWait(10);

    Func_02002a24(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(17, 1);
    Engine_ActorFaceEachOther(17, 0, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(17, 2);
    Engine_ActorSetAttachedEffect(17, 258);                     /* 129 << 1 */
    Engine_EventWait(60);

    Func_02002d0a(17, 376, 152, 0x60000);       /* 188 << 1 */
    Engine_ActorFaceActor(0, 17, 0);
    Engine_EventWait(10);

    Func_02002d2c(17, 328, 160, 0x30000);       /* 164 << 1, 192 << 10 */
    Engine_ActorFaceActor(0, 17, 0);
    Engine_EventWait(6);

    Func_02002d4a(17, 296, 160, 0x30000);       /* 148 << 1 */
    Engine_ActorFaceActor(0, 17, 0);
    Engine_EventWait(6);

    Engine_CameraFollowActor(0, 1);
    Engine_ActorSetPosition(17, 0, 0);
    Engine_EventWait(30);
    Func_02004d7c(0x309);
    Engine_ActorSetPosition(21, 0, 0);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunScene39f_02002004(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Engine_EventBegin();
    Func_02002eb8(18, 1);
    Call4(Engine_CameraMoveTo, 0x2e80000, -1, 0x1f80000, 1);
    Call4(Func_02002dc4, 18, 0x2e8, 0x1f8, 0x90000);
    Func_02002e52(18);
    Engine_ActorSetChildValue(18, 15);
    record = Func_02004e04(18);
    Func_02004daa(record, 0);
    Engine_EventWait(30);
    Call1(Func_02004dee, 0x30a);
    Call3_02002004(Engine_ActorSetPosition, 22, 0x2e80000, 0x1f80000);
    Engine_EventEnd();
}

void FieldScene_RunActorEighteenEffectSequence(void)
{
    extern u8 Data_03001ebc[];

    u8 *slot;

    slot = Func_02004e40(18);

    /* r0 still holds the record returned above. */
    Engine_EventBegin();

    Func_02002f3c(18, 1);
    Func_02002e38(18, 712, 536, 0x60000);       /* 178 << 2, 134 << 2, 192 << 11 */
    Engine_EventWait(10);

    Func_02002bb4(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Engine_CameraFollowActor(18, 1);
    Engine_ActorFaceEachOther(18, 0, 0);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(18, 2);
    Engine_ActorSetAttachedEffect(18, 258);                     /* 129 << 1 */
    Engine_EventWait(60);

    Func_02002e9a(18, 712, 568, 0x60000);       /* 142 << 2 */
    Engine_ActorFaceActor(0, 18, 0);
    Engine_EventWait(10);

    Func_02002ebc(18, 712, 600, 0x30000);       /* 150 << 2, 192 << 10 */
    Engine_ActorFaceActor(0, 18, 0);
    Engine_EventWait(6);

    Func_02002edc_a(18, 736, 640, 0x30000);       /* X += 24, 160 << 2 */
    Engine_ActorFaceActor(0, 18, 0);
    Engine_EventWait(6);

    Func_02002efa(18, 736, 704, 0x30000);       /* 176 << 2 */
    Engine_ActorFaceActor(0, 18, 0);
    Engine_EventWait(6);

    Engine_CameraFollowActor(0, 1);
    Engine_ActorSetPosition(18, 0, 0);
    Engine_EventWait(30);
    Func_02004f2c(0x30b);

    /* Common exit; no argument registers are set. */
    Engine_EventEnd();
}

void FieldScene_RunScene39f_020021b0(void)
{
    extern u8 Data_03001ebc[];

    s32 rec7;

    rec7 = Value1(Func_02004f74, 18);
    Engine_EventBegin();
    Call3_020021b0(Engine_ActorSetPosition, 18, 0x880000, 0x1680000);
    Func_0200307c(18, 1);
    Func_02002f74(18, 136, 0x198, 0x80000);
    Engine_EventWait(10);
    Func_02002cf0(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + 0x40000), 0, 0, 0, 1, 0);
    Call3_020021b0(Func_02005048, 18, 0xc000, 40);
    Call2(Engine_ActorSetAttachedEffect, 18, 0x102);
    Func_02005032(18, 2);
    Engine_CameraFollowActor(18, 1);
    Func_02002fce(18, 136, 0x1b8, 0x60000);
    Engine_ActorFaceActor(0, 18, 0);
    Engine_EventWait(10);
    Func_02002ff0(18, 136, 0x1d8, 0x30000);
    Engine_ActorFaceActor(0, 18, 0);
    Engine_EventWait(6);
    Func_0200300e(18, 136, 0x1f8, 0x30000);
    Engine_ActorFaceActor(0, 18, 0);
    Engine_EventWait(6);
    Engine_CameraFollowActor(0, 1);
    Engine_ActorSetPosition(18, 0, 0);
    Engine_EventWait(60);
    Call1(Func_02005040, 0x89d);
    Engine_EventEnd();
}

s32 SceneData_SelectTableBa48ByRuntimeSelector(void)
{
    extern u8 PrimaryOverlayData_020024ac[];
    extern u8 SecondaryOverlayData_020024ac[];
    extern u8 TertiaryOverlayData_020024ac[];
    extern u8 DefaultOverlayData_020024ac[];

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_020024ac;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_020024ac;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_020024ac;
    }
    return (s32)DefaultOverlayData_020024ac;
}

s32 Func_02002ba4(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Engine_MathSin(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Engine_MathCos(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Engine_MathCos(p[12] + 0x8000) / 8;
    a = Engine_RandomNext();
    b = Engine_RandomNext();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

void InitializeOrbitingEffect(void)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = GetOrbitingSceneObject();
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    NormalizeOrbitingSceneObject(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (IsGameFlagSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = AllocateEffectTransfer(17, 0x608);
    LoadEffectResource(181);
    transfer += 0x400;
    ConfigurePaletteTransfer(sprite->palette, 128, transfer);
    ReleaseEffectTransfer(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)&UpdateOrbitingSceneObject;
    actor->state = zero;
}
