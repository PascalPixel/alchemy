#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "probe.h"


void OverlayObject_SpawnKind24AtObject(struct FieldActor *src)
{
    struct FieldActor *obj;
    struct FieldSprite *sprite;

    obj = Engine_ObjectCreate(24, src->x.fixed, src->y.fixed, src->z.fixed);
    if (obj == 0) {
        return;
    }

    sprite = obj->sprite;
    Engine_ObjectSetScript(obj, MakyuriHeya_RiseScript);

    obj->motion_flags = 0;
    obj->terrain_id = 1;
    obj->priority_flags = 2;

    if (sprite != 0) {
        AnimationObjects_SelectAnimation(sprite, 2);
        sprite->flags = 0;
        ((struct FieldSpritePriority *)sprite)->priority = 3;
    }
}

s32 SceneEffect_AdvanceAnchoredRiseFrame(struct AnchoredEffect *work)
{
    struct EffectAnchor *src = work->f104;
    s16 step = ++work->f100;
    s32 seed;

    if (step > 31) {
        return 0;
    }

    seed = Engine_MathSin(step << 10);
    work->f24 = seed;
    work->f28 = seed;
    work->f8 = src->f8;
    work->f12 += 0x10000;
    work->f16 = src->f16;
    return 1;
}

s32 SceneEffect_AdvanceScaleOverSixteenFrames(struct FieldActor *o)
{
    u16 *c = &o->timer;
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    o->scale_x = (n * 3) << 10;
    o->scale_y = (n * 3) << 10;
    return 1;
}

s32 OverlayObject_StepScaleUpSixteenFrames(struct FieldActor *o)
{
    u16 *c = &o->timer;
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    o->scale_x = (n << 11) + 0x10000;
    o->scale_y = (n << 11) + 0x10000;
    return 1;
}
