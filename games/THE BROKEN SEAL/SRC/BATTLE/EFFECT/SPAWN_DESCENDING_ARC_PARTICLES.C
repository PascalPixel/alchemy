/* Battle effect: spawn the two particles that fall in arcs either side of
   the source object, one for each of the descending-arc update callbacks. */
#include "TYPES.H"

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

struct ResourceSlot {
    u16 id;
    u16 address;
};

extern u8 *gEffectWork;
extern struct ResourceSlot ResourceTableEntries[];

struct ArcObject *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void AnimationObjects_SelectAnimationFar(struct ArcSprite *sprite, s32 animation);
void Resource_ResetEntry(s32 id);

void BattleFx_SpawnDescendingArcParticles(struct ArcObject *source)
{
    u8 *work = gEffectWork;
    struct ArcObject *leader = *(struct ArcObject **)(work + 16);
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
        sprite->resource = *(u16 *)(work + 0x71a);
        sprite->active = 1;
        sprite->attr.half.tile = ResourceTableEntries[sprite->resource].address >> 5;
        sprite->flags05_5 = 0;
        sprite->layer = 1;
        sprite->size = 2;
        sprite->frame->visible = zero;
    }
    objects[0]->callback = (void *)0x0809b3d9;
    objects[0]->sprite->attr.byte.variant = 0;
    objects[1]->callback = (void *)0x0809b365;
    objects[1]->sprite->attr.byte.variant = leader->sprite->attr.byte.variant;
}
