typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_383 owner at 0x020019a4, 64 bytes (0x020019a4-0x020019e3):
 * code 0x020019a4-0x020019df and a one-word literal pool at 0x020019e0
 * (0x03001ebc, the overlay workspace pointer).
 *
 * A cutscene beat: place the camera/actor pair, publish a workspace word and
 * hand off to the next step.  The neighbouring byte-exact source
 * assets/code/resource_383_c_02001990.c is the same shape two rows earlier.
 *
 * `bl` targets are resolved with tools/overlay_call_targets.ts, i.e. by
 * `target_offset = stored_displacement + 2`, and every one of the four sites
 * lands in this overlay's import veneer table (0x02004cxx-0x02004f2b, eight
 * bytes per entry: `ldr r4,[pc,#0] / bx r4 / .word <main-image address>`), so
 * the callees are named by the main-image address the veneer holds.  The
 * disassembler's own `bl` annotations are wrong here and are not used.
 *
 * Epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address and
 * the owner is void.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: these imports take different argument counts at
 * different call sites in this overlay. */
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a248();
void Func_080f9010();

void Func_020019a4(void)
{
    u8 *state;

    /* 128 << 8 and 128 << 7. */
    Func_0808a090(0, 0x8000, 0x4000);
    /* 182 << 2 and 204 << 1. */
    Func_0808a0c8(0, 728, 408);

    /* Workspace word at +456 (228 << 1); the same block whose +472 halfword
     * the byte-exact sources in this overlay bump. */
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 456) = 16;

    Func_080f9010(123);
    Func_0808a248(15);
}
