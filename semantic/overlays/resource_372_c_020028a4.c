typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 372 scene setup at 0x020028a4 (1156 bytes, 109 distinct call
 * targets across 120 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x020028a4, matching
 * `pop {r5} / pop {r0} / bx r0` at 0x02002cb8.  `pop {r0} ; bx r0` — r0 is the
 * popped return address, so the owner returns nothing.
 * 0x02002cbe-0x02002d27 is alignment plus the literal pool, reached only by
 * `ldr rN, [pc, #imm]`.
 *
 * `out/decomp/overlays.json` seeds three further rows inside this span
 * (0x020028d8, 0x020028ec, 0x02002abc), all marked `contained_by` this entry.
 * None of them is a function: each is a plain `movs` in the middle of an
 * argument block with this owner's `push {r5, lr}` already executed.  They are
 * artefacts of the encoded-`bl` reading discussed below, and the owner is
 * reconstructed whole from its own prologue to its own epilogue.
 *
 * Structure: build handles 9, 10, 22, 24, 25, 26; set three actors' frame
 * counters from a queried value; then two nested two-way script branches, and
 * the usual optional position fix-up through `*(s16 *)(p + 10)` /
 * `*(s16 *)(p + 18)`.
 *
 * Three field idioms here are proven by byte-exact siblings, so they are not
 * re-derived: `*(u16 *)(obj + 100) = query(...) + 60` with the constant 90
 * (0x5a) as the second argument is exactly the `+0x64` / `0x5A` / `+60`
 * sequence of `assets/code/resource_372_c_02000434.c`; `Data_0200cec8` is the
 * table that file installs into the same kind of object; and the two-field
 * descriptor copy `place(handle, p->f08, p->f10)` is from
 * `assets/code/resource_372_c_0200173c.c`.
 *
 * Message identifiers are carried in r5 and advanced in place: 0xe9b (+4, +5)
 * for the first branch and 0xea1 (+1, +2) for the second.
 *
 * Several imports are reached with different argument counts at different
 * sites: 0x0200714e (four then two), 0x02007124 (one at both, different
 * handles), 0x02007176, 0x02007320, 0x02007328, 0x0200732e, 0x0200734a,
 * 0x02007356, 0x0200737a and 0x020073a4.  Old-style declarations are therefore
 * mandatory.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes; that address is an import identity for this overlay, not a place to
 * disassemble.
 *
 * Uncertainties: handles, message ids and the 16.16-looking constants are
 * unresolved; the two branch predicates 0x020072aa(0, 0) and 0x02007304(0, 0)
 * are tested against zero exactly as written.
 */

/* Imports, old-style: interfaces unknown, several with varying arity. */
void Func_0808a1b8();
void Func_0808a100();
void Func_0808a110();
void Func_0808a208();
void Func_0808a210();
void Func_0808a090();
void Func_0808a098();
void Func_0808a0b0();
void Func_080f9010();
void Func_08009178();
void Func_0808a0d0();
void Func_0808a0f0();
void Func_0808a5e8();
u8 *Func_0808a080();
s32 Func_080000f8();
s32 Func_030003e0();
void Func_0808a010();
void Func_0808a0a0();
void Func_0808a138();
void Func_0808a170();
void Func_0808a188();
void Func_0808a1e8();
void Func_0808a178();
s32 Func_0808a070();
void Func_0808a180();
void Func_0808a150();
void Func_0808a0b8();
void Func_0808a0e8();

/* In-image data blocks. */
extern u8 Data_0200cab4[];
extern u8 Data_0200ca78[];
extern u8 Data_0200d7a0[];
extern u8 Data_0200cec8[];
extern u8 Data_0200d7e2[];
extern u8 Data_0200d78a[];
extern u8 Data_0200cb28[];
extern u8 Data_0200cb9c[];
extern u8 Data_0200d7cc[];
extern u8 Data_0200cc0c[];
extern u8 Data_0200cc5c[];
extern u8 Data_0200cca8[];

void Func_020028a4(void)
{
    u8 *obj;
    s16 *pos;
    s32 msg;

    Func_0808a1b8(26, 0x3000, 0);      /* 192 << 6 */
    Func_0808a1b8(24, 0xd000, 0);      /* 208 << 8 */
    Func_0808a1b8(25, 0xb000, 0);      /* 176 << 8 */
    Func_0808a1b8(9, 0x3000, 0);
    Func_0808a1b8(10, 0xd000, 20);
    Func_0808a100(26, 3);
    Func_0808a100(24, 3);
    Func_0808a100(25, 3);
    Func_0808a100(9, 3);
    Func_0808a110(25, 3);
    Func_0808a010(20);
    Func_0808a208(0x10000, 0x2000);    /* 128<<9, 128<<6 */
    Func_0808a210(0x860000, -1, 0x4ab0000, 1);   /* 134 << 16 */
    Func_0808a090(26, 0x19999, 0xcccc);
    Func_0808a090(9, 0x19999, 0xcccc);
    Func_0808a098(26, Data_0200cab4);
    Func_0808a0b0(9, Data_0200ca78);
    Func_080f9010(158);
    Func_08009178(Data_0200d7a0, 38, 72);
    Func_0808a010(10);
    Func_0808a0d0(9, 149, 0x497);
    Func_0808a0f0(9, 0, 0);
    Func_0808a0d0(25, 250, 0x4be);
    Func_0808a5e8();
    Func_0808a1b8(10, 0x3000, 0);
    Func_0808a1b8(24, 0x3000, 0);
    Func_0808a1b8(25, 0x3000, 0);
    Func_0808a100(10, 5);
    Func_0808a100(24, 6);              /* two arguments here */
    Func_0808a100(25, 6);

    /* Three copies of the byte-exact "+0x64 = query(...) + 60" idiom. */
    obj = Func_0808a080(10);
    *(u16 *)(obj + 100) = (u16)(Func_030003e0(Func_080000f8(), 90) + 60);
    obj = Func_0808a080(24);
    *(u16 *)(obj + 100) = (u16)(Func_030003e0(Func_080000f8(), 90) + 60);
    obj = Func_0808a080(25);
    *(u16 *)(obj + 100) = (u16)(Func_030003e0(Func_080000f8(), 90) + 60);

    Func_0808a098(10, Data_0200cec8);
    Func_0808a098(24, Data_0200cec8);
    Func_0808a098(25, Data_0200cec8);
    Func_0808a0a0(26);
    Func_0808a010(10);                 /* one argument here */
    Func_080f9010(159);
    Func_08009178(Data_0200d7e2, 38, 72);
    Func_0808a010(30);
    Func_0808a5e8();
    Func_0808a210(0x700000, -1, 0x4c90000, 1);   /* 224 << 15 */
    Func_080f9010(158);
    Func_08009178(Data_0200d78a, 35, 73);
    Func_0808a010(20);
    Func_0808a5e8();
    Func_0808a098(9, Data_0200cb28);
    Func_0808a010(20);
    Func_0808a098(26, Data_0200cb9c);
    Func_0808a010(40);
    Func_080f9010(159);
    Func_08009178(Data_0200d7cc, 35, 73);
    Func_0808a0a0(26);
    Func_0808a5e8();
    Func_0808a010(40);

    msg = 0xe9b;
    Func_0808a170(msg);
    Func_0808a188(9, 0, 20);
    Func_0808a110(26, 3);
    Func_0808a188(0x201a, 0, 40);
    Func_0808a100(9, 3);
    Func_0808a110(26, 3);
    Func_0808a010(30);
    Func_0808a098(9, Data_0200cc0c);
    Func_0808a098(26, Data_0200cc5c);
    Func_0808a010(40);
    Func_0808a208(0x20000, 0x4000);              /* 128<<10, 128<<7 */
    Func_0808a210(0x690000, -1, 0x43e0000, 1);   /* 210 << 15 */
    Func_0808a0a0(9);
    Func_0808a1b8(9, 0, 0);                      /* three arguments here */
    Func_0808a1e8(9, 0x100, 40);                 /* 128 << 1 */
    Func_0808a188(9, 0, 10);
    Func_0808a1b8(0, 0x8000, 0);                 /* 128 << 8 */
    Func_0808a1b8(22, 0x8000, 10);
    Func_0808a0d0(9, 105, 0x43e);
    Func_0808a138(9, 2);
    Func_0808a178(0x8009, 0);
    Func_0808a1b8(22, 0, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a110(9, 3);                     /* two arguments here */
        Func_0808a170(msg + 4);
    } else {
        Func_0808a138(9, 2);
        Func_0808a170(msg + 5);
    }

    Func_0808a180(0x8009, 0);
    Func_0808a1b8(22, 0x8000, 40);
    Func_0808a1e8(9, 0x100, 30);

    msg = 0xea1;
    Func_0808a170(msg);
    Func_0808a178(0x8009, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a110(9, 3);
        Func_0808a170(msg + 1);
        Func_0808a188(0x8009, 0, 30);
        Func_0808a1b8(22, 0x8000, 20);
        Func_0808a100(0, 3);                     /* two arguments here */
        Func_0808a100(22, 3);
        Func_0808a110(9, 3);
        Func_0808a010(40);
    } else {
        Func_0808a1e8(9, 0x105, 90);
        Func_0808a1e8(9, 0x103, 40);
        Func_0808a100(9, 4);
        Func_0808a170(msg + 2);
        Func_0808a180(0x8009, 0);
    }

    Func_0808a098(9, Data_0200cca8);
    Func_0808a010(90);
    Func_0808a150(0, 22, 0);
    Func_0808a010(40);
    Func_0808a100(0, 3);
    Func_0808a110(22, 3);
    Func_0808a010(20);                           /* one argument here */
    Func_0808a100(22, 2);

    pos = Func_0808a080(0);
    if (pos != 0) {
        Func_0808a0b8(22, pos[5], pos[9]);       /* +10 and +18, signed */
    }

    Func_0808a0e8(22);
    Func_0808a0f0(22, 0, 0);
}
