typedef unsigned char u8;

/*
 * Resource 3b3 unindexed helper at 0x020025f8 (68 bytes, 4 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,lr}` at 0x020025f8, epilogue `pop {r5} / pop {r0} / bx r0` at
 * 0x02002634-0x02002638; the halfword at 0x0200263a is the alignment
 * pad, immediately followed by the next owner's push {r5,r6,lr} at
 * 0x0200263c, already this overlay's row `0x0200263c | 3 calls`
 * (drafted in this reconstruction), so the span is 0x020025f8-0x0200263c,
 * 68 bytes.
 *
 * Fetches an object, clears bit 0 of `+0x59`, sets bit 1 of `+0x23`,
 * clears `+0x55`, runs a finishing call, then sets bit 3 (clearing
 * bits 2-3 first, same mask idiom as 0x02001938) of the linked
 * record's `+9` byte.
 *
 * Raw callee naming.
 */

u8 *Func_0808a080(void);
void Func_080091e0();

void Func_020025f8(void)
{
    u8 *obj = Func_0808a080();
    u8 *linked;

    obj[0x59] &= 0xfe;
    obj[0x23] |= 2;
    obj[0x55] = 0;

    Func_080091e0(obj);

    linked = *(u8 **)(obj + 0x50);
    linked[9] = (linked[9] & 0xf3) | 8;
}
