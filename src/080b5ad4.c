/*
 * VENEER AUDIT NOTE (2026-08-01) -- COMMENT ONLY, NO CODE CHANGE.
 *
 * This file is byte-exact, so nothing below is rewritten.  The note records
 * what the call sites actually are, so the next reader is not misled.
 *
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank: fifteen four-byte
 * `bx rN; nop` entries in register order r0..lr, ending at 0x08007320.  A
 * `bl` into that range is an INDIRECT CALL through the named register, not a
 * call to a function at the branch target.  The `Func_080072f*` prototype
 * this file declares is therefore a phantom, and the register load that
 * precedes each site -- which reads like dead code -- is the callee load.
 *
 * Why the file is still byte-identical while being wrong: a direct call to a
 * declared function at 0x080072f0 emits exactly the same `bl` the real
 * indirect call emits.  Converting to a function-pointer call would require
 * the compiler to choose the same register and therefore the same veneer
 * entry, which is a byte-exact source question and is deliberately NOT attempted
 * here.
 *
 * Sites in this owner, resolved with tools/veneer_resolve.ts:
 *
 *   0x080b5ae8  __call_via_r3  ->  0x03000164
 *     a relocated IWRAM routine. NOT established -- reached with two
 *     arguments at some sites and three at others. Do not assume
 */
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

s32 Func_080072f0(void *, u32, u32, void *);

extern char Value_06000290;
extern char Value_06000280;
extern char Value_0600028c;
extern char Value_03000164;

s32 Func_080b5ad4(void)
{
    u32 *dma = (u32 *)0x040000d4;
    u32 source = (u32)&Value_06000290;
    u32 destination = (u32)&Value_06000280;
    u32 control = 0x80000008;

    dma[0] = source;
    dma[1] = destination;
    dma[2] = control;
    return Func_080072f0(&Value_0600028c, 20, control, &Value_03000164);
}

void Func_080b5b08(void)
{
    u16 zero = 0;

    *(u16 *)0x04000012 = zero;
}
