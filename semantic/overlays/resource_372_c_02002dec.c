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
s32 Func_020074d0();
s32 Func_020074dc();
void Func_02007506();
void Func_02007554();
void Func_02007562();
void Func_02007570();
u8 *Func_0200757e();
void Func_020075c8();
void Func_020075ec();
void Func_020075e6();
void Func_02007672();
u8 *Func_020075a8();
void Func_02007616();
u8 *Func_02007610();
void Func_0200769c();
void Func_02007638();
void Func_020076b4();
u8 *Func_020075ea();
u8 *Func_0200762c();
void Func_0200769a();
void Func_02007694();
void Func_02007720();
void Func_020076e0();
void Func_02007706();
void Func_02007728();
void Func_020076d4();
void Func_02007750();
void Func_0200774a();
void Func_02007766();
void Func_0200778e();
void Func_020077a6();
void Func_020076c8();
void Func_020076d8();
void Func_0200768e();
void Func_020077be();
void Func_020077d8();
void Func_020077e4();
void Func_020076b2();
void Func_0200777a();
void Func_020077bc();
void Func_0200778c();
void Func_020077ce();
void Func_0200780a();
void Func_02007822();
void Func_020077c4();
void Func_0200781a();
void Func_02007854();
void Func_020077b8();
void Func_02007802();
void Func_02007740();
void Func_020077f0();
void Func_0200774e();
void Func_02007820();
void Func_0200782a();
void Func_02007834();
void Func_0200781c();
void Func_0200786c();
void Func_02007844();
void Func_02007886();
void Func_020078a2();
void Func_020078ae();
void Func_02007856();
void Func_020078a6();
void Func_02007890();
void Func_0200789a();
void Func_020077d0();
void Func_020078ac();
void Func_020078b6();
void Func_020078c0();
void Func_02007806();
void Func_02007908();
void Func_020078b8();
void Func_020078c8();
void Func_020078d8();
u8 *Func_02007876();
void Func_02006304();
void Func_02007842();
void Func_02007866();

/* In-image data block (pool word 0x0200cd1c). */
extern u8 Data_0200cd1c[];

void Func_02002dec(void)
{
    u8 *p;
    u8 *obj;
    u8 *spr;
    u8 *flag0;      /* r7: obj0 + 35, revisited at the end */

    if (Func_020074d0(0x840) == 0) {    /* 132 << 4 */
        return;
    }
    if (Func_020074dc(0x841) != 0) {
        return;
    }

    Func_02007506();
    Func_02007554(0, 0x10000, 0x8000);   /* 128<<9, 128<<8 */
    Func_02007562(22, 0x10000, 0x8000);
    Func_02007570(26, 0x10000, 0x8000);
    Func_0200757e(8, 0x10000, 0x8000);
    Func_020075c8(0, 217, 0x557);

    p = Func_0200757e(0);                /* one argument here */
    if (p != 0) {
        Func_020075ec(22, *(s32 *)(p + 8), *(s32 *)(p + 16));
    }
    Func_020075e6(22, 235, 0x557);
    Func_02007672(22, 0xb000, 0);        /* 176 << 8 */

    p = Func_020075a8(0);
    if (p != 0) {
        Func_02007616(26, *(s32 *)(p + 8), *(s32 *)(p + 16));
    }
    Func_02007610(26, 199, 0x557);
    Func_0200769c(26, 0xd000, 0);        /* 208 << 8 */

    Func_02007638(25, 0xf70000, 0x4ba0000);   /* 247 << 16 */
    Func_020076b4(25, 0x6000, 0);             /* 192 << 7 */

    obj = Func_020075ea(8);
    spr = *(u8 **)(obj + 80);
    obj[35] = (u8)(obj[35] & 254);
    spr[9] = (u8)((spr[9] & ~12) | 4);

    obj = Func_02007610(0);              /* one argument here */
    flag0 = obj + 35;
    spr = *(u8 **)(obj + 80);
    *flag0 = (u8)(*flag0 & 254);
    spr[9] = (u8)((spr[9] & ~12) | 8);

    p = Func_0200762c(0);
    if (p != 0) {
        Func_0200769a(8, *(s32 *)(p + 8), *(s32 *)(p + 16));
    }
    Func_02007694(8, 221, 0x569);
    Func_02007720(8, 0xb000, 60);        /* 176 << 8 */
    Func_020076e0(26, 2);
    Func_02007706(0xec6);
    Func_02007728(26, 0, 40);
    Func_020076d4(9, 0x650000, 0x4ad0000);    /* 202 << 15 */
    Func_02007750(9, 0x2000, 0);              /* 128 << 6 */
    Func_0200774a(0x1009, 0, 10);
    Func_02007766(26, 0xa000, 0);             /* 160 << 8 */
    Func_0200778e(0x13333, 0x2666);
    Func_020077a6(0x650000, -1, 0x4ad0000, 1);
    Func_020076c8(9, 0x16666, 0xb333);
    Func_020076d8(9, Data_0200cd1c);
    Func_0200768e(60);
    Func_020077be(0x9999, 0x1333);
    Func_020077d8(0xbb0000, -1, 0x5300000, 1);  /* 187<<16, 166<<19 */
    Func_020077e4();
    Func_020076b2(40);
    Func_0200777a(26, 2);
    Func_020077bc(26, 0, 20);
    Func_0200778c(9, 2);
    Func_020077ce(0x4009, 0, 20);
    Func_0200780a(0x20000, 0x4000);           /* 128<<10, 128<<7 */
    Func_02007822(0xdd0000, -1, 0x5690000, 1); /* 221 << 16 */
    Func_020077c4(0, 8, 0);
    Func_020077ce(22, 8, 0);
    Func_0200781a(26, 0x3000, 80);            /* 192 << 6 */
    Func_02007854(0xb60000, -1, 0x5500000, 1); /* 182<<16, 170<<19 */
    Func_020077b8(8, 182, 0x568);             /* 173 << 3 */
    Func_02007802(8, 9, 0);
    Func_02007740(30);
    Func_020077f0(8, 3);
    Func_0200774e(10);
    Func_02007820(0, 9, 0);
    Func_0200782a(22, 9, 0);
    Func_02007834(26, 9, 0);
    Func_0200781c(9, 3);
    Func_0200786c(9, 0);
    Func_02007844(26, 2);
    Func_02007886(26, 0, 10);
    Func_020078a2(9, 0xe000, 40);             /* 224 << 8 */
    Func_020078ae(9, 0x3000, 20);             /* 192 << 6 */
    Func_02007856(9, 3);
    Func_020078a6(9, 0);
    Func_02007890(26, 8, 0);
    Func_0200789a(22, 0, 0);
    Func_020077d0(40);
    Func_020078a2(0, 9, 0);
    Func_020078ac(22, 9, 0);
    Func_020078b6(26, 9, 0);
    Func_020078c0(8, 9, 0);
    Func_020078c0(9, 2);                      /* two arguments here */
    Func_02007806(20);
    Func_02007908(9, 0, 10);
    Func_020078b8(0, 3);
    Func_020078c0(26, 3);
    Func_020078c8(22, 3);
    Func_020078d8(8, 3);

    *flag0 = (u8)(*flag0 | 1);

    obj = Func_02007876(8);
    obj[35] = (u8)(obj[35] | 1);

    Func_02006304();
    Func_02007842(0x841);
    Func_02007866();
}
