/* The gated rise counter. */
#include "CHOJO.H"

void SceneEffect_AdvanceGatedRiseCounter(u8 *obj)
{
    if (*(u8 *)(obj + 99) != 0) {
        u8 counter = *(u8 *)(obj + 98);

        *(u32 *)(obj + 12) = *(u32 *)(obj + 76) + ((u32)(counter >> 2) << 16);

        SceneActor_ParkRecord(obj);

        {
            if (*(u8 *)(obj + 98) != 0) {
                if (*(u8 *)(obj + 98) <= 31) {
                    ++*(u8 *)(obj + 98);
                }
            }
        }
    }
}

#include "MAP_SCROLL.H"

extern struct MapScrollWork *gMapWork;
extern const s32 VinasuChojo_BesideParticleScript[];
void FieldScene_RunScene3c9_02005b90(union FieldObject *object);

/*
 * Keeps actor 23 beside the view while the view is high enough, pulsing its
 * size on alternate frames, and every sixteenth frame releases a particle
 * that circles it.
 */
void SceneEffect_SpawnParticlesBesideActor(void)
{
    struct FieldActor *center;
    struct FieldActor *effect;
    struct FieldSprite *sprite;
    struct MapScrollWork *map;
    s32 drift;
    s32 phase;
    s32 angle;

    center = Actor_Get(23);
    map = gMapWork;
    drift = (u32)(Random_Next() * 48) >> 16 << 16;
    if ((s16)(map->view_y >> 16) <= 129) {
        if (gFrameCount & 1) {
            Actor_SetPosition(23, 152 << 17, 164 << 16);
            Actor_Get(23)->scale_x = 0x10000;
            Actor_Get(23)->scale_y = 0x10000;
        } else {
            Actor_SetPosition(23, 152 << 17, 171 << 16);
            Actor_Get(23)->scale_x = 0x14ccc;
            Actor_Get(23)->scale_y = 0x14ccc;
        }
    } else {
        Actor_SetPosition(23, 0, 0);
    }
    if (center == 0)
        return;
    phase = gFrameCount & 15;
    if (phase != 0)
        return;
    effect = Object_Create(284, center->x.fixed + 0x80000, center->y.fixed + drift + 0x80000,
                                 center->z.fixed);
    drift /= 0x60000;
    drift <<= 16;
    if (effect == 0)
        return;
    sprite = effect->sprite;
    Object_SetScript(effect, VinasuChojo_BesideParticleScript);
    Object_SetPalette(effect, 5);
    effect->motion_flags = phase;
    angle = Random_Next() & 0xffff000;
    effect->unknown_64 = angle;
    effect->unknown_66 = phase;
    *(struct FieldActor **)effect->unknown_68 = center;
    effect->update = FieldScene_RunScene3c9_02005b90;
    effect->speed = Math_Sin((drift & 0xfffff) >> 4) * 24 >> 16;
    /* FAKEMATCH: the byte store outside the sprite record keeps the game's order. */
    *(u8 *)((u8 *)sprite + 38) = 0;
    sprite->priority = center->sprite->priority;
}
