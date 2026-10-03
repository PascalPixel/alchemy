/*
 * Canonical draft API context; no match or adoption is claimed.
 * API context measured 2026-10-02 with ordinary target flags:
 * all six prior successful objects retain every allocated byte and
 * normalized call/pool operand; no new match or adoption is claimed.
 * The shared void DispatchObject/u32 contract uses ordinary data casts;
 * every original matching-body and trial annotation is retained.
 *
 * 2026-10-03 ArcTan2 return-width trial: the measured inputs were eight
 * maintained battle source files, including COMMON EFFECT/FRAGMENT.C,
 * not this draft. This draft was not compiled or changed below its header.
 * Only their local ArcTan2 return declarations changed from s32 to u16.
 * Ten representative complete objects were compared with 54 existing
 * edition objects: 30 complete ELF comparisons stayed exact, 24 changed.
 * Four object families changed, with all local frames/save sets preserved:
 * - TLA shared FRAGMENT: 200 to 204 bytes; lsl/lsr zero-extension appears
 *   before its existing read/write assembly boundary. Frame 12, saved 12.
 * - TBS MOTION: 1046 bytes in both; Actor_ResetMotionAtAnchor remains 100.
 *   Its constructed +0x8000 changes to a pool load of -32768, adding a
 *   pool word while instruction/alignment bytes shrink. Frame 0, saved 12.
 * - TBS PRESENT8: 2620 to 2624 bytes; SpawnActorObject 1228 to 1232, with
 *   return zero-extension. Frame 20, saved 32. Later symbols move by four
 *   bytes, including two internal Summon_LayoutPositions call targets;
 *   external call targets and call-symbol order remain unchanged.
 * - TLA RESET_MOTION_AT_ANCHOR: 94 to 96 bytes; the same -32768 pool-load
 *   change replaces the +0x8000 construction. Frame 0, saved 12.
 * Other representative objects retained complete text, pools, symbols and
 * relocations. No body, cast, existing device or compiler option changed.
 * The trial is stopped; these production measurements are not a score,
 * match, adoption or renewed measurement of this canonical draft.
 * Coordinator selected s32 ArcTan2/ArcTan2Far(s32, s32): a nonnegative
 * modulo-16-bit angle transported in a 32-bit word. Its TBS producer keeps
 * the explicit body narrowing; changing only its return token to s32
 * preserves complete objects and assembly in all six editions. A u32
 * token also preserved the measured EN producer, so the chosen signed
 * convention is not claimed to prove unique original C signedness.
 * The eight local word-return declarations are restored under that shared
 * API decision; producer/header integration belongs to Coordinator.
 *
 * Follow-on EFFECT11 trial, also 2026-10-03: these measurements concern the
 * complete maintained TBS EFFECT11 object, not a compile of this draft.
 * Changing only its local u16 ArcTan2 declaration to the selected s32
 * contract emits 754/native 758 bytes. BattleEffect_SelectNearbyObject
 * shrinks 304 to 300 bytes by losing the native lsl/lsr16 after ArcTan2;
 * its other caller already has an explicit u16 cast and is unchanged.
 * One authorized ordinary consumer trial changes only the first caller's
 * used angle local from s32 to u16, preserving the signed modular consumer.
 * It emits 762/native 758 bytes; the first function is 308/native 304.
 * GCC sign-extends the result after the call, then zero-extends it again
 * before the signed modular subtraction, adding two instructions overall.
 * Both failed forms preserve the 4-byte local frame and 32 saved bytes;
 * the complete objects, symbols and relocations differ in all six existing
 * edition comparisons. Their first positional text difference is +0x58.
 * No further angle form or device was tried. After recording both failures,
 * EFFECT11 is restored byte-for-byte to the current index; its old u16
 * declaration remains an explicit unresolved caller-contract boundary.
 * EFFECT43's declaration-only s32 change retains the complete 1228-byte
 * object in all six comparisons. Neither trial recompiles this draft.
 */
#include "FIXED_MATH.H"
#include "OBJDISP.H"
#include "TYPES.H"
#include "OBJECT_EFX.H"
#include "SYSTEM.H"

struct ItemBreakFragmentPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct ItemBreakFragmentSource {
    u8 reserved_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[70];
    u8 flag_5a;
    u8 reserved_5b[13];
    struct ItemBreakFragmentSource *target;
};

struct ItemBreakFragmentObject {
    u8 reserved_00[72];
    s32 field_48;
    u8 reserved_4c[9];
    u8 mode_55;
    u8 reserved_56[8];
    u16 field_5e;
};

extern s32 ArcTan2(s32, s32);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
extern void Vector_AddPolarOffset(s32, s32, struct ItemBreakFragmentPosition *);
extern struct ItemBreakFragmentObject *Object_Spawn(s32, s32, s32, s32);
extern void Object_SetMode(struct ItemBreakFragmentObject *, s32);

void BattleFx_UpdateItemBreakFragment(struct ItemBreakFragmentSource *source)
{
    struct ItemBreakFragmentSource *target;
    struct ItemBreakFragmentPosition position;
    struct ItemBreakFragmentObject *object;
    s32 steering_delta;
    s32 drift_magnitude;

    target = source->target;
    if (target != 0) {
        s32 dx = target->x - source->x;
        s32 dz = target->z - source->z;

        if (dx != 0 || dz != 0) {
            steering_delta = (s16)(ArcTan2(dz, dx) - source->angle);
            if (steering_delta > 0x1000)
                steering_delta = 0x1000;
            if (steering_delta < -0x1000)
                steering_delta = -0x1000;
            source->angle += steering_delta;
        }
        source->flag_5a = 0;
    }

    position.x = source->x;
    position.y = source->y - (Random16() << 4) - 0x80000;
    position.z = source->z;
    drift_magnitude = Random16() * 3;
    drift_magnitude <<= 4;
    Vector_AddPolarOffset(drift_magnitude, Random16(), &position);

    object = Object_Spawn(0x11D, position.x, position.y, position.z);
    if (object != 0) {
        object->mode_55 = 2;
        object->field_48 = 0x1999;
        Object_SetMode(object, 0);
        object->field_5e = 12;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_CommonParticleScript);
    }
}
