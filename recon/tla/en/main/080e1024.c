/* Near miss, differ 8 (four reordered rows, same instructions): the twin of The
 * Broken Seal's BattleFx_SpawnDescendingArcParticles with this game's sprite
 * layout (resource 16, active 17, animation 26) and tile slot (0x710). The
 * listing stores the leader to its stack slot before the loop registers are
 * set, loads the pooled zero before it builds 0x6666, stores the tile word and
 * loads byte 5 before the -33 mask is built, and loads byte 9 before the -13
 * mask. No barrier was tried yet: each row is a load or store one step early. */
#include "resource.h"
#include "types.h"
#include "ram_buffer.h"

void BattleFx_UpdateDescendingParticleNegativeArc(void);
void BattleFx_UpdateDescendingParticlePositiveArc(void);

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
            u8 tileLow;
            u8 flags09_0 : 2;
            u8 variant : 2;
            u8 flags09_4 : 4;
        } byte;
    } attr;
    u8 padding0c[4];
    u8 resource;
    u8 active : 1;
    u8 flags11_1 : 7;
    u8 padding12[8];
    u8 animation;
    u8 padding1b[13];
    struct ArcFrame *frame;
};

struct ArcObject {
    u8 padding00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 unk14;
    s32 scaleX;
    s32 scaleY;
    u8 padding20[0x30];
    struct ArcSprite *sprite;
    u8 padding54;
    u8 unk55;
    u8 padding56[0x0e];
    u16 unk64;
    u8 padding66[2];
    struct ArcObject *source;
    void *callback;
};

#include "vram_tab.h"

struct ArcObject *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildArgumentFar(struct ArcSprite *sprite, s32 animation);

void Func_080e1024(struct ArcObject *source)
{
    u8 *work = Ram_HeapSlots->effectWork;
    struct ArcObject *leader = *(struct ArcObject **)(work + 16);
    struct ArcObject *objects[2];
    struct ArcObject *object;
    struct ArcSprite *sprite;
    s32 i;
    u8 zero;

    for (i = 0; i <= 1; i++) {
        object = Func_080200c0(271, source->x, source->y + 0x400000, source->z);
        objects[i] = object;
        if (object == NULL)
            continue;
        object->unk14 = source->unk14;
        sprite = object->sprite;
        object->unk55 = 0;
        object->unk64 = 0;
        zero = 0;
        object->source = source;
        object->scaleY = 0x6666;
        object->scaleX = 0x6666;
        if (sprite == NULL)
            continue;
        Animation_ApplyChildArgumentFar(sprite, 0);
        sprite->animation = zero;
        Resource_ResetEntry(sprite->resource);
        sprite->resource = *(u16 *)(work + 0x710);
        sprite->active = 1;
        sprite->attr.half.tile = ResourceTableEntries[sprite->resource].offset >> 5;
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
