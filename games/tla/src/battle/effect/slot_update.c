#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "effect_slot.h"
#include "iwram_call.h"

void EffectSlot_Update(struct EffectSlot *effect)
{
    if (effect->active != 0)
    {
        effect->age++;
        if (effect->callbackDelay != 0)
            effect->callbackDelay--;
        else if (effect->callback != 0)
            effect->callback(effect);
        if (effect->active != 0)
        {
            if (effect->updateMotion != 0)
                EffectSlot_UpdateMotion(effect);
            if (effect->render != 0)
                BattleFx_DrawScaledObject(effect);
        }
    }
}

struct ScaledSprite
{
    u8  unk00[0xc];
    s32 scale;
};

void BattleFx_DrawScaledObject(struct EffectSlot *object)
{
    struct ScaledSprite *sprite;
    s32                  offset;
    s32                  scale[2];
    s32                  position[4];

    sprite = object->object;
    offset = 0;
    if (object->flags & 4)
        offset = 0x1fc0000 - object->z;
    scale[0] = Iwram_MulQ16(object->scaleX, sprite->scale);
    scale[1] = Iwram_MulQ16(object->scaleY, sprite->scale);
    position[0] = object->x;
    position[1] = offset;
    position[2] = object->z + offset;
    position[3] = 0;
    if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000
        && object->z < 0xe00000)
        Render_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}
