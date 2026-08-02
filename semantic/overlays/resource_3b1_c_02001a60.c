typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;

/*
 * resource_3b1 owner at 0x02001a60, 212 bytes.  It is gated by flag 0x922,
 * returns through `pop {r0} / bx r0` at 0x02001b16-0x02001b18, includes the
 * alignment halfword at 0x02001b1a and six pool words at
 * 0x02001b1c-0x02001b33, and ends immediately before the next owner's
 * saved-register prologue at 0x02001b34.
 *
 * The three local helper owners are the already-described resource_3b1
 * helpers at 0x0200486c, 0x02004880 and 0x020048ac.  Their calls are kept
 * explicit here so the source remains ordinary C while retaining the
 * machine's call order and argument values.
 */

extern void Func_0200486c(s32 id);
extern void Func_02004880(s32 id, u16 value);
extern void Func_020048ac(s32 a, s32 b, s32 c, u32 flags);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a170(s32 message);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a460(void);
extern s32 Func_080770c0(s32 flag);
extern void Func_080770c8(s32 flag);

void Func_02001a60(void)
{
    if (Func_080770c0(0x922) == 0)
        return;

    Func_0808a018();
    Func_0808a460();
    Func_0808a208(0x19999, 0x3333);
    Func_020048ac(0xe0 << 17, -1, 0x027e0000, 0x10000028u);
    Func_0808a170(0x1d26);

    Func_0200486c(8);
    Func_0200486c(10);
    Func_02004880(8, 0x3000);
    Func_0200486c(8);
    Func_02004880(10, 0xd000);
    Func_0200486c(10);
    Func_02004880(9, 0x5000);
    Func_0200486c(9);

    Func_0808a1b8(8, 0, 20);
    Func_0200486c(8);
    Func_02004880(9, 0x8000);
    Func_0200486c(10);
    Func_0200486c(8);
    Func_0200486c(8);
    Func_02004880(10, 0xb000);
    Func_0200486c(8);

    Func_080770c8(0x920);
    Func_0808a020();
}
