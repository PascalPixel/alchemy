typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_39f owner at 0x02001454, 204 bytes: code 0x02001454-0x0200151b and
 * the pool word 0x00000311 at 0x0200151c.
 *
 * The same shape as 0x02000f94: read a six-word probe through Func_02000474,
 * republish it through Func_02000608, then dispatch on the probe's kind word
 * and its quantised coordinate.  Kind 8 at Z tile 23 posts one message; kind 10
 * at X tile 35 runs a presentation sequence for slot 10 and repaints a
 * 1x4 collision strip at (35, 30).
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule,
 * and resource_39f_c_02001520.c for the byte-exact Func_02000244 interface.
 *
 * The epilogue is `add sp, #32 / pop {r5} / pop {r0} / bx r0`, so the owner is
 * void.  Func_0808a020 at 0x02001510 is the common exit and runs on every path.
 */

void Func_0808a018();
/* Fills the caller's six-word record; nonzero means it produced one. */
s32 Func_02000474();
/* Four register arguments plus two stack words. */
void Func_02000608();
/* Four register arguments plus two stack words. */
void Func_080091c0();
void Func_080770c8();
void Func_0808a100();
void Func_0808a0e0();
void Func_0808a010();
void Func_080f9010();
u8 *Func_0808a080();
/* Byte-exact: (layer, x, z, width, height, value). */
s32 Func_02000244();
void Func_0808a020();

void Func_02001454(void)
{
    s32 probe[6];

    /* No argument register is written before this branch. */
    Func_0808a018();

    if (Func_02000474(probe) != 0) {
        Func_02000608(probe[0], probe[1], probe[2], probe[3],
                      probe[4], probe[5]);

        if (probe[1] == 8 && (probe[4] >> 20) == 23) {
            Func_080091c0(35, 67, 4, 1, 35, 68);
        } else if (probe[1] == 10 && (probe[2] >> 20) == 35) {
            Func_080770c8(0x311);
            Func_0808a100(10, 3);
            Func_0808a0e0(10, -16, 6);
            Func_0808a010(30);
            Func_0808a100(10, 8);
            Func_080f9010(240);

            Func_0808a080(10)[35] = 2;

            Func_080091c0(44, 30, 2, 4, 34, 30);
            Func_02000244(2, 35, 30, 1, 4, 0);
        }
    }

    /* Common exit; no argument registers are set. */
    Func_0808a020();
}
