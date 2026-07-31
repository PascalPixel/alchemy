typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 372 scene setup at 0x02002dec (960 bytes, 78 distinct call targets
 * across 84 call sites).
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02002dec, matching
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02003164.  `pop {r0} ; bx r0` —
 * r0 is the popped return address, so the owner returns nothing.
 * 0x0200316a-0x020031ab is alignment plus the literal pool, reached only by
 * `ldr rN, [pc, #imm]`.
 *
 * Two entry guards (flag 0x840 must be set, flag 0x841 must be clear), then a
 * long straight-line build of a five-actor scene (handles 0, 8, 9, 22, 25, 26)
 * with three optional position fix-ups of the shape
 * `p = query(0); if (p) place(handle, p->f08, p->f10)`, which is the same
 * descriptor-copy idiom the byte-exact `assets/code/resource_372_c_0200173c.c`
 * uses.
 *
 * Two u8 flag pairs are edited in place and one of them is revisited at the very
 * end, which is why r7 is callee-saved across ~60 calls:
 *   obj8  = 0x020075ea(8):  obj8[35] &= ~1;  (*(u8**)(obj8+80))[9] = value with
 *           bits 2-3 replaced by 4
 *   obj0  = 0x02007610(0):  obj0[35] &= ~1;  (*(u8**)(obj0+80))[9] = value with
 *           bits 2-3 replaced by 8; later obj0[35] |= 1
 * The +0x50 / +9 pair matches `assets/code/resource_372_c_02000434.c`.
 *
 * Six imports are reached with different argument counts at different sites:
 * 0x0200757e, 0x02007610, 0x020077ce, 0x020078a2 and 0x020078c0 (three sites,
 * 3/2/2 arguments).  Old-style declarations are therefore mandatory.
 *
 * Observation: r4 is written at 0x02002ec4 and 0x02002eea but is never saved by
 * the prologue — the same original-codegen oddity recorded for the owner at
 * 0x0200441c.  It has no effect on the reconstructed behaviour.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity for this overlay, not a place to
 * disassemble.  0x02006304 is in that class.
 *
 * Uncertainties: handles, flag identifiers and the 16.16-looking constants are
 * unresolved.
 */

/* Imports, old-style: interfaces unknown, several with varying arity. */
s32 Func_080770c0();
void Func_0808a018();

u8 *Func_0808a090();
u8 *Func_0808a080();

void Func_0808a0f0();
void Func_0808a1b8();
u8 *Func_0808a0d0();
void Func_0808a138();
void Func_0808a170();
void Func_0808a188();
void Func_0808a208();
void Func_0808a210();
void Func_0808a098();
void Func_0808a010();
void Func_0808a218();
void Func_0808a148();
void Func_0808a110();
void Func_0808a180();
void Func_0808a150();
void Func_0808a100();
void Func_020031ac();
void Func_080770c8();
void Func_0808a020();

/* In-image data block (pool word 0x0200cd1c). */
extern u8 Data_0200cd1c[];

void Func_02002dec(void)
{
    u8 *p;
    u8 *obj;
    u8 *spr;
    u8 *flag0;      /* r7: obj0 + 35, revisited at the end */

    if (Func_080770c0(0x840) == 0) {    /* 132 << 4 */
        return;
    }
    if (Func_080770c0(0x841) != 0) {
        return;
    }

    Func_0808a018();
    Func_0808a090(0, 0x10000, 0x8000);   /* 128<<9, 128<<8 */
    Func_0808a090(22, 0x10000, 0x8000);
    Func_0808a090(26, 0x10000, 0x8000);
    Func_0808a090(8, 0x10000, 0x8000);
    Func_0808a0d0(0, 217, 0x557);

    p = Func_0808a080(0);                /* one argument here */
    if (p != 0) {
        Func_0808a0f0(22, *(s32 *)(p + 8), *(s32 *)(p + 16));
    }
    Func_0808a0d0(22, 235, 0x557);
    Func_0808a1b8(22, 0xb000, 0);        /* 176 << 8 */

    p = Func_0808a080(0);
    if (p != 0) {
        Func_0808a0f0(26, *(s32 *)(p + 8), *(s32 *)(p + 16));
    }
    Func_0808a0d0(26, 199, 0x557);
    Func_0808a1b8(26, 0xd000, 0);        /* 208 << 8 */

    Func_0808a0f0(25, 0xf70000, 0x4ba0000);   /* 247 << 16 */
    Func_0808a1b8(25, 0x6000, 0);             /* 192 << 7 */

    obj = Func_0808a080(8);
    spr = *(u8 **)(obj + 80);
    obj[35] = (u8)(obj[35] & 254);
    spr[9] = (u8)((spr[9] & ~12) | 4);

    obj = Func_0808a080(0);              /* one argument here */
    flag0 = obj + 35;
    spr = *(u8 **)(obj + 80);
    *flag0 = (u8)(*flag0 & 254);
    spr[9] = (u8)((spr[9] & ~12) | 8);

    p = Func_0808a080(0);
    if (p != 0) {
        Func_0808a0f0(8, *(s32 *)(p + 8), *(s32 *)(p + 16));
    }
    Func_0808a0d0(8, 221, 0x569);
    Func_0808a1b8(8, 0xb000, 60);        /* 176 << 8 */
    Func_0808a138(26, 2);
    Func_0808a170(0xec6);
    Func_0808a188(26, 0, 40);
    Func_0808a0f0(9, 0x650000, 0x4ad0000);    /* 202 << 15 */
    Func_0808a1b8(9, 0x2000, 0);              /* 128 << 6 */
    Func_0808a188(0x1009, 0, 10);
    Func_0808a1b8(26, 0xa000, 0);             /* 160 << 8 */
    Func_0808a208(0x13333, 0x2666);
    Func_0808a210(0x650000, -1, 0x4ad0000, 1);
    Func_0808a090(9, 0x16666, 0xb333);
    Func_0808a098(9, Data_0200cd1c);
    Func_0808a010(60);
    Func_0808a208(0x9999, 0x1333);
    Func_0808a210(0xbb0000, -1, 0x5300000, 1);  /* 187<<16, 166<<19 */
    Func_0808a218();
    Func_0808a010(40);
    Func_0808a138(26, 2);
    Func_0808a188(26, 0, 20);
    Func_0808a138(9, 2);
    Func_0808a188(0x4009, 0, 20);
    Func_0808a208(0x20000, 0x4000);           /* 128<<10, 128<<7 */
    Func_0808a210(0xdd0000, -1, 0x5690000, 1); /* 221 << 16 */
    Func_0808a148(0, 8, 0);
    Func_0808a148(22, 8, 0);
    Func_0808a1b8(26, 0x3000, 80);            /* 192 << 6 */
    Func_0808a210(0xb60000, -1, 0x5500000, 1); /* 182<<16, 170<<19 */
    Func_0808a0d0(8, 182, 0x568);             /* 173 << 3 */
    Func_0808a148(8, 9, 0);
    Func_0808a010(30);
    Func_0808a110(8, 3);
    Func_0808a010(10);
    Func_0808a148(0, 9, 0);
    Func_0808a148(22, 9, 0);
    Func_0808a148(26, 9, 0);
    Func_0808a110(9, 3);
    Func_0808a180(9, 0);
    Func_0808a138(26, 2);
    Func_0808a188(26, 0, 10);
    Func_0808a1b8(9, 0xe000, 40);             /* 224 << 8 */
    Func_0808a1b8(9, 0x3000, 20);             /* 192 << 6 */
    Func_0808a110(9, 3);
    Func_0808a180(9, 0);
    Func_0808a150(26, 8, 0);
    Func_0808a150(22, 0, 0);
    Func_0808a010(40);
    Func_0808a148(0, 9, 0);
    Func_0808a148(22, 9, 0);
    Func_0808a148(26, 9, 0);
    Func_0808a148(8, 9, 0);
    Func_0808a138(9, 2);                      /* two arguments here */
    Func_0808a010(20);
    Func_0808a188(9, 0, 10);
    Func_0808a100(0, 3);
    Func_0808a100(26, 3);
    Func_0808a100(22, 3);
    Func_0808a110(8, 3);

    *flag0 = (u8)(*flag0 | 1);

    obj = Func_0808a080(8);
    obj[35] = (u8)(obj[35] | 1);

    Func_020031ac();
    Func_080770c8(0x841);
    Func_0808a020();
}
