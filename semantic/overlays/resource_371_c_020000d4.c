typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 371 owner at 0x020000d4 (72 bytes, 2 calls).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020000d4 and the interworking
 * return `pop {r5, r6} ; pop {r0} ; bx r0` at 0x0200010a.  r0 holds the
 * popped return address, so the owner returns nothing.  Bytes
 * 0x02000110-0x0200011b are the literal pool (0x03001ebc, 0x02000240,
 * 0x0000080a).
 *
 * Instruction-for-instruction the same guard as 0x0200008c and as the
 * byte-exact sibling `assets/code/resource_371_c_02000030.c`; only the two
 * imports and the two constants (0x80a, 24) differ.  Workspace pointer and
 * field offsets come from that proven source.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is a per-call-site label for a
 * load-time-relocated import, not a place to disassemble and not a global
 * identity (see the note in resource_371_c_0200008c.c).  Old-style
 * declarations, because the interfaces are unknown.
 */

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_03000380();
void Func_0808a258();

void Func_020000d4(void)
{
    u8 *workspace = Data_03001ebc;
    s16 *table = Data_02000240;
    s32 *progress = (s32 *)&table[284];
    s32 *level = (s32 *)(workspace + 428);

    if (*progress >= Func_03000380(*level * 9, 10)) {
        Func_0808a258(0x80a, 24);
        *(s32 *)(workspace + 424) = 0;
    }
}
