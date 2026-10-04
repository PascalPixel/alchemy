/* Draft of BattleFx_DrawScaledObject (080ebc94). Matches in tla-ja and tla-en.
   tla-de/es/fr/it fail to link: their TABLE.S names the stub at 0x08020010,
   whose bytes are the same as the English Render_ApplyProjectedPlacementFar,
   ResourceObject_CreateFar. Resolve the far-call table names before adopting. */
#include "TYPES.H"
#include "EFFECT_SLOT.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"

/* The sprite object this draw reads: its own scale at 0xc. */
struct ScaledSprite {
    u8 unknown_00[0xc];
    s32 scale;
};

void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite, s32 *position, s32 *scale, s32 mode);

/* Draws an effect object at its position, scaled by its own and its
   sprite's factors, when it lies near the screen; flag 4 mirrors it about
   the floor line. */
void BattleFx_DrawScaledObject(struct EffectSlot *object)
{
    struct ScaledSprite *sprite;
    s32 offset;
    s32 scale[2];
    s32 position[4];

    sprite = object->object;
    offset = 0;
    if (object->flags & 4)
        offset = 0x1fc0000 - object->z;
    scale[0] = Iwram_MulQ16(object->scale_x, sprite->scale);
    scale[1] = Iwram_MulQ16(object->scale_y, sprite->scale);
    position[0] = object->x;
    position[1] = offset;
    position[2] = object->z + offset;
    position[3] = 0;
    if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000 && object->z < 0xe00000)
        Render_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}
