typedef signed int s32;

/*
 * resource_39f owner at 0x02000d80, 14 bytes (0x02000d80-0x02000d8d).
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * The call resolves through veneer slot 0x02002d5c to main-image
 * Func_080091e0, which is this overlay's most-used general import.
 *
 * The epilogue is `pop {r1} / bx r1`, so r0 survives and is the result; r0 is
 * set to 0 immediately before it, so the owner always returns 0.
 */

void Func_080091e0();

s32 Func_02000d80(s32 subject)
{
    /* r0 is never written before the branch, so the incoming first argument is
     * forwarded unchanged; r1 is forced to 0. */
    Func_080091e0(subject, 0);
    return 0;
}
