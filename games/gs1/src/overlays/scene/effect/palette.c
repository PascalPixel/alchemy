#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define PaletteScene_RunActorTransitionSequence Func_02000488
#define PaletteScene_GetScriptData Func_02000030
#define PaletteScene_GetMessageData Func_02000038
#define PaletteScene_GetActorData Func_02000040
#define PaletteScene_GetEffectData Func_02000048
#define PaletteScene_Initialize Func_02000050
#define PaletteScene_GetState Func_02000080
#define PaletteScene_RunActorNineBranch Func_02000248
#define PaletteScene_RunActorEightBranch Func_02000284
#define PaletteScene_RunFlaggedBranch Func_020002c0
#define PaletteScene_AdvanceEffectFrame Func_020011e8
#define PaletteScene_SpawnEffect Func_02001218
#define PaletteScene_AdvanceTransition Func_020012b4
#define PaletteScene_AdvanceOrbit Func_0200152c
#define PaletteScene_SetRecordValue Func_0200172c
#define PaletteScene_AdjustPaletteWindow Func_02001768
#define PaletteScene_AdjustColor Func_020017d0
#define FieldScene_RunScene395_02000158 Func_02000158
#define RunEventScript01 Func_020002ec

#include "palette_scene.h"

struct PaletteEffectFrame {
    s32 pad00[6];
    s32 progress;
    s32 pad1c[7];
    s32 limit;
    s32 second_limit;
    s32 third_limit;
};

struct EffectSprite { u8 pad00[9]; u8 flags; u8 pad0a[28]; u8 state; };

struct PaletteEffect {
    u8 pad00[0x18];
    s32 progress;
    u8 pad1c[7];
    u8 flags;
    u8 pad24[12];
    s32 rate_x;
    s32 rate_y;
    u8 pad38[24];
    struct EffectSprite *sprite;
    u8 pad54[1];
    u8 mode;
};

struct OrbitingPaletteEffect {
    s32 pad00[2];
    s32 x;
    s32 y;
    s32 z;
    s32 pad14;
    s32 angle_x;
    s32 angle_y;
    s32 pad20[6];
    s32 anchor_x;
    s32 anchor_y;
    s32 anchor_z;
    s32 pad44[3];
    u8 *owner;
    s32 pad54[4];
    s16 step;
    s16 heading;
};

struct PaletteSceneRecord {
    u8 pad00[216];
    u16 values[1];                 /* +216 */
};

extern s32 Data_02009dd4;
extern s32 Data_02009dcc;
extern s32 Data_02009dc0[3];
extern const s32 SceneAction_ActorOneEntry[];
extern const s32 SceneAction_ActorTwoEntry[];
extern const s32 SceneAction_ActorThreeEntry[];
extern const s32 SceneAction_GroupFinish[];
extern s32 Data_03001e40;
extern u8 Data_02009d9c[];
extern u8 Data_03001ebc[];

void Func_020019f0();
void Func_02001a14();
void Func_02001ae0(s16);
void Func_02001b0a(s32, s32, s32);
void Func_02001be4(void);
s32 Func_02001bd2(s32);
void Func_02001c7c(s32);
void Func_02001c84(s32);
void Func_02001c9c(s32, s32);
void Func_02001c10(void);
void Func_02001c20(void);
s32 Func_02001c0e(s32);
void Func_02001cb8(s32);
void Func_02001cc0(s32);
void Func_02001cd8(s32, s32);
void Func_02001c4c(void);
void Func_02001c5c(void);
void Func_02001d58(void);
s32 Func_02001c4e(s32);
void Func_020005c2(void);
void Func_02000764(void);
void Func_02001c80(void);
void Func_020012b4(void);
void Func_020015a0(void);
s32 Func_02001980();
void Func_020019e0();
void Func_02001a40();
void Func_02001ab0();
void Func_02001960();
void Func_020019b8();
u8 *Func_020019b0();
void Func_020019e8();
void Func_020019c0();
void Func_020019c8();
void Func_02001990();
void Func_020018b8();
void Func_02001a88();
void Func_02001908();
s32 Func_02001910();
void Func_02001a58();
void Func_02001a60();
void Func_02001a68();
s32 Func_02001918();
void Func_020012f4();
void Func_02001a20();
void Func_02001a30();
void Func_02001a08();
void Func_02001a10();
void Func_02001a50();
void Func_02001a38();
s32 Func_02001a28();
s32 Func_020019a8();
void Func_020019f0_a();
void Func_020019f8();
void Func_02001a78();
void Func_020019d0();
s32 Func_02001988();
void Func_0200172c();
void Func_02002b52(struct PaletteEffectFrame *frame);
void Func_02002ce2(s32);
struct PaletteEffect *Func_02002b84(s32, s32, s32, s32);
void Func_02002bb4(struct PaletteEffect *, s32);
void Func_02002be4(struct PaletteEffect *, s32, s32, s32);
void Func_02002bd4(struct PaletteEffect *, u8 *);
void Func_02002b7a(s32);
void Func_02002d50(s32);
void Func_02002b8c(s32);
void Func_02002d62(s32);
void Func_02002e82();
void Func_02002eb4();
void Func_02002eda();
s32 Func_020030b0();
s32 Func_020030b8();
struct PaletteSceneRecord *Func_020030bc();
void Func_02002fa6();
u16 Func_02002f6c(u16, s32);
void Func_0200302a();
void Func_0200300e();
void Func_02003238();
s32 Func_020030ea();
s32 Func_020030f8();
s32 Func_02003106();
void Func_02001462();
void Func_02001478();
void Func_0200148c();
s32 Func_020014a2();
s32 Func_02001ae2();
void Func_02001af4();
void Func_02001af6();
s32 Func_02001b0c();
void Func_02001b26();
s32 Func_02001b2c();
void Func_02001b94();
void Func_02001bac();
void Func_02001bbe();
void Func_02001bc2();
void Func_02001bd6();
void Func_02001bf2();
void Func_02001c0a();
void Func_02001c10_a();
void Func_02001c30();
void Func_02001c38();
void Func_02001c5a();
void Func_02001c70();
void Func_02001c8a();
void Func_02001ca0();
void Func_02001c1c();
void Func_02001c38_a();
void Func_02001c3a();
s32 Func_02001ca2();
void Func_02001cac();
void Func_02001cb8_a();
void Func_02001ce2();
void Func_02001cea();
s32 Func_02001d04();
void Func_02001d2c();
void Func_02001d3a();
s32 Func_02001d40();
void Func_02001d56();
void Func_02001d5a();
void Func_02001d70();
void Func_02001d80();
void Func_02001d82();
u8 *Func_02001d92();
void Func_02001d96();
void Func_02001d96_a();
void Func_02001da0();
void Func_02001da6();
void Func_02001db4();
void Func_02001dca();
void Func_02001dde();
void Func_02001df6();
void Func_02001e18();
void Func_02001e1a();
void Func_02001e2a();
void Func_02001e3a();
void Func_02001e48();
void Func_02001e50();
void Func_02001e58();
void Func_02001e58_a();
void Func_02001e6c();
void Func_02001e9e();
void Func_02001eb4();
void Func_02001ebc();

/* One symbol per call site, named at the site's decoded address. All three
 * reach the same helper, which scales one channel by the adjustment, and each
 * site still needs its own name. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern s32 Data_02009dd0;

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern s32 Data_02009dd0;

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern s32 Data_02009dd0;

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern s32 Data_02009dd0;

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern s32 Data_02009dd0;

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern s32 Data_02009dd0;

    f(a0, a1, a2, a3);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000158(void (*f)(), s32 a0)
{
    f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020002ec(void (*f)(), s32 a0)
{
    extern u8 Data_02009dd0[];

    f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_02009dd0[];

    return f(a0, a1, a2);
}

/*
 * Returns the in-image table at 0x02009ba4. The eight-byte owner at 0x02000030
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetScriptData(void)
{
    extern s32 Data_02009dd0;

    return (u8 *)0x02009ba4;
}

/*
 * Returns the in-image table at 0x02009c04. The eight-byte owner at 0x02000038
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetMessageData(void)
{
    extern s32 Data_02009dd0;

    return (u8 *)0x02009c04;
}

/*
 * Returns the in-image table at 0x02009c24. The eight-byte owner at 0x02000040
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetActorData(void)
{
    extern s32 Data_02009dd0;

    return (u8 *)0x02009c24;
}

/*
 * Returns the in-image table at 0x02009c34. The eight-byte owner at 0x02000048
 * includes its one pool word, which holds that address and is returned
 * without being dereferenced.
 */
u8 *PaletteScene_GetEffectData(void)
{
    extern s32 Data_02009dd0;

    return (u8 *)0x02009c34;
}

void PaletteScene_Initialize(void)
{
    extern s32 Data_02009dd0;

    void *scene;

    scene = *(void **)0x03001EBC;
    Func_020019f0();
    Func_02001b0a(0, 0, 0);
    Func_02001ae0(FIELD_AT_OFFSET(scene, s16 *, 0x16C));
    Func_02001a14();
}

/* Returns this overlay's state block. */
u8 *PaletteScene_GetState(void) { return (u8 *)0x02009d3c; }

void FieldScene_RunScene395_02000158(void)
{
    u32 i;
    s32 record;

    Func_02001af4();
    if (Value1(Func_02001ae2, 0x845) != 0) {
        Func_02001462(10, 1);
        Call1_02000158(Func_02001b94, 0x151c);
        Func_02001bac(8, 0);
        Func_02001478(10, 0);
    } else {
        if (Value1(Func_02001b0c, 0x844) != 0) {
            Func_0200148c(10, 1);
            Call1_02000158(Func_02001bbe, 0x14eb);
            Func_02001bd6(8, 0);
            Value2(Func_020014a2, 10, 0);
            record = Func_02001b2c(184);
            if (record == -1) {
                goto L_02000220;
            }
            {
                u16 *target = (u16 *)(*(u8 *volatile *)Data_03001ebc + 0x172);
                s32 shown = 1;

                *target = shown;
            }
        } else {
            Call1_02000158(Func_02001bf2, 0x14c9);
            Func_02001c0a(8, 0);
            Call2(Func_02001c5a, 0x406218, 1);
            Func_02001c70(20);
            Func_02001af6(40);
            Call3(Func_02001c30, 0x200e, 0, 10);
            Func_02001c10_a(0, 2);
            Call2(Func_02001c38, 0x200e, 0);
            Call2(Func_02001c8a, 0x10000, 1);
            Func_02001ca0(20);
            Func_02001b26(40);
        }
    }
    L_02000220:;
    Func_02001bc2();
}

void PaletteScene_RunActorNineBranch(void)
{
    extern s32 Data_02009dd0;

    Func_02001be4();
    if (Func_02001bd2(0x845) != 0) {
        Func_02001c7c(0x151F);
    } else {
        Func_02001c84(0x14C8);
    }
    Func_02001c9c(9, 0);
    Func_02001c10();
}

void PaletteScene_RunActorEightBranch(void)
{
    extern s32 Data_02009dd0;

    Func_02001c20();
    if (Func_02001c0e(0x845) != 0) {
        Func_02001cb8(0x151E);
    } else {
        Func_02001cc0(0x14EC);
    }
    Func_02001cd8(8, 0);
    Func_02001c4c();
}

void PaletteScene_RunFlaggedBranch(void)
{
    extern s32 Data_02009dd0;

    Func_02001c5c();
    Func_02001d58();
    if (Func_02001c4e(0x844) == 0) {
        Func_020005c2();
    } else {
        Func_02000764();
    }
    Func_02001c80();
}

void RunEventScript01(void)
{
    extern u8 Data_02009dd0[];

    u32 i;
    s32 rec8;
    u8 *record;
    s32 base5_2009dd0;

    rec8 = Func_02001ca2(0);
    Value3(Func_02001d40, 0, 0xc000, 0);
    Call2(Func_02001d80, 0x406218, 1);
    Func_02001d96(20);
    Func_02001c1c(40);
    Func_02001dca(17);
    base5_2009dd0 = (s32)Data_02009dd0;
    *(volatile s32 *)base5_2009dd0 = 1;
    Call2(Func_02001c3a, 0x2009219, 0xc80);
    Func_02001c38_a(30);
    *(volatile s32 *)base5_2009dd0 = 0;
    Call4(Func_02001da6, 0x1480000, -1, 0xeb0000, 1);
    Func_02001d96_a(0, 1);
    *(u8 *)(Func_02001d04(0) + 90) &= 254;
    Func_02001d56(0, 16);
    Call3(Func_02001d2c, 0, 0x20000, 0x20000);
    Func_02001e2a(133);
    *(volatile s32 *)(rec8 + 40) = 0x50000;
    *(volatile s32 *)(rec8 + 72) = 0x4000;
    *(volatile s32 *)(rec8 + 68) = 0xa000;
    Call3(Func_02001d70, 0, 0x14f, 0x102);
    if (*(volatile s32 *)(rec8 + 40) >= 0) {
        do {
            Func_02001cac(1);
        } while (*(volatile s32 *)(rec8 + 40) >= 0);
    }
    do {
        Func_02001cb8_a(1);
    } while (*(volatile s32 *)(rec8 + 40) <= 0);
    Func_02001e6c(161);
    Func_02001db4(0, 19);
    Func_02001d5a(120);
    Call1_020002ec(Func_02001cea, 0x2009219);
    Func_02001ce2(40);
    *(volatile s32 *)(rec8 + 68) = 0x4000;
    {
        u8 *record = Func_02001d92(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Func_02001d82(80);
    Call1_020002ec(Func_02001e18, 0x14cc);
    Call3(Func_02001e3a, 0x200e, 0, 20);
    Func_02001e1a(0, 2);
    Func_02001da0(20);
    Call2(Func_02001e48, 0x200e, 0);
    Func_02001ebc();
    Call2(Func_02001e9e, 0x10000, 1);
    Func_02001eb4(20);
    Func_02001d3a(40);
    {
        s32 shown = 0xc000;

        *(volatile u16 *)(rec8 + 6) = shown;
    }
    *(volatile s32 *)(rec8 + 72) = 0x10000;
    *(volatile s32 *)(rec8 + 68) = 0x4000;
    Func_02001e58(0, 2);
    Func_02001dde(40);
    Func_02001e58_a(0, 4, 0);
    Func_02001e50(0, 1);
    Func_02001df6(20);
}

/* The scene's actor transition sequence. Actor three takes part only when the
 * saved flag reports it enabled, and the arms that skip it bump the step
 * counter instead. */
void PaletteScene_RunActorTransitionSequence(void)
{
    extern s32 Data_02009dd0;

    s32 actorThreeEnabled;
    u8 *object;
    s32 *transitionState;
    s32 cycle;
    s32 sceneWorkSlot;
    s32 effectCallback;
    const s32 *finalActions;

    actorThreeEnabled = Func_02001980(3);
    Call3(Func_020019e0, 0, 0x148, 212);
    Call3(Func_02001a40, 0, 0xc000, 20);
    Func_02001ab0(17);
    Call2(Func_02001960, 0x14ed, 1);
    Call3(Func_020019b8, 1, 0x10000, 0x8000);
    Call3(Func_020019b8, 2, 0x10000, 0x8000);
    object = Func_020019b0(0);
    if (object != 0) {
        Func_020019e8(1, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    object = Func_020019b0(0);
    if (object != 0) {
        Func_020019e8(2, *(s32 *)(object + 8), *(s32 *)(object + 16));
    }
    Func_020019c0(1, SceneAction_ActorOneEntry);
    Func_020019c0(2, SceneAction_ActorTwoEntry);
    if (actorThreeEnabled != 0) {
        Call3(Func_020019b8, 3, 0x10000, 0x8000);
        object = Func_020019b0(0);
        if (object != 0) {
            Func_020019e8(3, *(s32 *)(object + 8), *(s32 *)(object + 16));
        }
        Func_020019c0(3, SceneAction_ActorThreeEntry);
    }
    Func_020019c8(2);
    Func_02001990(40);
    Func_020018b8(0);
    Func_02001a88(32);
    Func_02001908(40);
    transitionState = &Data_02009dd4;
    *transitionState = 0;
    Value2(Func_02001910, (s32)Func_020012b4, 0xc80);
    Func_02001990(40);
    Call3(Func_02001a40, 1, 0x6000, 20);
    Call2(Func_02001a58, 0x33333, 0x6666);
    Call4(Func_02001a60, 0x1000000, -1, 0xfe0000, 1);
    Func_02001a68();
    Func_02001ab0(246);
    Func_02001990(40);
    Call3(Func_02001a40, 2, 0x2000, 20);
    Call4(Func_02001a60, 0x19d0000, -1, 0x1050000, 1);
    Func_02001a68();
    Func_02001ab0(246);
    Func_02001990(40);
    Call3(Func_02001a40, 0, 0x4000, 0);
    Call3(Func_02001a40, 3, 0x4000, 20);
    Call4(Func_02001a60, 0x1460000, -1, 0x1800000, 1);
    Func_02001a68();
    Func_02001ab0(246);
    if (*transitionState != 24) {
        do {
            Func_02001908(1);
        } while (*transitionState != 24);
    }
    Value1(Func_02001918, (s32)Func_020012b4);
    Func_02001908(10);
    cycle = 0;
    do {
        Func_020018b8(0);
        Func_02001a88(6);
        Func_02001908(6);
        Func_020018b8(1);
        Func_02001a88(6);
        cycle = (cycle + 1);
        Func_02001908(6);
    } while ((u32)cycle <= 3);
    Func_020018b8(0);
    Func_02001a88(40);
    Func_02001908(80);
    Call4(Func_02001a60, 0x1480000, 0x80000, 0xd40000, 1);
    Func_02001a68();
    Func_02001990(40);
    Func_020012f4(10, 1);
    Func_02001990(40);
    Func_02001ab0(7);
    Call1(Func_02001a20, 0x14ee);
    Func_02001a30(8, 0);
    Func_02001a08(0, 2);
    Func_02001a08(1, 2);
    Func_02001a08(3, 2);
    Func_02001a10(2, 2);
    Call3(Func_02001a40, 0, 0xc000, 0);
    Call3(Func_02001a40, 1, 0xc000, 0);
    Call3(Func_02001a40, 3, 0xc000, 0);
    Call3(Func_02001a40, 2, 0xc000, 20);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Func_020012f4(10, 3);
    Func_02001990(40);
    Func_020012f4(10, 1);
    Func_02001990(20);
    Func_02001a30(8, 0);
    Call3(Func_02001a50, 0, 0x105, 0);
    Call3(Func_02001a50, 1, 0x105, 0);
    Call3(Func_02001a50, 3, 0x105, 0);
    Call3(Func_02001a50, 2, 0x105, 40);
    Call4(Func_02001a60, 0xea0000, 0, 0xe80000, 1);
    Func_02001a68();
    Func_02001990(40);
    Func_020012f4(11, 1);
    Func_02001990(40);
    Func_020012f4(11, 3);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 20);
    Func_020012f4(11, 2);
    Func_02001990(10);
    Call3(Func_02001a40, 0, 0x6000, 0);
    Call3(Func_02001a40, 1, 0x6000, 0);
    Call3(Func_02001a40, 2, 0x6000, 0);
    Call3(Func_02001a40, 3, 0x6000, 20);
    Func_020012f4(11, 3);
    Func_02001990(20);
    Func_020012f4(11, 2);
    Func_02001990(20);
    Func_020012f4(11, 3);
    Call3(Func_02001a38, 0x4009, 0, 10);
    Func_020012f4(10, 0);
    Func_02001990(20);
    Call2(Func_02001a30, 0x8008, 0);
    Func_020012f4(10, 1);
    Func_02001990(20);
    Value2(Func_02001a28, 0x8008, 0);
    Call3(Func_02001a40, 0, 0xc000, 0);
    Call3(Func_02001a40, 1, 0xe000, 0);
    Call3(Func_02001a40, 2, 0xa000, 0);
    Call3(Func_02001a40, 3, 0xc000, 0);
    if (Value2(Func_020019a8, 0, 0) == 0) {
        Call2(Func_02001a30, 0x4009, 0);
        Call2(Func_02001a30, 0x8008, 0);
    } else {
        sceneWorkSlot = 0x3001ebc;
        *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 2;
        Call3(Func_02001a50, 3, 0x103, 0);
        Call3(Func_02001a50, 1, 0x103, 0);
        Call3(Func_02001a50, 2, 0x103, 40);
        Func_020019f0_a(1, 4);
        Func_02001a30(1, 0);
        if (actorThreeEnabled != 0) {
            Func_02001a10(3, 2);
            Func_02001a30(3, 0);
        } else {
            *(u16 *)((*(s32 *)sceneWorkSlot + 0x1d8)) += 1;
        }
        Func_020019f8(2, 3);
        Func_02001a30(2, 0);
        Call2(Func_02001a30, 0x4009, 0);
        Call2(Func_02001a30, 0x8008, 0);
    }
    Func_020019f0_a(0, 3);
    Func_020019f0_a(1, 3);
    Func_020019f0_a(3, 3);
    Func_020019f8(2, 3);
    Call4(Func_02001a60, 0x1480000, 0x80000, 0xd40000, 1);
    Func_02001a68();
    Func_02001990(20);
    Func_020012f4(10, 0);
    Func_02001990(20);
    Func_020018b8(0);
    Func_02001a88(1);
    Func_02001908(1);
    Call2(Func_02001a78, 0x406218, 1);
    Func_02001a88(40);
    Func_02001990(60);
    Data_02009dcc = 0;
    Data_02009dc0[0] = 0x1480000;
    Data_02009dc0[1] = 0x300000;
    effectCallback = (s32)Func_020015a0;
    Data_02009dc0[2] = 0xcd0000;
    Value2(Func_02001910, effectCallback, 0xc80);
    Func_02001990(100);
    Func_02001918(effectCallback);
    Call2(Func_02001a78, 0x7fff, 0);
    Func_02001a88(60);
    Func_02001990(100);
    Func_020018b8(0);
    Func_02001a88(20);
    Func_02001990(40);
    Func_020012f4(10, 1);
    Func_02001990(10);
    Call1(Func_02001a20, 0x14fb);
    Call2(Func_02001a30, 0x8008, 0);
    Func_020019f0_a(0, 3);
    Func_020019f0_a(1, 3);
    Func_020019f0_a(3, 3);
    Func_020019f8(2, 3);
    Call4(Func_02001a60, 0xea0000, 0, 0xe80000, 1);
    Func_02001a68();
    Func_02001990(20);
    Call2(Func_02001a30, 0x4009, 0);
    Call3(Func_02001a38, 0x8008, 0, 10);
    Func_02001a10(1, 2);
    Call3(Func_02001a40, 0, 0x6000, 0);
    Call3(Func_02001a40, 1, 0xe000, 10);
    Value2(Func_02001a28, 1, 0);
    if (Value2(Func_020019a8, 0, 0) == 0) {
        Call3(Func_02001a50, 1, 0x102, 40);
    } else {
        Func_020019f8(1, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02001a30(1, 0);
    Func_020012f4(10, 4);
    Func_02001990(20);
    Call1(Func_02001a20, 0x1501);
    Call2(Func_02001a30, 0x8008, 0);
    Call3(Func_02001a40, 0, 0xc000, 0);
    Call3(Func_02001a40, 1, 0xc000, 0);
    Call2(Func_02001a30, 0x8008, 0);
    Func_020019f0_a(0, 3);
    Func_020019f0_a(1, 3);
    Func_020019f0_a(3, 3);
    Func_020019f8(2, 3);
    Func_020012f4(10, 4);
    Func_02001990(20);
    Call3(Func_02001a38, 0x8008, 0, 20);
    Func_020012f4(11, 0);
    Call3(Func_02001a38, 0x4009, 0, 20);
    Func_020012f4(11, 3);
    Func_02001990(40);
    Func_020012f4(11, 1);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 20);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Call2(Func_02001a30, 0x8008, 0);
    Call3(Func_02001a50, 0, 0x102, 0);
    Call3(Func_02001a50, 1, 0x102, 0);
    Call3(Func_02001a50, 3, 0x102, 0);
    Call3(Func_02001a50, 2, 0x102, 80);
    Func_020012f4(11, 5);
    Func_02001990(60);
    Func_020012f4(11, 3);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 20);
    Func_020012f4(10, 5);
    Func_02001990(40);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4008, 0, 20);
    Func_02001a10(1, 2);
    Call3(Func_02001a40, 1, 0x8000, 10);
    Func_02001a30(1, 0);
    Call3(Func_02001a40, 2, 0x8000, 20);
    Call2(Func_02001a30, 0x8002, 0);
    Func_020012f4(11, 4);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 20);
    Call3(Func_02001a40, 1, 0xe000, 0);
    Call3(Func_02001a40, 0, 0x6000, 10);
    Func_02001a38(1, 0, 20);
    Func_020012f4(10, 1);
    Call3(Func_02001a38, 0x8008, 0, 10);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Func_020012f4(11, 3);
    Func_02001990(40);
    Func_020012f4(11, 0);
    Func_02001990(20);
    Func_020018b8(0);
    Func_02001a88(1);
    Func_02001908(1);
    Call2(Func_02001a78, 0x406218, 1);
    Func_02001a88(40);
    Func_02001990(60);
    Data_02009dcc = 0;
    Data_02009dc0[0] = 0x880000;
    Data_02009dc0[1] = 0x140000;
    effectCallback = (s32)Func_020015a0;
    Data_02009dc0[2] = 0x1020000;
    Value2(Func_02001910, effectCallback, 0xc80);
    Func_02001990(100);
    Call3(Func_02001a40, 0, 0x6000, 0);
    Call3(Func_02001a40, 1, 0x6000, 0);
    Call3(Func_02001a40, 3, 0x6000, 0);
    Call3(Func_02001a40, 2, 0x6000, 40);
    Func_02001a08(2, 1);
    Call3(Func_02001a50, 2, 0x100, 20);
    Call3(Func_02001a38, 0x8002, 0, 10);
    Func_02001a10(0, 2);
    Call3(Func_02001a40, 0, 0x2000, 10);
    Func_020019f8(0, 3);
    Func_020012f4(10, 4);
    Func_02001990(20);
    Call2(Func_02001a30, 0x8008, 0);
    Call3(Func_02001a50, 2, 0x101, 60);
    Call3(Func_02001a40, 2, 0xc000, 10);
    Call3(Func_02001a38, 0x8002, 0, 10);
    Call3(Func_02001a40, 0, 0xc000, 0);
    Call3(Func_02001a40, 3, 0xc000, 0);
    Call3(Func_02001a40, 1, 0xc000, 20);
    Call3(Func_02001a38, 0x8008, 0, 10);
    Func_020019f0_a(0, 3);
    Func_020019f0_a(1, 3);
    Func_020019f0_a(3, 3);
    Func_020019f8(2, 3);
    Func_02001990(10);
    Call3(Func_02001a40, 0, 0x6000, 0);
    Call3(Func_02001a40, 1, 0x6000, 0);
    Call3(Func_02001a40, 3, 0x6000, 0);
    Call3(Func_02001a40, 2, 0x6000, 120);
    Func_02001918(effectCallback);
    Func_02001990(60);
    Func_020018b8(0);
    Func_02001a88(40);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Call3(Func_02001a38, 0x8008, 0, 20);
    Func_020012f4(11, 3);
    Call2(Func_02001a30, 0x4009, 0);
    Call2(Func_02001a30, 0x8008, 0);
    Func_020012f4(11, 4);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 10);
    Func_02001a08(0, 2);
    Func_02001a08(1, 2);
    Func_02001a08(3, 2);
    Func_02001a10(2, 2);
    Func_020012f4(10, 1);
    Value2(Func_02001a28, 0x8008, 0);
    Call3(Func_02001a40, 0, 0xc000, 0);
    Call3(Func_02001a40, 1, 0xe000, 0);
    Call3(Func_02001a40, 3, 0xc000, 0);
    Call3(Func_02001a40, 2, 0xa000, 0);
    if (Value2(Func_020019a8, 0, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02001990(10);
    Func_020012f4(10, 2);
    Func_02001990(20);
    Func_020012f4(11, 3);
    Func_02001990(40);
    Func_020012f4(10, 1);
    Func_02001990(20);
    Call3(Func_02001a38, 0x8008, 0, 10);
    Call3(Func_02001a40, 0, 0x4000, 0);
    Func_02001a40(1, 0, 0);
    Call3(Func_02001a40, 3, 0xc000, 0);
    Call3(Func_02001a40, 2, 0x8000, 10);
    Func_020019f0_a(0, 3);
    Func_020019f0_a(1, 3);
    Func_020019f0_a(3, 3);
    Func_020019f8(2, 3);
    Func_02001ab0(17);
    finalActions = SceneAction_GroupFinish;
    Func_020019c0(1, finalActions);
    if (actorThreeEnabled != 0) {
        Func_020019c0(3, finalActions);
    }
    Call2(Func_020019d0, 2, (s32)finalActions);
    Func_020012f4(10, 4);
    Func_020012f4(10, 4);
    Func_02001990(20);
    Call1(Func_02001a20, 0x1519);
    Call2(Func_02001a30, 0x8008, 0);
    Func_020012f4(11, 4);
    Func_020012f4(11, 4);
    Func_02001990(20);
    Call3(Func_02001a38, 0x4009, 0, 10);
    Func_020019f8(0, 3);
    Value1(Func_02001988, 0x845);
    Func_02001ab0(1);
    Func_0200172c(184, 185);
}

s32 PaletteScene_AdvanceEffectFrame(struct PaletteEffectFrame *frame)
{
    extern s32 Data_02009dd0;

    frame->progress += 0x1EB8;
    if (frame->limit == 0x80000000) {
        if (frame->second_limit == frame->limit) {
            if (frame->third_limit == frame->second_limit) {
                Func_02002b52(frame);
            }
        }
    }
    return 1;
}

void PaletteScene_SpawnEffect(void)
{
    extern s32 Data_02009dd0;

    struct PaletteEffect *effect;
    struct EffectSprite *sprite;
    s32 phase;
    s32 effect_flags;
    s32 sprite_flags;
    s32 spawn_x = 0x01460000;
    s32 spawn_y = 0x00200000;
    s32 spawn_z = 0x00c00000;
    s32 target_x = 0x01460000;
    s32 target_z = 0x00f00000;

    phase = Data_03001e40 & 3;
    if (phase != 0) return;
    if (Data_02009dd0 != 0) Func_02002ce2(200);
    effect = Func_02002b84(26, spawn_x, spawn_y, spawn_z);
    if (effect == 0) return;
    sprite = effect->sprite;
    sprite->state = phase;
    effect_flags = 0xfe;
    effect_flags &= effect->flags;
    effect->flags = effect_flags;
    sprite_flags = ~12;
    sprite_flags &= sprite->flags;
    sprite_flags |= 4;
    sprite->flags = sprite_flags;
    effect->progress = 0x1999;
    effect->rate_x = 0x40000;
    effect->rate_y = 0x40000;
    effect->mode = phase;
    Func_02002bb4(effect, 2);
    Func_02002be4(effect, target_x, 0, target_z);
    Func_02002bd4(effect, Data_02009d9c);
}

/* Steps the shared transition counter, firing at 0 and at 20 and wrapping at
 * 30. */
void PaletteScene_AdvanceTransition(void)
{
    extern s32 Data_02009dd0;

    s32 step = Data_02009dd4;

    if (step == 0) {
        Func_02002b7a(0);
        Func_02002d50(20);
    } else if (step == 20) {
        Func_02002b8c(1);
        Func_02002d62(8);
    }
    step = Data_02009dd4 + 1;
    Data_02009dd4 = step;
    if (step == 30) {
        Data_02009dd4 = 0;
    }
}

void PaletteScene_AdvanceOrbit(struct OrbitingPaletteEffect *effect)
{
    extern s32 Data_02009dd0;

    s32 position[3];
    s32 step = effect->step;
    s32 heading;

    if (step <= 119) {
        position[0] = effect->anchor_x;
        position[1] = effect->anchor_y;
        position[2] = effect->anchor_z;
        heading = effect->heading;
        Func_02002e82(step << 16, step * 768 + heading, position);
        effect->x = position[0];
        effect->y = position[1];
        effect->z = position[2];
        effect->angle_x += 0x147;
        effect->angle_y += 0x147;
        effect->step++;
    } else {
        Func_02002eb4(effect->owner[0x1c]);
        Func_02002eda(effect);
    }
}

/* Two lookups, each of which can fail with -1; on success stores the caller's
 * halfword into the table at +216 of the record the first index names. */
void PaletteScene_SetRecordValue(s32 key, s32 value)
{
    extern s32 Data_02009dd0;

    s32 slot = Func_020030b0(key);

    if (slot != -1) {
        s32 index = Func_020030b8(slot, key);

        if (index != -1) {
            Func_020030bc(slot)->values[index] = value;
        }
    }
}

/* Applies the adjustment to palette RAM, skipping two protected windows. */
void PaletteScene_AdjustPaletteWindow(s32 adjustment)
{
    extern s32 Data_02009dd0;

    volatile u16 *palette = (volatile u16 *)0x05000000;
    u32 phase;
    u32 next_phase;
    Func_02002fa6();
    phase = 0;
    do {
        u32 index = phase >> 16;
        u32 second_window;

        if ((u32)(phase + 0xffef0000) > 0x60000) {
            second_window = (index + 0xff3f) << 16;
            if (second_window > 0x70000)
                palette[index] = Func_02002f6c(palette[index], adjustment);
        }
        next_phase = phase + 0x10000;
        phase = next_phase;
    } while (next_phase <= 0x00df0000);
    Func_0200302a(); Func_0200300e(); Func_02003238(0x10000, 0);
}

/*
 * Applies the asymmetric RGB555 colour adjustment: red rises, green and blue
 * fall. Control jumps over a mask literal inside the span and rejoins before
 * the common return, so the literal belongs to this owner.
 */
u16 PaletteScene_AdjustColor(u16 color, s32 adjustment)
{
    extern s32 Data_02009dd0;

    s16 green = (s16)((color >> 5) & 31);
    s16 red = (s16)(color & 31);
    s16 blue = (s16)((color >> 10) & 31);
    u32 packed;

    red = (s16)(red + Func_020030ea(
        red,
        (s32)((u32)adjustment << 2)
    ));
    green = (s16)(green - Func_020030f8(green, adjustment));
    blue = (s16)(blue - Func_02003106(blue, adjustment));

    /* Only the increasing channel is explicitly saturated by this owner. */
    if (red > 31)
        red = 31;

    packed = (u32)(s32)red;
    packed |= ((u32)(s32)blue << 10) | ((u32)(s32)green << 5);
    return (u16)packed;
}
