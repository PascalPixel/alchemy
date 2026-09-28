/* H5 (2026-09-27): stage snapped X in the existing position buffer before
 * deriving Z, then extract X from that buffer. Complete 192/188-byte result,
 * 68 differing halfwords / 42 aligned edits. The frame stays 12, but the
 * compiler adds an X stack store, the mask pool shifts four bytes, and the
 * counter remains sl while Z remains r8. Reject this memory producer model;
 * the five-edit body is restored below. Do not transfer it to
 * the 02003668 twin. */
/* Astra 2026-09-27 branch-lifetime test: duplicate the x-position store
 * in if(n == 0)/else, predicting an extra counter use until late tail
 * merging. Complete result remains 188/188, five halfwords / five edits;
 * counter is still sl and z is still r8. Initial RTL has the conditional
 * (insn 100), but the first jump pass already removes it, before allocation.
 * This gives no counter lifetime and no matching transfer to the twin.
 * Full normalized diff read; restore the unconditional store and stop.
 * NONMATCHING: 188/188 bytes, 5 differing halfwords / 5 aligned edits.
 * Astra rounding-order transfer (2026-09-27): calculating snapped z before
 * snapped x emits 188 bytes, 8 halfwords / 8 edits. The counter remains sl;
 * z becomes r9 and x r8, with reversed field loads. Frame stays 12. Full
 * normalized diff rejects the predicted counter/centre allocation; retain
 * the original order and do not propagate this trial to the twin.
 * H4 negative witness is preserved in a0d14b1cb; canonical body restored.
 * Sol H4 2026-09-27: start + (n + 1) * 0x400 produces each turn angle.
 * Initial u16 facing is masked to 0/0x4000/0x8000/0xc000; start is
 * 0x8000..0x14000 and the 16 angles are 0x8400..0x18000, without s32
 * overflow. Facing still truncates angle + 0x4000 to the same u16.
 * Prediction: strength reduction retains angle induction and frame 12,
 * allocates n=r8, x=r9, z=sl, with identical calls, stores and all 188 bytes.
 * One model only; reject any extra multiply, prologue, spill or pool change.
 * Rejected trial: 188/188 bytes, 55 differing halfwords / 45 aligned edits.
 * Its frame 12, extent 188 and final mask pool are unchanged, but gate fails.
 * Full normalized diff: the initial angle is now r5 (not r6), p is r6
 * (not r5), and the first step folds into start + 0x400 before the loop.
 * Subsequent angle increments move to the loop tail. Centre stores also
 * reorder; n remains sl and z remains r8. There is no emitted multiply.
 * -dL/-da: frame n is biv 38, angle is reduced to generated biv 82;
 * after reduction the compiler reverses the count-only n loop. Final
 * allocation keeps x=5 refs/31 length, z=5/27, n=7/38, exactly their prior
 * priorities. New angle biv has 9 refs/20 length and wins r5, then p=r6,
 * object=r7, z=r8, x=r9, n=sl. The angle derivation changes induction
 * ancestry and placement, not the missing n/z priority. Reject this axis
 * after one trial; do not propagate to the twin or try type/permutation
 * variants. Both owners remain C not yet written; no DONE or alignment.
 * Prior canonical: 188/188 bytes, 5 differing halfwords / 5 aligned edits.
 * Sol H3 2026-09-27: reuse n as rounding work before the frame loop.
 * Full normalized diff and diagnostic assembly are baseline-identical.
 * Combine/regmove erase the earlier n values: the final allocator keeps
 * x=5 refs/31 length, z=5/27, n=7/38, sorted z then x then n after the
 * three low-register owners. floor_log2(refs)*refs/length gives z 10/27,
 * n 14/38, x 10/31. The close z/n rank is a fact, not a declaration tie.
 * Prediction n=r8 is falsified; complete extent, frame and pool unchanged.
 * Reject reuse-counter axis after one trial. No adoption or new DONE.
 * H3 witness body is preserved in afb77e41c; the original candidate is restored.
 * Historical facts:
 * (2026-09-27). H2 is byte-identical to the original baseline.
 * Whole extent [020035ac,02003668), pool included. Independent twin extent
 * [02003668,02003724) also scores 188/188, 5 halfwords/5 edits.
 * ROLL_OBJECT.C dispatches these opposite 16-frame turns on tiles 98/97.
 * ROLL_STEP.C and the canonical polar callee agree on the mutable three-word
 * position and void call; animation, cue and wait calls also match their ABI.
 * H1 separates fixed snapped centre into a two-coordinate record from the
 * mutable polar-position buffer. Frame stays 12, but centre occupies r6/r7,
 * displacing object/angle into high registers and moving mask operations.
 * H1 scored 188/188 bytes, 67 halfwords / 46 edits. Complete normalized diff
 * rejects that aggregate ownership model; do not tune its allocation.
 * Negative witness is preserved in 5be878f9a.
 * H2: compiler loop.c check_dbra_loop reverses a forward count-only loop
 * even with calls. Natural 0..15 frame iteration does become the reference
 * descending loop, but both twins remain independently binary-identical
 * to their five-edit baselines. Keep the forward source, not H1.
 * Stop centre-record and loop-direction axes. The invariant still missing
 * is n in r8 and snapped z in sl (x already r9); declarations are not a
 * new hypothesis. Full frame, loop, calls, epilogue and pool were compared.
 * Missing owner/unit registrations now reproduce both complete baselines;
 * this adds no credit. The original baseline diagnosis follows.
 * Hand-written
 * from the resolved disassembly as a single-overlay unit binding Engine_* and
 * Vector_AddPolarOffset at their import veneers. Everything matches except global
 * allocation of the three call-crossing locals: the reference gives the loop
 * counter r8, the snapped x r9 and the snapped z sl; this draft gives z r8
 * and the counter sl (greg sorts z ahead of the counter). Twin shape of
 * resource_3a4:02003668 (the other turn direction); prove it independently
 * before adopting any shared implementation.
 * Astra scalar-container trial (2026-09-27): only snapped z becomes one
 * union FieldCoordinate local, and its accesses use .fixed. Prediction:
 * remove the z/n priority reversal without changing the 12-byte frame.
 * The complete result is baseline-identical, 188 bytes and 5 halfwords /
 * 5 edits; the value view disappears before allocation. Reject this
 * single-word container axis, retain the scalar model, and do not transfer
 * the failed trial or sweep its widths.
 * Astra wait-result trial (2026-09-27): use the ignored s32 return view
 * of Engine_TaskWait found in MAKYURI_HEYA/FADE_TO_BLACK.C and
 * FADE_TO_WHITE.C, keeping the shared service declaration unchanged.
 * The complete result is again 188 bytes, 5 halfwords / 5 edits and
 * baseline-identical. This call-result spelling does not change the
 * counter/centre allocation. Retain the ordinary void call.
 * Astra 2026-09-27: stage x rounding across the z calculation: 196/188,
 * 70 halfwords / 32 edits. This gives n=r8, z=sl, x=r9, but adds early
 * high-register copies and eight bytes. A separate rounded_x local gives
 * the same binary. A one-pass animation block gives 188/11/8 without
 * fixing the allocation; a one-word frame-counter record is baseline-
 * identical (188/5/5). Reject all four; retain the five-edit body.
 * A pointer-valued z carrier is also baseline-identical: its integer uses
 * restore the same allocation. A signed word-bitfield frame count grows
 * to 212/188 bytes, 75 halfwords / 44 edits: it prevents loop reversal
 * and emits truncation/mask operations. Neither is a matching transfer. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Vector_AddPolarOffset(s32 distance, s32 angle, union FieldCoordinate *pos);

void ArutinYama_TurnRollingObjectA(struct FieldActor *object)
{
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;
    s32 angle;
    s32 x;
    s32 z;
    s32 n;

    angle = (object->facing + 0x4000) & 0xc000;
    p = pos;
    p[0].fixed = object->x.fixed;
    p[1].fixed = object->y.fixed;
    p[2].fixed = object->z.fixed;
    Vector_AddPolarOffset(0x180000, angle, p);
    x = (p[0].fixed + 0x80000) & 0xfff00000;
    z = (p[2].fixed + 0x80000) & 0xfff00000;
    angle += 0x8000;
    Engine_ObjectSetAnimation(object, 5);
    Engine_AudioPlayCue(184);
    for (n = 0; n < 16; n++) {
        angle += 0x400;
        p[0].fixed = x;
        p[2].fixed = z;
        Vector_AddPolarOffset(0x180000, angle, p);
        object->x.fixed = p[0].fixed;
        object->z.fixed = p[2].fixed;
        object->facing = angle + 0x4000;
        Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(233);
}
