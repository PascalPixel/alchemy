#include "types.h"

#define FieldScene_DispatchBySlotZeroFacing Func_020018e0
#define SceneState_ApplyRectAndSetActor8Byte35 Func_02001a70
#define SceneState_ApplyFourRectsAt48_55 Func_02001da0
s32 *Func_02003e28();
s32 Func_02003e00();
void Func_02003de8();
void Func_02003e2e();
void Func_02003ef2();
void Func_02003ef8();
s32 Func_02003e3c();
void Func_02003e24();
void Func_02003e62();
void Func_02003f1e();
void Func_020034ca();
void Func_02003f38();
s32 Func_02003f5e(s32, s32, s32, s32, s32, s32);
u8 *Func_02003fcc(s32);
void Func_0200428e(s32, s32, s32, s32, s32, s32);
void Func_020042a0(s32, s32, s32, s32, s32, s32);
void Func_020042b2(s32, s32, s32, s32, s32, s32);
void Func_020042c4(s32, s32, s32, s32, s32, s32);

/*
 * Scene state interaction for resource_3b4.
 *
 * A Func_ name in the import veneer band 0x02002468-0x0200261f names the
 * main-image address held in the veneer's trailing word, not a runtime
 * address the call reaches directly.  Declarations are old-style because
 * those imports are reached with differing argument counts from different
 * call sites.
 */

/* Slot record lookup, then the scene flag test, clear and set imports. */

/*
 * Dispatch on slot 0's halfword at +6, the facing field, which takes the
 * values 0, 0x4000, 0x8000 and 0xc000.  The 160-byte owner includes its two
 * pool words, 0x206 and 0x207.  Two arms repaint one collision cell, but
 * only while scene flag 0x206 is set, and then move flag 0x207.  The 0x8000
 * arm does no flag work and branches on slot 0's height word at +12, read
 * from the record pointer already in hand rather than a fresh lookup.
 */
void FieldScene_DispatchBySlotZeroFacing(void)
{
    s32 *slot = Func_02003e28(0);
    u16 facing = *(u16 *)((u8 *)slot + 6);

    if (facing == 0xc000) {
        if (Func_02003e00(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Func_02003de8(46, 43, 1, 1, fifth, last); }
        }
        Func_02003e2e(0x207);
        Func_02003ef2();
    } else if (facing == 0x4000) {
        Func_02003ef8();
    } else if (facing == 0) {
        if (Func_02003e3c(0x206) != 0) {
            { s32 fifth = 45; s32 last = 43; Func_02003e24(58, 36, 1, 1, fifth, last); }
        }
        Func_02003e62(0x207);
        Func_02003f1e();
    } else if (facing == 0x8000) {
        if (slot[3] == 0) {          /* +12 */
            Func_020034ca();
        } else {
            Func_02003f38();
        }
    }
}

void SceneState_ApplyRectAndSetActor8Byte35(void)
{
    s32 w = 41;
    s32 h = 42;
    u8 *p;

    Func_02003f5e(44, 42, 1, 1, w, h);
    p = Func_02003fcc(8) + 35;
    *p = 2;
}

void SceneState_ApplyFourRectsAt48_55(void)
{
    s32 base = 55;

    Func_0200428e(48, 55, 1, 1, 49, base);
    Func_020042a0(48, 55, 1, 1, 50, base);
    Func_020042b2(48, 55, 1, 1, 51, base);
    Func_020042c4(48, 55, 1, 1, 52, base);
}
