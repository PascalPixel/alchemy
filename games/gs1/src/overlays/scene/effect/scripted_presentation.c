#include "types.h"

#define CreateOverlayObject Func_02005d1e
#define SetOverlayObjectMode Func_02005d88
#define SetOverlayObjectSlot Func_02005ed0
#define OvObj_SetRecordField1 Func_02000030
#define OvObj_CreateConfiguredObject Func_020000a0
#define OvObj_StepScaleByCounter Func_02000314
#define OvObj_DecayRecordField1e Func_02000400
#define OvObj_UpdateHeadingTimer Func_02000424
#define NULL ((void *)0)
#define OvObj_CreateAndInitialize Func_02000048
#define Effect_UpdateObjectByFrameParity Func_020020dc
#define Effect_AdvanceGatedRiseCounter Func_020059f0
#define EffectDescriptorTable Data_0200dfb8
#define GetPartyEffect Func_02005eaa
#define SpawnEffect Func_02005df0
#define SetEffectVariant Func_02005dfa
#define SetEffectDescriptor Func_02005e14
#define SetEffectMode Func_02006006
#define ScaleEffectDeltaFromAccumulated Func_02005e68
#define ScaleEffectDeltaFromOrigin Func_02005e80
#define ScaleEffectVerticalDelta Func_02005e8e
#define SetEffectCallbackMode Func_02005f14
#define SetEffectCallbackArgument Func_02005f24
#define Effect_SpawnConfigured Func_0200013c
#define SceneData_GetTablee3d4 Func_02000464
#define SceneData_ReturnZero Func_0200046c
#define SceneData_GetTablee464 Func_02000470
#define SceneData_GetTablee478 Func_02000478
#define SceneData_GetTableE6ec Func_02000714
#define MeasureFixedPointPositionDistance Func_02000480
#define Actor_FindNearestSlotOfKindF2 Func_020004bc
#define InitializeActorZeroMotion Func_02000518
#define Actor_ParkRecord Func_02005688
#define Effect_SpawnAndBobWithActorZero Func_020005ec
#define Actor_SetByte55ForActorZeroAnd12To17 Func_020006c0
#define Scene_InitActorsAndDispatchBySubstate Func_0200071c
#define Scene_RunSetupSequence35c4 Func_020035c4
#define Scene_RunThreeStepsInBracket Func_020038c0
#define Scene_RunBracketedSceneWithFlag282 Func_020038dc
#define State_ApplyArgMode0AndSet10 Func_02000894
#define State_ForwardByRuntimeWordBits Func_0200211c
#define State_ApplyPair140And0 Func_02002334
#define Scene_CallPairWith10 Func_020008a8
#define Scene_ForwardValue81fc Func_02002344
#define FieldScene_RunScene3c9_02001280 Func_02001280
#define Scene_RunStep6 Func_02002350
#define FieldScene_RunScene3c9_02003924 Func_02003924
#define FieldScene_RunScene3c9_02004b28 Func_02004b28
#define FieldScene_RunScene3c9_02005b90 Func_02005b90
#define Effect_AnimateVerticalPositive Func_0200215c
#define Effect_AnimateVerticalNegative Func_020021ac
#define Effect_UpdateCounterDrivenOrbit Func_02003660
#define Scene_RunMultiActorPresentation Func_02003e9c

#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"

typedef struct {
    u8 filler0[9];
    u8 f0 : 2;
    u8 f1 : 2;
    u8 f2 : 4;
} Sub;

typedef struct {
    u8 filler0[0x50];
    Sub *sub;
} Obj;

typedef struct {
    u8 filler0[0x18];
    s32 unk18;
    s32 unk1c;
    u8 filler20[0x44];
    u16 unk64;
} Spr;

typedef struct {
    u8 filler0[0x1e];
    u16 unk1e;
} Obj_02000400;

typedef struct {
    u8 filler0[0x50];
    Obj_02000400 *obj;
} Spr_02000400;

typedef struct {
    u8 filler0[6];
    u16 unk06;
} Spr_02000424;

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Obj_020004bc;

typedef struct {
    u8 filler0[0x50];
    Obj_020004bc *obj;
} Spr_020004bc;

struct SceneActor {
    u8 pad00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[20];
    s32 motion28;
    u8 pad2c[4];
    s32 motion30;
    s32 motion34;
    u8 pad38[29];
    u8 flags55;
};

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[4];
    s32 unk14;
    u8 filler18[0x3d];
    u8 unk55;
} Spr_020005ec;

typedef struct {
    u8 filler0[0xc];
    s32 unk0c;
    u8 filler10[8];
    s32 unk18;
    u8 filler1c[7];
    u8 unk23;
    u8 filler24[0x31];
    u8 unk55;
} Spr_0200071c;

struct StagedVerticalEffect {
    s32 pad0[2];
    s32 f8;
    s32 fc;
    s32 f10;
    s32 pad14;
    s32 f18;
    s32 f1c;
    s32 pad20[17];
    s16 f64;
    s16 pad66;
    s32 *f68;
};

extern unsigned char Data_0200e3d4[];
extern unsigned char Data_0200e464[];
extern unsigned char Data_0200e478[];
extern unsigned char Data_0200e6ec[];
extern Spr *Data_0200e6e8;
extern s16 Data_02000240[];

s32 Func_02005f8e(Spr *);
s32 Func_02005f74(s32, s32);
s32 Func_0200604c(void);
s32 Func_0200605a(void);
void *Func_02005cc6(s32, s32, s32, s32);
void Func_02005d2c(void *, s32);
void Func_02005d44(void *, s32);
void Func_02005e74(void *, s32);
void Func_02007dc4(s32, s32);
void Func_02007dce(s32, s32);
s32 Func_02007cf8(s32, s32);
void Func_0200430e(s32);
void Func_0200b096(void *record);
typedef s32(*IwramIntegerSquareRoot)(s32);
Spr *Func_0200621c(s32);
s32 Func_02000976(u8 *, u8 *);
void Func_02000c3a();
void Func_02006184();
void Func_0200618e();
void Func_020061de();
void Func_020061ec();
s32 Func_0200620e();
void Func_0200622a();
void Func_02006264();
void *Func_02006270();
void Func_02006282();
void Func_02006292();
void Func_02006352();
void Func_0200646e();
Spr *Func_0200633e(s32);
void Func_0200632c(void);
Spr *Func_02000ab8(void);
void Func_02006320(s32);
Spr *Func_0200635e(Spr *);
void Func_0200623e(s32);
void Func_02006258(s32);
void Func_02006274(s32);
void Func_0200628c(s32);
void Func_020063e6(void);
u8 *Func_02006412(s32);
u8 *Func_0200641e(s32);
u8 *Func_0200642a(s32);
u8 *Func_02006434(s32);
u8 *Func_0200643e(s32);
u8 *Func_02006448(s32);
u8 *Func_02006452(s32);
void Func_02006438(s32);
void Func_02006438_a(Spr *, s32);
void Func_02006326(s32);
void Func_02006446(s32);
Spr *Func_02006484(s32);
void Func_020063ea(Spr *, s32);
Spr *Func_02006490(s32);
void Func_020063f6(Spr *, s32);
Spr *Func_0200649c(s32);
void Func_02006402(Spr *, s32);
Spr *Func_020064a8(s32);
void Func_0200640e(Spr *, s32);
Spr *Func_020064b4(s32);
Spr *Func_020064be(s32);
Spr *Func_020064ca(s32);
Spr *Func_020064d2(s32);
void Func_020065b0(s32, s32);
s32 Func_020064f8(s32);
void Func_020010a8(void);
void Func_02002b5a(void);
void Func_020040c0(void);
void Func_020040e2(void);
void Func_02004130(void);
void Func_02006546(void);
s32 Func_02006524(s32);
s32 Func_02006532(s32);
s32 Func_02006540(s32);
void Func_02006534(s32, s32);
void Func_0200653e(s32, s32);
void Func_020066ac(s32);
s32 Func_02006562(s32);
s32 Func_02000d1a(void);
Spr *Func_020065b2(void);
void Func_02009496(s32);
void Func_0200944e(s32, s32);
void Func_02009464(s32);
void Func_020091da(s32);
void Func_02009462(s32, s32);
void Func_02009478(s32);
void Func_020091ee(s32);
void Func_020095f8(void);
void Func_02009610(void);
void Func_02007490();
void Func_02007768();
void Func_02007b0c();
void Func_0200669e(s32, s32);
void Func_020065cc(s32);
s32 Func_02007d28(u32, s32);
void Func_02007e10(s32, s32);
s32 Func_02007d38(u32, s32);
void Func_0200434e(s32);
void Func_020081e8(s32, s32);
void Func_020066c2(s32, s32, s32);
void Func_020081fc(s32);
void Func_02006f44();
void Func_02006f64();
s32 Func_02006fde();
s32 Func_02006ffe();
s32 Func_02006ffe_a();
void Func_02007070();
void Func_02007090();
void Func_020080a2(s32);
void Func_02004476(void);
void Func_02008784();
void Func_020090a2();
void Func_020090d4();
void Func_02009106();
void Func_02009134();
void Func_0200917e();
void Func_0200918e();
void Func_020091a2();
void Func_020091b2();
void Func_020095b6();
void Func_020095cc();
void Func_020095d6();
void Func_020095ea();
void Func_020095fe();
void Func_0200961c();
void Func_0200962e();
void Func_02009640();
void Func_02009650();
void Func_02009650_a();
void Func_0200966c();
s32 Func_0200967e();
void Func_020096a6();
void Func_020096da();
void Func_0200970c();
void Func_02009734();
void Func_0200973a();
void Func_0200973e();
s32 Func_02009740();
s32 Func_02009774();
s32 Func_02009774_a();
s32 Func_02009780();
s32 Func_02009788();
void Func_02009790();
void Func_020097a2();
s32 Func_020097a6_a();
s32 Func_020097b2_a();
void Func_020097d6();
s32 Func_020097d8();
s32 Func_020097e4();
void Func_020097f2();
void Func_020097f6();
void Func_02009806();
void Func_02009808();
s32 Func_0200982a();
s32 Func_02009836();
void Func_0200983a();
s32 Func_02009848();
s32 Func_02009858();
void Func_02009866();
s32 Func_0200986c();
void Func_02009876();
void Func_0200987c();
s32 Func_0200988c();
s32 Func_0200988c_a();
s32 Func_02009898();
s32 Func_020098ac();
void Func_020098b2();
void Func_020098c0();
void Func_0200993e();
void Func_02009a00();
void Func_02009a1e();
void Func_02009a32();
void Func_020053c8();
void Func_0200542c();
void Func_02005464();
void Func_0200a806();
void Func_0200a8a4();
void Func_0200a8d4();
void Func_0200a8ee();
void Func_0200a922();
void Func_0200a95c();
void Func_0200a96e();
void Func_0200a986();
void Func_0200a992();
void Func_0200a996();
void Func_0200a99e();
void Func_0200a9aa();
void Func_0200a9b6();
void Func_0200a9be();
void Func_0200a9ce();
void Func_0200a9e8();
void Func_0200aa06();
s32 Func_0200b7ce();
s32 Func_0200b7dc();
void Func_02007dec(s32);
s32 Func_02007d9c(s32);
void Func_02007e3c(s32);
s32 Func_02007dec_a(s32);
void Func_0200478e();
void Func_020047be();
void Func_020047fa();
void Func_02004808();
void Func_02004834();
void Func_02004842();
void Func_0200484e();
void Func_02004874();
void Func_02004888();
void Func_020048da();
void Func_020048fe();
void Func_02004902();
void Func_02004926();
void Func_0200497c();
void Func_02004990();
void Func_0200499e();
void Func_020049b0();
void Func_020049d0();
void Func_020049e6();
void Func_020049fe();
void Func_02004a04();
void Func_02004a16();
void Func_02004a24();
void Func_02004a34();
void Func_02004a42();
void Func_02004a5a();
void Func_02004a68();
void Func_02009bdc();
void Func_02009bf4();
void Func_02009c26();
s32 Func_02009c34();
void Func_02009c38();
void Func_02009c86();
void Func_02009c9e();
void Func_02009ca6();
void Func_02009cbe();
void Func_02009cc6();
s32 Func_02009ccc();
void Func_02009cf0();
void Func_02009cf0_a();
void Func_02004834_a();
void Func_02009e1e_a();
void Func_02009cf4();
void Func_02009d86();
void Func_02009d92();
void Func_02009da2();
void Func_02009dc8();
void Func_02009dce();
void Func_02009dda();
void Func_02009ddc();
void Func_02009dee();
void Func_02009df8();
void Func_02009dfe();
void Func_02009e06();
void Func_02009e12();
void Func_02009e1e();
void Func_02009e22();
void Func_02009e2a();
void Func_02009e34();
void Func_02009e36();
void Func_02009e3a();
void Func_02009e58();
void Func_02009e5a();
void Func_02009e5c();
void Func_02009e60();
void Func_02009e78();
void Func_02009e88();
void Func_02009e8a();
void Func_02009ea6();
void Func_02009eb2();
void Func_02009ec0();
void Func_02009ecc();
void Func_02009eec();
void Func_02009f04();
void Func_02009f0a();
void Func_02009f0e();
void Func_02009f10();
void Func_02009f20();
void Func_02009f2a();
void Func_02009f40();
void Func_02009f46();
void Func_02009f4e();
void Func_02009f52();
void Func_02009f5c();
void Func_02009f82();
void Func_02009f88();
void Func_02009f94();
void Func_02009fa4();
void Func_02009fb6();
void Func_02009fc6();
void Func_02009fdc();
void Func_02009ff4();
void Func_0200a000();
void Func_0200a022();

/*
 * resource_3c9: a scripted-scene bracket around three setup steps.
 */

/* The three inner steps are old-style: their signatures are not established. */

/*
 * One symbol per call site, spelled as the raw pc-relative decode of the
 * reference halfwords rather than the semantic import address. Listed in
 * call order.
 */

/*
 * These three symbols name the call words each site holds, not the imports
 * they eventually reach. Respelling them as the semantic imports changes the
 * call encodings, so leave the addresses as they are.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call3_02003924(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call3_02004b28(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 */
static __inline__ void Call2_02003e9c(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3_02003e9c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void Func_02009614();  /* Func_0808a018 */

void Func_020081bc();  /* Func_020048d8, this overlay's own */

void Func_02008410();  /* Func_02004b28, this overlay's own */

void Func_02009604();  /* GameFlag_Set */

void Func_020097a6();  /* Func_0808a368 */

void Func_020097b2();  /* Func_0808a370 */

void Func_02009770();  /* Func_0808a248 */

void Func_02009654();  /* Func_0808a020 */

u8 *Func_020093ba(s32 index);   /* scene-record accessor (Scene_GetRecord) */

s32 Func_020092b8(s32 angle);   /* sine of a binary angle (Func_08000118) */

s32 Func_020092a0(s32 angle);   /* cosine of a binary angle (Func_08000120) */

void OvObj_SetRecordField1(Obj *o, u32 v)
{
    o->sub->f1 = v;
}

/* The +98 byte is read directly in both tests and incremented in place rather
 * than cached in a local; that shape is what the reference holds. */

/*
 * Gated by the byte flag at +99: recompute a fixed-point offset from the byte
 * counter at +98 into the u32 field at +12, call a leaf helper, then advance
 * that counter while it stays in (0, 31]. The 56-byte owner has no literal
 * pool. obj is read before it is overwritten, so it is a real argument. The
 * layout behind +12, +76, +98 and +99 is not established.
 */
void *OvObj_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret = Func_02005cc6(kind, x, y, z);

    if (ret != NULL) {
        u8 *obj = *(u8 **)(ret + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = obj[9];
        mask = -mask;
        mask &= flags;
        obj[9] = mask;
        ret[0x55] = 0;
        ret[0x59] = 8;
        Func_02005d2c(ret, 0);
        Func_02005e74(ret, 14);
        Func_02005d44(ret, 1);
        return ret;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void Effect_SpawnConfigured(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

s32 OvObj_StepScaleByCounter(Spr *s)
{
    s16 *p = (s16 *)((u8 *)s + 100);

    switch (*p) {
    case 6:
        s->unk18 += -0x2000;
        s->unk1c += 0x1000;
        break;
    case 4:
        s->unk18 += 0x1000;
        s->unk1c += -0x800;
        break;
    case 2:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        break;
    case 0:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        *p = Func_02005f74(Func_02005f8e(s), 80) + 80;
        break;
    }
    {
        s32 t = s->unk64;
        t = t - 1;
        s->unk64 = t;
    }
    return 1;
}

void OvObj_DecayRecordField1e(Spr_02000400 *s)
{
    Obj_02000400 *o = s->obj;
    u16 h = o->unk1e;

    if ((s32)((h + 0xffff) << 16) < 0) {
        {
            s32 t = h + (s32)0xfffff600;
            o->unk1e = t;
        }
    }
}

s32 OvObj_UpdateHeadingTimer(Spr_02000424 *s)
{
    u16 *q = (u16 *)((u8 *)s + 0x66);
    s32 c = *q;
    s16 v = *(s16 *)q;

    if (v == 0) {
        {
            s32 t = ((u32)(Func_0200604c() << 15)) >> 16;
            s->unk06 = s->unk06 + t;
        }
        {
            s32 n = ((u32)(Func_0200605a() * 80)) >> 16;
            *q = n;
            if (n == 0) {
                goto out;
            }
            c = n;
        }
    }
    *q = c - 1;
out:
    return 1;
}

/* Contiguous unnamed leaf-owner run for resource_3c9. */
s32 SceneData_GetTablee3d4(void)
{
    return (s32)Data_0200e3d4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTablee464(void)
{
    return (s32)Data_0200e464;
}

s32 SceneData_GetTablee478(void)
{
    return (s32)Data_0200e478;
}

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return ((IwramIntegerSquareRoot)0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 Actor_FindNearestSlotOfKindF2(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    Spr_020004bc **p;
    s32 best = 0;
    Spr_020004bc *ref;
    s32 limit;
    u32 i;

    limit = 640;
    ref = Func_0200621c(0);
    i = 8;
    p = (Spr_020004bc **)(work + 0x34);
    do {
        Spr_020004bc *spr = *p++;
        if (spr != 0) {
            if (*spr->obj->unk28 == 0xf2) {
                s32 dist = Func_02000976((u8 *)ref + 8, (u8 *)spr + 8);
                if (dist < limit) {
                    limit = dist;
                    best = i;
                }
            }
        }
        i++;
    } while (i <= 65);
    return best;
}

void InitializeActorZeroMotion(void)
{
    struct SceneActor *actor;
    s32 position[3];
    s32 angle;
    u8 flags;

    actor = Func_02006270(0);
    angle = (actor->angle + 0x1000) & 0xe000;
    flags = actor->flags55;
    position[0] = (actor->x & 0xfff00000) + 0x80000;
    position[1] = actor->y;
    position[2] = (actor->z & 0xfff00000) + 0x80000;
    Func_0200618e(0x200000, angle, position);
    if (Func_0200620e(actor, position) == 0) {
        Func_02006292(592);
        Func_02000c3a();
        Func_020061de(actor, 6);
        Func_02006184(6);
        Func_020061ec(actor, 7);
        actor->motion30 = 0x30000;
        actor->motion34 = 0x20000;
        Func_0200646e(152);
        actor->motion28 = 0x40000;
        actor->flags55 &= 0x7e;
        Func_02006264(actor, 0);
        Func_02006352(0, (s16)(position[0] >> 16),
                         (s16)(position[2] >> 16));
        Func_0200622a(actor, 6);
        Func_02006282(actor, 1);
        actor->flags55 = flags;
    }
}

void Effect_SpawnAndBobWithActorZero(void)
{
    Spr_020005ec *a = Func_0200633e(0);
    Spr_020005ec *b;
    Spr_020005ec *r;
    s32 k;

    Func_0200632c();
    r = Func_02000ab8();
    Data_0200e6e8 = r;
    if (r != 0) {
        Func_02006320(592);
        b = Func_0200635e(Data_0200e6e8);
        b->unk55 = 0;
        a->unk55 &= 0xfe;
        b->unk0c += (s32)0xfffd0000;
        a->unk0c += (s32)0xfffd0000;
        a->unk14 += (s32)0xfffd0000;
        Func_0200623e(2);
        b->unk0c += (s32)0xfffe0000;
        a->unk0c += (s32)0xfffe0000;
        a->unk14 += (s32)0xfffe0000;
        Func_02006258(10);
        k = 0x20000;
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Func_02006274(4);
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Func_0200628c(4);
        b->unk0c += 0x10000;
        a->unk0c += 0x10000;
        a->unk14 += 0x10000;
    }
    Func_020063e6();
}

void Actor_SetByte55ForActorZeroAnd12To17(void)
{
    s32 val;

    *(Func_02006412(0) + 0x55) = 3;
    val = 4;
    *(Func_0200641e(12) + 0x55) = val;
    *(Func_0200642a(13) + 0x55) = val;
    *(Func_02006434(14) + 0x55) = val;
    *(Func_0200643e(15) + 0x55) = val;
    *(Func_02006448(16) + 0x55) = val;
    *(Func_02006452(17) + 0x55) = val;
}

s32 SceneData_GetTableE6ec(void)
{
    return (s32)Data_0200e6ec;
}

s32 Scene_InitActorsAndDispatchBySubstate(void)
{
    extern u8 *Data_03001ebc;

    s32 v;
    u32 i;
    s32 z;
    s16 mode;
    Spr_0200071c *obj;

    Func_02006438(324);
    Func_02006326(1);
    Func_02006446(272);
    Func_020063ea(Func_02006484(8), 0);
    Func_020063f6(Func_02006490(9), 0);
    Func_02006402(Func_0200649c(10), 0);
    Func_0200640e(Func_020064a8(11), 0);
    v = (s32)0xffff0000;
    Func_020064b4(10)->unk18 = v;
    Func_020064be(11)->unk18 = v;
    i = 12;
    z = 0;
    do {
        obj = Func_020064ca(i);
        Func_02006438_a(Func_020064d2(i), z);
        Func_020065b0(i, 1);
        obj->unk55 = 4;
        obj->unk23 |= 2;
        obj->unk0c = 0x8000;
        i++;
    } while (i <= 17);

    mode = Data_02000240[225];

    switch (mode) {
    case 1:
        if (Func_020064f8(0x109) == 0) {
            Func_020010a8();
        }
        break;
    case 2:
        Func_02002b5a();
        break;
    case 3:
        Func_020040c0();
        break;
    case 0x5d:
        Func_020040e2();
        break;
    case 4:
        Func_02004130();
        break;
    case 9:
        Func_02006546();
        if (Func_02006524(0x345) != 0) {
            Func_02006534(0, 65);
        } else if (Func_02006532(0x346) != 0) {
            Func_02006534(1, 65);
        } else if (Func_02006540(0x347) != 0) {
            Func_02006534(2, 65);
        } else {
            Func_0200653e(3, 65);
        }
        Func_020066ac(9);
        break;
    }

    if (Func_02006562(0x109) != 0) {
        if (Func_02000d1a() != 0) {
            Spr_0200071c *p = Func_020065b2();
            if (p != 0) {
                p->unk55 = 0;
            }
        }
    }
    return z;
}

void State_ApplyArgMode0AndSet10(s32 a)
{
    Func_0200669e(a, 0);
    Func_020065cc(10);
}

void Scene_CallPairWith10(s32 a, s32 b)
{
    Func_020066c2(a, b, 10);
}

void FieldScene_RunScene3c9_02001280(s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (a1 != 0) {
        Func_02007070(a0, 0);
        record = Func_02006fde(a0);
        Func_02006f44(record, 1);
        Call3(Func_02006ffe, a0, 0xcccc, 0x6666);
    } else {
        Func_02007090(a0, 15);
        record = Func_02006ffe_a(a0);
        Func_02006f64(record, 0);
    }
}

void Effect_UpdateObjectByFrameParity(s32 a)
{
    if (*(s32 *)0x03001e40 & 2) {
        Func_02007dc4(a, 7);
    } else {
        Func_02007dce(a, 0);
    }
    if (Func_02007cf8(*(s32 *)0x03001e40, 15) == 0) {
        Func_0200430e(a);
    }
}

void State_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)0x03001e40;

    if (*p & 1) {
        Func_02007e10(a, Func_02007d28(*p >> 1, 6));
    }
    if (Func_02007d38(*p, 15) == 0) {
        Func_0200434e(a);
    }
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Func_02007dec((s32)effect);
    } else {
        s32 amplitude = Func_02007d9c(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] + offset * 5 + 0x80000;
    }
}

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Func_02007e3c((s32)effect);
    } else {
        s32 amplitude = Func_02007dec_a(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}

void State_ApplyPair140And0(void)
{
    Func_020081e8(140, 0);
}

void Scene_ForwardValue81fc(s32 a)
{
    Func_020081fc(a);
}

void Scene_RunStep6(void)
{
    extern u8 Data_03001ebc[];

    Func_020080a2(6);
    Func_02004476();
}

void Scene_RunSetupSequence35c4(void)
{
    extern u8 *Data_03001ebc;

    Func_02009496(187);
    Func_0200944e(0x7fff, 1);
    Func_02009464(1);
    Func_020091da(4);
    Func_02009462(0x40250d, 1);
    Func_02009478(1);
    Func_020091ee(1);
}

/*
 * Per-frame orbit step for one actor: read the binary angle at +100, place
 * the actor on a circle around scene record 23, mirror the placement into
 * +56/+64, and advance the angle by -0x800, a thirty-second of a turn. The
 * two arms use deliberately different radius terms -- the +98 counter enters
 * both -- and must not be unified. The actor layout is raw offsets: nothing
 * establishes which of +8 and +16 is which world axis, so they are unnamed.
 */
void Effect_UpdateCounterDrivenOrbit(u8 *actor)
{
    u8 *anchor = Func_020093ba(23);
    u16 *pangle = (u16 *)(actor + 100);
    s32 angle = *pangle;
    s32 cosine;
    s32 sine;
    s32 along;
    s32 across;

    cosine = Func_020092a0(angle);
    along = *(s32 *)(anchor + 8)
          + cosine *(*(s32 *)(actor + 48) + *(u8 *)(actor + 98) + 6);
    *(s32 *)(actor + 8) = along;

    sine = Func_020092b8(angle);
    across = *(s32 *)(anchor + 16)
           + sine *(*(u8 *)(actor + 98) + 4);
    *(s32 *)(actor + 16) = across;

    *(s32 *)(actor + 56) = *(s32 *)(actor + 8);
    *(s32 *)(actor + 64) = across;

    {
        s32 next = *pangle;
        next = next + (s32)0xfffff800;
        *pangle = (u16)next;
    }
}

void Scene_RunThreeStepsInBracket(void)
{
    extern u8 *Data_03001ebc;

    Func_020095f8();
    Func_02007490();
    Func_02007768();
    Func_02007b0c();
    Func_02009610();
}

/*
 * Brackets a scripted scene: opens it, runs two of this overlay's own
 * steps, sets the story flag, writes the workspace phase and timer, then
 * closes. The 72-byte owner includes its alignment halfword and one pool
 * word. No incoming argument is read before being overwritten, so this is
 * void.
 */
void Scene_RunBracketedSceneWithFlag282(void)
{
    extern u8 *Data_03001ebc;

    u8 *workspace;

    Func_02009614();
    Func_020081bc();
    Func_02008410();
    /* The flag id is built as 141 << 1 rather than folded. */
    Func_02009604(141 << 1);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 512;
    *(s32 *)(workspace + 456) = 24;

    Func_020097a6();
    Func_020097b2();
    Func_02009770(1);
    Func_02009654();
}

void FieldScene_RunScene3c9_02003924(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec4;
    s32 record;
    u8 *work;

    rec4 = Value1(Func_0200967e, 0);
    Func_0200966c();
    *(u8 *)(Func_02009788() + 85) = 0;
    Call6(Func_020095ea, 102, 4, 74, 4, 18, 23);
    Call6(Func_020095fe, 39, 72, 11, 72, 16, 20);
    Call6(Func_0200961c, 19, 6, 3, 7, 22, 6);
    Call6(Func_0200962e, 19, 6, 3, 7, 13, 6);
    Call6(Func_02009640, 19, 6, 3, 7, 22, 13);
    Call6(Func_02009650, 19, 6, 3, 7, 13, 13);
    Func_020095b6(1);
    Call4(Func_02009806, 0xc00000, -0x400000, 0xee0000, 0);
    Func_020095cc(1);
    Func_02009650_a();
    Func_020095d6(1);
    Func_02009734(20);
    Func_0200973a(19);
    Func_020097a2(0, 19);
    record = Func_02009740(0);
    Func_020096a6(record, 0);
    *(s32 *)(rec4 + 8) = 0x15a0000;
    *(s32 *)(rec4 + 16) = 0xcd0000;
    *(s32 *)(rec4 + 12) = 0x200000;
    {
        s32 shown = 0x6000;

        *(u16 *)(rec4 + 6) = shown;
    }
    Func_020090a2(rec4);
    Func_020097d6(1, 18);
    record = Func_02009774(1);
    Func_020096da(record, 0);
    record = Func_02009780(1);
    *(s32 *)(record + 8) = 0x1640000;
    *(s32 *)(record + 16) = 0xc00000;
    {
        s32 shown = 0xa000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    Func_020090d4();
    Func_02009808(2, 18);
    record = Func_020097a6_a(2);
    Func_0200970c(record, 0);
    record = Func_020097b2_a(2);
    *(s32 *)(record + 8) = 0x1680000;
    {
        s32 shown = 0x2000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    Func_02009106();
    Func_0200983a(3, 18);
    record = Func_020097d8(3);
    Func_0200973e(record, 0);
    record = Func_020097e4(3);
    *(s32 *)(record + 8) = 0x14e0000;
    {
        s32 shown = 0x8000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 12) = 0x200000;
    *(s32 *)(record + 16) = 0xde0000;
    Func_02009134();
    Call3_02003924(Func_02009866, 21, 0xc40000, 0xdc0000);
    Func_02009876(21, 5);
    Call3_02003924(Func_0200987c, 6, 0xbc0000, 0x13c0000);
    Call2((void (*)())Func_0200988c, 6, 5);
    record = Func_0200982a(6);
    Func_02009790(record, 0);
    record = Func_02009836(8);
    *(s32 *)(record + 8) += -0x100000;
    Func_0200917e();
    record = Value1(Func_02009848, 9);
    *(s32 *)(record + 8) += -0x100000;
    Func_0200918e();
    record = Func_02009858(10);
    *(s32 *)(record + 8) += 0x100000;
    Func_020091a2();
    record = Value1(Func_0200986c, 11);
    *(s32 *)(record + 8) += 0x100000;
    Func_020091b2();
    Call3_02003924(Func_020097f6, 0x10000, 0x10000, 0x10000);
    record = Func_0200988c_a(23);
    Func_020097f2(record, 0);
    *(u8 *)(Func_02009898(23) + 85) = 4;
    Func_0200993e(23, 4);
    record = Func_020098ac(23);
    *(s32 *)(record + 12) = 0x280000;
    Call2(Func_02009774_a, 0x200da29, 0xc80);
    work = *(u8 *volatile *)Data_03001ebc;
    *(s32 *)(((s32)work + 0x1c0)) = 0x200;
    *(s32 *)(((s32)work + 0x1c8)) = 24;
    Func_02009a1e();
    Func_02009a32();
    Func_020098c0(40);
    Func_02008784();
    Call1(Func_020098b2, 0x9a7);
    Func_02009a00(2);
}

void Scene_RunMultiActorPresentation(void)
{
    s32 count_flag;
    s32 request_a;
    s32 request_b;

    Func_02009c86(1, 1);
    Func_02009bdc(20);
    Func_02009cc6(1, 0, 20);
    Func_02009c9e(2, 1);
    Func_02009bf4(20);
    Value2(Func_02009ccc, 2, 0);
    Call3_02003e9c(Func_02009cf0, 2, 0xc000, 0);
    Func_0200478e(0, 0x4000);
    count_flag = 0;
    if (Value2(Func_02009c34, 0, 0) == 0) {
        Func_02009c26(20);
        Func_02009cbe(2, 4);
        count_flag = 1;
    } else {
        Func_02009c38(20);
        Func_02009cf0_a(2, 1);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_020047be(2);
    if (count_flag != 0) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call3_02003e9c(Func_02009ca6, 1, 0x19999, 0xcccc);
    request_a = 0x1001;
    Call3_02003e9c(Func_02009cf4, 1, 0x141, 174);
    Func_02004808(1, 0x2000);
    Func_020047fa(request_a);
    Call3_02003e9c(Func_02009d86, 0, 0xa000, 0);
    Call3_02003e9c(Func_02009d92, 2, 0xa000, 0);
    Func_02004834(3, 0xc000);
    Func_02004842(1, 0x4000);
    Func_02004834_a(request_a);
    Call3_02003e9c(Func_02009dce, 1, 0x105, 40);
    Func_02009dc8(1, 0x2000, 20);
    Func_0200484e(request_a);
    Call3_02003e9c(Func_02009dda, 1, 0xa000, 40);
    Call3_02003e9c(Func_02009ddc, 0x8001, 0, 40);
    Func_02009dee(1, 0x2000, 20);
    Func_02004874(request_a);
    Func_02009eb2(17);
    Func_02009da2(3, 4);
    Func_02004888(3);
    Func_02009e12(1, 0x4000, 80);
    Call3_02003e9c(Func_02009e1e, 3, 0x6000, 80);
    Call3_02003e9c(Func_02009e3a, 3, 0x100, 40);
    Call3_02003e9c(Func_02009e36, 0, 0x6000, 60);
    Func_02009e06(0, 2);
    Func_020048da(2, 0xc000);
    Call3_02003e9c(Func_02009e60, 2, 0x101, 60);
    Call3_02003e9c(Func_02009e5c, 2, 0x6000, 40);
    Func_02009e1e_a(2, 4, 60);
    Func_02004902(1, 0x2000);
    Call3_02003e9c(Func_02009e88, 1, 0x101, 40);
    Func_020048fe(request_a);
    Call3_02003e9c(Func_02009e8a, 1, 0x6000, 40);
    Call3_02003e9c(Func_02009ea6, 1, 0x100, 0);
    Func_02009e58(1, 4, 40);
    Func_02004926(1);
    Call3_02003e9c(Func_02009df8, 21, 0xcccc, 0x6666);
    Func_02009e2a(21, 200, 188);
    Func_02009e34(6, 200, 204);
    Call2_02003e9c(Func_02009eec, 0x33333, 0x6666);
    Call4(Func_02009f04, 0xfc0000, 0, 0xbe0000, 1);
    Func_02009f10();
    Func_02009dfe(40);
    Func_02009fa4(23);
    Call2_02003e9c(Func_02009f0e, 21, 0x102);
    Func_0200497c(21);
    Func_02009ecc(21, 1);
    Func_02009e22(20);
    Func_02004990(21);
    Func_02009ec0(21, 4);
    Func_0200499e(21);
    Call3_02003e9c(Func_02009f2a, 21, 0x3000, 20);
    Func_020049b0(21);
    Call2_02003e9c(Func_02009f52, 6, 0x102);
    request_b = 0x2003;
    Func_02009e5a(20);
    Func_02009f0a(3, 2);
    Func_020049d0(request_b);
    Func_02009f20(21, 2);
    Func_02009e78(20);
    Func_020049e6(0x2002);
    Func_02004a04(21, 0xe000);
    Func_02009f40(1, 1);
    Func_020049fe(1);
    Func_02009f4e(21, 1);
    Func_02009f88(21, 0, 20);
    Func_02004a16(request_b);
    Func_02009f46(21, 4);
    Func_02004a24(21);
    Call2_02003e9c(Func_02009fc6, 2, 0x102);
    Func_02004a34(0x2002);
    Func_02009f5c(21, 3);
    Func_02004a42(21);
    Call3_02003e9c(Func_02009fdc, 1, 0x103, 40);
    Func_02009f94(1, 2);
    Func_02004a5a(1);
    Func_02009f82(21, 4);
    Func_02004a68(21);
    Call3_02003e9c(Func_02009ff4, 21, 0x5000, 20);
    Func_02009fb6(3, 4, 20);
    Func_0200a000(request_b, 0, 20);
    Call3_02003e9c(Func_0200a022, 21, 0x103, 40);
}

void FieldScene_RunScene3c9_02004b28(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Call1(Func_0200a922, 0x2829);
    Func_020053c8(21);
    Func_0200aa06(62);
    Call3_02004b28(Func_0200a806, 0x10000, 0x10000, 0x10000);
    Call2(Func_0200a986, 0x4cccc, 0x9999);
    Call2(Func_0200a992, 0x40000, 0x8000);
    Call4(Func_0200a9aa, 0xc00000, -0x400000, 0xee0000, 1);
    Func_0200a9b6();
    Func_0200a8a4(40);
    Func_0200a95c(21, 1);
    Call3_02004b28(Func_0200a996, 0x2015, 0, 40);
    Func_0200a96e(6, 3);
    Func_0200542c(6);
    Call2(Func_0200a9ce, 21, 0x102);
    Func_0200a8d4(60);
    Call3_02004b28(Func_0200a9be, 0x2015, 0, 80);
    Call2(Func_0200a9e8, 6, 0x102);
    Func_0200a8ee(40);
    Func_0200a99e(6, 2);
    Func_02005464(6);
}

/*
 * Park a record: stamp the sentinel 0x80000000 into the three mirror fields at
 * +56, +60 and +64, zero the three at +36, +40 and +44, and clear the u16
 * angle at +100.  The 24-byte owner at 0x02005688 has no prologue, no stack
 * use and no literal pool.  0x80000000 is read as a sentinel because it is
 * stamped into three position-family fields at once and then tested for
 * equality; the roles of +36, +40 and +44 are not established.
 */
void Actor_ParkRecord(u8 *record)
{
    /* The sentinel is built as 128 shifted left by 24, not pooled. */
    *(s32 *)(record + 56) = (s32)0x80000000;
    *(s32 *)(record + 60) = (s32)0x80000000;
    *(s32 *)(record + 64) = (s32)0x80000000;

    *(s32 *)(record + 36) = 0;
    *(s32 *)(record + 40) = 0;
    *(s32 *)(record + 44) = 0;

    *(u16 *)(record + 100) = 0;
}

void Effect_AdvanceGatedRiseCounter(u8 *obj)
{
    if (*(u8 *)(obj + 99) != 0) {
        u8 counter = *(u8 *)(obj + 98);

        *(u32 *)(obj + 12) = *(u32 *)(obj + 76) + ((u32)(counter >> 2) << 16);

        Func_0200b096(obj);

        {
            if (*(u8 *)(obj + 98) != 0) {
                if (*(u8 *)(obj + 98) <= 31) {
                    ++*(u8 *)(obj + 98);
                }
            }
        }
    }
}

void FieldScene_RunScene3c9_02005b90(u8 *a0)
{
    extern u8 Data_03001ebc[];

    s32 p10;
    s32 p8;
    s32 p8b;
    s32 record;
    s32 value;

    p10 = *(s32 *)(a0 + 104);
    p8 = *(u16 *)(a0 + 100);
    record = Func_0200b7ce(p8);
    *(volatile s32 *)(a0 + 8) = (*(s32 *)(p10 + 8) + (record *(*(s32 *)(a0 + 48) + 28)));
    value = Func_0200b7dc(p8);
    *(volatile s32 *)(a0 + 16) = ((value << 4) + 0xa40000);
    *(volatile s32 *)(a0 + 56) = *(s32 *)(a0 + 8);
    *(volatile s32 *)(a0 + 64) = ((value << 4) + 0xa40000);
    p8b = *(volatile u16 *)(a0 + 100);
    p8b = p8b + -0x200;
    *(volatile u16 *)(a0 + 100) = (u16)p8b;
}
