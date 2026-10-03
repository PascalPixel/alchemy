#include "TYPES.H"
#include "METADATA_LOOKUP.H"
#include "ANIMSPR.H"
#include "HEAP_STATE.H"

extern struct AnimationObject *gSpriteObjects;

void Animation_InitWorkFromMetadata(struct AnimationEntry *entry);
void Render_ApplyProjectedPlacement(struct AnimationObject *object, s32 *position, s32 *scale, s32 mode);

struct Vec2 {
    s32 x;
    s32 y;
};

extern struct Vec2 Battle_FormationPlacementScale;

void Ui_SetGridColumnByte5(s32 slot, s32 value)
{
    struct AnimationObject *object = gSpriteObjects;
    /* FAKEMATCH: typed entries[column] moves the offset addition after
       the counter setup; retain the existing precomputed byte lane. */
    s32 offset = (slot & 3) * sizeof(void *) + (u32)&((struct AnimationObject *)0)->entries;
    s32 count = 9;

    do {
        struct AnimationEntry *entry = *(struct AnimationEntry **)((u8 *)object + offset);

        count--;
        entry->param = value;
        object++;
    } while (count >= 0);
}

void Ui_SetGridColumnByte6(s32 slot, s32 value)
{
    struct AnimationObject *object = gSpriteObjects;
    /* FAKEMATCH: typed entries[column] moves the offset addition after
       the counter setup; retain the existing precomputed byte lane. */
    s32 offset = (slot & 3) * sizeof(void *) + (u32)&((struct AnimationObject *)0)->entries;
    s32 count = 9;

    do {
        struct AnimationEntry *entry = *(struct AnimationEntry **)((u8 *)object + offset);

        count--;
        entry->priority = value;
        object++;
    } while (count >= 0);
}

void Ui_FillGridColumnFromMetadata(s32 slot, s32 value)
{
    s32 index;
    s32 count;
    s32 offset;
    struct AnimationMetadata *metadata;
    struct AnimationEntry *entry;
    struct AnimationObject *object;

    object = gSpriteObjects;
    count = 0;
    /* FAKEMATCH: typed indexing moves the slot offset addition into
       the loop; the native loop uses this existing precomputed lane. */
    offset = (3 & slot) * sizeof(void *) + (u32)&((struct AnimationObject *)0)->entries;
    index = 0;
    do {
        entry = *(struct AnimationEntry **)((u8 *)object + offset);
        if (entry->field_0c != 0) {
            metadata = Resource_GetMetadataRecordFar(entry->anim_id);
            if (value < metadata->animation_count) {
                entry->kind = metadata->draw_kind;
                entry->script = ((u8 **)entry->field_0c)[value];
                entry->timer = count * 0x10;
                entry->step = 0x10;
                entry->pos = index;
                entry->frame_base = index;
                entry->frame = 0xff;
            }
            object->offset_y = metadata->adjust_y;
            object->rotation = index;
        }
        count++;
        object++;
    } while (count <= 9);
}

void Ui_SetGridColumnNumber(s32 slot, s32 no)
{
    struct AnimationObject *object = gSpriteObjects;
    s32 offset;
    s32 count;

    Resource_GetMetadataRecordFar(no);
    /* FAKEMATCH: typed indexing reverses the offset/counter move order;
       keep the existing byte lane over the actual entries array. */
    offset = (slot & 3) * sizeof(void *) + (u32)&((struct AnimationObject *)0)->entries;
    count = 9;
    do {
        struct AnimationEntry *entry = *(struct AnimationEntry **)((u8 *)object + offset);

        count--;
        entry->anim_id = no;
        Animation_InitWorkFromMetadata(entry);
        object++;
    } while (count >= 0);
}

void Battle_PlaceActorsByFormationKind(void)
{
    struct AnimationObject *actor = gSpriteObjects;
    u32 kind = actor->entries[0]->kind;
    struct Vec2 scale;
    u8 *table;
    u16 angle;
    u16 step;
    u16 odd = 0;
    u32 count;
    u32 i;

    scale = Battle_FormationPlacementScale;
    /* FAKEMATCH: naming heap slot9 independently adds a second RAM-base
       load and eight native bytes. The monitor stores this 160-byte block
       in slot9; gSpriteObjects is the slot4 cell in the same heap bank.
       Preserve the existing relative pointer-cell load without a false
       record spanning the intervening allocations. */
    table = *(u8 **)((u8 *)&gSpriteObjects + (9 - 4) * sizeof(((union HeapState *)0)->slots[0]));

    switch (kind) {
    case 3:
        angle = 0;
        step = 0x2aaa;
        count = 6;
        break;
    case 5:
    case 8:
    case 44:
    case 88:
        angle = 0;
        step = 0x2000;
        count = 8;
        break;
    case 4:
    case 6:
        angle = 0;
        step = 0x1999;
        count = 10;
        break;
    case 20:
        angle = 0;
        step = 0;
        odd = 0x8000;
        count = 4;
        break;
    default:
        angle = 0x2000;
        step = 0x4000;
        count = 4;
        break;
    }

    for (i = 0; i < count; i++) {
        Render_ApplyProjectedPlacement(actor, (s32 *)(table + i * 16), (s32 *)&scale, angle);
        actor++;
        angle += step;
        if (i & 1)
            angle += odd;
    }
}
