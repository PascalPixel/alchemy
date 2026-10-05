#include "GLOBAL.H"
#include "CHOJO.H"

void VinasuChojo_FlashScreen(void)
{
    Engine_AudioPlayCue(187);
    Engine_ColorBufferApplyTarget(0x7fff, 1);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(4);
    Engine_ColorBufferApplyTarget(0x40250d, 1);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
}

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

void SceneEffect_UpdateOrbitAroundActor(union OrbitEffect *effect)
{
    struct FieldActor *center = Object_GetById(ORBIT_CENTER_ACTOR);
    u16 angle = effect->orbit.angle;

    effect->orbit.x = center->x.fixed + Engine_MathCos(angle) * (effect->orbit.radius + 3);
    effect->orbit.z = center->z.fixed + (Engine_MathSin(angle) << 1);
    effect->orbit.saved_x = effect->orbit.x;
    effect->orbit.saved_z = effect->orbit.z;
    effect->orbit.angle -= 0x800;
}

void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor)
{
    struct FieldActor *anchor = Object_GetById(23);
    u16 *pangle = (u16 *)(actor + 100);
    s32 angle = *pangle;
    s32 cosine;
    s32 sine;
    s32 along;
    s32 across;

    cosine = Engine_MathCos(angle);
    along = anchor->x.fixed
          + cosine *(*(s32 *)(actor + 48) + *(u8 *)(actor + 98) + 6);
    *(s32 *)(actor + 8) = along;

    sine = Engine_MathSin(angle);
    across = anchor->z.fixed
           + sine *(*(u8 *)(actor + 98) + 4);
    *(s32 *)(actor + 16) = across;

    *(s32 *)(actor + 56) = *(s32 *)(actor + 8);
    *(s32 *)(actor + 64) = across;

    {
        s32 next = *pangle;
        next = next + (s32)0xfffff800;
        *pangle = (u16)next;
    }
}


void SceneEffect_SpawnParticlesAboveActor(void)
{
    struct FieldActor *center;
    union OrbitEffect *effect;
    struct FieldSprite *sprite;
    s32 angle;

    if (GameFlag_IsSet(0x236) == 0 && Math_RemainderUnsigned(gFrameCount, 3) != 0) {
        return;
    }
    center = Actor_Get(ORBIT_CENTER_ACTOR);
    if (GameFlag_IsSet(0x236) != 0) {
        effect = (union OrbitEffect *)Object_Create(
            284, center->x.fixed,
            (s32)((u32)(Random_Next() << 8) >> 16 << 16) + center->y.fixed - 0x1c0000,
            center->z.fixed);
    } else {
        effect = (union OrbitEffect *)Object_Create(
            284, center->x.fixed,
            (s32)((u32)(Random_Next() << 6) >> 16 << 16) + center->y.fixed - 0x1c0000,
            center->z.fixed);
    }
    if (effect != 0) {
        sprite = ((struct FieldEffect *)effect)->sprite;
        Object_SetScript((struct FieldActor *)effect, VinasuChojo_OrbitParticleScript);
        ObjectGroup_SetChildValue((struct FieldActor *)effect, 1);
        ((struct FieldEffect *)effect)->motion_flags = 0;
        angle = Random_Next() & 0xffff000;
        effect->orbit.angle = angle;
        effect->orbit.unknown_66 = 0;
        ((struct FieldEffect *)effect)->update =
            (void (*)(union FieldObject *))SceneEffect_UpdateOrbitAroundActor;
        effect->orbit.radius = Engine_MathSin((u32)(Random_Next() * 0xffff) >> 20) * 24 >> 16;
        sprite->flags = 0;
        sprite->priority = 1;
    }
}
