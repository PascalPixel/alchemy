/*
 * resource_3c4 @ 0x02001458 (84 bytes: 80 code and one pool word).
 *
 * This is the selector-reversed sibling immediately before 0x020014ac.  It
 * initializes query 0x200, tests flag 0x201, then mirrors the queried state
 * into slot 14's byte at +98 and bit 3 of the byte at +89.  The zero halfword
 * at 0x02001456 is alignment after the preceding owner, not part of this one.
 */
typedef signed int s32;
typedef unsigned char u8;

s32 Func_080770c0();
void Func_080770c8();
u8 *Func_0808a080();

void Func_02001458(void)
{
    u8 *state;
    u8 *flags;

    Func_080770c8(0x200);
    if (Func_080770c0(0x201) != 0) {
        state = Func_0808a080(14);
        state[98] = 0;
        flags = Func_0808a080(14);
        flags[89] = (u8)(flags[89] & 0xf7);
    } else {
        state = Func_0808a080(14);
        state[98] = 1;
        flags = Func_0808a080(14);
        flags[89] = (u8)(flags[89] | 8);
    }
}
