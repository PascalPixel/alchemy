typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 372 giant cutscene sheet at 0x020031ac (2716 bytes, 228 distinct
 * call targets across 245 call sites).
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the high-register saves and
 * `sub sp, #4` at 0x020031ac, matching `add sp, #4 / pop {r3, r5, r6, r7} /
 * mov r8..fp / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02003c1c.
 * `pop {r0} ; bx r0` — r0 is the popped return address, so the owner returns
 * nothing.  Three literal pools sit inside the span and are all branched over:
 * 0x02003392-0x020033c3 (`b.n` at 0x02003390), 0x020037ea-0x0200382b (`b.n` at
 * 0x020037e8) and 0x02003abc-0x02003adb (`b.n` at 0x02003aba); the trailing
 * pool is 0x02003c2e-0x02003c47.
 *
 * The third interior pool is the documented argument-carrying case: r0 and r3
 * are live across the `b.n` at 0x02003aba and are only finished at 0x02003adc
 * (`adds r0, #60` / `adds r3, #100`), so the store after it belongs to the
 * 0x020080ea result computed before the branch.
 *
 * `work/claude/notes/resource_372-31ac.md` parked this row for the byte-exact
 * lane on size and on those mid-function pools.  Neither is a semantic
 * obstacle; there are no jump tables and no DMA blocks in the span.
 *
 * The single stack word holds `obj19 + 85`, saved at 0x020035b0 and reused at
 * 0x020037bc — that is the only reason the frame exists.
 *
 * Two long-lived objects drive the whole sheet:
 *   obj19 = 0x020078ec(19), spr19 = *(void **)(obj19 + 0x50)
 *   obj27 = 0x020078f4(27), spr27 = *(void **)(obj27 + 0x50)
 * The +0x50 / +9 sprite-flag pair and the "+0x64 = query(...) + 60 with 90 as
 * the second argument" idiom are both proven by the byte-exact
 * `assets/code/resource_372_c_02000434.c`; the +0x66 field used as the
 * do-while guard and as a per-actor index is the same u16 that file writes.
 *
 * 17 imports are reached with two different argument counts at their two sites
 * (0x02007984, 0x020079ec, 0x02007a90, 0x02007a96, 0x02007aa6, 0x02007af2,
 * 0x02007b1c, 0x02007b5e, 0x02007b8a, 0x02007e98, 0x02007ebe, 0x02007eda,
 * 0x02007f1a, 0x02008018, 0x0200809a, 0x02008224, 0x02008234).  Old-style
 * declarations are therefore mandatory; 245 sites minus 228 distinct targets is
 * exactly those 17 second uses, which is the completeness check for this owner.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity for this overlay, not a place to
 * disassemble.  0x02003390, 0x02007412 and 0x0200755e are in that class —
 * 0x02003390 in particular is this owner's own `b.n` at the first interior
 * pool, which is what the identity reading predicts and a location reading
 * cannot explain.
 *
 * Uncertainties: handles, message ids and the 16.16-looking constants are
 * unresolved.  0x80000000 stored into +0x38/+0x3c/+0x40 reads like "unset".
 * r5 changes role three times (message-table pointer, then the mask -13, then
 * the merged sprite-flag byte); each use is written out explicitly below.
 */

/* Imports, old-style: interfaces unknown, 17 of them with varying arity. */
void Func_02000134();
void Func_02003c48();
u8 *Func_0808a080();
void Func_0808a010();
void Func_0808a090();

s32 Func_0808a098();
s32 Func_030003e0();
void Func_080091f0();
s32 Func_080000f8();
void Func_0808a0a0();
void Func_0808a208();
void Func_0808a0d0();

void Func_0808a0b0();
u8 *Func_0808a210();
void Func_0808a2d8();
void Func_0808a1b8();
void Func_0808a2d0();
void Func_0808a150();
void Func_020040f0();
void Func_0808a1e8();
void Func_080f9010();
void Func_080000c0();
void Func_080091e0();
void Func_080000d0();
void Func_08009128();
void Func_080000d8();
void Func_0808a158();
void Func_080091f8();
void Func_0808a330();
void Func_0808a348();
u8 *Func_0808a228();
void Func_0808a0a8();

u8 *Func_0808a0f0();
u8 *Func_0808a100();
void Func_08009190();
void Func_080770c8();
void Func_08009188();
void Func_080770d0();
void Func_0808a058();
void Func_0808a2e0();
void Func_0808a5e8();

/* In-image data blocks. */
extern u8 Data_0200cd6c[];
extern u8 Data_0200ce04[];
extern u8 Data_0200ce30[];
extern u8 Data_0200ce5c[];
extern u8 Data_0200ce88[];
extern u8 Data_0200ceb4[];
extern u8 Data_0200bce5[];
extern u8 Data_0200cedc[];
extern u8 Data_0200be19[];
extern u8 Data_0200cec8[];
extern u8 Data_0200c5b9[];

void Func_020031ac(void)
{
    u8 *obj19;      /* r7 */
    u8 *obj27;      /* r6, later reused for the four sub-actors */
    u8 *spr27;      /* fp = *(void **)(obj27 + 0x50) */
    u8 *spr19;      /* sl = *(void **)(obj19 + 0x50), later reused as a value */
    u8 *o;
    u8 *guard;      /* obj19 + 102 */
    u8 *saved;      /* the stack word: obj19 + 85 */
    s32 mask;       /* r5 in its second role: -13 */
    s32 hidden;     /* r8: 0, then 0x80000000, then 0xe000 */
    s32 keep;       /* r9: 0, then 0xb000 */
    s32 lift;       /* sl in its second role: 0x20000 */
    s32 clear;      /* sl in its third role: the 254 mask */

    obj19 = Func_0808a080(19);
    obj27 = Func_0808a080(27);
    spr27 = *(u8 **)(obj27 + 80);
    spr19 = *(u8 **)(obj19 + 80);

    Func_0808a208(0x10000, 0x2000);                 /* 128<<9, 128<<6 */
    Func_0808a210(0x6e0000, -1, 0x58b0000, 1);      /* 220<<15 */
    Func_0808a090(8, 0x13333, 0x9999);
    Func_0808a090(26, 0x13333, 0x9999);
    Func_0808a090(0, 0x13333, 0x9999);
    Func_0808a090(22, 0x13333, 0x9999);
    Func_0808a098(8, Data_0200cd6c);
    Func_0808a010(10);
    Func_0808a098(26, Data_0200cd6c);
    Func_0808a2d8();
    Func_0808a010(10);
    Func_0808a098(0, Data_0200cd6c);
    Func_0808a010(10);
    Func_0808a2d0();
    Func_0808a098(22, Data_0200cd6c);
    Func_0808a010(128);
    Func_02000134();
    Func_0808a210(0xae0000, -1, 0x5940000, 1);      /* 174<<16 */
    Func_0808a010(104);
    Func_0808a210(0x990000, -1, 0x52d0000, 1);      /* 153<<16, four arguments */
    Func_0808a0d0(9, 158, 0x4f8);                   /* 159<<3 */
    Func_0808a1b8(9, 0x2000, 0);                    /* three arguments */
    Func_0808a0a0(8);
    Func_0808a098(8, Data_0200ce04);                /* two arguments */
    Func_0808a098(26, Data_0200ce30);
    Func_0808a098(0, Data_0200ce5c);
    Func_0808a0b0(22, Data_0200ce88);
    Func_080091f0(0x40000, 0x40000, 0x10000);       /* 128<<11 */
    Func_080f9010(145);
    Func_0808a010(20);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(60);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(26, 0x101, 0);
    Func_0808a1e8(22, 0x101, 0);
    Func_0808a1e8(8, 0x101, 0);
    Func_0808a1e8(9, 0x101, 60);
    Func_0808a150(26, 8, 0);
    Func_0808a150(22, 0, 0);
    Func_0808a010(20);

    /* Five copies of the "+0x64 = query(...) + 20" frame-counter idiom. */
    o = Func_0808a080(0);
    *(u16 *)(o + 100) = (u16)(Func_030003e0(Func_080000f8(), 20) + 20);
    hidden = 0;
    keep = 0;
    o = Func_0808a080(22);                          /* one argument */
    *(u16 *)(o + 100) = (u16)(Func_030003e0(Func_080000f8(), 20) + 20);
    o = Func_0808a080(26);
    *(u16 *)(o + 100) = (u16)(Func_030003e0(Func_080000f8(), 20) + 20);
    o = Func_0808a080(8);
    *(u16 *)(o + 100) = (u16)(Func_030003e0(Func_080000f8(), 20) + 20);
    o = Func_0808a080(9);
    *(u16 *)(o + 100) = (u16)(Func_030003e0(Func_080000f8(), 20) + 20);

    Func_0808a098(9, Data_0200ceb4);
    Func_0808a010(30);
    Func_0808a098(0, Data_0200ceb4);
    Func_0808a098(26, Data_0200ceb4);
    Func_0808a098(22, Data_0200ceb4);
    Func_0808a098(8, Data_0200ceb4);
    Func_0808a010(10);                              /* one argument */
    Func_080f9010(17);
    Func_080091f0(0x30000, 0x30000, 0x10000);       /* 192<<10 */
    Func_080f9010(145);
    Func_0808a010(30);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(120);
    Func_080091f0(0x30000, 0x30000, 0x10000);
    Func_080f9010(145);
    Func_0808a010(40);
    Func_080091f0(0x20000, 0x20000, 0x10000);       /* 128<<10 */
    Func_0808a010(60);
    Func_080091f0(0x40000, 0x40000, 0x10000);       /* three arguments */
    Func_080f9010(145);
    Func_0808a010(20);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(60);
    Func_080091f0(0x30000, 0x30000, 0x10000);
    Func_080f9010(145);
    Func_0808a010(40);
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(60);
    Func_0808a2d0();
    Func_080091f0(0x10000, 0x10000, 0x10000);
    Func_0808a010(1);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a208(0x80000, 0x80000);                /* 128<<12 */
    Func_0808a210(0xd90000, -1, 0x43c0000, 1);      /* 217<<16 */
    Func_0808a330(0, 0);
    Func_0808a348(40);
    Func_080000c0(40);                              /* one argument */
    Func_0808a158(19, 0);
    Func_080091e0(Func_0808a080(19), 0);
    Func_080091e0(Func_0808a080(27), 0);

    *(s32 *)(obj27 + 24) = 0xcccc;
    *(s32 *)(obj27 + 28) = 0xcccc;
    obj27[35] = (u8)(obj27[35] & 254);
    spr27[9] = (u8)((spr27[9] & ~12) | 4);

    *(s32 *)(obj19 + 8) = 0xc80000;                 /* 200<<16 */
    *(s32 *)(obj19 + 12) = 0xc80000;
    *(s32 *)(obj19 + 56) = 0xc80000;
    *(s32 *)(obj19 + 60) = 0xc80000;
    *(s32 *)(obj19 + 16) = 0x3820000;
    *(s32 *)(obj19 + 64) = 0x3820000;
    saved = obj19 + 85;
    *saved = (u8)hidden;                            /* 0 */
    obj19[35] = (u8)(obj19[35] & 254);
    spr19[9] = (u8)(spr19[9] & ~12);

    o = Func_0808a228(); *(s32 *)(o + 56) = (s32)0x80000000;
    o = Func_0808a228(); *(s32 *)(o + 60) = (s32)0x80000000;
    o = Func_0808a228(); *(s32 *)(o + 64) = (s32)0x80000000;
    o = Func_0808a228(); *(s32 *)(o + 36) = keep;   /* 0 */
    o = Func_0808a228(); *(s32 *)(o + 40) = keep;
    o = Func_0808a228(); *(s32 *)(o + 44) = keep;

    Func_080000c0(1);
    Func_0808a210(0xf70000, 0x800000, 0x3950000, 0);  /* 247<<16, 128<<16 */
    Func_08009128();
    Func_080000c0(1);
    Func_0808a330(0x10003, 1);
    Func_0808a330(0x10000, 2);                      /* 128<<9 */
    Func_0808a348(30);
    Func_080000c0(30);
    Func_080000d0(Data_0200bce5, 3200);             /* 200<<4 */
    Func_0808a098(19, Data_0200cedc);
    Func_0808a208(0x20000, 0x7ae);                  /* 128<<10 */
    Func_0808a210(0xaf0000, 0x600000, 0x43e0000, 1); /* 175<<16, 192<<15 */

    /* Spin until obj19's +0x66 counter reaches 8. */
    guard = obj19 + 102;
    do {
        Func_080000c0(1);
    } while (*(s16 *)guard != 8);

    Func_0808a330(0, 0);
    Func_0808a348(60);
    Func_080000c0(60);
    Func_080091f8();

    hidden = (s32)0x80000000;
    o = Func_0808a228(); *(s32 *)(o + 56) = hidden;
    o = Func_0808a228(); *(s32 *)(o + 60) = hidden;
    o = Func_0808a228(); *(s32 *)(o + 64) = hidden;
    o = Func_0808a228(); *(s32 *)(o + 36) = 0;
    o = Func_0808a228(); *(s32 *)(o + 40) = 0;
    o = Func_0808a228(); *(s32 *)(o + 44) = 0;

    Func_080000d8(Data_0200bce5);
    Func_0808a0a8(19);
    Func_080000c0(1);
    Func_0808a100(19, 0);

    *(s32 *)(obj27 + 24) = 0x14000;                 /* 160<<9 */
    *(s32 *)(obj27 + 28) = 0x14000;
    spr27[35] = 2;
    *(s32 *)(spr27 + 24) = 0x14000;
    lift = 0x20000;                                 /* 128<<10 */
    *(s32 *)(obj19 + 24) = lift;
    *(s32 *)(obj19 + 28) = lift;
    *(s32 *)(obj19 + 8) = 0;
    *(s32 *)(obj19 + 16) = 0;
    *(s32 *)(obj19 + 56) = 0;
    *(s32 *)(obj19 + 64) = 0;

    Func_080000c0(1);
    Func_0808a100(23, 8);                           /* two arguments */
    Func_0808a0f0(9, 0xa90000, 0x4f00000);          /* 169<<16, 158<<19 */
    Func_0808a1b8(9, 0xc000, 0);                    /* 192<<8 */
    Func_0808a100(9, 9);
    Func_0808a0f0(26, 0x970000, 0x50c0000);         /* three arguments, 151<<16 */
    Func_0808a1b8(26, 0x8000, 0);                   /* 128<<8 */
    Func_0808a100(26, 5);                           /* two arguments */
    Func_0808a0f0(8, 0xaa0000, 0x5210000);          /* 170<<16 */
    Func_0808a1b8(8, 0x6000, 0);                    /* 192<<7 */
    Func_0808a100(8, 5);
    Func_0808a0f0(0, 0xb90000, 0x5350000);          /* 185<<16 */
    Func_0808a1b8(0, 0x2000, 0);                    /* 128<<6 */
    Func_0808a100(0, 17);                           /* two arguments */
    Func_0808a0f0(22, 0xa90000, 0x5680000);         /* 169<<16, 173<<19 */
    Func_0808a1b8(22, 0x4000, 0);                   /* 128<<7 */
    Func_0808a100(22, 0);
    Func_0808a210(0xa60000, 0, 0x5390000, 0);       /* 166<<16 */
    Func_08009128();

    *saved = 0;
    *(s32 *)(obj19 + 56) = hidden;
    *(s32 *)(obj19 + 60) = hidden;
    *(s32 *)(obj19 + 64) = hidden;

    Func_02003c48();
    Func_0808a0f0(27, 0xda0000, 0x4980000);         /* 218<<16, 147<<19 */
    Func_0808a210(0xd20000, 0, 0x4ac0000, 0);       /* 210<<16 */
    Func_08009128();

    *(s32 *)(obj27 + 24) = lift;
    *(s32 *)(obj27 + 28) = lift;

    Func_080000d0(Data_0200be19, 3200);
    Func_0808a0a8(10);
    Func_0808a0a8(24);
    Func_0808a0a8(25);
    Func_080000c0(1);

    /* Four sub-actors, each: clear bit 0 of +0x23, set +0x18/+0x1c, set the
     * +0x06 halfword and clear bits 2-3 of the sprite's +9 flags. */
    clear = 254;
    mask = -13;
    obj27 = Func_0808a080(10);
    spr27 = *(u8 **)(obj27 + 80);
    obj27[35] = (u8)(obj27[35] & clear);
    *(s32 *)(obj27 + 24) = 0x10000;                 /* 128<<9 */
    *(s32 *)(obj27 + 28) = 0x10000;
    *(u16 *)(obj27 + 6) = 0xd000;                   /* 208<<8 */
    spr27[9] = (u8)(spr27[9] & mask);
    Func_0808a100(10, 0);

    obj27 = Func_0808a080(24);
    spr27 = *(u8 **)(obj27 + 80);
    obj27[35] = (u8)(obj27[35] & clear);
    *(s32 *)(obj27 + 24) = 0x10000;
    *(s32 *)(obj27 + 28) = 0x10000;
    keep = 0xb000;                                  /* 176<<8 */
    spr27[9] = (u8)(spr27[9] & mask);
    *(u16 *)(obj27 + 6) = (u16)keep;
    Func_0808a100(24, 5);

    obj27 = Func_0808a080(25);
    spr27 = *(u8 **)(obj27 + 80);
    obj27[35] = (u8)(obj27[35] & clear);
    *(s32 *)(obj27 + 24) = 0x10000;
    *(s32 *)(obj27 + 28) = 0x10000;
    *(u16 *)(obj27 + 6) = (u16)keep;
    spr27[9] = (u8)(spr27[9] & mask);
    Func_0808a100(25, 5);

    obj27 = Func_0808a080(27);
    spr27 = *(u8 **)(obj27 + 80);
    Func_02003c48();
    *(s32 *)(obj19 + 12) = 0xc00000;                /* 192<<14 */
    *(s32 *)(obj19 + 8) = 0xd60000;                 /* 214<<16 */
    *(s32 *)(obj19 + 16) = 0x4c00000;               /* 152<<19 */
    *(s32 *)(obj19 + 56) = hidden;
    *(s32 *)(obj19 + 60) = hidden;
    *(s32 *)(obj19 + 64) = hidden;
    /* r5 stops being the -13 mask here: it becomes the merged flag byte. */
    mask = (spr27[9] & mask) | 4;
    spr27[9] = (u8)mask;

    Func_0808a0f0(27);
    Func_0808a1b8(24, 0xc000, 0);                   /* 192<<8 */
    Func_0808a1b8(25, 0xc000, 20);
    Func_080770c8(0x166);                           /* 179<<1 */
    Func_08009190(0);
    Func_08009190(1);
    Func_08009190(2);
    Func_08009190(3);
    Func_08009190(4);
    Func_08009190(5);
    Func_0808a330(0x10003, 1);
    Func_0808a330(0x10000, 2);
    Func_0808a348(120);
    Func_080000c0(160);
    Func_0808a330(0x7fff, 1);
    Func_0808a330(0x7fff, 2);
    Func_0808a348(80);
    Func_0808a010(80);
    Func_0808a010(100);
    Func_080000d8(Data_0200be19);                   /* one argument */

    *(s32 *)(spr27 + 24) = *(s32 *)(obj27 + 24);

    Func_080770d0(0x166);
    Func_08009188(0);
    Func_08009188(1);
    Func_08009188(2);
    Func_08009188(3);
    Func_08009188(4);
    Func_08009188(5);                               /* one argument */
    Func_020040f0();                                /* no arguments */

    /* Four billboards: set +0x06, then "+0x64 = query(...) + 60" with 90, then
     * a per-actor index into +0x66. */
    Func_0808a0f0(9, 0xa50000, 0x4cd0000);          /* 165<<16 */
    Func_0808a100(9, 1);
    o = Func_0808a080(9);
    hidden = 0xe000;                                /* 224<<8 */
    *(u16 *)(o + 6) = (u16)hidden;
    *(u16 *)(o + 100) = (u16)(Func_030003e0(Func_080000f8(), 90) + 60);
    *(u16 *)(o + 102) = 1;
    Func_0808a098(9, Data_0200cec8);

    Func_0808a0f0(26, 0xa50000, 0x4e60000);
    Func_0808a100(26, 1);
    o = Func_0808a080(26);
    *(u16 *)(o + 6) = (u16)hidden;
    *(u16 *)(o + 100) = (u16)(Func_030003e0(Func_080000f8(), 90) + 60);
    *(u16 *)(o + 102) = 2;
    Func_0808a098(26, Data_0200cec8);

    Func_0808a0f0(22, 0x980000, 0x5050000);         /* 152<<16 */
    Func_0808a100(22, 1);
    o = Func_0808a080(22);
    *(u16 *)(o + 6) = (u16)hidden;
    /* r0 and r3 are carried across the `b.n` that hops the third pool. */
    *(u16 *)(o + 100) = (u16)(Func_030003e0(Func_080000f8(), 90) + 60);
    *(u16 *)(o + 102) = 3;
    Func_0808a098(22, Data_0200cec8);               /* two arguments */

    Func_0808a0f0(8, 0xb40000, 0x51f0000);          /* 180<<16 */
    o = Func_0808a080(8);
    *(u16 *)(o + 6) = (u16)hidden;
    *(u16 *)(o + 100) = (u16)(Func_030003e0(Func_080000f8(), 90) + 60);
    *(u16 *)(o + 102) = 4;
    Func_0808a098(8, Data_0200cec8);
    Func_0808a100(8, 6);

    o = Func_0808a080(22);
    o[35] = (u8)(o[35] & clear);
    o = Func_0808a080(8);
    clear = o[35] & clear;
    o[35] = (u8)clear;

    Func_080000d0(Data_0200c5b9, 3200);
    Func_0808a0f0(0, 0xb50000, 0x4f90000);          /* 181<<16 */
    o = Func_0808a080(0);
    *(u16 *)(o + 6) = (u16)hidden;
    Func_0808a100(0, 1);
    Func_0808a210(0xb50000, 0, 0x4f90000, 0);
    Func_08009128();
    Func_0808a0f0(10, 0, 0);
    Func_0808a0f0(19, 0, 0);
    Func_0808a0f0(24, 0, 0);
    Func_0808a0f0(25, 0, 0);
    Func_0808a0f0(23, 0, 0);
    Func_0808a0f0(27, 0, 0);
    Func_0808a0f0(17, 0x900000, 0x42e0000);         /* 144<<16 */
    Func_0808a0f0(18, 0x1140000, 0x4f60000);        /* 138<<17 */
    Func_080000c0(60);                              /* one argument */
    Func_0808a330(0x10003, 1);
    Func_0808a330(0x10000, 2);
    Func_0808a348(80);
    Func_0808a010(60);
    Func_0808a5e8();
    Func_0808a010(60);
    Func_0808a058(1);
    Func_0808a2e0();
}
