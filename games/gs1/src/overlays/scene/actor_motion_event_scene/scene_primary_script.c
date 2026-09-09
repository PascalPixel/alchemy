#include "types.h"

#define SceneDialogue_RunActorFifteenFacingPreservedDialogue Func_02000640
#define FieldScene_RunSupplementalSequenceOne Func_02000804
#define FieldScene_RunScene385_020009f8 Func_020009f8
#define FieldScene_SetActor21Values0And4 Func_02000d74
#define FieldScene_RunScene385_02000d84 Func_02000d84

/* Complete actor-15 facing-preserving dialogue scene through its two-word pool. */
struct Actor_02000640 {
    u8 reserved00[6];
    u16 facing;
    u8 reserved08[92];
    u16 state_flags;
};

extern u8 Data_00000002[];
extern u8 Data_00001cc0[];
extern u8 Data_03001ebc[];

struct Actor_02000640 *Func_0200174c(s32 actor);
void Func_02001750(void);
void Func_020017d6(s32 dialogue);
void Func_020017ae(s32 actor, s32 mode);
void Func_020017d8(s32 actor, s32 mode, s32 value);
void Func_0200180a(s32 actor, s32 mode, s32 frames);
void Func_020016fe(s32 frames);
void Func_02001792(void);
void Func_02000ce2();
void Func_02000dce();
s32 Func_020018de();
void Func_02001900();
s32 Func_0200190a();
void Func_02001910();
void Func_0200193c();
u8 *Func_02001952();
void Func_02001990();
void Func_020013bc();
s32 Func_02001ace();
void Func_02001ae4();
void Func_02001ae8();
void Func_02001b0c();
void Func_02001b1c();
void Func_02001b32();
void Func_02001b38();
void Func_02001b64();
void Func_02001b66();
void Func_02001b76();
void Func_02001b86();
void Func_02001b92();
void Func_02001bb2();
void Func_02001bb4();
void Func_02001f26(s32 actor, s32 mode, s32 value);
void Func_02001e62();
void Func_02001e74();
void Func_02001e76();
void Func_02001e98();
void Func_02001eae();
void Func_02001eb4();
void Func_02001ec4();
void Func_02001ed4();
void Func_02001eda();
void Func_02001ee0();
void Func_02001ee4();
void Func_02001ee6();
void Func_02001f0c();
void Func_02001f0e();
void Func_02001f1e();
void Func_02001f28();
void Func_02001f2e();
void Func_02001f4c();
void Func_02001f9a();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/*
 * Complete actor-21 mode-four reset wrapper.
 *
 * Call symbol resolved directly against the raw region in
 * games/gs1/asm/overlays/resource_385_overlay. s(`push {lr} / movs r0,#21 / movs r1,#0 /
 * movs r2,#4 / bl sub_02001f26 / pop {r0} / bx r0`): the per-site overlay
 * veneer is sub_02001f26, not a main-image address guessed from elsewhere.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
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
static __inline__ void Call1_02000d74(void (*f)(), s32 a0)
{
    f(a0);
}

void SceneDialogue_RunActorFifteenFacingPreservedDialogue(void)
{
    struct Actor_02000640 *actor;
    s16 facing0;

    actor = Func_0200174c(15);
    facing0 = (s16)actor->facing;
    actor->state_flags |= 2;
    Func_02001750();
    Func_020017d6(0x1cb4);
    Func_020017ae(15, 0);
    Func_020017d8(15, 0, 2);
    Func_0200180a(15, 0, 10);
    actor->facing = (u16)facing0;
    Func_020016fe(1);
    Func_02001792();
    actor->state_flags = 0;
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    u8 *record;

    {
        u16 *flags = (u16 *)(Func_0200190a(14) + 100);
        u16 bits = (u16)(s32)Data_00000002;

        *flags |= bits;
    }
    Func_02001900();
    if (Value1(Func_020018de, 0x307) != 0) {
        Func_02001990((s32)Data_00001cc0);
        Func_02000ce2(14);
    } else {
        Func_02000dce();
        Call1(Func_02001910, 0x307);
    }
    Func_0200193c();
    {
        u8 *record = Func_02001952(14);
        s32 shown = 1;

        *(volatile u16 *)((s32)record + 100) = shown;
    }
}

void FieldScene_RunScene385_020009f8(void)
{
    u32 i;
    s32 record;

    Func_02001ae4();
    Func_02001bb2(100);
    Func_02001ae8(40);
    if (Value1(Func_02001ace, 0x867) == 0) {
        Call2(Func_02001bb4, 21, 0x102);
        Func_02001b66(21, 4, 0);
        Func_02001b0c(12);
        Func_02001b76(21, 4, 0);
        Func_02001b1c(20);
        Call4(Func_020013bc, 21, 0x188, 104, 0x70000);
        Func_02001b32(20);
        Call3(Func_02001b86, 21, 0x198, 104);
        Call3(Func_02001b92, 21, 0x198, 120);
        Call1(Func_02001b38, 0x867);
    }
    Func_02001b64();
}

void FieldScene_SetActor21Values0And4(void)
{
    Func_02001f26(21, 0, 4);
}

void FieldScene_RunScene385_02000d84(void)
{
    u32 i;
    s32 record;

    Func_02001e62(231);
    Func_02001e76();
    Func_02001e74(10);
    Func_02001ee4(18, 2);
    Call3(Func_02001eae, 18, 0xcccc, 0x6666);
    Call3(Func_02001eda, 18, 216, 0x198);
    Func_02001e98(10);
    Call3(Func_02001f4c, 18, 0x4000, 20);
    Func_02001f0e(18, 6, 0);
    Func_02001eb4(30);
    Func_02001f1e(18, 6, 0);
    Func_02001ec4(30);
    Func_02001f2e(18, 6, 0);
    Func_02001ed4(30);
    Call3(Func_02001f28, 18, 216, 0x188);
    Func_02001ee6(10);
    Call3(Func_02001f9a, 18, 0x4000, 20);
    Call1_02000d74(Func_02001ee0, 0x858);
    Func_02001f0c();
}
