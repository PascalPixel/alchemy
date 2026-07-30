typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02000d6c, 20 bytes (0x02000d6c-0x02000d7f).
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * The call resolves through veneer slot 0x02002e2c to main-image
 * Func_0808a160.
 *
 * The epilogue is `pop {r1} / bx r1`, so r0 is the result; it is zeroed just
 * before, so the owner always returns 0.
 */

void Func_0808a160();

s32 Func_02000d6c(void *subject)
{
    /* `adds r3, r0, #100` then `ldrsh r1, [r3, r2]` with r2 = 0: a signed
     * halfword read at byte offset 100 of the incoming object. */
    s16 field = *(s16 *)((u8 *)subject + 100);

    Func_0808a160(subject, field);
    return 0;
}
