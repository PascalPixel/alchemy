typedef signed short s16;
typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_383 owner at 0x02004b2c, 320 bytes (0x02004b2c-0x02004c6b):
 * code 0x02004b2c-0x02004c47, an eight-word literal pool at 0x02004c48 that
 * the body branches over, and the epilogue at 0x02004c68.
 *
 * The overlay's scene-open routine: it resets the display, programs the three
 * background layers (scroll, scale, blend and priority), stages the two actors
 * through Func_02001e80, publishes zero into the in-image beat/state halfword
 * at 0x0200e4f8, installs the per-frame handler Func_020048c8 and starts the
 * music.
 *
 * All 26 `bl` sites are placed and reach 15 distinct callees.  Targets come
 * from tools/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's `bl` annotations, which are wrong for every
 * overlay branch.  Twenty-five reach the import veneer table at 0x02004cxx-
 * 0x02004f2b and are named by the main-image address in the veneer's trailing
 * word; one reaches this overlay's own prologue at file offset 0x1e80, whose
 * semantic source is already in the tree.
 *
 * Link-base witness: the pool word 0x0200c8c9 handed to Func_080000d0 is
 * file offset 0x48c8 plus the Thumb bit, i.e. this overlay's own function
 * Func_020048c8, and 0x0200e4f8 is the in-image halfword at file offset
 * 0x64f8.  Both confirm the 0x02008000 link base for resource_383.
 *
 * Epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address and
 * the owner is void.
 *
 * Uncertainty: the literal arguments are written as the code builds them
 * (`0xa8 << 16`, `0xae << 18`, and so on).  They are 16.16 fixed-point screen
 * quantities in the same family the byte-exact and semantic sources in this
 * overlay already use for Func_0808a090, but the individual field meanings of
 * Func_0808a210 and Func_0808a230 are not established anywhere in the tree.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
void Func_080000d0();
void Func_080770d0();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a1b8();
void Func_0808a210();
void Func_0808a230();
void Func_080f9010();

/* This overlay's actor-staging step (semantic source already in the tree). */
void Func_02001e80(void);

/* This overlay's per-frame handler, installed below with its Thumb bit set. */
void Func_020048c8(void);

/* In-image halfword at file offset 0x64f8: the beat counter this scene's
 * steppers read back. */
extern u16 Data_0200e4f8;

void Func_02004b2c(void)
{
    Func_0808a018();

    /* -1 in the second argument; 0xa8 << 16 and 0xa4 << 18. */
    Func_0808a210(0xa80000, -1, 0x2900000, 1);

    /* The same 0xcccc / 0x6666 pair for all three layers. */
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);

    Func_0808a0d0(0, 248, 696);                 /* 174 << 2 */
    Func_0808a0f0(1, 0xf80000, 0x2b80000);      /* 248 << 16, 174 << 18 */
    Func_0808a0f0(2, 0xf80000, 0x2b80000);

    Func_0808a0c8(0, 200, 696);
    Func_0808a0c8(1, 248, 712);                 /* 178 << 2 */
    Func_0808a0d0(2, 232, 696);

    Func_0808a0e8(1);
    Func_0808a1b8(1, 0xc000, 0);                /* 192 << 8 */
    Func_0808a1b8(2, 0x8000, 0);                /* 128 << 8 */
    Func_0808a0e8(0);
    Func_0808a100(1, 12);

    Func_02001e80();

    /* 192 << 14, 144 << 18, 144 << 17, 184 << 18. */
    Func_0808a230(0x300000, 0x2400000, 0x1200000, 0x2e00000);

    Func_0808a090(1, 0x10000, 0x8000);          /* 128 << 9, 128 << 8 */
    Func_0808a090(2, 0xc000, 0x6000);           /* 192 << 8, 192 << 7 */
    Func_0808a090(24, 0x10000, 0x13333);
    Func_0808a090(25, 0x18000, 0x18000);        /* 192 << 9 twice */

    Data_0200e4f8 = 0;
    Func_080000d0((void (*)(void))((s32)&Func_020048c8 | 1), 0x0c94, 0);

    Func_080770d0(0x1ff);
    Func_0808a020();
    Func_080f9010(9);
}
