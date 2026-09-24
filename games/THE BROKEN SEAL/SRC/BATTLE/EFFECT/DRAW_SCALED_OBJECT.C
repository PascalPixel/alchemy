#include "IWRAM_CALL.H"

struct EffectSprite {
    u8 unknown_00[24];
    s32 scale;
};

struct EffectObject {
    struct EffectSprite *sprite;
    s32 x;
    s32 y;
    u8 unknown_0c[28];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_30[23];
    u8 flags;
};

void Object_ApplyProjectedPlacementFar(struct EffectSprite *sprite, s32 *position, s32 *scale, s32 mode);

/* Draws an effect object at its position, scaled by its own and its
   sprite's factors, when it lies near the screen; flag 4 mirrors it about
   the floor line. */
void BattleFx_DrawScaledObject(struct EffectObject *object)
{
    struct EffectSprite *sprite;
    s32 offset;
    s32 scale[2];
    s32 position[4];

    sprite = object->sprite;
    offset = 0;
    if (object->flags & 4)
        offset = 0x1fc0000 - object->y;
    scale[0] = Iwram_MulQ16(object->scale_x, sprite->scale);
    scale[1] = Iwram_MulQ16(object->scale_y, sprite->scale);
    position[0] = object->x;
    position[1] = offset;
    position[2] = object->y + offset;
    position[3] = 0;
    if (object->x > -0x200000 && object->x < 0x1100000 && object->y > -0x200000 && object->y < 0xe00000)
        Object_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}
