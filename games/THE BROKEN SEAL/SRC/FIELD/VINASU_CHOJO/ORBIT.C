/* The screen flash and orbiting effects. */
#include "CHOJO.H"

void VinasuChojo_FlashScreen(void)
{
    Audio_PlayCue(187);
    ColorBuffer_ApplyTarget(0x7fff, 1);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(4);
    ColorBuffer_ApplyTarget(0x40250d, 1);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
}

/* An effect that circles an actor. */
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
    } orbit;
};

enum {
    ORBIT_CENTER_ACTOR = 24
};

/*
 * Places the effect on an ellipse around actor 24, its radius plus 3 across
 * and two sine units deep, keeps a copy of the new position, then steps the
 * angle back by a thirty-second of a turn.
 */
void SceneEffect_UpdateOrbitAroundActor(union OrbitEffect *effect)
{
    struct FieldActor *center = Actor_Get(ORBIT_CENTER_ACTOR);
    u16 angle = effect->orbit.angle;

    effect->orbit.x = center->x.fixed + Engine_MathCos(angle) * (effect->orbit.radius + 3);
    effect->orbit.z = center->z.fixed + (Engine_MathSin(angle) << 1);
    effect->orbit.saved_x = effect->orbit.x;
    effect->orbit.saved_z = effect->orbit.z;
    effect->orbit.angle -= 0x800;
}

/*
 * Per-frame orbit step for one actor: read the binary angle at +100, place
 * the actor on a circle around scene record 23, mirror the placement into
 * +56/+64, and advance the angle by -0x800, a thirty-second of a turn. The
 * two arms use deliberately different radius terms -- the +98 counter enters
 * both -- and must not be unified. The actor layout is raw offsets: nothing
 * establishes which of +8 and +16 is which world axis, so they are unnamed.
 */
void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor)
{
    u8 *anchor = Actor_Get(23);
    u16 *pangle = (u16 *)(actor + 100);
    s32 angle = *pangle;
    s32 cosine;
    s32 sine;
    s32 along;
    s32 across;

    cosine = Engine_MathCos(angle);
    along = *(s32 *)(anchor + 8)
          + cosine *(*(s32 *)(actor + 48) + *(u8 *)(actor + 98) + 6);
    *(s32 *)(actor + 8) = along;

    sine = Engine_MathSin(angle);
    across = *(s32 *)(anchor + 16)
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
