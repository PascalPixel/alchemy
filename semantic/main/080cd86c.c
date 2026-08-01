/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * UNCERTAINTY, and it is deliberate.  What 0x03000164 DOES is not
 * established.  semantic/main/080c1ffc.c calls it a resident two-argument
 * owner initializer; across the tree it is reached with two arguments at
 * some sites and three at others, and where a third is passed it is almost
 * always zero.  It also sits four bytes -- one ARM instruction -- from the
 * fill at 0x03000168, the way the sin/cos pair at 0x0800231c/0x08002322
 * does.  That is suggestive of two entry points into one routine and it is
 * NOT asserted here: the evidence is recorded so the exact reconstruction can settle
 * it, and the type below says only what this call site proves --
 * two arguments, reached through __call_via_r5.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef void (*Resident_03000164)(void *destination, u32 size);

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_080030f8(u32);
void Func_080041d8(s32, s32);
void Func_080b5028(s32, s32, s32, s32);
void Func_080b5038(s32, s32, s32);
void Func_080cd508(u32);

/* Configures and clears the alternate affine battle-background presentation. */
void Func_080cd86c(u32 arg0) {
    s32 sp0;
    s32 var_ip;
    s32 var_r0;
    s32 var_r1;
    s32 var_r4;
    s32 var_r6;
    s32 var_r7;
    u16 temp_r1;
    u16 temp_r1_2;
    u16 temp_r1_3;
    u16 temp_r1_4;
    u16 temp_r2;
    u16 temp_r2_2;
    u16 temp_r2_3;
    u16 temp_r2_4;
    void *temp_fp;
    void *temp_r3;
    void *temp_r3_2;
    void *temp_r3_3;
    void *temp_r3_4;
    void *temp_r3_5;
    void *temp_r3_6;
    void *temp_r3_7;
    void *temp_r3_8;
    void *temp_r3_9;
    void *temp_r9;
    void *temp_sl;
    void *var_r5;

    temp_sl = M2C_FIELD((void *)0x03001EEC, void **, 0);
    temp_r9 = *(void **)0x03001E74;
    sp0 = M2C_FIELD((void *)0x03001EEC, s32 *, 4);
    temp_fp = M2C_FIELD((void *)0x03001EEC, void **, 0x14);
    Func_080cd508(arg0);
    M2C_FIELD(temp_fp, s32 *, 0xC) = 1;
    Func_080030f8(1U);
    M2C_FIELD((void *)0x04000050, s16 *, 0) = 0;
    temp_r1 = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x0208;
    temp_r2 = *(u16 *)0x02002090;
    if ((s32) temp_r2 <= 0x1F) {
        temp_r3 = (void *)((temp_r2 * 0xC) + 0x02002090);
        *(u16 *)0x02002090 = temp_r2 + 1;
        temp_r3_2 = temp_r3 + 4;
        M2C_FIELD(temp_r3, s32 *, 4) = 0x1741;
        M2C_FIELD(temp_r3_2, s32 *, 4) = 0x04000000;
        M2C_FIELD((temp_r3_2 + 4), s32 *, 4) = 0x20000;
    }
    *(u16 *)0x04000208 = temp_r1;
    M2C_FIELD((void *)0x03001AD0, s16 *, 6) = 0x20;
    Func_080030f8(1U);
    Func_080b5038(1, M2C_FIELD(temp_r9, u16 *, 0x648), 0x80);
    M2C_FIELD(temp_sl, s32 *, 0x77B4) = 0x18;
    M2C_FIELD(temp_sl, s32 *, 0x77B8) = 0;
    Func_080041d8(0x080CD4B5, 0xC80);
    temp_r1_2 = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x0208;
    temp_r2_2 = *(u16 *)0x02002090;
    if ((s32) temp_r2_2 <= 0x1F) {
        temp_r3_3 = (void *)((temp_r2_2 * 0xC) + 0x02002090);
        *(u16 *)0x02002090 = temp_r2_2 + 1;
        temp_r3_4 = temp_r3_3 + 4;
        M2C_FIELD(temp_r3_3, s32 *, 4) = 0x1341;
        M2C_FIELD(temp_r3_4, s32 *, 4) = 0x04000000;
        M2C_FIELD((temp_r3_4 + 4), s32 *, 4) = 0x20000;
    }
    *(u16 *)0x04000208 = temp_r1_2;
    Func_080030f8(1U);
    *(s16 *)0x0400000C = arg0 | 0x784;
    temp_r1_3 = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x0208;
    temp_r2_3 = *(u16 *)0x02002090;
    if ((s32) temp_r2_3 <= 0x1F) {
        temp_r3_5 = (void *)((temp_r2_3 * 0xC) + 0x02002090);
        *(u16 *)0x02002090 = temp_r2_3 + 1;
        temp_r3_6 = temp_r3_5 + 4;
        M2C_FIELD(temp_r3_5, s32 *, 4) = 0x1341;
        M2C_FIELD(temp_r3_6, s32 *, 4) = 0x04000000;
        M2C_FIELD((temp_r3_6 + 4), s32 *, 4) = 0x20000;
    }
    *(u16 *)0x04000208 = temp_r1_3;
    Func_080b5028(0, 0, 0, 0x64);
    M2C_FIELD(temp_fp, s32 *, 0xC) = 0;
    Func_080030f8(1U);
    M2C_FIELD((void *)0x04000050, s16 *, 0) = 0x3F44;
    M2C_FIELD((void *)0x04000050, s16 *, 2) = 0x100E;
    *(s32 *)0x04000028 = 0;
    *(s32 *)0x0400002C = 0xFFFFF000;
    *(s16 *)0x04000020 = 0x80;
    M2C_FIELD((void *)0x04000022, s16 *, 0) = 0;
    M2C_FIELD((void *)0x04000022, s16 *, 2) = 0;
    *(s16 *)0x04000026 = 0x100;
    M2C_FIELD((void *)0x04000040, s16 *, 0) = 0xF0;
    M2C_FIELD((void *)0x04000040, s16 *, 4) = 0x1088;
    temp_r3_7 = ((void *)0x04000040 + 4) - 2;
    M2C_FIELD(temp_r3_7, s16 *, 0) = 0xF0;
    M2C_FIELD(temp_r3_7, s16 *, 4) = 0x1088;
    M2C_FIELD((void *)0x04000048, s16 *, 0) = 0x3537;
    M2C_FIELD((void *)0x04000048, s16 *, 2) = 0x3F21;
    temp_r1_4 = *(u16 *)0x04000208;
    *(u16 *)0x04000208 = 0x0208;
    temp_r2_4 = *(u16 *)0x02002090;
    if ((s32) temp_r2_4 <= 0x1F) {
        temp_r3_8 = (void *)((temp_r2_4 * 0xC) + 0x02002090);
        *(u16 *)0x02002090 = temp_r2_4 + 1;
        temp_r3_9 = temp_r3_8 + 4;
        M2C_FIELD(temp_r3_8, s32 *, 4) = 0x7741;
        M2C_FIELD(temp_r3_9, s32 *, 4) = 0x04000000;
        M2C_FIELD((temp_r3_9 + 4), s32 *, 4) = 0x20000;
    }
    *(u16 *)0x04000208 = temp_r1_4;
    Func_080030f8(1U);
    var_r5 = 0;
    var_ip = 0;
    var_r7 = 0;
    var_r6 = 0;
    do {
        var_r4 = 0;
        var_r0 = var_r6 + 0x100;
        var_r1 = var_r7 * 2;
loop_12:
        var_r4 += 1;
        M2C_FIELD(var_r5, s16 *, 0x06003800) = (s16) (var_r0 | var_r1);
        var_r0 += 0x200;
        var_r1 += 2;
        var_r5 += 2;
        if (var_r4 != 8) {
            goto loop_12;
        }
        var_ip += 1;
        var_r6 += 0x1000;
        var_r7 += 8;
    } while (var_ip != 0x10);
    ((Resident_03000164)0x03000164)(sp0, 0x4000);
    ((Resident_03000164)0x03000164)((void *)0x06004000, 0x4000);
    Func_080030f8(1U);
}
