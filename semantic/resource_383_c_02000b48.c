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
void Func_02005890();
void Func_020058da();
void Func_020058e4();
void Func_020058ee();
void Func_02005a94();
void Func_02005932();
void Func_020059b6();
void Func_02005964();
void Func_0200595a();
void Func_020059de();
s32 Func_020058ce();
void Func_02005228(s32, s32);
void Func_02005912();
void Func_0200525e(void);
void Func_020059f4();
void Func_02005b0a();
void Func_02005928();
void Func_02005232(s32, s32, s32);
void Func_020051f2(s32, s32);
void Func_020059fa();
void Func_02005a0a();
void Func_02005950();
void Func_02005274(s32, s32);
void Func_0200595e();
void Func_020052aa(void);
void Func_02005222(s32, s32);
void Func_0200525c(s32, s32, s32);
void Func_02005a70();
void Func_02005a7c();
void Func_02005992();
void Func_020052b6(s32, s32);
void Func_020059a0();
void Func_020052ec(void);
void Func_02005a6c();
void Func_0200526c(s32, s32);
void Func_02005a54();
void Func_020052c6(s32, s32, s32);
void Func_020052d0(s32, s32, s32);
void Func_02005290(s32, s32);
void Func_02005aa0();
void Func_020059e6();
void Func_02005ae2();
void Func_020059f8();
void Func_02005a7e();
void Func_02005a0c();
void Func_020052fe(s32, s32, s32);
void Func_02005b12();
void Func_02005b1e();
void Func_02005af6();
void Func_02005a3c();
void Func_02005346(s32, s32, s32);
void Func_02005b42();
void Func_02005a58();
void Func_02005b48();
void Func_02005c58();
void Func_02005b4e();
void Func_02005b5e();
s32 Func_02005abe();
void Func_02001b36(void);
void Func_02005a74();
void Func_02005b34();
u8 *Func_02005ae2_b();
void Func_02005b14();
void Func_02005b54();
void Func_02005bb6();
void Func_02005b6e();
u8 *Func_02005b1c();
void Func_02005b7c();
void Func_02005b8e();
void Func_02005cca();
void Func_02005b16();
                                /* tests a story flag */
                                /* clears / posts a story flag */

                     
                    
                                /* returns the object record, or 0 */

                     

                     

                     

                     

/* This overlay's own routines; byte-exact sources in assets/code. */

                                  

/* This overlay's routine at file offset 0x0de4 (not yet reconstructed). */

void Func_02000b48(void)
{
    u8 *anchor;

    Func_02005890();
    Func_020058da(0, 0xcccc, 0x6666);
    Func_020058e4(1, 0xcccc, 0x6666);
    Func_020058ee(2, 0xcccc, 0x6666);
    Func_02005a94(19);

    Func_02005932(0, 384, 408);         /* 192 << 1, 204 << 1 */
    Func_020059b6(0, 0xc000, 0);        /* 192 << 8 */
    Func_02005964(1, 0x1800000, 0x1980000);     /* 192 << 17, 204 << 17 */
    Func_0200595a(1, 368, 408);         /* 184 << 1 */
    Func_020059de(1, 0xd000, 20);       /* 208 << 8 */

    if (Func_020058ce(0x850) == 0) {    /* 133 << 4 */
        Func_020058e4(0x850);

        Func_02005228(2, 0);
        Func_02005912(40);
        Func_0200525e();
        Func_020059f4(0x1256);
        Func_02005b0a(60);
        Func_02005928(30);
        Func_02005232(2, 3, 30);
        Func_020051f2(2, 30);

        Func_020059fa(0, 1);
        Func_02005a0a(1, 1);
        Func_02005950(20);
        Func_02005274(2, 0);
        Func_0200595e(40);
        Func_020052aa();
        Func_02005222(2, 30);

        Func_0200525c(0, 1, 50);
        Func_02005a70(0, 0xc000, 0);
        Func_02005a7c(1, 0xd000, 0);
        Func_02005992(20);
        Func_020052b6(2, 0);
        Func_020059a0(40);
        Func_020052ec();
        Func_02005a6c(2, 1);
        Func_0200526c(2, 50);

        Func_02005a54(0, 3);
        Func_020052c6(1, 3, 20);
        Func_020052d0(2, 3, 20);
        Func_02005290(2, 40);
        Func_02005aa0(2, 1);
        Func_020059e6(30);
        Func_02005ae2(2, 0xc000, 0);
        Func_020059f8(30);
        Func_02005a7e(2, 376, 376);     /* 188 << 1 */
        Func_02005a0c(40);

        Func_020052fe(0, 1, 50);
        Func_02005b12(0, 0xc000, 0);
        Func_02005b1e(1, 0xd000, 0);
        Func_02005af6(2, 1);
        Func_02005a3c(50);
        Func_02005346(2, 3, 30);
        Func_02005b42(2, 0x4000, 0);    /* 128 << 7 */
        Func_02005a58(10);
        Func_02005b48(2, 0);
        Func_02005b48(2, 0);
    } else {
        Func_02005c58(60);
        Func_02005b4e(0x125d);
        Func_02005b5e(2, 0);
    }

    if (Func_02005abe(0, 0) == 0) {
        Func_02001b36();
        Func_02005a74(0x856);
        Func_02005b34(2, 2);
        anchor = Func_02005ae2_b(0);
        if (anchor != 0) {
            Func_02005b14(2, *(s16 *)(anchor + 10), *(s16 *)(anchor + 18));
        }
        Func_02005b42(2);
        Func_02005b54(2, 0, 0);
    } else {
        Func_02005bb6(2, 0);
    }

    Func_02005b6e(1, 2);
    anchor = Func_02005b1c(0);
    if (anchor != 0) {
        Func_02005b4e(1, *(s16 *)(anchor + 10), *(s16 *)(anchor + 18));
    }
    Func_02005b7c(1);
    Func_02005b8e(1, 0, 0);

    Func_02005cca();
    Func_02005b16();
}
