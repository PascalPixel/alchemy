typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_383 owner at 0x020019e4, 104 bytes (0x020019e4-0x02001a4b):
 * code 0x020019e4-0x02001a3f and a three-word literal pool at 0x02001a40
 * (0x00000854, 0x000012c3, 0x03001ebc).
 *
 * The same cutscene beat as 0x020019a4 with a one-shot guard in front of it:
 * scene flag 0x854 is tested through Func_080770c0 and, only while it is
 * clear, an extra four-call introduction runs.  The flag is never set here, so
 * some later beat owns the set; that is recorded rather than assumed.
 *
 * `bl` targets resolved with tools/overlay_call_targets.ts (target offset =
 * stored displacement + 2); all nine sites land in the import veneer table at
 * 0x02004cxx-0x02004f2b and are named by the main-image address the veneer
 * holds.
 *
 * Epilogue is `pop {r0} / bx r0`: void.
 */

/* Import veneers, named by the main-image function each one reaches. */
s32 Func_080770c0();            /* scene-flag test, used in a condition */
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0d0();
void Func_0808a170();
void Func_0808a180();
void Func_0808a248();
void Func_080f9010();

void Func_020019e4(void)
{
    u8 *state;

    /* 128 << 8 and 128 << 7. */
    Func_0808a090(0, 0x8000, 0x4000);
    /* 186 << 2 and 204 << 1. */
    Func_0808a0d0(0, 744, 408);

    if (Func_080770c0(0x854) == 0) {
        /* No argument register is written before this branch: the incoming
         * r0-r3 reach the import unchanged. */
        Func_0808a018();
        Func_0808a170(0x12c3);
        Func_0808a180(8, 0);
        Func_0808a020();
    }

    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 456) = 16;

    Func_080f9010(123);
    Func_0808a248(14);
}
