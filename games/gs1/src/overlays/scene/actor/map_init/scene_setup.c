#include "types.h"

#define FieldScene_RunFourCallSequenceB Func_020016f4
#define FieldScene_RunFourStepSequenceA Func_020018f8
#define FieldScene_RunTwoStepSequence Func_020019bc
#define FieldScene_RunFourStepSequenceB Func_020019cc
#define SceneActor_InstallSlotNineHandler Func_020019e4
#define SceneActor_SetupSlotNineAndInstallHandler Func_02001a10
#define FieldScene_RunThreeStepSequence Func_02001f5c
#define FieldScene_RunFourCallSequence Func_02001fc4
#define FieldScene_PlaceAndPinSlots8And9 Func_020023a0
typedef struct Slot_02001a10 {
    s32 pad0;
    s32 pad4;
    s32 col;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_02001a10;

typedef struct Slot_020023a0 {
    s32 pad0;
    s32 pad4;
    s32 column;            /* +8,  12.20 fixed point */
    s32 pad12;
    s32 row;               /* +16, 12.20 fixed point */
} Slot_020023a0;

void Func_020047ec(void);
void Func_020017c0(void);
void Func_020047fc(void);
void Func_02002c54(void);
void Func_020049f0(void);
void Func_020019c4(void);
void Func_02004a00(void);
void Func_0200304c(void);
void Func_02004ab4(void);
void Func_02004ac0(void);
void Func_02004ac4(void);
void Func_02001a98(void);
void Func_02004ad4(void);
void Func_02003398(void);
s32 Func_02004b08(s32, s32);
void Func_02004ace(s32);
u8 *Func_02004b04(s32);
void Func_02004b0a();
void Func_02004b8a_a();
void Func_02004b8a_b();
void Func_02004b6a();
void Func_02004b7a();
u8 *Func_02004b48();
void Func_02004b2a();
Slot_02001a10 *Func_02004b60();
Slot_02001a10 *Func_02004b68();
void Func_02004b0e();
u8 *Func_02004b84_a();
u8 *Func_02004b84_b();
u8 *Func_02004b8e();
s32 Func_02008ec8(struct Record_02000ec8 *record);
typedef s32(*Handler_02001a10)(struct Record_02000ec8 *record);
void Func_02005054(void);
void Func_02002028(void);
void Func_02005064(void);
void Func_020050bc(void);
void Func_02002090(void);
void Func_02003f40(void);
void Func_020050d0(void);
void Func_02005452();
void Func_020032ce();
Slot_020023a0 *Func_020054d0();
Slot_020023a0 *Func_020054d8();
void Func_0200547e();
Slot_020023a0 *Func_020054f4();
Slot_020023a0 *Func_020054fc();
void Func_020054a2();

/*
 * Slot nine setup for overlay resource_3c4.
 */

/*
 * Every call site branches through its own veneer, so the callees are named
 * per site. Func_02004b84 serves two sites with different argument shapes
 * and is declared without a prototype.
 */

/*
 * Setup step in resource_3c4. One six-argument placement, then two identical
 * follow-ups that pin an overlay at the 12.20 grid cell of slots 8 and 9: the
 * slot's +8 and +16 words are each shifted right by 20 and passed as the two
 * stack arguments of the placement call.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */

struct Record_02000ec8;

void FieldScene_RunFourCallSequenceB(void)
{
    Func_020047ec();
    Func_020017c0();
    Func_020047fc();
    Func_02002c54();
}

void FieldScene_RunFourStepSequenceA(void)
{
    Func_020049f0();
    Func_020019c4();
    Func_02004a00();
    Func_0200304c();
}

void FieldScene_RunTwoStepSequence(void)
{
    Func_02004ab4();
    Func_02004ac0();
}

void FieldScene_RunFourStepSequenceB(void)
{
    Func_02004ac4();
    Func_02001a98();
    Func_02004ad4();
    Func_02003398();
}

void SceneActor_InstallSlotNineHandler(void)
{
    u8 *owner;

    Func_02004b08(8, 0x0200B3B8);
    Func_02004ace(0x203);
    owner = Func_02004b04(9);
    *(s32 *)(owner + 108) = 0x02008FE9;
}

/*
 * Brings slot 9 up: four state writes, clear bit 1 of the byte at +35,
 * publish selector 0x204, pin an overlay at slot 9's 12.20 grid cell, then
 * install one handler on slots 9 and 8. The 136-byte owner at 0x02001a10
 * includes its alignment halfword and its one pool word; that word is an
 * odd Thumb pointer, so the handler is Func_02008ec8. The bit-clear folds
 * +35 into the returned pointer through the address local, not into a copy.
 */
void SceneActor_SetupSlotNineAndInstallHandler(void)
{
    u8 *desc;
    s32 col;
    s32 row;

    Func_02004b0a();
    Func_02004b8a_a(9, 1);
    Func_02004b6a(9, 1);
    Func_02004b8a_b(9, 0);
    Func_02004b7a(9, 2);

    {
        u8 *flag = Func_02004b48(9) + 35;
        *flag &= (u8)0xfd;
    }

    Func_02004b2a(0x204);

    col = Func_02004b60(9)->col;
    row = Func_02004b68(9)->row >> 20;
    Func_02004b0e(26, 8, 1, 1, col >> 20, row);

    desc = Func_02004b84_a(9);
    *(Handler_02001a10 *)(desc + 108) = Func_02008ec8;

    desc = Func_02004b8e(8);
    *(Handler_02001a10 *)(desc + 108) = Func_02008ec8;

    Func_02004b84_b(desc);
}

void FieldScene_RunThreeStepSequence(void)
{
    Func_02005054();
    Func_02002028();
    Func_02005064();
}

void FieldScene_RunFourCallSequence(void)
{
    Func_020050bc();
    Func_02002090();
    Func_02003f40();
    Func_020050d0();
}

void FieldScene_PlaceAndPinSlots8And9(void)
{
    s32 row;

    {
        s32 p5 = 9, p6 = 38;
        Func_02005452(73, 38, 5, 5, p5, p6);
    }
    Func_020032ce(9, 8);

    {
        s32 col = Func_020054d0(8)->column >> 20;
        row = Func_020054d8(8)->row >> 20;
        Func_0200547e(2, 36, 1, 1, col, row);
    }

    {
        s32 col = Func_020054f4(9)->column >> 20;
        row = Func_020054fc(9)->row >> 20;
        Func_020054a2(2, 36, 1, 1, col, row);
    }
}
