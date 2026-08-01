typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

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
 * Twenty-four `bl` sites reach twenty-three distinct targets - 0x0200672a is
 * called twice, with different argument counts, which is why every import is
 * declared old-style.  The full list: 0x020066bc, 0x020066aa, 0x020067ac,
 * 0x020066da, 0x02006678, 0x020066fc, 0x0200672a, 0x020066e8, 0x02006760,
 * 0x020066f6, 0x020067e8, 0x02006790, 0x0200670e, 0x0200678c, 0x0200679c,
 * 0x02001910, 0x020066fe, 0x0200221c, 0x0200680c, 0x02006882, 0x020068ae,
 * 0x020068ba, 0x02006786.  Import naming follows the note in
 * resource_3c8_c_020002f0.c; 0x02001910 and 0x0200221c fall numerically
 * inside this overlay's own code region but are import identities, not
 * locations.
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

/* Used for their return values. */
struct Object_02001808 *Func_020066bc();
void Func_020066aa();
void Func_020067ac();
s32 Func_020066da();
void Func_02006678();
void Func_020066fc();
void Func_0200672a();
void Func_020066e8();
void Func_02006760();
void Func_020066f6();
void Func_020067e8();
void Func_02006790();
void Func_0200670e();
void Func_0200678c();
void Func_0200679c();
void Func_02001910();
void Func_020066fe();
void Func_0200221c();
void Func_0200680c();
void Func_02006882();
void Func_020068ae();
void Func_020068ba();
void Func_02006786();

/* Old-style declarations: the imports' real interfaces are not known here. */

void Func_02001808(void)
{
    struct Object_02001808 *object;

    object = Func_020066bc(0);

    /* No argument register is written here; r0 still holds the pointer just
     * returned above, and that dataflow is preserved as written. */
    Func_020066aa(object);

    Func_020067ac(-1, -1, -1, 0);
    Func_02006678(Func_020066da(0), 0);

    object->unk6 = 0x4000;
    Func_020066fc(0, 0x00030000, 0x00018000);
    /* +0x0a is the integer part of the 16.16 x above. */
    Func_0200672a(0, *(s16 *)((u8 *)object + 10), 552);
    Func_020066e8(10);
    Func_02006760(0, 22);
    Func_020066f6(30);
    Func_020067e8(0, 258);
    Func_02006790(0, 2);
    Func_0200670e(20);

    object->unk6 = 0xc000;
    Func_0200678c(0, 5);
    Func_0200679c(0, 24);
    Func_0200672a(40);

    object->state48 = 0x9999;
    object->state44 = 0;
    Func_02001910(object->x, 0, object->z + 0x00480000, 223);

    Func_020066fe(34, 35, 5, 1, 34, 34);
    Func_0200221c(0);
    Func_0200680c(0, 15);
    Func_02006882(20);
    Func_020068ae();
    Func_020068ba();
    Func_02006786();
}
