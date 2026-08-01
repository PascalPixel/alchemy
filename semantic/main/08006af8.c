/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320.  A `bl` into that range
 * is an indirect call through the named register.
 *
 * The regularity that makes these readable, and it is CHECKED per site rather
 * than assumed: the callee value always appeared in the draft's argument list
 * at exactly the position matching the veneer's register index.  The compiler
 * loaded the callee into rN and the draft read rN as argument N.  So a
 * __call_via_r0 site has ZERO real arguments, r1 has one, r2 has two.  Each
 * site below was confirmed by checking that the register and the argument
 * position agree.
 *
 * The value at 0x02004C1C is the callee, reached through __call_via_r1, so
 * the call takes ONE argument. Two sites, both rewritten.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef s32 (*Callee_02004C1C)(s32 argument);

void Func_08006a00(s32);
void Func_08006a78(void);

s32 Func_08006af8(u8 value, s32 argument, u8 expected) {
    u32 packed;
    s32 result;

    packed = value;
    result = 0;
    Func_08006a00(value);
    packed = (packed | 0xC000) << 16;

    goto loop;

failure:
    if (*(u16 *)(*(u8 **)0x02004C08 + 20) == 0x1CC2)
        *(u8 *)0x0E005555 = 0xF0;
    result = packed >> 16;
    goto done;

loop:
    if ((u8)((Callee_02004C1C)*(s32 *)0x02004C1C)(argument) == expected)
        goto done;
    if (*(u8 *)0x02004C24 == 0)
        goto loop;
    if ((u8)((Callee_02004C1C)*(s32 *)0x02004C1C)(argument) != expected)
        goto failure;

done:
    Func_08006a78();
    return result;
}
