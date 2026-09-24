/* Draft, not exact (2026-09-24): 15 differing halfwords, 312 of 312 bytes.
   Storing literal zeros to sprite->frame and child->phase lets GCC keep
   the byte zero in r8, loaded from the mid-loop pool, as the reference
   does (a zero local cost 121 halfwords). Residual: the reference derives
   &object->step from &object->state (adds r3, #15) after the state store;
   here the two addresses are computed together into two registers. */
#include "TYPES.H"

struct ArcChild {
    u8 pad00[22];
    u8 phase;
};

struct ArcSprite {
    u8 pad00[4];
    u16 y:8;
    u16 affine:1;
    u16 double_size:1;
    u16 mode:2;
    u16 mosaic:1;
    u16 color:1;
    u16 shape:2;
    u16 x:9;
    u16 unused:3;
    u16 hflip:1;
    u16 vflip:1;
    u16 size:2;
    u16 tile:10;
    u16 priority:2;
    u16 palette:4;
    u8 pad0a[18];
    u8 resource;
    u8 flags;
    u8 pad1e[8];
    u8 frame;
    u8 pad27;
    struct ArcChild *child;
};

struct ArcObject {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 scale_y;
    s32 scale_x;
    u8 pad20[0x23 - 0x20];
    u8 layer;
    u8 pad24[0x50 - 0x24];
    struct ArcSprite *sprite;
    u8 pad54[1];
    u8 state;
    u8 pad56[0x64 - 0x56];
    u16 step;
    u8 pad66[2];
    struct ArcObject *link;
    void (*update)(struct ArcObject *);
};

struct VramEntry {
    u16 base;
    u16 unknown:5;
    u16 tile:10;
    u16 last:1;
};

struct ArcScene {
    u8 pad00[16];
    struct ArcObject *source;
    u8 pad14[70 - 20];
    u16 resource;
};

extern struct ArcScene *Data_03001f30;
extern struct VramEntry Data_03001b10[];

struct ArcObject *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void AnimationObjects_SelectAnimationFar(struct ArcSprite *sprite, s32 animation);
void Resource_ResetEntry(u32 index);
void BattleFx_UpdateScaledArcObjectA(struct ArcObject *obj);
void BattleFx_UpdateScaledArcObjectB(struct ArcObject *obj);

void BattleFx_SpawnScaledArcObjects(struct ArcObject *link)
{
    struct ArcScene *scene = Data_03001f30;
    struct ArcObject *source = scene->source;
    struct ArcObject *objects[2];
    struct ArcObject *object;
    struct ArcSprite *sprite;
    s32 i;

    for (i = 0; i <= 1; i++) {
        object = Object_CreateFar(26, link->x, link->y, link->z);
        objects[i] = object;
        if (object == 0)
            continue;
        object->w = link->w;
        object->state = 0;
        object->step = 0;
        sprite = object->sprite;
        object->link = link;
        object->scale_y = object->scale_x = 0x1999;
        if (sprite == 0)
            continue;
        AnimationObjects_SelectAnimationFar(sprite, 0);
        sprite->frame = 0;
        Resource_ResetEntry(sprite->resource);
        sprite->resource = scene->resource;
        sprite->flags |= 1;
        sprite->tile = Data_03001b10[sprite->resource].tile;
        sprite->color = 0;
        sprite->shape = 1;
        sprite->size = 2;
        sprite->child->phase = 0;
    }
    objects[0]->update = BattleFx_UpdateScaledArcObjectB;
    objects[0]->sprite->priority = 0;
    objects[1]->update = BattleFx_UpdateScaledArcObjectA;
    objects[1]->sprite->priority = source->sprite->priority;
    objects[1]->layer = 2;
}
