#include "types.h"

/*
 * resource_393 owner at 0x02000bf8, 148 bytes: the scene's phase-516 step.
 * Publish the phase id, run beat 10, and - only the first time through, while
 * event flag 0x0201 is still set - dress the screen for it: mark the subject,
 * repaint two tile rectangles and put the subject into presentation phase 0.
 * Then run beats 8 and 9, and unless event flag 0x0845 has been set, run the
 * 0x02000c8c follow-up with selector 6.
 *
 * Complete owner: 'push {r5, lr}', an 8-byte outgoing-argument frame for the
 * two six-argument calls, and the single interworking epilogue at 0x02000c78.
 * Control-flow walk: straight line with two forward 'b' diamonds, both landing
 * before 0x02000c80, so the 12-byte pool at 0x02000c80 (0x03001ebc, 0x201,
 * 0x845) is code-unreachable.  136 code + 12 pool = 148, the advertised span.
 *
 * Return type from the epilogue rule: 'pop {r1} ; bx r1' with N != 0, so r0
 * survives - the owner returns the constant 0.
 *
 * Calls: 11 sites over 7 targets, from
 * 'bun tools/lib/overlay_call_targets.ts resource_393 0bf8'.  Three of them are
 * intra-overlay prologues (0x020008c0 x3, 0x02000244, 0x02000c8c); the
 * remaining six are veneers.
 *
 * The workspace store is the documented additive displacement/value trap:
 * 'movs r2,#224 / lsls #1' makes 448 the DISPLACEMENT, and the following
 * 'adds r2,#68' makes 516 the stored VALUE.  Reading it as workspace + 516 is
 * the natural mistake.  Slot +448 is the established s32 scene/phase id.
 */

/* Pointer CELL holding the scene workspace - one dereference, not two. */
#define SCENE393_WORKSPACE (*(u8 **)0x03001ebc)

struct Beat393Subject {
    u8 unknown_00[0x23];
    u8 marker;                      /* 0x23 */
};

void Func_020014ce();
s32 Func_02001a70();
struct Beat393Subject *Func_02001aa2();
void Func_02001a7c();
s32 Func_02000e90();
struct Beat393Subject *Func_02001ad6();
void Func_02001aac();
void Func_0200151e();
void Func_02001524();
s32 Func_02001ac6();
void Func_02001900();
                                    /* run a numbered beat of this scene */
                                    /* read an event flag */
                                          /* subject record by id */
                                    /* six-argument renderer, last two on the stack */
                                    /* set presentation mode (record, phase) */
                                    /* fill a tile rectangle's attribute byte */
                                    /* the follow-up sequence */

s32 Func_02000bf8(void)
{
    u8 *workspace = SCENE393_WORKSPACE;

    *(s32 *)(workspace + 448) = 516;

    Func_020014ce(10);

    if (Func_02001a70(0x201) != 0) {
        struct Beat393Subject *subject = Func_02001aa2(10);

        subject->marker = 2;
        Func_02001a7c(32, 20, 2, 4, 11, 16);
        Func_02000e90(2, 12, 16, 1, 4, 0);
        Func_02001aac(Func_02001ad6(10), 0);
    }

    Func_0200151e(8);
    Func_02001524(9);

    if (Func_02001ac6(0x845) == 0) {
        Func_02001900(6);
    }
    return 0;
}
