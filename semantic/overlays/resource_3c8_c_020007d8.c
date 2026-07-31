typedef signed int s32;
typedef signed short s16;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x020007d8: one-shot cutscene step that runs the 0x020006a0
 * particle emitter for the duration of an effect sequence.
 *
 * The owner is complete: `push {r5, r6, r7, lr}` at 0x020007d8 and the
 * matching `pop {r5, r6, r7} ; pop {r0} ; bx r0` at
 * 0x02000886..0x0200088a, followed by a five-word literal pool at
 * 0x0200088c-0x0200089f.  No live frame or register state escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * `object->callback = 0x020086a1` installs Func_020006a0: 0x020086a1 is that
 * entry point plus the Thumb bit at this overlay's proven 0x02008000 link
 * base.  The store at 0x0200087c clears the slot again with r6, which is 0 on
 * this path.  The byte-exact sibling 0x02000754 performs exactly this
 * install/clear pair on the same +0x6c slot.
 *
 * Field offsets: 0x0a and 0x12 are the signed halfword pair the byte-exact
 * sibling 0x020014f4 reads as `unkA`/`unk12`; 0x55 is the `mode55` byte
 * written by semantic/overlays/resource_3c8_c_02001d48.c; 0x6c is the
 * callback slot of 0x02000754's `struct EffectObject`.
 *
 * Halfword coordinate views: the signed halfwords at +0x0a and +0x12 are the
 * high halves - the integer parts - of the 16.16 words at +0x08 and +0x10.
 * The byte-exact sibling 0x020014f4 models the same record both ways, as
 * `Struct_A` (s32 at 8 and 0x10) and `Struct_B` (s16 at 0x0a and 0x12).
 *
 * 0xfff00000 is a pooled constant (-0x00100000), added to the 16.16-promoted
 * halfword at +0x12.
 *
 * Nineteen `bl` sites reach seventeen distinct targets - 0x02005712 and
 * 0x0200583c are each called twice, with different argument counts, which is
 * why every import is declared old-style.  The full list: 0x0200568a,
 * 0x0200565a, 0x02005684, 0x0200578a, 0x02005712, 0x0200575a, 0x020056d8,
 * 0x02005676, 0x020057fa, 0x0200580e, 0x0200583c, 0x02005702, 0x02005794,
 * 0x020056b0, 0x0200585a, 0x02005874, 0x02005720.
 *
 * Registers that are not explicitly written before a branch are not asserted
 * as arguments, following the convention of
 * semantic/overlays/resource_3b8_c_02000108.c.  Two places where that matters
 * are noted inline.
 */

struct EffectObject_020007d8 {
    u8 unknown_00[10];
    s16 unkA;                   /* 0x0a */
    u8 unknown_0c[6];
    s16 unk12;                  /* 0x12 */
    u8 unknown_14[0x41];
    u8 mode55;                  /* 0x55 */
    u8 unknown_56[0x16];
    s32 callback;               /* 0x6c */
};

/* Used for their return values. */
s32 Func_080091e0();
s32 Func_080770c0();
void Func_0808a018();
void Func_0808a020();
struct EffectObject_020007d8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0f0();
void Func_0808a158();
void Func_0808a210();
void Func_0808a360();
void Func_0808a370();
void Func_0808a580();
void Func_0808a5e8();
void Func_080f9010();

/* Old-style declarations: the imports' real interfaces are not known here. */

void Func_020007d8(void)
{
    struct EffectObject_020007d8 *object;
    s32 busy;

    object = Func_0808a080(0);
    busy = Func_080770c0(0x109);
    if (busy != 0) {
        return;
    }

    /* No argument register is written here; r0 still holds the 0 just
     * returned by 0x0200565a, and that dataflow is preserved as written.
     * Whether the callee reads it is unverified. */
    Func_0808a018(0);

    Func_0808a210(-1, -1, -1, 0);
    object->mode55 = 0;

    Func_0808a0f0(0, object->unkA << 16, (object->unk12 << 16) + 0xfff00000);
    Func_0808a158(0, 15);
    Func_0808a080(Func_080091e0(0), 0);
    Func_0808a360();
    Func_0808a370();
    Func_080f9010(228);

    object->callback = 0x020086a1;      /* = Func_020006a0 | 1 */
    /* r3 still holds that same word at the branch below; it is treated as the
     * leftover of the store, not as a fourth argument. */
    Func_0808a090(0, 0x6666, 0x3333);

    Func_0808a580(0, 0, 8);
    Func_0808a158(0, 0);
    Func_0808a080(Func_080091e0(0), 1);
    Func_0808a580(0, 0, 8);

    object->mode55 = 3;
    object->callback = 0;

    Func_0808a5e8();
    Func_0808a020();
}
