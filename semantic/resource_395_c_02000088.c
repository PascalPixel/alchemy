typedef signed int s32;

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

void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
void Func_0808a010();           /* wait n frames */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_080770c8();           /* set a story flag */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */
void Func_0808a188();           /* as Func_0808a180, with a duration */
void Func_0808a110();           /* scene-presentation request */
void Func_020012f4();           /* overlay-local channel state (channel, state) */

void Func_02000088(void)
{
    s32 line;

    Func_0808a018();
    Func_020012f4(11, 1);

    if (Func_080770c0(0x845) != 0) {
        line = 0x151d;
        goto alreadySeen;
    }
    if (Func_080770c0(0x84c) != 0) {
        line = 0x1525;
        goto alreadySeen;
    }

    goto fullScene;

alreadySeen:
    Func_0808a170(line);
    Func_0808a180(9, 0);
    goto close;

fullScene:
    Func_0808a170(0x1520);
    Func_0808a188(9, 0, 20);

    Func_020012f4(11, 0);
    Func_0808a010(60);
    Func_020012f4(11, 1);
    Func_0808a188(9, 0, 10);

    Func_0808a110(0, 3);
    Func_0808a010(40);
    Func_0808a180(9, 0);

    Func_020012f4(11, 0);
    Func_0808a010(80);
    Func_0808a188(9, 0, 20);

    Func_020012f4(11, 1);
    Func_0808a188(9, 0, 20);

    Func_080770c8(0x84c);
    goto close;

close:
    Func_020012f4(11, 0);
    Func_0808a020();
}
