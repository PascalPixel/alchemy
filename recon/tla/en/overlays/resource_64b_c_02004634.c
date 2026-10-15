/* Draft of Func_02004634 (overlay 64B, 120 instructions), differ 69: the linked pair
 * of effects, ported from ☀️'s WorldMap_CreateLinkedEffects (⚓️ creates kind 271,
 * its updates are Func_020045e4 and Func_02004598, the sprite's detail sits at 0x28).
 * As a for loop (differ 95) loop.c hoists the 63 mask and the pair array's address
 * and a fourth high register is saved; as a goto loop the mask stays inside and the
 * prologue is the native's (71; 69 with the create through a forwarding inline). Left: the pair array's address is still copied
 * before the loop (the native copies sp to r8 after each create), the motion flags
 * and the action are stored through one pointer there and two here, the zero for
 * next_frame and the detail byte is the native's pool halfword but lands in r8 not
 * r10, and the tile field loads its mask first where the native loads the halfword
 * first. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "vram_tab.h"

struct LinkedDetail {
    u8 unknown_00[22];
    u8 field_16;
};

struct LinkedSprite {
    struct FieldSprite sprite;
    struct LinkedDetail *detail;
};

struct LinkedEffectWork {
    u8 unknown_000[0x7c0];
    u16 vram_block;
};

struct WorldMapOam {
    u8 unknown_00[6];
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

struct ObjectRuntime *Func_02004b30(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildArgument(void *sprite, s32 argument);
void Resource_ResetEntry(s32 entry);
void Func_02004598(struct ObjectRuntime *object);
void Func_020045e4(struct ObjectRuntime *object);

static __inline__ struct ObjectRuntime *Func_02004b30_(s32 kind, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline, as Func_020039f4 needs (71 direct). */
    return Func_02004b30(kind, x, y, z);
}

void Func_02004634(struct ObjectRuntime *parent)
{
    struct ObjectRuntime *pair[2];
    struct ObjectRuntime *child;
    struct LinkedSprite *part;
    struct FieldSprite *sprite;
    struct LinkedEffectWork *work = Ram_HeapSlots->effect_work;
    s32 i;

    /* goto loop: a for loop lets loop.c hoist the mask and the array address (95). */
    i = 0;
loop:
    {
        child = Func_02004b30_(271, parent->x, parent->y, parent->z);
        pair[i] = child;
        if (child != NULL) {
            child->terrain_height = parent->terrain_height;
            part = child->animation;
            child->motion_flags = 0;
            child->action = 0;
            child->linked_object = parent;
            if (part != NULL) {
                sprite = &part->sprite;
                Animation_ApplyChildArgument(sprite, 0);
                sprite->next_frame = 0;
                Resource_ResetEntry(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                sprite->unknown_11 |= 1;
                sprite->tile = (ResourceTableEntries[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                ((struct WorldMapOam *)sprite)->size = 2;
                part->detail->field_16 = 0;
            }
        }
    }
    if (++i <= 1)
        goto loop;
    {
        struct ObjectRuntime *p = pair[0];
        struct FieldSprite *sp = p->animation;

        p->update = (void *)Func_020045e4;
        sp->priority = 1;
    }
    {
        struct ObjectRuntime *p = pair[1];
        struct FieldSprite *sp = p->animation;

        sp->priority = 1;
        p->update = (void *)Func_02004598;
        p->priority_flags = 2;
    }
}
