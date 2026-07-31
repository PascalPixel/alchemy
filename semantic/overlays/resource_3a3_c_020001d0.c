typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 3a3 scripted sequence at 0x020001d0 (200 bytes,
 * 0x020001d0 .. 0x02000297, of which 0x02000288 .. 0x02000297 is the pool).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020001d0 and the interworking
 * return `pop {r5, r6} / pop {r0} / bx r0` at 0x02000282, so the owner
 * returns nothing.
 *
 * All 21 call sites are placed (row reports calls=21): Func_0808a018 x1,
 * Func_080770c0 x2, Func_0808a170 x2, Func_0808a190 x1, Func_0808a100 x1,
 * Func_0808a188 x1, Func_0808a080 x2, Func_0808a228 x1, Func_0808a208 x1,
 * Func_0808a210 x2, Func_0808a148 x1, Func_0808a1b8 x1, Func_0808a218 x2,
 * Func_0808a010 x1, Func_0808a110 x1, Func_0808a020 x1.
 *
 * SHARED CALL SITES.  Two joins are single sites and are spelled once:
 * Func_0808a110(14, 4) at 0x02000276 is entered both by falling through the
 * long body and by the `bne` when flag 0x8ff is already set, and the closing
 * Func_0808a020 at 0x0200027e is entered from both top-level arms.
 *
 * Uncertainty: at 0x0200021c Func_0808a228 is entered with r0 holding the
 * actor-18 record, and the `adds r0,#85 / strb` immediately after it writes
 * through r0 -- i.e. through the callee's RETURN value, since r0 is
 * call-clobbered.  Clearing a record's byte at +0x55 is the same idiom
 * Func_020006a4 uses on the Func_0808a080 result, so the callee is read as
 * returning a record pointer here.  If it were in fact void, the store would
 * land on the same record that was passed in.
 */

/* Imports.  Old-style declarations are mandatory in overlay sources. */
void Func_0808a018();       /* opens a scripted sequence */
s32 Func_080770c0();        /* tests a flag id; nonzero when set */
void Func_0808a170();       /* queues a cue / script id */
void Func_0808a190();
void Func_0808a100();
void Func_0808a188();
u8 *Func_0808a080();        /* scene/actor record accessor */
u8 *Func_0808a228();
void Func_0808a208();
void Func_0808a210();
void Func_0808a148();
void Func_0808a1b8();
void Func_0808a218();
void Func_0808a010();       /* waits the given number of frames */
void Func_0808a110();

void Func_020001d0(void)
{
    u8 *record;
    u8 *staged;

    Func_0808a018();

    if (Func_080770c0(0x909) != 0) {
        Func_0808a170(0x191f);
        Func_0808a190(14, 0);
        Func_0808a020();
        return;
    }

    Func_0808a100(14, 4);
    Func_0808a170(0x18c7);
    Func_0808a188(14, 0, 10);

    if (Func_080770c0(0x8ff) == 0) {
        record = Func_0808a080(18);

        staged = Func_0808a228(record);
        staged[0x55] = 0;

        Func_0808a208(0x10000, 0x2000);         /* 128<<9, 128<<6 */
        Func_0808a210(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16), 1);
        Func_0808a148(0, 0x4000, 0);            /* 128<<7 */
        Func_0808a1b8(14, 0x3000, 0);           /* 192<<6 */
        Func_0808a218();
        Func_0808a010(120);

        record = Func_0808a080(0);
        Func_0808a210(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16), 1);
        Func_0808a218();
    }

    Func_0808a110(14, 4);
    Func_0808a020();
}
