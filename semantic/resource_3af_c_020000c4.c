#include "types.h"

/*
 * resource_3af owner at 0x020000c4, 552 bytes: the per-frame step of an
 * ambient actor.  While idle it drifts on two axes under a pseudo-random
 * impulse, and roughly one frame in a hundred it starts an eight-phase scripted
 * routine which it then advances one phase per call.
 *
 * WHY THE INVENTORY UNDERSTATES THIS ROW: it advertises 222 code bytes and 5
 * calls against a 552-byte span, because the linear walk stops dead at the
 * 'mov pc, r3' dispatch and never reaches the 452 bytes of case bodies behind
 * it.  This is the documented hidden-dispatcher tier - a prologue row whose
 * 'calls' is small against a large span - and it converts normally.  The real
 * figures are 15 call sites over 6 targets.
 *
 * Complete owner: 'push {r5, r6, r7, lr}' plus the 'push {r7}' banking r8, and
 * the single interworking epilogue at 0x020002c4.  Layout from the
 * control-flow walk, seeding the jump table FIRST as the pool guard requires:
 *
 *   0x00c4 .. 0x00ec   40 bytes   entry and dispatch
 *   0x00ec .. 0x010c   32 bytes   jump table, 8 entries          (not code)
 *   0x010c .. 0x02d0  452 bytes   case bodies, physics, tail
 *   0x02d0 .. 0x02ec   28 bytes   seven-word literal pool        (not code)
 *
 * 492 code + 60 non-code = 552, the advertised span.
 *
 * The table's base pool word is 0x020080ec, which is file offset 0x00ec under
 * the proven 0x02008000 link base - the table is physically where its own base
 * says it is, one more witness for that base.  Its entries carry NO Thumb bit,
 * which is what distinguishes a 'mov pc' jump table from a handler table, and
 * they resolve to 0x010c, 0x01d4, 0x012c, 0x01d4, 0x01aa, 0x01d4, 0x01dc,
 * 0x0214.  Three phases therefore share the bare "advance" arm; that is one
 * arm reached from three cases, spelled as grouped cases, not copied.
 *
 * Return type from the epilogue rule: 'pop {r1} ; bx r1' with N != 0, so r0
 * survives - the owner always returns 1.
 *
 * Calls: 15 sites over 6 targets, from
 * 'bun tools/overlay_call_targets.ts resource_3af 00c4'.  All six are import
 * veneers; the overlay has no intra-image calls here.
 *
 * Func_080000f8 is the established pseudo-random draw returning a 16.16
 * fraction, and every use here is that fraction scaled by a shift:
 * '<< 2 >> 16' is 0..3, '<< 12 >> 16' is 0..4095, '<< 15 >> 16' is 0..32767,
 * and the tail's '* 100 >> 16' is 0..99.  Read as bit manipulation the whole
 * routine looks arbitrary; read as scaled draws it is a drift plus a 1-in-100
 * trigger.
 *
 * UNCERTAINTY: the three s32 words at +0x38/+0x3c/+0x40 are only ever tested
 * for all three being 0x80000000, the value this family uses as "unset".  The
 * gate therefore reads as "the previous motion has finished", but the fields
 * themselves are not identified.  The byte at +0x63 selects between two
 * variants of the same phase throughout and is likewise unnamed.
 */

struct Ambient3af {
    u8 unknown_00[8];
    s32 x;                          /* 0x08, 16.16 */
    s32 y;                          /* 0x0c, 16.16 */
    u8 unknown_10[0x20];
    s32 speedLimitA;                /* 0x30 */
    s32 speedLimitB;                /* 0x34 */
    s32 motionA;                    /* 0x38 */
    s32 motionB;                    /* 0x3c */
    s32 motionC;                    /* 0x40 */
    u8 unknown_44[8];
    s32 drift;                      /* 0x4c */
    u8 unknown_50[0x12];
    u8 phase;                       /* 0x62 */
    u8 variant;                     /* 0x63 */
    s16 xReversed;                  /* 0x64 */
    s16 yReversed;                  /* 0x66 */
};

/* The family's "no motion pending" sentinel. */
#define AMBIENT3AF_IDLE_MOTION ((s32)0x80000000)

s32 Func_080000f8();                /* pseudo-random draw, a 16.16 fraction */
void Func_08009150();               /* aim a record at a 16.16 destination */
struct Ambient3af *Func_0808a080(); /* record by id */
void Func_0808a1b8();               /* start a cue (id, parameter, delay) */
void Func_0808a1e8();               /* start a variant cue (id, script, delay) */
void Func_080f9010();               /* play a sound effect */

s32 Func_020000c4(struct Ambient3af *actor)
{
    u8 phase = actor->phase;
    struct Ambient3af *other;
    s32 draw;
    s32 value;

    if (phase == 0) goto physics;

    switch ((u32)(phase - 1)) {
    case 0:
        actor->speedLimitA = 128 << 11;
        actor->speedLimitB = 128 << 10;
        Func_08009150(actor, 134 << 17, 160 << 13, 173 << 18);
        goto advance;

    case 2:
        if (actor->motionA != AMBIENT3AF_IDLE_MOTION) goto tail;
        if (actor->motionB != actor->motionA) goto tail;
        if (actor->motionC != actor->motionB) goto tail;

        actor->phase = (u8)(actor->phase + 1);
        Func_080f9010(146);
        if (actor->variant != 0) {
            Func_0808a1b8(21, 208 << 8, 0);
        } else {
            Func_0808a1b8(21, 176 << 8, 0);
        }

        if ((u32)(Func_080000f8() << 2) >> 16 != 0) {
            other = Func_0808a080(21);
            other->motionA = 128 << 10;
        } else {
            Func_0808a1e8(21, 0x103, 0);
            other = Func_0808a080(21);
            other->motionA = 192 << 11;
        }
        goto tail;

    case 4:
        if (actor->variant != 0) {
            Func_08009150(actor, 141 << 17, 0, 0x02920000);
        } else {
            Func_08009150(actor, 254 << 16, 0, 167 << 18);
        }
        goto advance;

    case 6:
        if (actor->motionA != AMBIENT3AF_IDLE_MOTION) goto tail;
        if (actor->motionB != actor->motionA) goto tail;
        if (actor->motionC != actor->motionB) goto tail;

        actor->speedLimitA = 128 << 10;
        actor->xReversed = 0;
        actor->yReversed = 0;
        actor->speedLimitB = 128 << 9;
        actor->phase = (u8)(actor->phase + 1);
        actor->drift = 0;
        goto tail;

    case 7:
        actor->phase = 0;
        goto tail;

    case 1:
    case 3:
    case 5:
        goto advance;

    default:
        goto tail;
    }

advance:
    actor->phase = (u8)(actor->phase + 1);
    goto tail;

physics:
    /* Horizontal drift: accelerate one way until it saturates, then reverse. */
    if (actor->xReversed != 0) {
        draw = (s32)((u32)(Func_080000f8() << 12) >> 16);
        actor->drift -= draw;
        if (actor->drift < (s32)0xffffc000) {
            actor->xReversed = (s16)phase;      /* provably 0 on this path */
        }
    } else {
        draw = (s32)((u32)(Func_080000f8() << 12) >> 16);
        actor->drift += draw;
        if (actor->drift > (128 << 7)) {
            actor->xReversed = 1;
        }
    }

    /* Band guard: apply the drift only inside a wrapped half-open window. */
    value = actor->x;
    if ((u32)(value + 0xff07ffff) <= 0x002bfffe) {
        actor->x = value + actor->drift;
    }

    /* Vertical bob, the same shape with a fixed bias instead of a stored one. */
    if (actor->yReversed != 0) {
        draw = (s32)((u32)(Func_080000f8() << 15) >> 16);
        value = actor->y - draw + (s32)0xffff8000;
        actor->y = value;
        if (value >= 0) goto tail;
        value = 0;
    } else {
        draw = (s32)((u32)(Func_080000f8() << 15) >> 16);
        value = actor->y + draw + (128 << 8);
        actor->y = value;
        if (value <= (128 << 12)) goto tail;
        value = 1;
    }
    actor->yReversed = (s16)value;

tail:
    /* Roughly one idle frame in a hundred starts the scripted routine. */
    if ((100 * Func_080000f8()) >> 16 == 0) {
        actor->phase = 1;
    }
    return 1;
}
