/* Parking an actor record. */
#include "CHOJO.H"

/*
 * Park a record: stamp the sentinel 0x80000000 into the three mirror fields at
 * +56, +60 and +64, zero the three at +36, +40 and +44, and clear the u16
 * angle at +100.  The 24-byte owner at 0x02005688 has no prologue, no stack
 * use and no literal pool.  0x80000000 is read as a sentinel because it is
 * stamped into three position-family fields at once and then tested for
 * equality; the roles of +36, +40 and +44 are not established.
 */
void SceneActor_ParkRecord(u8 *record)
{
    /* The sentinel is built as 128 shifted left by 24, not pooled. */
    *(s32 *)(record + 56) = (s32)0x80000000;
    *(s32 *)(record + 60) = (s32)0x80000000;
    *(s32 *)(record + 64) = (s32)0x80000000;

    *(s32 *)(record + 36) = 0;
    *(s32 *)(record + 40) = 0;
    *(s32 *)(record + 44) = 0;

    *(u16 *)(record + 100) = 0;
}

extern s32 VinasuChojo_TransitionStep;
extern const u8 VinasuChojo_RiseActionScript[];
extern const s32 VinasuChojo_RiseParticleScript[];
void SceneEffect_UpdateCounterDrivenOrbit(u8 *actor);
void SceneEffect_AdvanceGatedRiseCounter(u8 *obj);

/*
 * The summit's rising transition, one step a frame: the flash and scroll
 * of its first steps, actor 23 rising from below the view while particles
 * stream up beside it, then the white flash and the flag that ends it. The
 * party's rise counters advance every frame.
 */
void VinasuChojo_RunTransitionStep(void)
{
    struct FieldActor *center;
    struct FieldActor *effect;
    struct FieldSprite *sprite;
    s32 spawn;
    u32 rise;
    s32 angle;

    center = Actor_Get(23);
    spawn = 0;
    switch (VinasuChojo_TransitionStep) {
    case 0:
        Audio_PlayCue(220);
        Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
        ColorBuffer_ApplyTarget(0x2063ff, 1);
        ColorBuffer_Interpolate(8);
        break;
    case 8:
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(8);
        break;
    case 16:
        Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
        break;
    case 24:
        center->x.fixed = 152 << 17;
        center->y.fixed = -0x1680000;
        center->z.fixed = 164 << 16;
        center->scale_x = 0x10000;
        center->scale_y = 0x10000;
        SceneActor_ParkRecord((u8 *)center);
        Actor_EnableActionCallback(23, VinasuChojo_RiseActionScript);
        break;
    case 25:
        VinasuChojo_TransitionStep--;
        if (center->y.fixed > 0) {
            ColorBuffer_ApplyTarget(0x203210, 0);
            ColorBuffer_Interpolate(16);
            VinasuChojo_TransitionStep++;
            Actor_Get(0)->rise_counter = 1;
            Actor_Get(1)->rise_counter = 1;
            Actor_Get(2)->rise_counter = 1;
            Actor_Get(3)->rise_counter = 1;
            Actor_Get(21)->rise_counter = 1;
            Actor_Get(6)->rise_counter = 1;
        } else {
            if ((gFrameCount & 7) == 0)
                Audio_PlayCue(246);
            spawn = 1;
            center->y.fixed += 0x24000;
        }
        break;
    case 26:
        VinasuChojo_TransitionStep--;
        if (center->y.fixed > (160 << 14)) {
            ColorBuffer_ApplyTarget(0x10000, 0);
            ColorBuffer_Interpolate(40);
            VinasuChojo_TransitionStep++;
        } else {
            if ((gFrameCount & 7) == 0)
                Audio_PlayCue(246);
            spawn = 1;
            center->y.fixed += 0x24000;
        }
        break;
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
        spawn = 1;
        break;
    case 36:
        Audio_PlayCue(187);
        ColorBuffer_ApplyTarget(0x7fff, 0);
        ColorBuffer_Interpolate(12);
        break;
    case 48:
        Actor_Stop(23);
        GameFlag_Set(0x237);
        break;
    }
    if (spawn) {
        rise = ((u32)(Random_Next() * 80) >> 16) << 16;
        effect = Object_Create(284, center->x.fixed, center->y.fixed - rise + (s32)0xfff80000, center->z.fixed);
        if (effect != 0) {
            sprite = effect->sprite;
            Object_SetScript(effect, VinasuChojo_RiseParticleScript);
            Object_SetPalette(effect, 1);
            effect->motion_flags = 0;
            angle = Random_Next() & 0xffff000;
            effect->unknown_64 = angle;
            effect->unknown_66 = 0;
            effect->rise_counter = (u32)Random_Next() >> 13;
            effect->update = (void (*)(union FieldObject *))SceneEffect_UpdateCounterDrivenOrbit;
            effect->speed = Math_Sin((u32)(Random_Next() * 0xffff) >> 20) * 24;
            effect->speed = center->speed >> 16;
            sprite->flags = 0;
            sprite->priority = 1;
        }
    }
    VinasuChojo_TransitionStep++;
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(0));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(1));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(2));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(3));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(21));
    SceneEffect_AdvanceGatedRiseCounter((u8 *)Actor_Get(6));
}
