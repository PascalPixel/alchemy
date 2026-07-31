typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_399 scripted step at 0x020007a4, 232 bytes.
 *
 * Complete owner: `push {r5, lr}` at 0x020007a4 and the interworking return
 * `pop {r5} / pop {r0} / bx r0` at 0x0200086c.  The popped register is r0, so
 * it holds the return address — the owner is `void`.  Bytes
 * 0x02000872-0x0200088b are alignment plus the six-word literal pool.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_399
 * 07a4 --annotate` (an overlay `bl` stores `target - 2`).  21 sites, 14
 * distinct imports; per-target multiset, matched by the C below:
 *   Func_0808a080 x1, Func_080770c0 x1, Func_080b0008 x1, Func_0808a018 x2,
 *   Func_0808a148 x1, Func_0808a010 x2, Func_0808a170 x2, Func_0808a180 x2,
 *   Func_0808a1b8 x1, Func_0808a020 x2, Func_0808a208 x1, Func_0808a210 x2,
 *   Func_0808a218 x2, Func_0808a0b0 x1.
 *
 * Same shape as the owner at 0x02000974: read the heading halfword at +6 of
 * the channel-0 scene record, then in each arm of the Func_080770c0(0x881)
 * story flag apply the biased UNSIGNED range test
 * `(u32)((heading << 16) + 0x5fff0000) <= 0x3ffe0000` — the actor facing
 * within one quadrant of a reference direction.  The test is assembled once
 * per flag arm; hoisting it into a local does not change the call multiset.
 *
 * Note the asymmetry in the flag-clear arm: when the heading test FAILS there,
 * the `bhi` goes straight to the shared epilogue and the owner does nothing at
 * all, so that arm has no else branch.
 *
 * Constants are built by shifts, as this compiler always spells 16.16 whole
 * numbers: `movs #192 ; lsls #11` = 0x60000 (6.0), `movs #192 ; lsls #8` =
 * 0xc000 (0.75), `movs #213 ; lsls #17` = 0x1aa0000 (426.0),
 * `movs #246 ; lsls #17` = 0x1ec0000 (492.0), `movs #154 ; lsls #18` =
 * 0x2680000 (616.0), and `movs #128 ; lsls #7` = 0x4000, a quarter turn in the
 * 16-bit angle unit.
 *
 * The pool word 0x0200a5ec passed to Func_0808a0b0 is EVEN and in the
 * 0x0200axxx band, so under the proven 0x02008000 link base it is in-image
 * data at file offset 0x25ec — an animation script, which is the established
 * role of the second argument of Func_0808a0b0.
 *
 * Uncertainty: Func_0808a210's four arguments read as a camera target with -1
 * meaning "leave this axis alone" (the same -1 placeholder the converted
 * resource_3aa uses at its Func_0808a210 sites), but that reading is inferred
 * from the argument shape rather than established from the import itself.
 */

extern u8 Data_0200a5ec[];      /* in-image animation script, file offset 0x25ec */

/* Imports, named by the main-image address in the trailing word of the overlay
 * veneer each call site reaches.  Old-style declarations are mandatory. */
void Func_0808a010();           /* frame wait */
void Func_0808a018();           /* scripted-section open */
void Func_0808a020();           /* scripted-section close */
void Func_0808a0b0();           /* (selector, script) */
void Func_0808a148();           /* scene request */
void Func_0808a170();           /* message line */
void Func_0808a180();
void Func_0808a1b8();           /* orientation over frames */
void Func_0808a208();
void Func_0808a210();           /* camera target */
void Func_0808a218();
void Func_080b0008();
u8 *Func_0808a080();            /* scene record for a selector, or 0 */
s32 Func_080770c0();            /* story-flag query */

void Func_020007a4(void)
{
    s16 heading;
    s32 facing_near;

    heading = *(s16 *)(Func_0808a080(0) + 6);
    facing_near = ((u32)(((s32)heading << 16) + 0x5fff0000) <= 0x3ffe0000);

    if (Func_080770c0(0x881) != 0) {
        if (facing_near) {
            Func_080b0008(10, 12);
        } else {
            Func_0808a018();
            Func_0808a148(12, 0, 0);
            Func_0808a010(10);
            Func_0808a170(0x164b);
            Func_0808a180(12, 0);
            Func_0808a1b8(12, 0x4000, 10);
            Func_0808a020();
        }
    } else if (facing_near) {
        Func_0808a018();
        Func_0808a208(0x60000, 0xc000);
        Func_0808a210(0x1aa0000, -1, 0x1ec0000, 1);
        Func_0808a218();
        Func_0808a010(20);
        Func_0808a0b0(12, Data_0200a5ec);
        Func_0808a170(0x153e);
        Func_0808a180(12, 0);
        Func_0808a210(0x1aa0000, -1, 0x2680000, 1);
        Func_0808a218();
        Func_0808a020();
    }
}
