/* NONMATCHING: 188/188 bytes, 5 differing halfwords / 5 aligned edits.
 * Sol H3 2026-09-27: reuse n as rounding work before the frame loop.
 * Full normalized diff and diagnostic assembly are baseline-identical.
 * Combine/regmove erase the earlier n values: the final allocator keeps
 * x=5 refs/31 length, z=5/27, n=7/38, sorted z then x then n after the
 * three low-register owners. floor_log2(refs)*refs/length gives z 10/27,
 * n 14/38, x 10/31. The close z/n rank is a fact, not a declaration tie.
 * Prediction n=r8 is falsified; complete extent, frame and pool unchanged.
 * Reject reuse-counter axis after one trial. No adoption or new DONE.
 * H3 witness body is checkpointed before restoring the original candidate.
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
 * Main_08000128 at their import veneers. Everything matches except global
 * allocation of the three call-crossing locals: the reference gives the loop
 * counter r8, the snapped x r9 and the snapped z sl; this draft gives z r8
 * and the counter sl (greg sorts z ahead of the counter). Twin shape of
 * resource_3a4:02003668 (the other turn direction); prove it independently
 * before adopting any shared implementation. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_08000128(s32 distance, s32 angle, union FieldCoordinate *pos);

void Local_020035ac(struct FieldActor *object)
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
    Main_08000128(0x180000, angle, p);
    /* FAKEMATCH: reuse the frame counter as coordinate-rounding work. */
    n = p[0].fixed + 0x80000;
    x = n & -0x100000;
    n = p[2].fixed + 0x80000;
    z = n & -0x100000;
    angle += 0x8000;
    Engine_ObjectSetAnimation(object, 5);
    Engine_AudioPlayCue(184);
    for (n = 0; n < 16; n++) {
        angle += 0x400;
        p[0].fixed = x;
        p[2].fixed = z;
        Main_08000128(0x180000, angle, p);
        object->x.fixed = p[0].fixed;
        object->z.fixed = p[2].fixed;
        object->facing = angle + 0x4000;
        Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(233);
}
