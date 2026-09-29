/* NONMATCHING: 188 of 188 bytes, 5 halfword edits (2026-09-24). Hand-written
 * from the resolved disassembly as a single-overlay unit binding Engine_* and
 * Vector_AddPolarOffset at their import veneers. Everything matches except global
 * allocation of the three call-crossing locals: the reference gives the loop
 * counter r8, the snapped x r9 and the snapped z sl; this draft gives z r8
 * and the counter sl (greg sorts z ahead of the counter). Twin shape of
 * resource_3a4:020035ac (the other turn direction); prove it independently
 * before adopting any shared implementation.
 * 2026-09-27 audit: whole extent [02003668,02003724), including three-word
 * pool, independently reproduces 188/188 bytes, 5 halfwords/5 aligned edits.
 * Exact ROLL_OBJECT.C dispatches this on tile 97 (opposite turn on tile 98).
 * Canonical polar/animation/cue/wait interfaces agree. Companion H1 fixed-
 * centre record regressed to 67 halfwords/46 edits; it was not propagated
 * here; witness 5be878f9a. H2: compiler check_dbra_loop supports count-only
 * forward-loop reversal despite calls. Forward 0..15 iteration is retained
 * but independently produces exactly the original candidate bytes: same
 * five-edit n/z register exchange. Full frame, body, calls and pool checked.
 * Stop centre-record and loop-direction axes; do not sweep declarations.
 * Complete owner/unit registration is not adoption or credit.
 * 2026-09-27 Sol lane audit: independently scored complete 188-byte extent,
 * including all three pool words: five halfwords/five aligned edits,
 * topology equal. Companion counter-reuse witness afb77e41c is identical
 * to its prior bytes, so it is not propagated to this canonical twin.
 * DRIFT/Haidia lookup-lifetime witnesses have no analogous disagreement in
 * this parameter-owned actor model. No new axis; leave C not yet written.
 * Alignment and ownership unchanged.
 * 2026-09-27 H4 sign audit: (u16 facing - 0x4000) & 0xc000 gives the
 * same four quarter turns as the complete ROM mask. After +0x8000,
 * start - (n + 1) * 0x400 yields 0x4000..0x13c00 for n=0..15, without
 * s32 overflow; each facing store truncates angle - 0x4000 to u16.
 * Companion frame-produced-angle trial a0d14b1cb failed its mandatory
 * register/loop gate (55 halfwords/45 edits, n still sl and z still r8).
 * The first owner is not exact, so no twin trial is authorized by that
 * conditional gate. Canonical body remains unchanged; both owners are C
 * not yet written. Stop the one model without type/permutation variants. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Vector_AddPolarOffset(s32 distance, s32 angle, union FieldCoordinate *pos);

void ArutinYama_TurnRollingObjectB(struct FieldActor *object)
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
    Vector_AddPolarOffset(0x180000, angle, p);
    x = (p[0].fixed + 0x80000) & 0xfff00000;
    z = (p[2].fixed + 0x80000) & 0xfff00000;
    angle += 0x8000;
    Engine_ObjectSetAnimation(object, 6);
    Engine_AudioPlayCue(184);
    for (n = 0; n < 16; n++) {
        angle -= 0x400;
        p[0].fixed = x;
        p[2].fixed = z;
        Vector_AddPolarOffset(0x180000, angle, p);
        object->x.fixed = p[0].fixed;
        object->z.fixed = p[2].fixed;
        object->facing = angle - 0x4000;
        Engine_TaskWait(1);
    }
    Engine_AudioPlayCue(233);
}
