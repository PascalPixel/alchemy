typedef signed int s32;

/*
 * Resource 3ad, conversation driver at 0x020011b8 (244 bytes, 25 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x020011b8 and `pop {r5} / pop {r0} /
 * bx r0` at 0x0200129e.  The popped branch register is r0, so it holds the
 * return address and the owner is `void`.  Two pool words sit at 0x020012a4 and
 * 0x020012a8 (dialogue ids 0x2547 and 0x254b), past the return and never
 * reached as code.
 *
 * This owner is a state machine over the overlay's own single-question steps.
 * Each of those steps returns a boolean in the low byte, which is why every
 * result is tested as `lsls r0,#24 / cmp r0,#0` - only bits 0-7 are examined.
 * The byte-exact siblings `assets/code/resource_3ad_c_020012ac.c` and
 * `..._0200141c.c` confirm the shape: each returns `Func_...(0, 0) == 0`.
 *
 * The control flow is genuinely a graph, not a tree - 0x0200122e is entered
 * from three places and 0x0200120c is a loop head re-entered from 0x0200128e -
 * so the C below uses labels and `goto` rather than duplicating call sites.
 * Duplicating them would inject phantom calls into the per-target multiset.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`.  Per-target
 * multiset over the 25 sites: Func_0808a148 x5, Func_0808a170 x4,
 * Func_0808a180 x2, Func_0808a178 x2, Func_0808a070 x1, and one site each for
 * the eleven in-overlay steps 0x020012ac, 0x020012c4, 0x020012f0, 0x0200131c,
 * 0x02001320, 0x0200134c, 0x02001368, 0x02001394, 0x0200141c, 0x02001448,
 * 0x02001760.  That is 25 call expressions below, matching the row's calls=25.
 *
 * 0x0200131c is reported `unknown` by the resolver because it has no `push`
 * prologue: it is a real four-byte leaf, `movs r0,#1 / bx lr`, sitting between
 * 0x020012f0's literal pool and 0x02001320's prologue.  Its result is discarded
 * here, so it is called for its side effect only (which is none) - kept because
 * the call site is real.
 */

/* Old-style declarations: overlay import arities vary per call site. */
void Func_0808a148();   /* scene request */
void Func_0808a170();   /* show a dialogue line by id */
void Func_0808a178();   /* dialogue-line variant with a mode word */
void Func_0808a180();   /* wait for the slot's action to finish */
s32 Func_0808a070();    /* dialogue prompt; result selects the branch */

/* Steps in this overlay.  All but 0x0200131c/0x02001448/0x02001760 return a
 * boolean in the low byte. */
s32 Func_020012ac();
s32 Func_020012c4();
s32 Func_020012f0();
s32 Func_0200131c();    /* four-byte leaf, returns 1; result discarded here */
s32 Func_02001320();
void Func_0200134c();
s32 Func_02001368();
s32 Func_02001394();
s32 Func_0200141c();
void Func_02001448();
void Func_02001760();

void Func_020011b8(void)
{
    s32 line;
    s32 retryFromStep134c;

    line = 0x2547;
    Func_0808a170(line);
    Func_0808a180(12, 0);
    line += 1;

    Func_0808a148(1, 0, 0);
    Func_0808a170(line);
    Func_0808a178(1, 0);

    Func_0808a148(2, 0, 0);
    Func_0808a148(3, 0, 0);
    Func_0808a148(13, 0, 0);
    Func_0808a148(12, 0, 0);

    retryFromStep134c = 0;

top:                                                    /* 0x0200120c */
    if ((Func_020012ac() & 0xff) == 0) {
        /* 0x0200124e */
        if ((Func_020012c4() & 0xff) != 0) {
            if ((Func_020012f0() & 0xff) == 0) {
                retryFromStep134c = 1;                  /* 0x0200122c */
                goto step134c;
            }
            goto finishAffirmative;                     /* 0x02001296 */
        }

        /* 0x02001264 */
        if ((Func_02001368() & 0xff) == 0) {
            /* 0x0200126e: repeat the prompt with the second line pair. */
            line = 0x254b;
            Func_0808a170(line);
            line += 1;
            Func_0808a180(2, 0);
            Func_0808a170(line);
            Func_0808a178(1, 0);
            goto top;
        }
        /* falls through to the 0x02001216 arm */
    }

    /* 0x02001216 */
    if ((Func_02001320() & 0xff) == 0) {
        goto finishNegative;                            /* 0x02001290 */
    }

    /* 0x02001220 */
    retryFromStep134c = 0;
    if ((Func_0200141c() & 0xff) == 0) {
        retryFromStep134c = 1;
step134c:                                               /* 0x0200122e */
        Func_0200134c();
        if (Func_0808a070(0, 0) == 0) {
            goto finishNegative;
        }
    }

    /*
     * 0x0200123e.  Both the "step succeeded" and the "step failed but this was
     * not the retry path" outcomes fall to the same exit, so there is exactly
     * one call site here despite three predecessors.
     */
    if ((Func_02001394() & 0xff) == 0 && retryFromStep134c != 0) {
        goto step134c;
    }
    goto finishAffirmative;

finishNegative:                                         /* 0x02001290 */
    Func_02001760();
    return;

finishAffirmative:                                      /* 0x02001296 */
    Func_0200131c();
    Func_02001448();
}
