typedef signed short s16;
typedef signed int s32;

/*
 * Resource 372 cutscene script step at 0x0200198c (216 bytes, 21 distinct
 * call targets across 22 call sites).
 *
 * Complete owner: `push {lr}` at 0x0200198c, matching `pop {r0} / bx r0` at
 * 0x02001a4e.  `pop {r0} ; bx r0` — r0 is the popped return address, so the
 * owner returns nothing.  0x02001a52-0x02001a63 is alignment plus the literal
 * pool (0x119, 0x1fb, 0xe7b, 0x205); it is only reached by `ldr rN, [pc,#imm]`.
 *
 * The body is the same actor-script shape as the byte-exact sibling
 * `assets/code/resource_372_c_0200173c.c`: query a descriptor, then drive
 * actor 22 through position/rotation/animation helpers, guarded twice by a
 * null descriptor test.  The two structure reads reproduce that file's proven
 * field offsets — `*(s16 *)(p + 10)` and `*(s16 *)(p + 18)` from the
 * `ldrsh rN, [r0, r3]` pair with r3 = 10 and r3 = 18.
 *
 * Note `Func_020061a6` is reached twice with different argument counts (once
 * with r0 only, once with r0 and r1).  That is the documented overlay idiom,
 * and it is why every import here carries an old-style declaration.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes.  For this overlay that address is a stable identity for an import,
 * not a place to disassemble.
 *
 * Uncertainty: the first call, 0x0200608c, is reached with no argument
 * register written since the prologue, so it is asserted as taking none.
 */

/* Imports, old-style: interfaces unknown; 0x020061a6 takes one argument at
 * one site and two at another. */
void Func_0200608c();
void Func_02006130();
void Func_020060ee();
void Func_02006138();
void Func_0200618a();
void Func_020060c0();
void Func_020061a6();
void Func_020061be();
void Func_020061a0();
void Func_020060de();
void Func_020060ec();
void Func_02006200();
void Func_020061f0();
void Func_020061a8();
void Func_02006190();
void Func_020061b6();
void Func_020061c8();
void Func_020061c4();
void Func_02006150();

/* Used for their return values: both yield a descriptor pointer or null. */
s32 *Func_020060c2();
s16 *Func_02006146();

void Func_0200198c(void)
{
    s32 *desc;
    s16 *pos;

    Func_0200608c();

    desc = Func_020060c2(0);
    if (desc != 0) {
        /* word fields at +8 and +16 of the descriptor */
        Func_02006130(22, desc[2], desc[4]);
    }

    Func_020060ee(22, 0x10000, 0x8000);   /* 128 << 9, 128 << 8 */
    Func_02006138(22, 0x119, 0x1fb);
    Func_0200618a(22, 0, 0);
    Func_020060c0(30);
    Func_020061a6(0xe7b);
    Func_020061be(22, 0);
    Func_020061a0(0, 22, 0);
    Func_020060de(10);
    Func_020061a6(0, 1);                  /* same name, two arguments here */
    Func_020060ec(20);
    Func_02006200(22, 0x4000, 0);         /* 128 << 7 */
    Func_020061f0(22, 0);
    Func_020061a8(22, 2);

    pos = Func_02006146(0);
    if (pos != 0) {
        /* signed halfword fields at +10 and +18, as in 0x0200173c */
        Func_02006190(22, pos[5], pos[9]);
    }

    Func_020061b6(22);
    Func_020061c8(22, 0, 0);
    Func_020061c4(0, 0x100, 0x205);       /* 128 << 1 */
    Func_02006150();
}
