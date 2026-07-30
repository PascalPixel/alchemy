typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02000cd0, 154 bytes (0x02000cd0-0x02000d69).
 *
 * Runs one scripted move for `request` against the subject record supplied by
 * import Func_0808a080.  It gates on Func_080091d8 (a zero result means the
 * move is allowed), then reconfigures the record, temporarily clears two bits
 * of the mode byte at record+85 for the duration of the move, and restores the
 * byte before returning.  Returns 1 when the move ran and 0 when it was
 * refused.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * Veneer 0x02002cec reaches 0x080000c0, entry 0 of the ROM dispatch stub table
 * in asm/080000c0.s, which forwards to Func_080030f8.
 *
 * The epilogue is `pop {r3} / ... / pop {r5, r6, r7} / pop {r1} / bx r1`, so r0
 * survives and is the result.
 */

u8 *Func_0808a080();
s32 Func_080091d8();
void Func_0808a018();
void Func_08009080();
void Func_080000c0();      /* ROM dispatch[0] -> Func_080030f8 */
void Func_080f9010();
void Func_080091e0();
void Func_0808a0c0();
void Func_0808a020();

s32 Func_02000cd0(u8 *request)
{
    u8 *record;
    u8 *mode;
    u8 saved;

    record = Func_0808a080(0);
    mode = record + 85;
    saved = *mode;

    /* r0 still holds the record returned above; r1 is the incoming request. */
    if (Func_080091d8(record, request) != 0) return 0;

    /* No argument register is set for this one; r0-r3 hold the values the
     * previous call left. */
    Func_0808a018();

    Func_08009080(record, 6);
    Func_080000c0(6);
    Func_080f9010(152);
    Func_08009080(record, 7);

    *(s32 *)(record + 48) = 0x30000;   /* 192 << 10 */
    *(s32 *)(record + 52) = 0x20000;   /* 128 << 10 */
    *(s32 *)(record + 40) = 0x40000;   /* 128 << 11 */

    /* Mask 0x7e: bit 0 and bit 7 are cleared while the move runs. */
    *mode = (u8)(*mode & 0x7e);

    Func_080091e0(record, 0);

    /* Two signed halfwords of the request, read with `ldrsh` at +10 and +2 and
     * passed as the second and third arguments with r0 forced to 0. */
    Func_0808a0c0(0, *(s16 *)(request + 2), *(s16 *)(request + 10));

    Func_08009080(record, 6);
    Func_080091e0(record, 1);

    *mode = saved;

    /* No argument registers are set. */
    Func_0808a020();
    return 1;
}
