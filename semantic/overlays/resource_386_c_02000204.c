typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 386 cutscene beat at 0x02000204 (branching dialogue with a
 * "seen it already" fast path).
 *
 * Complete owner: `push {lr}` at 0x02000204, interworking return
 * `pop {r0} / bx r0` at 0x020002e4.  A control-flow walk reaches every
 * halfword to 0x020002e6; 0x020002e8-0x020002fb is never reached as an
 * instruction and is the literal pool (0x0000085b, 0x0000137c, 0x00001385,
 * 0x00001384, 0x03001ebc).  The next owner begins at 0x020002fc.
 *
 * Signature: `pop {r0} / bx r0`, so the popped value is the return address
 * and the owner is `void`.  No argument register is read before being written.
 *
 * Link base 0x02008000 (see resource_386_c_0200011c.c for the witnesses);
 * 0x02008205 in the overlay's handler pool is `Func_02000204 + 1`, which is
 * how this row was identified as a scene handler before it was disassembled.
 *
 * Import naming uses `true_target_offset = stored_displacement + 2` and the
 * veneer band at 0x020005dc-0x020006dc, never the disassembler's annotation.
 *
 * Call sites: 27, against the manifest's `calls=25`.  The manifest field
 * counts distinct callees and is a floor, not an equality test; the
 * multiset of `bl` targets over 0x0206-0x02e0 matches the calls below
 * one for one, per target.
 */

void Func_0808a010();  /* veneer 0x634 */
void Func_0808a018();  /* veneer 0x63c — scene begin */
void Func_0808a020();  /* veneer 0x644 — scene end */
void Func_0808a060();  /* veneer 0x654 */
s32  Func_0808a070();  /* veneer 0x65c */
void Func_0808a110();  /* veneer 0x67c */
void Func_0808a138();  /* veneer 0x684 */
void Func_0808a150();  /* veneer 0x68c */
void Func_0808a170();  /* veneer 0x694 — message */
void Func_0808a178();  /* veneer 0x69c */
void Func_0808a180();  /* veneer 0x6a4 — act */
void Func_0808a1b8();  /* veneer 0x6b4 */
void Func_0808a398();  /* veneer 0x6c4 */
s32  Func_080770c0();  /* veneer 0x614 — event-flag test */
void Func_080770c8();  /* veneer 0x61c — event-flag set */
s32  Func_08077250();  /* veneer 0x62c */

#define WORKSPACE (*(u8 **)0x03001ebc)

void Func_02000204(void)
{
    Func_0808a018();
    Func_0808a150(18, 0, 0);

    /* Flag 0x85b selects which of two lines is spoken.  The same flag is
     * tested by the sibling at 0x020004a0 and set on one arm below. */
    if (Func_080770c0(0x85b) == 0) {
        Func_0808a170(0x137c);
        Func_0808a178(18, 0);
    } else {
        Func_0808a170(0x1385);
        Func_0808a178(18, 0);
    }

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a180(18, 0);
        Func_0808a010(20);
        Func_0808a138(18, 2);
        Func_0808a010(20);
        if (Func_08077250() == 0) {
            Func_0808a110(18, 4);
            Func_0808a010(20);
            Func_0808a170(0x1384);
            Func_0808a180(18, 0);
        } else {
            Func_0808a398(0xe7, 3);
            Func_0808a060(0xe7, 0);
            Func_080770c8(0x85b);
        }
    } else {
        /* Skip-beat counter at workspace + 472 (236 << 1). */
        ++*(u16 *)(WORKSPACE + 472);
        Func_0808a010(20);
        Func_0808a110(18, 3);
        Func_0808a010(20);
        Func_0808a180(18, 0);
    }

    /* r1 is built as 128 << 7. */
    Func_0808a1b8(18, 0x4000, 0);
    Func_0808a020();
}
