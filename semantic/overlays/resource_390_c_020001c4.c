typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_390 owner at 0x020001c4, 244 bytes: the map's entry step.  Publish
 * phase 0x209, put every record in the 8..22 range into presentation phase 0
 * the first time through, and then repaint three tile rectangles in one of two
 * variants chosen by the shared map selector - with a small extra flourish on
 * the second.
 *
 * Complete owner: 'push {r5, r6, lr}', an 8-byte outgoing-argument frame for
 * the six-argument renderer, and the single interworking epilogue at
 * 0x0200029e.  Control-flow walk: a loop and two forward exits, all landing on
 * or before 0x0200029c, so the alignment halfword at 0x020002a6 and the four
 * pool words after it are unreachable.  226 code + 18 non-code = 244, the
 * advertised span.
 *
 * Return type from the epilogue rule: 'pop {r1} ; bx r1' with N != 0, so r0
 * survives - the owner returns the constant 0.
 *
 * Calls: 12 sites over 5 targets, from
 * 'bun tools/overlay_call_targets.ts resource_390 01c4'.  Note that the two
 * calls inside the record loop are ONE site each; the multiset counts sites,
 * not executions.
 *
 * The workspace store is the documented additive displacement/value pair in its
 * simplest form: 'movs r1,#224 / lsls #1' makes 448 the DISPLACEMENT, and the
 * value 0x209 comes from its own pool word.  Slot +448 is the established s32
 * scene/phase id.
 *
 * The selector test in the second arm is the documented windowed range idiom -
 * 'subs #8 / lsls #16 / cmp' against 128 << 9 is exactly (u16)(selector - 8)
 * <= 1, i.e. the two-value set {8, 9}.  Undo the shift or it reads as a
 * comparison against 0x10000.
 *
 * UNCERTAINTY: 'Data_02000240 + 450' is read BOTH ways in the same two
 * instructions - as an unsigned halfword into r2 and as a signed halfword into
 * r3 - and only the signed view is compared against 7 while only the unsigned
 * view feeds the window test.  Both views are kept here rather than collapsed,
 * because which one the family intends is not established.
 */

/* Pointer CELL holding the scene workspace - one dereference, not two. */
#define MAP390_WORKSPACE (*(u8 **)0x03001ebc)

/* The cross-overlay map selector block. */
#define MAP390_SELECTOR_SIGNED (*(s16 *)((u8 *)0x02000240 + 450))
#define MAP390_SELECTOR_UNSIGNED (*(u16 *)((u8 *)0x02000240 + 450))

s32 Func_080770c0();                /* read an event flag */
void *Func_0808a080();              /* record by id */
void Func_080091e0();               /* set presentation mode (record, phase) */
void Func_08009180();               /* six-argument renderer, last two on the stack */
void Func_0808a0f0();               /* start an ambient actor (id, x, z) */

s32 Func_020001c4(void)
{
    u8 *workspace = MAP390_WORKSPACE;
    s32 id;

    *(s32 *)(workspace + 448) = 0x209;

    if (Func_080770c0(0x845) == 0) {
        id = 8;
        do {
            void *record = Func_0808a080(id);

            id++;
            Func_080091e0(record, 0);
        } while ((u32)id <= 22);
    }

    if (MAP390_SELECTOR_SIGNED == 7) {
        Func_08009180(34, 34, 18, 16, 13, 8);
        Func_08009180(34, 94, 18, 76, 13, 8);
        Func_08009180(94, 34, 78, 16, 13, 8);
    } else if ((u32)((MAP390_SELECTOR_UNSIGNED - 8) << 16) <= (128 << 9)) {
        Func_08009180(34, 43, 19, 23, 11, 8);
        Func_08009180(34, 94, 19, 83, 11, 8);
        Func_08009180(94, 34, 79, 23, 11, 8);
        Func_0808a0f0(10, 0, 0);
        Func_0808a0f0(11, 0, 0);
        Func_0808a0f0(12, 0, 0);
    }
    return 0;
}
