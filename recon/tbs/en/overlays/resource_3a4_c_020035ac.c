/* NONMATCHING H1 witness (rejected): 188/188 bytes, 67 differing halfwords,
 * 46 aligned edits (2026-09-27). Baseline: 188/188, 5 halfwords/5 edits.
 * Whole extent [020035ac,02003668), pool included. Independent twin extent
 * [02003668,02003724) also scores 188/188, 5 halfwords/5 edits.
 * ROLL_OBJECT.C dispatches these opposite 16-frame turns on tiles 98/97.
 * ROLL_STEP.C and the canonical polar callee agree on the mutable three-word
 * position and void call; animation, cue and wait calls also match their ABI.
 * H1 separates fixed snapped centre into a two-coordinate record from the
 * mutable polar-position buffer. Frame stays 12, but centre occupies r6/r7,
 * displacing object/angle into high registers and moving mask operations.
 * Complete normalized diff rejects that aggregate ownership model; do not
 * tune its allocation. Negative witness retained before restoring baseline.
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

struct RollCenter {
    s32 x;
    s32 z;
};

void Local_020035ac(struct FieldActor *object)
{
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;
    s32 angle;
    struct RollCenter center;
    s32 n;

    angle = (object->facing + 0x4000) & 0xc000;
    p = pos;
    p[0].fixed = object->x.fixed;
    p[1].fixed = object->y.fixed;
    p[2].fixed = object->z.fixed;
    Main_08000128(0x180000, angle, p);
    center.x = (p[0].fixed + 0x80000) & 0xfff00000;
    center.z = (p[2].fixed + 0x80000) & 0xfff00000;
    angle += 0x8000;
    Engine_ObjectSetAnimation(object, 5);
    Engine_AudioPlayCue(184);
    for (n = 15; n >= 0; n--) {
        angle += 0x400;
        p[0].fixed = center.x;
        p[2].fixed = center.z;
        Main_08000128(0x180000, angle, p);
        object->x.fixed = p[0].fixed;
        object->z.fixed = p[2].fixed;
        object->facing = angle + 0x4000;
        Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(233);
}
