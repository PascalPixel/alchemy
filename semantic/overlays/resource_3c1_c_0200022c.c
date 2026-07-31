typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3c1 owner at 0x0200022c, 92 bytes: the map's entry step.  Record
 * arrival at map 90 by setting its event flag, publish phase 256 and the
 * 24-frame counter, and - once event flag 0x0201 is set - run the 0x020000f8
 * dressing sequence and emit cue 16.
 *
 * Complete owner: 'push {lr}' and the single interworking epilogue at
 * 0x02000272.  Control-flow walk: two forward skips, both landing before
 * 0x02000276, so the alignment halfword there and the four pool words after it
 * are unreachable.  74 code + 18 non-code = 92, the advertised span.
 *
 * Return type from the epilogue rule: 'pop {r1} ; bx r1' with N != 0, so r0
 * survives - the owner returns the constant 0.
 *
 * Calls: 4 sites over 4 targets, from
 * 'bun tools/overlay_call_targets.ts resource_3c1 022c'.  0x020000f8 is an
 * ordinary intra-overlay call; it is a real 'push {lr}' function that the
 * inventory does not list at all, so it is declared here and left to its own
 * row.
 *
 * The two workspace stores are the documented displacement/value trap in its
 * SUBTRACTIVE form, and one register plays three roles in five instructions:
 * 'movs r3,#224 / lsls #1' makes 448 the DISPLACEMENT, 'subs r3,#192' makes
 * 256 the stored VALUE, and 'adds r3,#200' makes 456 the NEXT displacement.
 * Reading it as 448 - 192 + 200 is the natural mistake.  Slot +448 is the
 * established s32 scene/phase id, and 256 is a value another overlay
 * (resource_3a2:11b0) publishes into the same slot.
 *
 * The arrival flag proves itself: the guard reads the shared map selector for
 * the literal map id 90 and the body sets that map's own event flag 0x096f, so
 * gate and setter agree inside one owner.
 */

/* The cross-overlay map selector block. */
#define MAP3C1_SELECTOR (*(s16 *)((u8 *)0x02000240 + 450))

/* Pointer CELL holding the scene workspace - one dereference, not two. */
#define MAP3C1_WORKSPACE (*(u8 **)0x03001ebc)

s32 Func_080770c0();                /* read an event flag */
void Func_080770c8();               /* set an event flag */
void Func_020000f8();               /* this overlay's dressing sequence */
void Func_0808a100();               /* emit a cue (id, parameter) */

s32 Func_0200022c(void)
{
    u8 *workspace;

    if (MAP3C1_SELECTOR == 90) {
        Func_080770c8(0x96f);
    }

    workspace = MAP3C1_WORKSPACE;
    *(s32 *)(workspace + 448) = 256;
    *(s32 *)(workspace + 456) = 24;

    if (Func_080770c0(0x201) != 0) {
        Func_020000f8();
        Func_0808a100(16, 4);
    }
    return 0;
}
