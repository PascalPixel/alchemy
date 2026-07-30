typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x020022a4: place an actor one unit above the object its
 * kind word names.
 *
 * The owner is complete: `push {r5, lr}` at 0x020022a4 and the matching
 * `pop {r5} ; pop {r1} ; bx r1` at 0x020022c0..0x020022c4.  No literal pool
 * sits inside the span and no live frame or register state escapes the row.
 *
 * `pop {r1} ; bx r1` (N != 0) leaves r0 alive, and r0 is loaded with 0
 * immediately before the epilogue, so the owner returns 0.  Callbacks of this
 * shape in the byte-exact siblings (0x02000594, 0x02000690, 0x020005ac) all
 * return `s32` 0, so the type is taken as s32 rather than void.
 *
 * The +100 kind word is read with `ldrsh`, i.e. signed, through a register
 * offset of zero.  The byte-exact siblings read the same halfword unsigned
 * when they only want its low nibble; here the whole signed value is the
 * argument.
 *
 * 0x00100000 is built as `movs r2, #128 ; lsls r2, r2, #13`, i.e. one unit in
 * the 16.16 world coordinates used by this package (the same scale as the
 * `>> 20` grid conversions in the byte-exact siblings).
 *
 * Import naming: `Func_0200715e` is the address this call site computes; see
 * the note in resource_3c8_c_020002f0.c.
 */

struct Actor_020022a4 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[84];
    s16 kind;      /* offset 100 */
};

/* Old-style declaration: this import is used for its return value only. */
struct Actor_020022a4 *Func_0200715e();

s32 Func_020022a4(struct Actor_020022a4 *actor)
{
    struct Actor_020022a4 *anchor;

    anchor = Func_0200715e(actor->kind);
    actor->y = anchor->y + 0x00100000;
    return 0;
}
