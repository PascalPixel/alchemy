/* NONMATCHING: 292 bytes, candidate 292, 19 differing halfwords (2026-09-24).
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * the twin of world_map_actor_effects (371:02004058) with a cue and its own
 * update routines; the same residual: the reference computes the +85
 * motion-flags address in r3 after the +20 word copy and reaches +100 by
 * adding 15 (reload's move2add), but here local-alloc gives the +85 address
 * r1 because the zero stores expand as a load, an AND with a dead QImode zero
 * (pseudo 52) that holds r3. The tail stores also swap. Why (local-alloc
 * QTY_CMP_PRI, 2026-09-24): every constant byte/halfword store through a
 * struct field goes through store_bit_field here, so the dead mask QImode 52
 * (2 refs over 1 half-insn, priority 2.0) and the +100 address (2.0) are
 * allocated before the +85 address (4 refs over 6, 1.33); 52 takes r3 inside
 * its life and the address falls to r1. It would need 52 absent or the +85
 * address born next to its store. A byte-pointer store of motion_flags drops
 * 52 and gets the move2add, but then the zero is QImode and the halfword zero
 * separate (21); a u16 zero variable set before the loop, spilled and
 * rematerialized, also gets the move2add with the registers swapped (17).
 * Zero variables inside the loop are hoisted (+8 bytes). */
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

void Local_02002440(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = Data_03001f30;
    s32 i;

    Engine_AudioPlayCue(131);
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
    {
        union PairObject *p = pair[0];
        struct FieldSprite *sp = p->object.actor.sprite;

        p->object.actor.update = (void (*)(union FieldObject *))0x0200a3ed;
        sp->priority = 2;
    }
    {
        union PairObject *p = pair[1];
        struct FieldSprite *sp = p->object.actor.sprite;

        sp->priority = 2;
        p->object.actor.update = (void (*)(union FieldObject *))0x0200a39d;
        p->object.actor.priority_flags = 2;
    }
}
