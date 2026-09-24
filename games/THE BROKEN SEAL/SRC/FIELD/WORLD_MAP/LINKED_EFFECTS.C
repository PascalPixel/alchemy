#include "TYPES.H"
#include "FIELD_EFFECT.H"

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
struct WorldMapVramBlock {
    u16 base;
    u16 offset;
};
extern struct WorldMapVramBlock gWorldMapVramBlocks[];
void WorldMap_MoveEffectDown(union FieldObject *object);
void WorldMap_MoveEffectUp(union FieldObject *object);
s32 Main_08009020(struct FieldSprite *sprite, s32 animation);
void Main_080001b8(s32 block);

/* The OAM view with attribute 1 ending in the two-bit size field. */
struct WorldMapOam {
    u8 unknown_00[4];
    u16 attr0;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

/* Spawns two effect objects linked to the parent actor, one drifting down
 * and one up, sharing the effect work VRAM block. Built into resource_371
 * and resource_373. */
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
                Main_08009020(sprite, 0);
                sprite->flags = 0;
                Main_080001b8(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                /* FAKEMATCH: a plain byte access; the struct field store
                 * leaves a dead QImode zero that takes r3 from the +85
                 * address. */
                *(u8 *)&sprite->unknown_1d |= 1;
                sprite->tile = (gWorldMapVramBlocks[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                ((struct WorldMapOam *)sprite)->size = 2;
                part->detail->field_16 = 0;
            }
        }
    }
    /* The reference dereferences both final entries without another null check. */
    {
        union PairObject *p = pair[0];
        struct FieldSprite *sp = p->object.actor.sprite;

        p->object.actor.update = WorldMap_MoveEffectDown;
        sp->priority = 1;
    }
    {
        struct FieldActor *p = &pair[1]->object.actor;
        struct FieldSprite *sp = p->sprite;

        sp->priority = 1;
        p->update = WorldMap_MoveEffectUp;
        p->priority_flags = 2;
    }
}
