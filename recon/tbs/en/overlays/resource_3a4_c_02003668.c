/* NONMATCHING: 188 of 188 bytes, 5 halfword edits (2026-09-24). Hand-written
 * from the resolved disassembly as a single-overlay unit binding Engine_* and
 * Main_08000128 at their import veneers. Everything matches except global
 * allocation of the three call-crossing locals: the reference gives the loop
 * counter r8, the snapped x r9 and the snapped z sl; this draft gives z r8
 * and the counter sl (greg sorts z ahead of the counter). Twin shape of
 * resource_3a4:020035ac (the other turn direction); whatever fixes one fixes
 * both. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Main_08000128(s32 distance, s32 angle, union FieldCoordinate *pos);

void Local_02003668(struct FieldActor *object)
{
    union FieldCoordinate pos[3];
    union FieldCoordinate *p;
    s32 angle;
    s32 x;
    s32 z;
    s32 n;

    angle = (object->facing - 0x4000) & 0xc000;
    p = pos;
    p[0].fixed = object->x.fixed;
    p[1].fixed = object->y.fixed;
    p[2].fixed = object->z.fixed;
    Main_08000128(0x180000, angle, p);
    x = (p[0].fixed + 0x80000) & 0xfff00000;
    z = (p[2].fixed + 0x80000) & 0xfff00000;
    angle += 0x8000;
    Engine_ObjectSetAnimation(object, 6);
    Engine_AudioPlayCue(184);
    for (n = 15; n >= 0; n--) {
        angle -= 0x400;
        p[0].fixed = x;
        p[2].fixed = z;
        Main_08000128(0x180000, angle, p);
        object->x.fixed = p[0].fixed;
        object->z.fixed = p[2].fixed;
        object->facing = angle - 0x4000;
        Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(233);
}
