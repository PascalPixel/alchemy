typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001078, 180 bytes: code 0x02001078-0x02001123 and
 * the pool words 0xfff00000 at 0x02001124 and 0xffff0000 at 0x02001128.
 *
 * Offers a scripted move one tile ahead of the party record: it quantises the
 * record's X and Z to whole units, biases them by +0.5 and +2.5 tiles, and asks
 * the overlay's Func_02000cd0 to run the move.  If the move happened it walks
 * the record backwards in four steps - three of -1.0 and one of +1.0 in 16.16 -
 * while the mode byte at +85 is forced to 0, restoring it afterwards.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * Veneer 0x02002cec reaches 0x080000c0, the first entry of the ROM dispatch
 * stub table (asm/080000c0.s), which forwards to Func_080030f8; it is called
 * three times with a different argument each time and is the routine that
 * actually advances the frame between steps.
 *
 * Func_0808a018 and Func_0808a020 bracket the animated section here, the same
 * pairing the other scripted owners in this overlay use.
 *
 * Uncertainty worth recording: at 0x020010b8 no argument register is written
 * before the branch, so Func_0808a018 sees the nonzero result Func_02000cd0
 * left in r0.  Nothing asserts that it reads it.
 *
 * The epilogue is `add sp, #12 / pop {r3} / ... / pop {r0} / bx r0`, so the
 * owner is void.
 */

/* Returns the party record; its +8/+12/+16 words are the position and +85 the
 * mode byte. */
u8 *Func_0808a080();
/* The overlay's scripted-move driver; nonzero means the move ran. */
s32 Func_02000cd0();
void Func_0808a018();
void Func_0808a100();
void Func_080000c0();      /* ROM dispatch[0] -> Func_080030f8 */
void Func_0808a020();

void Func_02001078(void)
{
    u8 *record;
    u8 *mode;
    u8 saved;
    s32 target[3];

    record = Func_0808a080(0);
    mode = record + 85;
    saved = *mode;

    /* Mask 0xfff00000 keeps the sign and integer part of a 16.16 coordinate
     * and discards the low 20 fraction bits; the Y word at +12 is copied
     * unquantised. */
    target[0] = (*(s32 *)(record + 8) & (s32)0xfff00000) + 0x80000;
    target[1] = *(s32 *)(record + 12);
    target[2] = (*(s32 *)(record + 16) & (s32)0xfff00000) + 0x280000;

    if (Func_02000cd0(target) != 0) {
        /* r0 still holds the nonzero result of the test above. */
        Func_0808a018();

        *mode = 0;
        Func_0808a100(9, 7);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Func_080000c0(2);

        *(s32 *)(record + 12) += (s32)0xffff0000;
        *(s32 *)(record + 20) += (s32)0xffff0000;
        Func_080000c0(10);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;
        Func_080000c0(4);

        *(s32 *)(record + 12) += 0x10000;
        *(s32 *)(record + 20) += 0x10000;

        *mode = saved;
        Func_0808a020();
    }
}
