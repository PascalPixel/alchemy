typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 372 guarded cutscene step at 0x02000f38 (364 bytes, 32 calls).
 *
 * Complete owner: `push {r5, lr}` at 0x02000f38, matching
 * `pop {r5} / pop {r0} / bx r0` at 0x02001070.  `pop {r0} ; bx r0` — r0 is the
 * popped return address, so the owner returns nothing.
 * 0x02001076-0x020010a3 is alignment plus the literal pool, reached only by
 * `ldr rN, [pc, #imm]`.
 *
 * Third member of the same guarded-step family as
 * `semantic/overlays/resource_372_c_02000d5c.c` and `..._02001348.c`: outer
 * flag guard 0x311, one-shot object-setup guard 0x831, then a trailing section
 * guarded by 0x837 / 0x841 / 0x30c that picks one of two script branches on a
 * signed comparison of the word at +12 of a queried record against 0x800000.
 *
 * Object layout touched here (p = 0x02005684(12)):
 *   p[+0x0c]  s32, advanced by 0x1000000 and mirrored to p[+0x3c]
 *   p[+0x30]  s32 = 0x18000        p[+0x34]  s32 = 0x18000
 *   p[+0x44]  s32 = 0x8000
 * — the same field set as 0x02000d5c and 0x02001348.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity for this overlay, not a place
 * to disassemble.  0x020020a8, 0x02002aa6 and 0x02002abc are in that class.
 *
 * Correction to the byte-exact draft in
 * `work/claude/notes/resource_372-0f38-best.c`: it spells the last three
 * message constants as 0x34b0000 / 0x33d0000 / 0x38f0000.  The pool words at
 * 0x02001098, 0x0200109c and 0x020010a0 are 0x0000034b, 0x0000033d and
 * 0x0000038f, and they are loaded unshifted; the unshifted values are used
 * here.
 *
 * Uncertainties: all flag and message identifiers are unresolved; 0xe666
 * recurs across this overlay as the third argument of a (-1, -1, k) call.
 */

/* Imports, old-style: interfaces unknown. */
s32 Func_0200561a();
void Func_02005644();
s32 Func_0200562a();
u8 *Func_02005684();
void Func_02005626();
void Func_02005824();
void Func_020055b2();
void Func_02005830();
void Func_02005714();
void Func_0200572a();
void Func_020057ba();
void Func_02005740();
void Func_020057d0();
void Func_02005754();
void Func_020056d2();
void Func_02005898();
void Func_020056ae();
void Func_020056ba();
void Func_020058a6();
void Func_020056e4();
void Func_020020a8();
void Func_020056ee();
s32 Func_020056ec();
s32 Func_020056f6();
s32 Func_02005702();
u8 *Func_0200575c();
void Func_02002aa6();
void Func_020057c8();
void Func_02002abc();
void Func_020057de();
void Func_0200574e();
void Func_02005772();

void Func_02000f38(void)
{
    u8 *p;
    u8 *rec;

    if (Func_0200561a(0x311) != 0) {
        return;
    }

    Func_02005644();

    if (Func_0200562a(0x831) == 0) {
        p = Func_02005684(12);
        Func_02005626(0x40000, 0x40000, 0x10000);  /* 128<<11, 128<<11, 128<<9 */
        Func_02005824(141);
        Func_020055b2(40);
        Func_02005830(145);
        Func_02005714(12, 0x17d0000, 0x3280000);   /* 202 << 18 */

        *(s32 *)(p + 48) = 0x18000;                /* 192 << 9 */
        *(s32 *)(p + 52) = 0x18000;
        *(s32 *)(p + 12) += 0x1000000;             /* 128 << 17 */
        *(s32 *)(p + 60) = *(s32 *)(p + 12);
        *(s32 *)(p + 68) = 0x8000;                 /* 128 << 8 */

        Func_0200572a(12, 0x122, 0x341);           /* 145 << 1 */
        Func_020057ba(12, 1);
        Func_02005740(12, 0x102, 0x354);           /* 129<<1, 213<<2 */
        Func_020057d0(12, 2);
        Func_02005754(12, 224, 0x368);             /* 218 << 2 */
        Func_020056d2(40);
        Func_02005898(0x121);
        Func_020056ae(-1, -1, 0xe666);
        Func_020056ba();
        Func_020058a6();
        Func_020056e4(0x831);
    }

    Func_020020a8();
    Func_020056ee(0x311);

    if (Func_020056ec(0x837) != 0 && Func_020056f6(0x841) == 0 &&
        Func_02005702(0x30c) == 0) {               /* 195 << 2 */
        rec = Func_0200575c(0);
        if (*(s32 *)(rec + 12) > 0x800000) {       /* 128 << 16, signed */
            Func_02002aa6(219, 0x34b);
            Func_020057c8(0, 179, 0x33d);
        } else {
            Func_02002abc(214, 0x38c);             /* 227 << 2 */
            Func_020057de(0, 219, 0x38f);
        }
        Func_0200574e(0x30c);
    }

    Func_02005772();
}
