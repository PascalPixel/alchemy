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
 * 'bun tools/overlay_call_targets.ts resource_393 0bf8'.  Three of them are
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

void Func_020008c0();               /* run a numbered beat of this scene */
s32 Func_080770c0();                /* read an event flag */
struct Beat393Subject *Func_0808a080();   /* subject record by id */
void Func_080091c0();               /* six-argument renderer, last two on the stack */
void Func_080091e0();               /* set presentation mode (record, phase) */
s32 Func_02000244();                /* fill a tile rectangle's attribute byte */
void Func_02000c8c();               /* the follow-up sequence */

s32 Func_02000bf8(void)
{
    u8 *workspace = SCENE393_WORKSPACE;

    *(s32 *)(workspace + 448) = 516;

    Func_020008c0(10);

    if (Func_080770c0(0x201) != 0) {
        struct Beat393Subject *subject = Func_0808a080(10);

        subject->marker = 2;
        { s32 f1 = 11; s32 g1 = 16; Func_080091c0(32, 20, 2, 4,  f1, g1); }
        { s32 f2 = 4; s32 g2 = 0; Func_02000244(2, 12, 16, 1,  f2, g2); }
        Func_080091e0(Func_0808a080(10), 0);
    }

    Func_020008c0(8);
    Func_020008c0(9);

    if (Func_080770c0(0x845) == 0) {
        Func_02000c8c(6);
    }
    return 0;
}
