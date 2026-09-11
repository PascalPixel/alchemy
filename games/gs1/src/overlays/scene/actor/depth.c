#include "types.h"

#define SetEffectRecordMode Func_02000030
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OvObj_PrepareObject      Func_02000048
#define AcquireOverlayObject      Func_0200110e
#define RunOverlayObjectCommand0  Func_02001144
#define RunOverlayObjectCommand1  Func_0200115c
#define RunOverlayObjectCommand14 Func_02001244
#define CreateOverlayObject Func_02001166
#define SetOverlayObjectMode Func_020011a0
#define SetOverlayObjectSlot Func_020012a0
#define OvObj_CreateConfiguredObject Func_020000a0
#define OvObj_TurnStateByEighth Func_02000da8
#define OvObj_WaitForHeight Func_02000db8
#define EffectDescriptorTable Data_020092dc
#define GetPartyEffect Func_020012a2
#define SpawnEffect Func_02001238
#define SetEffectVariant Func_02001242
#define SetEffectDescriptor Func_0200125c
#define SetEffectMode Func_020013d6
#define ScaleEffectDeltaFromAccumulated Func_02001300
#define ScaleEffectDeltaFromOrigin Func_02001318
#define ScaleEffectVerticalDelta Func_02001326
#define SetEffectCallbackMode Func_0200135c
#define SetEffectCallbackArgument Func_0200136c
#define SpawnConfiguredEffect Func_0200013c
#define Overlay387_ConfigureActorEightAtDepth Func_02000d04
#define Scene_RequestAndWaitFrames Func_02000314
#define SceneData_GetTable92f8 Func_0200032c
#define SceneData_ReturnZero Func_02000334
#define SceneData_GetTable9358 Func_02000338
#define SceneData_GetTable9368 Func_02000340
#define Scene_RunOpeningAuxiliarySequence Func_02000348
#define Scene_RunScene387SequenceC Func_020003d0
#define Scene_RunScene387SequenceD Func_0200043c
#define Scene_RunScene387SequenceA Func_0200067c
#define Resource387_NoOpCallbackA Func_020004cc
#define Resource387_NoOpCallbackB Func_020004d0
#define Scene_RunStepWithValue866 Func_020004d4
#define SceneData_GetTable9488 Func_020004ec
#define Actor_ApplyValueAndMatchingSlots Func_02000f8c

#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"

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

struct OverlayActorPosition {
    u8 pad00[8];
    s32 depth_fixed;
};

struct OverlayActorState {
    u8 pad00[35];
    u8 flags;
};

extern u8 Data_03001ebc[];

void *Func_0200110e(s32, s32, s32, s32);
void Func_02001144(void *, s32);
void Func_0200115c(void *, s32);
void Func_02001244(void *, s32);
void Func_02001e54(s32 cnt);
void Func_02001e36();
struct OverlayActorPosition *Func_02001e54_a();
void Func_02001af6();
struct OverlayActorState *Func_02001e68();
void Func_02001dfe();
void Func_02001e0e();
void Func_02001e66();
void Func_02001e92();
void Func_020014e8();
void Func_020007f8();
void Func_0200143c();
void Func_02001454();
void Func_02001470();
void Func_02001476();
void Func_0200147c();
s32 Func_02001494();
void Func_020014aa();
void Func_020014c6();
void Func_020014d6();
void Func_020014e0();
void Func_02001512();
s32 Func_0200151c();
void Func_02001536();
s32 Func_02001538();
s32 Func_02001544();
s32 Func_02001550();
void Func_02001570();
void Func_02001576();
void Func_020015a8();
void Func_020015b2();
void Func_020015c8();
void Func_020015d0();
void Func_020015ec();
void Func_020015f4();
void Func_020015fe();
void Func_0200160c();
void Func_02001612();
void Func_02001630();
void Func_02001698();
void Func_020016d8();
void Func_02000cb0();
void Func_02000cfc();
void Func_02000d6c();
void Func_02000d92();
void Func_02000dda();
void Func_02000e0a();
void Func_02000e4a();
void Func_02000eaa();
void Func_02000ec0();
void Func_02000eec();
void Func_02000f3a();
void Func_02000f60();
void Func_020016c2();
void Func_020017b0();
void Func_020017b2();
void Func_020017e8();
void Func_020017f2();
s32 Func_020017fc();
void Func_02001808();
s32 Func_02001814();
void Func_02001816();
void Func_0200182e();
void Func_02001836();
void Func_02001842();
void Func_02001856();
s32 Func_02001866();
void Func_02001878();
void Func_0200187c();
void Func_02001888();
void Func_0200188e();
void Func_02001890();
void Func_020018a0();
void Func_020018a6();
void Func_020018ae();
void Func_020018ba();
void Func_020018be();
void Func_020018c0();
void Func_020018c8();
void Func_020018d0();
void Func_020018e2();
void Func_020018e4();
void Func_020018ee();
void Func_020018f0();
void Func_02001942();
void Func_0200195c();
void Func_0200195e_a();
void Func_0200195e_b();
void Func_02001970();
void Func_02001972();
void Func_02001978();
void Func_02001982_a();
void Func_02001982_b();
void Func_02001986();
void Func_02001996();
void Func_02001998();
void Func_020019a2_a();
void Func_020019a2_b();
void Func_020019bc();
void Func_020019c8();
void Func_020019dc();
void Func_020019e4();
void Func_020019f4();
void Func_020019fa_a();
void Func_020019fa_b();
void Func_02001a04();
void Func_02001a06();
void Func_02001a14();
void Func_02001a28();
void Func_02001a2c();
void Func_02001a3c();
void Func_02001a40();
void Func_02001a60();
void Func_02001a66_a();
void Func_02001a66_b();
void Func_02001a68();
void Func_02001a6e();
void Func_02001a82();
void Func_02001a86();
void Func_02001a8c_a();
void Func_02001a8c_b();
void Func_02001aa0();
void Func_02001aaa();
void Func_02001aac();
void Func_02001ab2();
void Func_02001ab8();
void Func_02001aba();
void Func_02001ac2();
void Func_02001ac6();
void Func_02001ace();
void Func_02001ad0();
void Func_02001ad2();
void Func_02001ae2();
void Func_02001ae6();
void Func_02001af6_a();
void Func_02001afa();
void Func_02001b04();
void Func_02001b0c();
void Func_02001b22();
void Func_02001b32();
void Func_02001b3a();
void Func_02001b56();
void Func_02001b5c();
void Func_02001b64();
void Func_02001b66();
void Func_02001b68();
void Func_02001b6e();
void Func_02001b74();
void Func_02001b78();
void Func_02001b8c();
void Func_02001b90();
void Func_02001b9a();
void Func_02001bc8();
void Func_02001bd4();
void Func_02001bd6();
void Func_02001be0();
void Func_02001be2();
void Func_02001bea();
void Func_02001bf4();
void Func_02001bfc();
void Func_02001bfe();
void Func_02001c04();
void Func_02001c08();
void Func_02001c12();
void Func_02001c1a();
void Func_02001c26();
void Func_02001c34();
void Func_02001c3e();
void Func_02001c44();
void Func_02001c46();
void Func_02001c52();
void Func_02001c5c();
void Func_02001c74();
void Func_02001c76();
void Func_02001c82();
void Func_02001c8e();
void Func_02001c9a();
void Func_02001ca2();
void Func_02001ca4();
void Func_02001cb2();
void Func_02001cb4_a();
void Func_02001cb4_b();
void Func_02001cbe();
void Func_02001cc8();
void Func_02001ce6();
void Func_02001cf4();
void Func_02001cfc();
void Func_02001d06();
void Func_02001d08();
void Func_02001d12();
void Func_02001d14();
void Func_02001d1c();
void Func_02001d20();
void Func_02001d26();
void Func_02001d28();
void Func_02001d2a();
void Func_02001d32_a();
void Func_02001d32_b();
void Func_02001d42();
void Func_02001d50();
void Func_02001d56();
void Func_02001d60();
void Func_02001d64();
void Func_02001d68();
void Func_02001d74();
void Func_02001d7c();
void Func_02001d94();
void Func_02001da0();
void Func_02001dac();
void Func_02001db2();
void Func_02001de4();
void Func_02001dee();
void Func_02001df6();
void Func_02001dfc();
void Func_02001e02();
void Func_02001e04();
void Func_02001e08();
void Func_02001e0c();
void Func_02001e0e_a();
void Func_02001e14();
void Func_02001e16();
void Func_02001e1a_a();
void Func_02001e1a_b();
void Func_02001e22();
void Func_02001e3a();
void Func_02001e3c();
void Func_02001e4c();
void Func_02001604(void);
void Func_020015ea(s32 flag);
void Func_02001616(void);
u8 *Func_0200207c(s32 group);
s32 Func_02002096(s32 group, s32 value);
void Func_020020b2(s32 group, s32 index);

/* Wait at most sixty frames for the object to reach the requested height. */

/*
 * resource_387: issue a scene request and then wait.
 */

/* Old-style: the two imports are called with different arities. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* resource_387 three-call story-flag wrapper at 0x020004d4. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void Func_02001446();          /* Wait this many frames. */

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OvObj_PrepareObject(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

/*
 * The owner exists for the argument shuffle: the frame count is saved before
 * the first call clobbers its register, so it survives to reach the second.
 * The twenty-two byte owner loads no literal and has no pool.
 */
void Scene_RequestAndWaitFrames(s32 selector, s32 frames)
{
    Func_020014e8(selector, 0);
    Func_02001446(frames);
}

/* Contiguous unnamed leaf-owner run for resource_387. */

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable92f8(void)
{
    return (void *)0x020092f8;
}

/* resource_387 zero-return leaf at 0x02000334. */
int SceneData_ReturnZero(void)
{
    return 0;
}

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable9358(void)
{
    return (void *)0x02009358;
}

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable9368(void)
{
    return (void *)0x02009368;
}

void Scene_RunOpeningAuxiliarySequence(void)
{
    s32 record;
    s32 v3;

    record = Value1(Func_02001494, 9);
    v3 = *(volatile s32 *)(record + 8) / 0x100000;
    Call1(Func_02001476, 0x861);
    Call1(Func_0200147c, 0x862);
    if (v3 == 15) {
        Call6(Func_0200143c, 47, 18, 1, 2, 16, 18);
    } else if (v3 == 16) {
        Call6(Func_02001454, 48, 18, 1, 2, v3, 18);
        Call1(Func_020014aa, 0x861);
    } else {
        Call6(Func_02001470, 47, 18, 1, 2, 16, 18);
        Call1(Func_020014c6, 0x862);
    }
}

void Scene_RunScene387SequenceC(void)
{
    s32 record;
    s32 v3;
    s32 v5;

    record = Value1(Func_0200151c, 10);
    v3 = *(volatile s32 *)(record + 8) / 0x100000;
    if (v3 == 23) {
        Func_02001512(10);
        *(u8 *)(Func_02001538(10) + 35) = 2;
        v5 = 0;
        *(u8 *)(Func_02001544(10) + 85) = v5;
        Func_020014d6(Func_02001550(10), 0);
        Call6(Func_020014e0, 54, 17, 1, 1, v3, 17);
        Call1(Func_02001536, 0x863);
    }
}

void Scene_RunScene387SequenceD(void)
{
    u8 *p5;
    s32 v5;

    p5 = *(u8 **)Data_03001ebc;
    Func_02001570();
    Func_020015d0(0, 8);
    Func_02001576(20);
    Call3(Func_020015a8, 0, 0x3333, 0x1999);
    Call3(Func_020015b2, 9, 0x3333, 0x1999);
    Func_02001698(185);
    v5 = (11 - (*(s16 *)(p5 + 0x16c) << 1)) << 4;
    Func_020015f4(0, v5, 0);
    Func_020015fe(9, v5, 0);
    Func_0200160c(0);
    Func_02001612(9);
    Func_020015c8(20);
    Func_02001630(0, 1);
    Func_020007f8();
    Func_020016d8();
    Func_020015ec();
}

/* resource_387 deliberate no-op leaf at 0x020004cc. */
void Resource387_NoOpCallbackA(void)
{
}

/* resource_387 deliberate no-op leaf at 0x020004d0. */
void Resource387_NoOpCallbackB(void)
{
}

void Scene_RunStepWithValue866(void)
{
    Func_02001604();
    Func_020015ea(0x866);
    Func_02001616();
}

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable9488(void)
{
    return (void *)0x02009488;
}

void Scene_RunScene387SequenceA(void)
{
    u32 i;
    s32 record;

    Func_020018a6();
    Func_020017b2();
    Func_020017b0(30);
    Call1(Func_02001856, 0x138f);
    Call3(Func_020017e8, 0, 0xcccc, 0x6666);
    Call3(Func_020017f2, 1, 0xcccc, 0x6666);
    Call3(Func_0200188e, 0, 0xe000, 20);
    record = Value1(Func_020017fc, 0);
    if (record != 0) {
        Func_02001842(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Call3(Func_02001836, 1, 0x108, 168);
    Call3(Func_020018ba, 1, 0x6000, 20);
    Func_02001808(20);
    Func_02001878(1, 4);
    Func_02001816(20);
    Func_020018d0(1, 0, 20);
    Func_02001890(0, 3);
    Func_0200182e(20);
    if (Value1(Func_02001814, 0x855) == 0) {
        Func_020018a0(1, 2);
        record = Value1(Func_02001866, 0);
        if (record != 0) {
            Func_02001888(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_020018ae(1);
        Func_020018c0(1, 0, 0);
        Func_0200187c();
    } else {
        Call3(Func_020018e4, 2, 0x1680000, 0xf80000);
        Call3(Func_020018be, 2, 0xcccc, 0x6666);
        Call3(Func_020018e2, 2, 0x110, 248);
        Call3(Func_020018ee, 2, 0x110, 208);
        Call3(Func_02001972, 2, 0xa000, 20);
        Func_02001942(2, 2);
        Func_020018c8(20);
        Func_02001982_a(2, 0, 20);
        Call3(Func_02001996, 0, 0x2000, 0);
        Call3(Func_020019a2_a, 1, 0x4000, 0);
        Func_020018f0(20);
        Call3(Func_020019bc, 0, 0x100, 0);
        Call3(Func_020019c8, 1, 0x100, 60);
        Func_02001978(2, 3);
        Call3(Func_0200195c, 2, 0x108, 200);
        Func_0200195e_a(0, 248, 168);
        Func_02001970(2, 248, 184);
        Func_02001986(0);
        Call3(Func_020019fa_a, 0, 0x6000, 0);
        Call3(Func_02001a06, 1, 0x6000, 0);
        Func_02001998(2, 232, 184);
        Func_0200195e_b(20);
        Call3(Func_02001a28, 2, 0x105, 60);
        Call3(Func_02001a2c, 2, 0xe000, 20);
        Func_020019e4(2, 4);
        Func_02001982_b(20);
        Func_02001a3c(2, 0, 20);
        Func_020019f4(0, 3);
        Func_02001a04(1, 3);
        Func_020019a2_b(20);
        Call3(Func_02001a66_a, 2, 0x8000, 20);
        Func_02001a68(2, 0, 120);
        Call3(Func_02001a82, 0, 0x105, 0);
        Call3(Func_02001a8c_a, 1, 0x105, 60);
        Func_02001a66_b(0, 1, 0);
        Func_020019dc(60);
        Call3(Func_02001aa0, 0, 0x6000, 0);
        Call3(Func_02001aac, 1, 0x6000, 0);
        Func_020019fa_b(60);
        Call3(Func_02001ac6, 2, 0x106, 0);
        Func_02001a86(2, 1);
        Func_02001a14(30);
        Func_02001ace(2, 0, 30);
        Call3(Func_02001ae2, 2, 0xe000, 20);
        Func_02001aaa(0, 2);
        Func_02001aba(1, 2);
        Func_02001a40(20);
        Func_02001afa(2, 0, 20);
        Func_02001ab2(0, 3);
        Func_02001ac2(1, 3);
        Func_02001a60(20);
        Func_02001ad0(2, 3);
        Func_02001a6e(20);
        Call3(Func_02001b32, 2, 0x8000, 20);
        Func_020016c2();
        Func_02001ae6(2, 1);
        Func_02001a8c_b(20);
        Func_02001b90();
        Call3(Func_02001b5c, 0, 0x100, 0);
        Call3(Func_02001b68, 1, 0x100, 60);
        Func_02001b22(1, 2, 0);
        Func_02001ab8(20);
        Func_02000cb0(1, 20);
        Call3(Func_02001b0c, 2, 0x108, 184);
        Func_02001ad2(10);
        Func_02001b64(2, 1, 0);
        Func_02001b6e(1, 2, 0);
        Func_02001b78(0, 2, 0);
        Func_02001af6_a(20);
        Func_02001b66(2, 3);
        Func_02001b04(20);
        Func_02000cfc(2, 60);
        Call3(Func_02001bd6, 0, 0x105, 0);
        Call3(Func_02001be0, 1, 0x105, 60);
        Call3(Func_02001bea, 0, 0x101, 0);
        Call3(Func_02001bf4, 1, 0x101, 0);
        Func_02001b3a(60);
        Call3(Func_02001bfe, 1, 0x8000, 0);
        Func_02001c08(0, 0, 0);
        Func_02001b56(60);
        Call3(Func_02001c1a, 1, 0x4000, 0);
        Call3(Func_02001c26, 0, 0x2000, 0);
        Func_02001b74(10);
        Func_02000d6c(1, 20);
        Call3(Func_02001c46, 2, 0x105, 0);
        Func_02001b8c(60);
        Func_02001bfc(2, 4);
        Func_02001b9a(20);
        Func_02000d92(2, 20);
        Func_02001c34(0, 2, 0);
        Func_02001c3e(1, 2, 0);
        Call3(Func_02001c82, 0, 0x102, 0);
        Call3(Func_02001c8e, 1, 0x102, 0);
        Func_02001bd4(60);
        Func_02001c5c(2, 2);
        Func_02001be2(20);
        Func_02000dda(2, 30);
        Call3(Func_02001cb4_a, 0, 0x101, 0);
        Call3(Func_02001cbe, 1, 0x101, 0);
        Func_02001c04(80);
        Func_02001c74(2, 3);
        Func_02001c12(20);
        Func_02000e0a(2, 20);
        Func_02001c9a(0, 1);
        Func_02001ca2(1, 1);
        Call2(Func_02001cfc, 0, 0x102);
        Call2(Func_02001d06, 1, 0x102);
        Func_02001c44(60);
        Func_02001cb4_b(2, 4);
        Func_02001c52(20);
        Func_02000e4a(2, 20);
        Func_02001d1c(0, 0, 0);
        Call3(Func_02001d28, 1, 0x8000, 0);
        Func_02001c76(80);
        Func_02001d08(0, 2, 0);
        Func_02001d26(1, 2, 0);
        Func_02001ca4(30);
        Func_02001d14(2, 3);
        Func_02001cb2(20);
        Func_02000eaa(2, 30);
        Func_02001d2a(2, 4);
        Func_02001cc8(20);
        Func_02000ec0(2, 20);
        Func_02001d50(0, 2);
        Func_02001d60(1, 2);
        Func_02001ce6(20);
        Func_02001d56(2, 3);
        Func_02001cf4(20);
        Func_02000eec(2, 40);
        Func_02001d64(0, 3);
        Func_02001d74(1, 3);
        Func_02001d12(20);
        Func_02001d32_a(2, 1);
        Func_02001d20(60);
        Func_02001bc8();
        Func_02001dac(2, 1);
        Func_02001d32_b(20);
        Func_02001d7c(2, 248, 184);
        Func_02001d42(20);
        Func_02000f3a(2, 20);
        Call3(Func_02001e0e_a, 0, 0x8000, 0);
        Call3(Func_02001e1a_a, 1, 0x8000, 0);
        Func_02001d68(120);
        Func_02000f60(2, 30);
        Func_02001e02(0, 2, 0);
        Func_02001e0c(1, 2, 0);
        Func_02001e16(2, 0, 0);
        Func_02001d94(20);
        Func_02001dfc(0, 3);
        Func_02001e04(1, 3);
        Func_02001e14(2, 3);
        Func_02001db2(50);
        Call3(Func_02001de4, 1, 0xcccc, 0x6666);
        Call3(Func_02001dee, 2, 0xcccc, 0x6666);
        Func_02001e08(1, 248, 168);
        Func_02001e1a_b(2, 248, 168);
        Func_02001e3c(2, 0, 0);
        Func_02001e3a(1);
        Func_02001e4c(1, 0, 0);
        Call6(Func_02001da0, 74, 11, 1, 1, 73, 11);
        Call1(Func_02001df6, 0x865);
        Func_02001e22();
    }
}

void Overlay387_ConfigureActorEightAtDepth(void)
{
    s32 depth;
    s32 span;
    struct OverlayActorState *state;

    Func_02001e36();
    depth = Func_02001e54_a(8)->depth_fixed >> 20;
    if (depth == 11) {
        Func_02001af6(8);
        state = Func_02001e68(8);
        state->flags |= 2;
        span = 12;
        Func_02001dfe(39, 12, 3, 1, 8, span);
        Func_02001e0e(43, 11, 3, 1, span, depth);
        Func_02001e66(2144);
    }
    Func_02001e92();
}

/* Turn the object's attached presentation state by one eighth-turn. */
void OvObj_TurnStateByEighth(u8 *obj)
{
    u8 *state = *(u8 **)(obj + 80);
    s32 v = *(u16 *)(state + 30) - 0x800;

    *(u16 *)(state + 30) = v;
}

void OvObj_WaitForHeight(u8 *obj, s32 height)
{
    s32 cnt = 60;
    while (cnt != 0) {
        Func_02001e54(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= height)
            break;
    }
}

/* Apply a value to every matching member of a fifteen-slot group. */
void Actor_ApplyValueAndMatchingSlots(s32 group, s32 value)
{
    u8 *work = Func_0200207c(group);
    s32 i;
    Func_02002096(group, value);
    for (i = 0; i < 15; i++) {
        if (*(u16 *)(work + 216 + i * 2) == value)
            Func_020020b2(group, i);
    }
}
