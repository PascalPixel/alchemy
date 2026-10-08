#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "vram_block.h"
#include "types.h"
#include "scene_ids.h"
#include "fx_scene.h"

struct ArcParticle {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 floor;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[0x48];
    struct ArcParticle *source;
};

struct BattleEffect16GlobalState {
    u8 unknown_000[0x1DA];
    s16 scene;
};

extern struct BattleEffect16GlobalState gGameState;

void ObjectDispatch_ReleaseFar(void);

void BattleFx_UpdateDescendingParticlePositiveArc(struct ArcParticle *particle)
{
    struct ArcParticle *source;
    s32 y;
    s32 limit;
    s32 scale;

    limit = particle->floor + 0xA0000;
    source = particle->source;
    if (gGameState.scene == (s32)&SceneId_Clear)
        limit = particle->floor + 0x40000;

    y = particle->y;
    if (y <= limit) {
        ObjectDispatch_ReleaseFar();
        return;
    }

    scale = particle->scale_x + 0xC00;
    if (scale > 0x10000)
        scale = 0x10000;
    particle->scale_x = scale;
    particle->scale_y = scale;
    particle->x = source->x;
    particle->y = y - 0x20000;
    particle->z = source->z + ((0x10000 - scale) * 5) + 0x90000;
}

void BattleFx_UpdateDescendingParticleNegativeArc(struct ArcParticle *particle)
{
    struct ArcParticle *source;
    s32 y;
    s32 limit;
    s32 scale;

    limit = particle->floor + 0xA0000;
    source = particle->source;
    if (gGameState.scene == (s32)&SceneId_Clear)
        limit = particle->floor + 0x40000;

    y = particle->y;
    if (y <= limit) {
        ObjectDispatch_ReleaseFar();
        return;
    }

    scale = particle->scale_x + 0xC00;
    if (scale > 0x10000)
        scale = 0x10000;
    particle->scale_x = scale;
    particle->scale_y = -scale;
    particle->x = source->x;
    particle->y = y - 0x20000;
    particle->z = source->z - ((0x10000 - scale) * 5) + 0x100000;
}

struct ArcParticle;

struct ArcFrame {
    u8 padding00[22];
    u8 visible;
};

struct ArcSprite {
    u8 padding00[5];
    u8 flags05_0 : 5;
    u8 flags05_5 : 1;
    u8 layer : 2;
    u8 padding06;
    u8 flags07_0 : 6;
    u8 size : 2;
    union {
        struct {
            u16 tile : 10;
            u16 flags08_10 : 6;
        } half;
        struct {
            u8 tile_low;
            u8 flags09_0 : 2;
            u8 variant : 2;
            u8 flags09_4 : 4;
        } byte;
    } attr;
    u8 padding0c[16];
    u8 resource;
    u8 active : 1;
    u8 flags1d_1 : 7;
    u8 padding1e[8];
    u8 animation;
    u8 padding27;
    struct ArcFrame *frame;
};

struct ArcObject {
    u8 padding00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_14;
    s32 scale_x;
    s32 scale_y;
    u8 padding20[0x30];
    struct ArcSprite *sprite;
    u8 padding54;
    u8 unknown_55;
    u8 padding56[0x0e];
    u16 unknown_64;
    u8 padding66[2];
    struct ArcObject *source;
    void *callback;
};

extern struct BattleFxScene *gEffectWork;

struct ArcObject *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);

void BattleFx_SpawnDescendingArcParticles(struct ArcObject *source)
{
    struct BattleFxScene *scene = gEffectWork;
    struct ArcObject *leader = scene->main_object;
    struct ArcObject *objects[2];
    struct ArcObject *object;
    struct ArcSprite *sprite;
    s32 i;
    u8 zero;

    for (i = 0; i <= 1; i++) {
        object = Object_CreateFar(26, source->x, source->y + 0x400000, source->z);
        objects[i] = object;
        if (object == NULL)
            continue;
        object->unknown_14 = source->unknown_14;
        sprite = object->sprite;
        object->unknown_55 = 0;
        object->unknown_64 = 0;
        zero = 0;
        object->source = source;
        object->scale_y = 0x6666;
        object->scale_x = 0x6666;
        if (sprite == NULL)
            continue;
        AnimationObjects_SelectAnimationFar(sprite, 0);
        sprite->animation = zero;
        Resource_ResetEntry(sprite->resource);
        sprite->resource = scene->arc_tile_slot;
        sprite->active = 1;
        sprite->attr.half.tile = gVramBlockCache[sprite->resource].offset >> 5;
        sprite->flags05_5 = 0;
        sprite->layer = 1;
        sprite->size = 2;
        sprite->frame->visible = zero;
    }
    objects[0]->callback = (void *)BattleFx_UpdateDescendingParticleNegativeArc;
    objects[0]->sprite->attr.byte.variant = 0;
    objects[1]->callback = (void *)BattleFx_UpdateDescendingParticlePositiveArc;
    objects[1]->sprite->attr.byte.variant = leader->sprite->attr.byte.variant;
}
