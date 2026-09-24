/* NONMATCHING: 320 bytes, candidate 324, 129 differing halfwords
 * (2026-09-24). Single-overlay unit binding Engine_* at their import veneers.
 * Remaining: a third member of the linked-effect family (371:02004058,
 * 374:02002440) with the same +85/+100 residual: in the reference the +85
 * address takes r3 and reload derives +100 by adding 15; here regclass
 * prefers STACK_REG for the +85 pseudo, so local-alloc leaves it and it lands
 * in r1. The tail stores of the second effect are also ordered differently. */
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

extern struct PairWork *Data_03001f30;
struct WorldMapVramBlock {
    u16 base;
    u16 offset;
};
extern struct WorldMapVramBlock Data_03001b10[];
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

void Local_02002eb0(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = Data_03001f30;
    s32 i;

    Engine_AudioPlayCue(292);
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
                sprite->unknown_1d |= 1;
                sprite->tile = (Data_03001b10[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                ((struct WorldMapOam *)sprite)->size = 2;
                part->detail->field_16 = 0;
            }
        }
    }
    pair[0]->object.actor.update = (void (*)(union FieldObject *))0x0200ae5d;
    pair[0]->object.actor.sprite->priority = Engine_ActorGet(15)->sprite->priority;
    {
        union PairObject *p = pair[1];

        p->object.actor.sprite->priority = Engine_ActorGet(15)->sprite->priority;
        p->object.actor.update = (void (*)(union FieldObject *))0x0200ae0d;
        p->object.actor.priority_flags = 2;
    }
}
