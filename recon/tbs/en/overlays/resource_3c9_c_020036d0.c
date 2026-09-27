/* NONMATCHING: 238 of 244 bytes, 73 differing halfwords, 16 halfword edits
 * (2026-09-27). Full-width Value_0ffff000 restores the separate field-store
 * addresses and mask register. The plain sprite-flags zero retains r8 and
 * both pool boundaries. Remaining: the object-type shift is early, the
 * script call lacks the reference's r7-to-r0 copy, and the tail is short.
 * Three structural hypotheses tested: link constants (retained mask only;
 * a symbolic zero loses r8 and the first pool), integer object return
 * (unchanged), Object_Create inline helper (234 bytes, 28 edits). Missing
 * flag/script bindings restored from the overlay's own import and pool.
 *
 * 2026-09-27 one bounded family-ownership model:
 * Exact SCRIPTED_PRESENTATION.C:SceneEffect_UpdateOrbitAroundActor at
 * 02003600 consumes +30 as integer orbit radius and +64 as binary angle.
 * Transfer that view into the spawn owner and bind the existing callback
 * name at runtime 0200b600. Own RUNTIME_DIVIDE.S/DIVISION_ENTRY.S prove
 * 030003e0 is unsigned remainder; gFrameCount is u32, so ordinary % 3
 * binds __umodsi3 to the same 0200dbf4 import instead of a signed prototype.
 * Result: 238/73/16, byte-identical to the retained baseline. The full
 * normalized diff and both pool boundaries were checked. This clarifies
 * module ownership but does not repair factory type scheduling, the script
 * argument copy or the short tail. Stop with no second spelling trial and
 * no DONE credit. No exact consumer or shared header was changed.
 *
 * 2026-09-27 bounded producer/consumer boundary audit:
 * Own full listing and canonical allocator dumps locate the missing script
 * argument copy precisely: r0 = spark survives through local allocation,
 * then reload CSE deletes it, forwarding the factory return across the
 * fall-through null check. The approved compiler forgets those equivalences
 * at labels, not conditional fall-throughs. A tagged one-pass boundary
 * around factory-result publication was tested using the exact world-map
 * DISPLAY_TRANSITION.C boundary precedent. Result: 238/73/16, binary-identical
 * to baseline; its exit label is gone before reload and the copy is still
 * deleted. Full normalized diff retains r8 zero and both pool shapes.
 * The second pool remains four bytes early: the absent two-byte copy plus
 * two-byte pool alignment account for that displacement. The reference also
 * ends with a two-byte zero pad after bx r0; no setup call or tail operation
 * is absent. The factory type shift still precedes x/z loads, rather than
 * following them. This boundary trial did not satisfy its admission check.
 * Failed model preserved at 5270c6cd7; restore the simpler baseline here.
 * Stop without a second spelling/prototype trial; zero DONE credit. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern const s32 VinasuChojo_SparkScript[];
extern u8 Value_0ffff000;

/* The orbit callback consumes these words as radius and angle, rather than
 * walking speed and actor state. Its ordinary actor prefix owns the sprite. */
union OrbitEffect {
    struct FieldActor actor;
    struct {
        u8 unknown_00[8];
        s32 x;
        s32 y;
        s32 z;
        u8 unknown_14[0x1c];
        s32 radius;
        u8 unknown_34[4];
        s32 saved_x;
        s32 unknown_3c;
        s32 saved_z;
        u8 unknown_44[0x20];
        u16 angle;
        u16 unknown_66;
    } orbit;
};

void SceneEffect_UpdateOrbitAroundActor(union OrbitEffect *effect);

void Func_020036d0(void)
{
    struct FieldActor *source;
    union OrbitEffect *spark;
    struct FieldSprite *sprite;
    s32 rise;

    if (!GameFlag_IsSet(0x236) && gFrameCount % 3 != 0)
        return;
    source = Engine_ActorGet(24);
    if (GameFlag_IsSet(0x236)) {
        rise = Engine_RandomNext();
        rise <<= 8;
    } else {
        rise = Engine_RandomNext();
        rise <<= 6;
    }
    spark = (union OrbitEffect *)Engine_ObjectCreate(
        0x11c, source->x.fixed, ((u32)rise >> 16 << 16) + source->y.fixed - 0x1c0000,
        source->z.fixed);
    if (spark != 0) {
        sprite = spark->actor.sprite;
        Engine_ObjectSetScript(&spark->actor, VinasuChojo_SparkScript);
        Engine_ObjectSetPalette(&spark->actor, 1);
        spark->actor.motion_flags = 0;
        spark->orbit.angle = Engine_RandomNext() & (u32)&Value_0ffff000;
        spark->orbit.unknown_66 = 0;
        spark->actor.update = (void (*)(union FieldObject *))SceneEffect_UpdateOrbitAroundActor;
        spark->orbit.radius = Engine_MathSin((u32)(Engine_RandomNext() * 0xffff) >> 20) * 24 >> 16;
        sprite->flags = 0;
        sprite->priority = 1;
    }
}
