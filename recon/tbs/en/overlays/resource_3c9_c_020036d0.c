/* NONMATCHING: resource_3c9 0x0200b6d0 (244 bytes with its pool),
 * SceneEffect_SpawnParticlesAboveActor, meant for the end of
 * FIELD/VINASU_CHOJO/ORBIT.C (which also needs OrbitEffect's u16
 * unknown_66 after angle), stays listing. Its script is the listing's
 * VinasuChojo_OrbitParticleScript.
 *
 * Remaining difference: one instruction. After the null check the game
 * copies the new effect back into r0 (adds r0, r7, #0) before the script
 * call; GCC still knows r0 holds it and drops the copy, so the body is two
 * bytes short and loses its alignment halfword. Everything else matches,
 * including the pooled zero kept in r8 for sprite->flags. Tried: inline and
 * direct create and script calls, Call2 and Value4 wrappers, if/else, goto
 * and do-while forms, sprite load order, and 50k permuter candidates.
 * 2026-09-29 (Mars): -dg shows the copy (set r0 r7) survives local-alloc
 * and dies in greg: reload_cse's cselib still knows r0 == r7 from the
 * `adds r7, r0, #0`. The game's copy needs r0 unknown there, as a CODE_LABEL
 * between copy and use gives (UiText_PrepareMessageWork keeps the same copy
 * after its have_work join). A label kept alive by a static &&label table
 * restores exactly `ldr r1; adds r0, r7, #0; ldr r6` but emits rodata, so it
 * is not admissible; unused labels, (void)&&label, a dead label-address
 * local, switch, while/break and goto forms are all deleted before reload.
 * A long long return cast also restores the copy but spills through the
 * stack. Next idea: a real join label reached from a path jump2 removes.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/VINASU_CHOJO/CHOJO.H"

union OrbitEffect {
    s32 words[26];
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

enum {
    ORBIT_CENTER_ACTOR = 24
};

void SceneEffect_UpdateOrbitAroundActor(union OrbitEffect *effect);
extern const s32 VinasuChojo_OrbitParticleScript[];

void SceneEffect_SpawnParticlesAboveActor(void)
{
    struct FieldActor *center;
    union OrbitEffect *effect;
    struct FieldSprite *sprite;
    u32 rise;
    s32 angle;

    if (GameFlag_IsSet(0x236) == 0 && IwramUnsignedRemainder(gFrameCount, 3) != 0)
        return;
    center = Actor_Get(ORBIT_CENTER_ACTOR);
    if (GameFlag_IsSet(0x236) != 0) {
        rise = Random_Next();
        rise <<= 8;
    } else {
        rise = Random_Next();
        rise <<= 6;
    }
    effect = (union OrbitEffect *)Engine_ObjectCreate(284, center->x.fixed,
                                                (s32)(rise >> 16 << 16) + center->y.fixed + (s32)0xffe40000,
                                                center->z.fixed);
    if (effect != 0) {
        sprite = ((struct FieldEffect *)effect)->sprite;
        Object_SetScript((struct FieldActor *)effect, VinasuChojo_OrbitParticleScript);
        Object_SetPalette((struct FieldActor *)effect, 1);
        ((struct FieldEffect *)effect)->motion_flags = 0;
        angle = Random_Next() & 0xffff000;
        effect->orbit.angle = angle;
        effect->orbit.unknown_66 = 0;
        ((struct FieldEffect *)effect)->update = (void (*)(union FieldObject *))SceneEffect_UpdateOrbitAroundActor;
        effect->orbit.radius = Math_Sin((u32)(Random_Next() * 0xffff) >> 20) * 24 >> 16;
        sprite->flags = 0;
        sprite->priority = 1;
    }
}
