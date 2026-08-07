#include "types.h"

/*
 * resource_383 owner at 0x02000b48, 668 bytes (0x02000b48-0x02000de3):
 * code 0x02000b48-0x02000dcf and a five-word literal pool at 0x02000dd0.
 *
 * A cutscene beat played on background layers 0-2 and object 2.  It sets the
 * three layers up, then asks Func_080770c0 whether flag 0x850 is already set:
 * on the first visit it clears the flag and plays the full staged sequence
 * (about forty steps of pose, walk, wait and camera work); on a repeat visit
 * it plays only the short 0x125d line.  Either way it finishes by parking
 * objects 2 and 1 on object 0's position, read from the halfwords at +10 and
 * +18 of object 0's record.
 *
 * All 75 `bl` sites are placed and reach 27 distinct callees - the inventory
 * row's `calls=70` predates the corrected decoding.  Targets come from
 * tools/lib/overlay_call_targets.ts (target offset = stored displacement + 2),
 * never from the disassembler's `bl` annotations, which are wrong for every
 * overlay branch.  Fifty-eight reach the import veneer table at 0x02004cxx-
 * 0x02004f2b and are named by the main-image address in the veneer's trailing
 * word; seventeen reach this overlay's own Func_020045f4, Func_02004624,
 * Func_0200463c, Func_02004658, Func_02004684 (all byte-exact in assets/code)
 * and Func_02000de4.
 *
 * Epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address and
 * the owner is void.
 *
 * Uncertainty: Func_02000de4, Func_02004684 and Func_0808a4f0 are reached
 * with no argument register set up by this owner, so they are treated as
 * taking none.
 */

/* Import veneers, named by the main-image function each one reaches.
 * Old-style declarations: arities vary between call sites in this overlay. */
s32 Func_080770c0();            /* tests a story flag */
void Func_080770c8();           /* clears / posts a story flag */
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
s32 Func_0808a070();
u8 *Func_0808a080();            /* returns the object record, or 0 */
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a1b8();
void Func_0808a4f0();
void Func_080f9010();

/* This overlay's own routines; byte-exact sources in assets/code. */
void Func_020045f4(s32, s32);
void Func_02004624(s32, s32, s32);
void Func_0200463c(s32, s32, s32);
void Func_02004658(s32, s32);
void Func_02004684(void);

/* This overlay's routine at file offset 0x0de4 (not yet reconstructed). */
void Func_02000de4(void);

void Func_02000b48(void)
{
    u8 *anchor;

    Func_0808a018();
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);
    Func_080f9010(19);

    Func_0808a0d0(0, 384, 408);         /* 192 << 1, 204 << 1 */
    Func_0808a1b8(0, 0xc000, 0);        /* 192 << 8 */
    Func_0808a0f0(1, 0x1800000, 0x1980000);     /* 192 << 17, 204 << 17 */
    Func_0808a0d0(1, 368, 408);         /* 184 << 1 */
    Func_0808a1b8(1, 0xd000, 20);       /* 208 << 8 */

    if (Func_080770c0(0x850) == 0) {    /* 133 << 4 */
        Func_080770c8(0x850);

        Func_02004658(2, 0);
        Func_0808a010(40);
        Func_02004684();
        Func_0808a170(0x1256);
        Func_080f9010(60);
        Func_0808a010(30);
        Func_0200463c(2, 3, 30);
        Func_020045f4(2, 30);

        Func_0808a130(0, 1);
        Func_0808a138(1, 1);
        Func_0808a010(20);
        Func_02004658(2, 0);
        Func_0808a010(40);
        Func_02004684();
        Func_020045f4(2, 30);

        Func_02004624(0, 1, 50);
        Func_0808a1b8(0, 0xc000, 0);
        Func_0808a1b8(1, 0xd000, 0);
        Func_0808a010(20);
        Func_02004658(2, 0);
        Func_0808a010(40);
        Func_02004684();
        Func_0808a138(2, 1);
        Func_020045f4(2, 50);

        Func_0808a100(0, 3);
        Func_0200463c(1, 3, 20);
        Func_0200463c(2, 3, 20);
        Func_020045f4(2, 40);
        Func_0808a138(2, 1);
        Func_0808a010(30);
        Func_0808a1b8(2, 0xc000, 0);
        Func_0808a010(30);
        Func_0808a0d0(2, 376, 376);     /* 188 << 1 */
        Func_0808a010(40);

        Func_02004624(0, 1, 50);
        Func_0808a1b8(0, 0xc000, 0);
        Func_0808a1b8(1, 0xd000, 0);
        Func_0808a138(2, 1);
        Func_0808a010(50);
        Func_0200463c(2, 3, 30);
        Func_0808a1b8(2, 0x4000, 0);    /* 128 << 7 */
        Func_0808a010(10);
        Func_0808a180(2, 0);
        Func_0808a178(2, 0);
    } else {
        Func_080f9010(60);
        Func_0808a170(0x125d);
        Func_0808a178(2, 0);
    }

    if (Func_0808a070(0, 0) == 0) {
        Func_02000de4();
        Func_080770c8(0x856);
        Func_0808a100(2, 2);
        anchor = Func_0808a080(0);
        if (anchor != 0) {
            Func_0808a0b8(2, *(s16 *)(anchor + 10), *(s16 *)(anchor + 18));
        }
        Func_0808a0e8(2);
        Func_0808a0f0(2, 0, 0);
    } else {
        Func_0808a180(2, 0);
    }

    Func_0808a100(1, 2);
    anchor = Func_0808a080(0);
    if (anchor != 0) {
        Func_0808a0b8(1, *(s16 *)(anchor + 10), *(s16 *)(anchor + 18));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_0808a4f0();
    Func_0808a020();
}
