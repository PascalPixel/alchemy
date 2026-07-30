typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_383 owner at 0x0200091c, 248 bytes (0x0200091c-0x02000a13):
 * code 0x0200091c-0x020009ff and a five-word literal pool at 0x02000a00
 * (0x0000085b, 0x0000137c, 0x00001385, 0x00001384, 0x03001ebc).
 *
 * A conversation beat.  Scene flag 0x85b selects which of two lines is spoken;
 * an availability query then splits into "run the exchange" and "it has
 * already been seen" arms, and the exchange's own follow-up query decides
 * between a normal close and the one-shot close that sets flag 0x85b.  The
 * already-seen arm is the one that bumps the workspace halfword at +472, the
 * same counter the byte-exact sources in this overlay bump
 * (assets/code/resource_383_c_020003bc.c, ..._0200082c.c).
 *
 * All 27 `bl` sites are placed; they reach 16 distinct callees.  Targets are
 * resolved with tools/overlay_call_targets.ts (target offset = stored
 * displacement + 2); 26 land in the import veneer table at
 * 0x02004cxx-0x02004f2b and are named by the main-image address in the
 * veneer's trailing word.  The remaining site reaches this overlay's own
 * 0x02004624, whose byte-exact source is in assets/code and which takes
 * exactly the three arguments set here.  (overlay_call_targets.ts reports that
 * one as `unknown` only because 0x4624 is not carried in the inventory's
 * prologue set; the target is a genuine, already-converted function.)
 *
 * Epilogue is `pop {r0} / bx r0`, so the owner is void.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay.
 * The three used in conditions must still declare a return type. */
s32 Func_080770c0();            /* scene-flag test */
void Func_080770c8();           /* scene-flag set */
s32 Func_08077250();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a060();
s32 Func_0808a070();
void Func_0808a110();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a398();

/* This overlay's own routine at file offset 0x4624 (byte-exact source in
 * assets/code/resource_383_c_02004624.c). */
void Func_02004624(s32, s32, s32);

void Func_0200091c(void)
{
    u8 *state;

    /* No argument register is written before this branch: the incoming r0-r3
     * reach the import unchanged. */
    Func_0808a018();
    Func_02004624(18, 0, 2);

    if (Func_080770c0(0x85b) == 0) {
        Func_0808a170(0x137c);
        Func_0808a178(18, 0);
    } else {
        Func_0808a170(0x1385);
        Func_0808a178(18, 0);
    }

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a010(20);
        Func_0808a180(18, 0);
        Func_0808a010(20);
        Func_0808a138(18, 2);
        Func_0808a010(20);

        /* No argument register is written before this branch either; r0-r2
         * still hold Func_0808a010's call-clobbered values, so no arguments
         * are asserted. */
        if (Func_08077250() == 0) {
            Func_0808a110(18, 4);
            Func_0808a010(20);
            Func_0808a170(0x1384);
            Func_0808a180(18, 0);
        } else {
            Func_0808a398(231, 3);
            Func_0808a060(231, 0);
            Func_080770c8(0x85b);
        }
    } else {
        /* 236 << 1 = 472. */
        state = *(u8 **)0x03001ebc;
        *(u16 *)(state + 472) += 1;

        Func_0808a010(20);
        Func_0808a110(18, 3);
        Func_0808a010(20);
        Func_0808a180(18, 0);
    }

    Func_0808a1b8(18, 0x4000, 0);       /* 128 << 7 */
    Func_0808a020();
}
