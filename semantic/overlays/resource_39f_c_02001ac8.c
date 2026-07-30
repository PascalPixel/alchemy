typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001ac8, 188 bytes: code 0x02001ac8-0x02001b79, an
 * alignment halfword at 0x02001b7a and the pool words 0xffe00000 at
 * 0x02001b7c and 0xffff0000 at 0x02001b80.
 *
 * The same shape as 0x02001078 for a different destination: it builds a target
 * two units behind the party record in X (bias -2.0, no quantisation this time)
 * and, when Func_02000cd0 runs the move, animates the record back in four
 * 16.16 steps with the mode byte at +85 forced to 0.
 *
 * Note that the probe is assembled from four separate Func_0808a080(0) calls
 * rather than one - the record pointer is re-fetched for each field.  That is
 * what the assembly does; only the field offsets 8, 12 and 16 differ between
 * them.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule;
 * veneer 0x02002cec reaches ROM dispatch[0] -> Func_080030f8.
 *
 * The epilogue is `add sp, #12 / pop {r3} / ... / pop {r0} / bx r0`, so the
 * owner is void.
 */

u8 *Func_0808a080();
s32 Func_02000cd0();
void Func_0808a018();
void Func_0808a100();
void Func_080000c0();      /* ROM dispatch[0] -> Func_080030f8 */
void Func_0808a020();

void Func_02001ac8(void)
{
    u8 *record;
    u8 *mode;
    u8 saved;
    s32 target[3];

    record = Func_0808a080(0);
    mode = record + 85;
    saved = *mode;

    target[0] = *(s32 *)(Func_0808a080(0) + 8) + (s32)0xffe00000;
    target[1] = *(s32 *)(Func_0808a080(0) + 12);
    target[2] = *(s32 *)(Func_0808a080(0) + 16);

    if (Func_02000cd0(target) != 0) {
        /* r0 still holds the nonzero result of the test above. */
        Func_0808a018();

        *mode = 0;
        Func_0808a100(11, 7);

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
