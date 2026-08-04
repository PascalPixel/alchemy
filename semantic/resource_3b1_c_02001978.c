typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_3b1 owner at 0x02001978, 144 bytes.  It is the first ordinary
 * owner after the preceding 0x02001894 row: `push {lr}` starts the body,
 * `pop {r0} / bx r0` returns at 0x020019ea-0x020019ec, the halfword at
 * 0x020019ee is alignment, and six literal words at 0x020019f0-0x02001a07
 * carry the workspace base, offsets 0x928-0x92a, and the 0x6f/0x3e values.
 * The next owner's `push {lr}` is 0x02001a08, so the complete measured span
 * is 0x02001978-0x02001a07.
 *
 * The three nested tests preserve the machine's fall-through shape: clear
 * flag 0x8f0 gates the first arm, then flags 0x928, 0x929 and 0x92a each
 * select one setup pair.  The final call is shared by every path.
 */

extern u8 Data_02000240[];

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a460(void);
extern s32 Func_080770c0(s32 flag);
extern s32 Func_080770d0(s32 flag);
extern void Func_0808a250(s32 group, s32 mode);
extern void Func_0808a260(s32 group, s32 mode);

void Func_02001978(void)
{
    Func_0808a018();
    Func_0808a460();
    *(u8 *)(Data_02000240 + 0x22b) = 3;

    if (Func_080770d0(0x8f0) == 0) {
        if (Func_080770c0(0x928) == 0) {
            Func_0808a260(0x6f, 16);
            Func_0808a250(0x3e, 0);
        } else if (Func_080770c0(0x929) == 0) {
            Func_0808a260(0x6f, 18);
            Func_0808a250(0x3e, 1);
        } else if (Func_080770c0(0x92a) == 0) {
            Func_0808a260(0x6f, 20);
            Func_0808a250(0x3e, 2);
        }
    }

    Func_0808a020();
}
