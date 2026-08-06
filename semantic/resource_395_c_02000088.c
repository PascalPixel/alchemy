#include "types.h"

/*
 * Resource 395 scripted scene at 0x02000088 (208 bytes,
 * 0x02000088-0x02000157).
 *
 * Complete owner: `push {lr}` prologue at 0x02000088 and the matching
 * `pop {r0} / bx r0` interworking return at 0x02000140.  The popped return
 * address lands in r0, so nothing is returned and the owner is `void`.
 * 0x02000144-0x02000157 is this owner's five-word literal pool, after the
 * epilogue and never reached as code; all five words are small ids (two story
 * flags 0x845/0x84c and three dialogue ids 0x151d/0x1525/0x1520), not
 * addresses.
 *
 * All 23 call sites were resolved with tools/overlay_call_targets.ts
 * (true_target_offset = stored_displacement + 2), never from overlay_show.ts's
 * `bl` annotations, which are off by one veneer entry on this overlay.  Placed
 * multiset, matching the 23 sites exactly: Func_020012f4 x6, Func_0808a188 x4,
 * Func_0808a010 x3, Func_0808a170 x2, Func_0808a180 x2, Func_080770c0 x2, and
 * one each of Func_0808a018, Func_0808a110, Func_080770c8, Func_0808a020.
 * The inventory reports calls=23, which agrees here.
 *
 * ONE CALL SITE IS SHARED between two arms with different arguments: the
 * Func_0808a170 at 0x020000b0 is reached with 0x151d from the flag-0x845 arm
 * (which `b.n`s to it) and with 0x1525 by falling through the flag-0x84c test.
 * It is spelled once through the `alreadySeen` join below rather than
 * duplicated per arm, so the call multiset stays exact.  The Func_020012f4 /
 * Func_0808a020 pair at 0x02000134 is likewise a shared tail reached from both
 * the short and the long arm.
 *
 * Behaviour: open a scripted scene, put local channel 11 into state 1, and
 * pick one of three lines.  If either progress flag is already set, say the
 * short line and leave.  Otherwise play the full sequence and post flag 0x84c
 * so the next visit takes the short path.
 *
 * Uncertainty: Func_0808a188 is Func_0808a180 with a third argument (a
 * duration in the 10/20 range here); the exact relationship between the two is
 * not established, so they are declared separately and left open.
 */

           /* scripted-scene bracket: open */
           /* scripted-scene bracket: close */
           /* wait n frames */
            /* story-flag test; used in a condition */
           /* set a story flag */
           /* show a dialogue line by id */
           /* wait for the slot's action to finish */
           /* as Func_0808a180, with a duration */
           /* scene-presentation request */
           /* overlay-local channel state (channel, state) */

extern void Func_02001a24();
extern void Func_02001388();
extern s32 Func_02001a1a();
extern s32 Func_02001a28();
extern void Func_02001ad2();
extern void Func_02001aea();
extern void Func_02001ae2();
extern void Func_02001b04();
extern void Func_020013c8();
extern void Func_02001a6a();
extern void Func_020013d6();
extern void Func_02001b24();
extern void Func_02001aec();
extern void Func_02001a8a();
extern void Func_02001b32();
extern void Func_020013fe();
extern void Func_02001aa0();
extern void Func_02001b52();
extern void Func_02001416();
extern void Func_02001b64();
extern void Func_02001aba();
extern void Func_0200142e();
extern void Func_02001ade();
void Func_02000088(void)
{
    s32 line;

    Func_02001a24();
    Func_02001388(11, 1);

    if (Func_02001a1a(0x845) != 0) {
        line = 0x151d;
        goto alreadySeen;
    }
    if (Func_02001a28(0x84c) != 0) {
        line = 0x1525;
        goto alreadySeen;
    }

    goto fullScene;

    goto close;
alreadySeen:
    Func_02001ad2(line);
    Func_02001aea(9, 0);

fullScene:
    Func_02001ae2(0x1520);
    Func_02001b04(9, 0, 20);

    Func_020013c8(11, 0);
    Func_02001a6a(60);
    Func_020013d6(11, 1);
    Func_02001b24(9, 0, 10);

    Func_02001aec(0, 3);
    Func_02001a8a(40);
    Func_02001b32(9, 0);

    Func_020013fe(11, 0);
    Func_02001aa0(80);
    Func_02001b52(9, 0, 20);

    Func_02001416(11, 1);
    Func_02001b64(9, 0, 20);

    Func_02001aba(0x84c);
    goto close;

close:
    Func_0200142e(11, 0);
    Func_02001ade();
}
