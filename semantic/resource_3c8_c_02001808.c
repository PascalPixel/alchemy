#include "types.h"

/*
 * Resource 3c8 at 0x02001808: 23-call cutscene step.
 *
 * The owner is complete: `push {r5, r6, lr} ; sub sp, #8` at 0x02001808 and
 * the matching `add sp, #8 ; pop {r5, r6} ; pop {r0} ; bx r0` at
 * 0x020018ec..0x020018f2, followed by a one-word literal pool at
 * 0x020018f4-0x020018f7.  No live frame or register state escapes the row;
 * the next prologue is at 0x020018f8.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * The eight reserved stack bytes carry arguments five and six of this
 * package's six-argument placement ABI (r0-r3 then [sp,#0] then [sp,#4]), the
 * order fixed by the byte-exact sibling 0x020010cc.
 *
 * Constants are materialised as `movs #imm ; lsls #shift`: 128<<7 = 0x4000,
 * 192<<10 = 0x30000, 192<<9 = 0x18000, 138<<2 = 552, 129<<1 = 258,
 * 192<<8 = 0xc000, 144<<15 = 0x480000.  0x9999 is pooled.
 *
 * Field offsets: 0x08 and 0x10 are the 16.16 x/z pair of the byte-exact
 * siblings' record.  The signed halfword read at +0x0a is the *high half* of
 * that same x word, i.e. its integer part; the byte-exact sibling 0x020014f4
 * models exactly this with two struct views over one record (`Struct_A`'s
 * s32 pair at 8/0x10 and `Struct_B`'s s16 pair at 0x0a/0x12).  It is spelled
 * here as an explicit halfword read rather than as an overlapping field.
 * 0x44 and 0x48 are the `state44`/`state48` pair written together in
 * semantic/overlays/resource_3c8_c_02001d48.c (`actor->state48 = 0x1999 ;
 * actor->state44 = 0`), here with 0x9999.
 *
 * The calls below use their resolved resident-service or local-helper
 * identities.  In particular, the two sites once conflated under one
 * overlay-relative alias are different services: actor movement setup and a
 * scripted delay.
 */

struct Object_02001808 {
    u8 unknown_00[6];
    u16 unk6;                   /* 0x06 */
    s32 x;                      /* 0x08 - 16.16 */
    u8 unknown_0c[4];
    s32 z;                      /* 0x10 - 16.16 */
    u8 unknown_14[0x30];
    s32 state44;                /* 0x44 */
    s32 state48;                /* 0x48 */
};

struct Object_02001808 *Func_0808a080();
s32 Func_02000058();

void Func_0808a018();
void Func_0808a210();
void Func_080091e0();
void Func_0808a090();
void Func_0808a0c0();
void Func_0808a010();
void Func_0808a100();
void Func_0808a1f0();
void Func_0808a138();
void Func_0808a108();
void Func_080091c0();
void Func_0200094c();
void Func_0808a158();
void Func_0808a248();
void Func_0808a368();
void Func_0808a370();
void Func_0808a020();

/* Old-style declarations: the imports' real interfaces are not known here. */

void Func_02001808(void)
{
    struct Object_02001808 *object;

    object = Func_0808a080(0);

    /* No argument register is written here; r0 still holds the pointer just
     * returned above, and that dataflow is preserved as written. */
    Func_0808a018(object);

    Func_0808a210(-1, -1, -1, 0);
    Func_080091e0(Func_0808a080(0), 0);

    object->unk6 = 0x4000;
    Func_0808a090(0, 0x00030000, 0x00018000);
    /* +0x0a is the integer part of the 16.16 x above. */
    Func_0808a0c0(0, *(s16 *)((u8 *)object + 10), 552);
    Func_0808a010(10);
    Func_0808a100(0, 22);
    Func_0808a010(30);
    Func_0808a1f0(0, 258);
    Func_0808a138(0, 2);
    Func_0808a010(20);

    object->unk6 = 0xc000;
    Func_0808a100(0, 5);
    Func_0808a108(0, 24);
    Func_0808a010(40);

    object->state48 = 0x9999;
    object->state44 = 0;
    Func_02000058(object->x, 0, object->z + 0x00480000, 223);

    Func_080091c0(34, 35, 5, 1, 34, 34);
    Func_0200094c(0);
    Func_0808a158(0, 15);
    Func_0808a248(20);
    Func_0808a368();
    Func_0808a370();
    Func_0808a020();
}
