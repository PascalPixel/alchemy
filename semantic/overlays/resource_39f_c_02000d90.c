typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02000d90, 136 bytes (0x02000d90-0x02000e17).  It is
 * the overlay's most-called internal routine: 49 `bl` sites reach it.
 *
 * Creates a subordinate record for `subject` (import Func_0808a080 returns it),
 * initialises three of its 32-bit fields, then drives `subject` to the
 * requested position both in whole units and in 16.16.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * The prologue saves r5, r6, lr and then r9, sl, r8; the epilogue restores
 * them and ends `pop {r0} / bx r0`, so the owner is void.
 */

/* Used for its return value: the record whose +40, +68 and +72 words are
 * written below. */
u8 *Func_0808a080();
void Func_0808a1e0();
void Func_0808a090();
void Func_080f9010();
void Func_080091e0();
void Func_0808a0c0();
void Func_0808a0f0();

void Func_02000d90(s32 subject, s32 x, s32 z, s32 detail)
{
    u8 *record;

    /* r0-r3 are untouched by the prologue, so all four incoming arguments
     * reach this import unchanged. */
    record = Func_0808a080(subject, x, z, detail);

    Func_0808a1e0(subject, 1);

    /* 192 << 10 and 192 << 9, built with movs/lsls. */
    Func_0808a090(subject, 0x30000, 0x18000);

    Func_080f9010(152);

    *(s32 *)(record + 40) = detail;
    *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0;

    Func_080091e0(record, 0);

    /* First with the whole-unit coordinates ... */
    Func_0808a0c0(subject, x, z);
    /* ... then with the same pair promoted to 16.16 by `lsls #16`.  The high
     * registers holding x and z are overwritten with the shifted values, so
     * the shift is not undone afterwards. */
    Func_0808a0f0(subject, x << 16, z << 16);

    Func_080091e0(record, 1);

    *(s32 *)(record + 72) = 0x10000;
}
