typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_393 owner at 0x02000ba4, 76 bytes: aim the climb-down beat at the
 * square two and a half tiles along +x from the current subject, and run it.
 *
 * Complete owner: 'push {r5, lr}', a 12-byte local frame for the probe, and
 * the single interworking epilogue at 0x02000be0.  Control-flow walk: pure
 * straight line, so the 8-byte pool at 0x02000be8 (0x02000240, 0xfff00000) is
 * everything after the 'bx r0'.  68 code + 8 pool = 76, the advertised span.
 * The 'ldr r0,[pc,#0] / bx lr' pair at 0x02000bf0 is an exported constant
 * accessor for 0x02009098, NOT part of this row and not a call.
 *
 * Return type from the epilogue rule: 'pop {r0} ; bx r0', so r0 is the popped
 * return address and the owner is void.  The s32 the callee returns is
 * discarded here.
 *
 * Calls: 2 sites, from 'bun tools/overlay_call_targets.ts resource_393 0ba4' -
 * Func_0808a080 and this overlay's own 0x02000aac.  The listing prints the
 * second as 'bl 0x2000aac' only by coincidence of the +2 rule; it is resolved,
 * not read off.
 *
 * Cross-check on the subject id: 'Data_02000240 + 500' is the established
 * cross-overlay subject-record selector - semantic/overlays/resource_370_c_
 * 02000054.c passes the same word to Func_0808a080 and then clears byte +85 of
 * the result, which is the same control byte 0x02000aac brackets.
 *
 * NOTE, deliberate: r4 is used as call-clobbered scratch without being saved
 * by the 'push {r5, lr}' prologue.  That is the documented family habit, not a
 * decoding error.
 *
 * NOTE, also deliberate: probe.x is stored TWICE.  The first store (tile-snap
 * plus half a tile) is dead - the second overwrites it with tile-snap plus two
 * and a half tiles from the same pre-add value in r2.  Folding it away would
 * hide that the two constants are built separately.
 */

struct Aim393Subject {
    u8 unknown_00[8];
    s32 x;                          /* 0x08, 16.16 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
};

struct Aim393Probe {
    s32 x;
    s32 y;
    s32 z;
};

/* The cross-overlay subject-record selector. */
#define AIM393_SUBJECT_ID (*(s32 *)((u8 *)0x02000240 + 500))

/* 16.16: one tile is 0x00100000, so this snaps a coordinate to its tile. */
#define AIM393_TILE_MASK ((s32)0xfff00000)

struct Aim393Subject *Func_0808a080();  /* subject record by id */
s32 Func_02000aac();                    /* the climb-down beat */

void Func_02000ba4(void)
{
    struct Aim393Subject *subject = Func_0808a080(AIM393_SUBJECT_ID);
    struct Aim393Probe probe;
    s32 snappedX = subject->x & AIM393_TILE_MASK;
    s32 half = 128 << 12;           /* 0x00080000, half a tile */

    probe.x = snappedX + half;      /* dead: overwritten below */
    probe.y = subject->y;
    probe.z = (subject->z & AIM393_TILE_MASK) + half;
    probe.x = snappedX + (160 << 14);   /* 0x00280000, two and a half tiles */

    Func_02000aac(&probe);
}
