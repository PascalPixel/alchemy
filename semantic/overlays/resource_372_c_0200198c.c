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
 * Note `Func_0808a170` is reached twice with different argument counts (once
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
void Func_0808a018();
void Func_0808a0f0();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a150();
void Func_0808a010();
void Func_0808a170();
void Func_0808a138();
void Func_0808a180();
void Func_0808a148();
void Func_0808a1b8();
void Func_0808a100();
void Func_0808a0b8();
void Func_0808a0e8();
void Func_0808a020();

/* Used for their return values: both yield a descriptor pointer or null. */
s32 *Func_0808a080();

void Func_0200198c(void)
{
    s32 *desc;
    s16 *pos;

    Func_0808a018();

    desc = Func_0808a080(0);
    if (desc != 0) {
        /* word fields at +8 and +16 of the descriptor */
        Func_0808a0f0(22, desc[2], desc[4]);
    }

    Func_0808a090(22, 0x10000, 0x8000);   /* 128 << 9, 128 << 8 */
    Func_0808a0d0(22, 0x119, 0x1fb);
    Func_0808a150(22, 0, 0);
    Func_0808a010(30);
    Func_0808a170(0xe7b);
    Func_0808a180(22, 0);
    Func_0808a148(0, 22, 0);
    Func_0808a010(10);
    Func_0808a138(0, 1);                  /* same name, two arguments here */
    Func_0808a010(20);
    Func_0808a1b8(22, 0x4000, 0);         /* 128 << 7 */
    Func_0808a180(22, 0);
    Func_0808a100(22, 2);

    pos = Func_0808a080(0);
    if (pos != 0) {
        /* signed halfword fields at +10 and +18, as in 0x0200173c */
        Func_0808a0b8(22, pos[5], pos[9]);
    }

    Func_0808a0e8(22);
    Func_0808a0f0(22, 0, 0);
    Func_0808a0d0(0, 0x100, 0x205);       /* 128 << 1 */
    Func_0808a020();
}
