#include "types.h"

#define InspectOrdinaryObject Func_02001dc4
#define InspectEmptyChest Func_02001e2c
#define InspectEmptySceneObject Func_020020cc
#define InspectVillageWell Func_02002308
#define InspectWardrobe Func_020024c0
#define InspectFirewood Func_020024e4
#define InspectBooks Func_02002504
extern u8 Data_02000240[];

void Func_02007474(s32, s32);
void Func_0200757a(s32);
void Func_020074f6(s32, s32, s32);
void Func_020039c8(s32);
void Func_02007564(s32, s32);
void Func_020074ae(s32, s32, s32);
void Func_0200744a(void);
void Func_02007430(s32);
void Func_020074dc(s32, s32);
void Func_020075e2(s32);
void Func_0200755e(s32, s32, s32);
void Func_02003a30(s32);
void Func_020075cc(s32, s32);
void Func_02007516(s32, s32, s32);
void Func_020074b2(void);
void Func_02007498(s32);
void Func_0200777c(s32, s32);
void Func_02007882(s32);
void Func_020077fe(s32, s32, s32);
void Func_02003cd0(s32);
void Func_0200786c(s32, s32);
void Func_020077b6(s32, s32, s32);
void Func_02007752(void);
void Func_02007738(s32);
void Func_020045d8();
void Func_0200791c();
void Func_0200792c();
s32 Func_02007930();
void Func_02007932();
void Func_02007960();
void Func_0200797a();
void Func_0200798a();
void Func_02007990();
void Func_02007ada();
void Func_02007ade(s32);
void Func_02007aec(s32);
void Func_02007b76(s32, s32, s32);
void Func_02007b02(s32);
void Func_020047c2(void);
void Func_02007b98(s32, s32, s32);
void Func_02007b22(s32);
void Func_02007b32(s32);
void Func_02007bbc(s32, s32, s32);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void InspectOrdinaryObject(void)
{
    extern u8 *Data_03001ebc;

    Func_02007474(0, 1);
    Func_0200757a(113);
    Func_020074f6(15, 256, 60);
    Func_020039c8(15);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_02007564(98, 2);
    Func_020074ae(15, 0, 0);
    Func_0200744a();
    Func_02007430(2380);
}

void InspectEmptyChest(void)
{
    extern u8 *Data_03001ebc;

    Func_020074dc(0, 1);
    Func_020075e2(113);
    Func_0200755e(11, 256, 60);
    Func_02003a30(11);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_020075cc(98, 2);
    Func_02007516(11, 0, 0);
    Func_020074b2();
    Func_02007498(2377);
}

void InspectEmptySceneObject(void)
{
    extern u8 *Data_03001ebc;

    Func_0200777c(0, 1);
    Func_02007882(113);
    Func_020077fe(16, 256, 60);
    Func_02003cd0(16);
    *(s32 *)(Data_03001ebc + 448) = 512;
    Data_02000240[0x22b] = 3;
    Func_0200786c(98, 2);
    Func_020077b6(16, 0, 0);
    Func_02007752();
    Func_02007738(2379);
}

void InspectVillageWell(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (*(s16 *)((*(u8 *volatile *)Data_03001ebc + 0xcb8)) != 0) {
        if (Value1(Func_02007930, 0x947) == 0) {
            Call2(Func_0200792c, 0x1528, 1);
            Func_02007ada(188);
            v5 = 17;
            v6 = 82;
            Func_02007960(1);
            Call6(Func_0200791c, 6, 77, 1, 2, v5, v6);
            Func_0200797a(5);
            Call6(Func_02007932, 7, 77, 1, 2, v5, v6);
            Func_02007990(1);
            Func_020045d8();
            Call1(Func_0200798a, 0x947);
        }
    }
}

void InspectWardrobe(void)
{
    Func_02007ade(2372);
    Func_02007aec(535);
    Func_02007b76(8, 0, 0);
}

void InspectFirewood(void)
{
    Func_02007b02(2373);
    Func_020047c2();
    Func_02007b98(9, 0, 0);
}

void InspectBooks(void)
{
    Func_02007b22(2374);
    Func_02007b32(536);
    Func_02007bbc(10, 0, 0);
}
