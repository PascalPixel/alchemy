#include "TYPES.H"
#include "FIELD_EFFECT.H"

#define WorldMap_CreateLinkedEffects Func_02004058

struct PairDetail {
    u8 unknown_00[22];
    u8 field_16;
};

struct PairSprite {
    struct FieldSprite sprite;
    struct PairDetail *detail;
};

union PairObject {
    union FieldObject object;
    s32 words[28];
    struct {
        u8 unknown_00[0x68];
        union PairObject *parent;
    } link;
};

struct PairWork {
    u8 unknown_00[70];
    u16 vram_block;
};

LAYOUT_OFFSET_GUARD(PairSprite_Detail, struct PairSprite, detail, 0x28);
LAYOUT_OFFSET_GUARD(PairObject_Parent, union PairObject, link.parent, 0x68);

extern struct PairWork *gWorldMapEffectWork;
extern u16 gWorldMapVramBlocks[][2];
void WorldMap_MoveEffectDown(union FieldObject *object);
void WorldMap_MoveEffectUp(union FieldObject *object);
s32 Func_0200c1f4(struct FieldSprite *sprite, s32 animation);
void Func_0200c1dc(s32 block);

void WorldMap_CreateLinkedEffects(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = gWorldMapEffectWork;
    s32 i;

    for (i = 0; i < 2; ++i) {
        child = (union PairObject *)Engine_ObjectCreate(26,
            parent->object.actor.x.fixed, parent->object.actor.y.fixed,
            parent->object.actor.z.fixed);
        pair[i] = child;
        if (child != NULL) {
            child->words[5] = parent->words[5];
            part = (struct PairSprite *)child->object.actor.sprite;
            child->object.actor.motion_flags = 0;
            child->object.effect.spin = 0;
            child->link.parent = parent;
            if (part != NULL) {
                sprite = &part->sprite;
                Func_0200c1f4(sprite, 0);
                sprite->flags = 0;
                Func_0200c1dc(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                sprite->unknown_1d |= 1;
                sprite->tile = (gWorldMapVramBlocks[sprite->vram_block][1] >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                sprite->flip_x = 0;
                sprite->flip_y = 1;
                part->detail->field_16 = 0;
            }
        }
    }
    /* The reference dereferences both final entries without another null check. */
    pair[0]->object.actor.update = WorldMap_MoveEffectDown;
    pair[0]->object.actor.sprite->priority = 1;
    pair[1]->object.actor.update = WorldMap_MoveEffectUp;
    pair[1]->object.actor.sprite->priority = 1;
    pair[1]->object.actor.priority_flags = 2;
}
