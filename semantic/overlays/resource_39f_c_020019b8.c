typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x020019b8, 272 bytes: code 0x020019b8-0x02001abd, an
 * alignment halfword at 0x02001abe and the pool words 0x00000312 at 0x02001ac0
 * and 0x00000313 at 0x02001ac4.
 *
 * The third member of the probe-dispatch family (0x02000f94 and 0x02001454 are
 * the others): read a six-word probe through Func_02000474, republish it
 * through Func_02000608, then branch on the probe's kind word.  Kinds 9 and 11
 * paint a marker and, at their respective trigger tiles, run the shared
 * presentation tail; kind 8 only paints.  `armed` (r6) is what selects the
 * tail, and it is set only on the two trigger paths.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 *
 * The epilogue is `add sp, #32 / pop {r5, r6} / pop {r0} / bx r0`, so the owner
 * is void.  The assembly reaches Func_0808a020 from two places (0x02001a5e and
 * 0x02001ab2) but exactly once on every path, so it is written once here.
 */

void Func_0808a018();
/* Fills the caller's six-word record; nonzero means it produced one. */
s32 Func_02000474();
/* Four register arguments plus two stack words. */
void Func_02000608();
/* Four register arguments plus two stack words. */
void Func_080091c0();
void Func_0808a1e0();
void Func_080770c8();
void Func_0808a100();
void Func_0808a0e0();
void Func_0808a010();
void Func_080f9010();
u8 *Func_0808a080();
void Func_0808a020();

void Func_020019b8(void)
{
    s32 probe[6];
    s32 armed = 0;

    /* No argument register is written before this branch. */
    Func_0808a018();

    if (Func_02000474(probe) != 0) {
        Func_02000608(probe[0], probe[1], probe[2], probe[3],
                      probe[4], probe[5]);

        if (probe[1] == 9) {
            Func_080091c0(38, 68, 1, 4, probe[2] >> 20, 68);
            if ((probe[2] >> 20) == 42) {
                Func_080091c0(26, 20, 2, 4, 42, 23);
                Func_0808a1e0(9, 1);
                armed = 1;
                Func_080770c8(0x312);
            }
        } else if (probe[1] == 11) {
            if ((probe[2] >> 20) == 40) {
                Func_080091c0(26, 20, 2, 4, 40, 32);
                Func_0808a1e0(11, 1);
                armed = 1;
                Func_080770c8(0x313);
            }
        } else {
            goto kind8;
        }

        if (armed == 0) {
            Func_0808a020();
            return;
        }

        Func_0808a100(probe[1], 3);
        Func_0808a0e0(probe[1], 18, 6);
        Func_0808a010(30);
        Func_0808a100(probe[1], 8);
        Func_080f9010(240);

        Func_0808a080(probe[1])[35] = 2;
        goto common_exit;

kind8:
        /* Kind 8 paints its marker and takes no shared presentation tail. */
        if (probe[1] == 8) {
            Func_080091c0(42, 49, 1, 4, probe[2] >> 20, 49);
        }
    }

common_exit:
    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
