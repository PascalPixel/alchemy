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
    s32 column = slot & 3;
    s32 count = 9;

    do {
        struct AnimationEntry *entry = object->entries[column];

        count--;
        entry->param = value;
        object++;
    } while (count >= 0);
}

void Ui_SetGridColumnByte6(s32 slot, s32 value)
{
    struct AnimationObject *object = gSpriteObjects;
    s32 column = slot & 3;
    s32 count = 9;

    do {
        struct AnimationEntry *entry = object->entries[column];

        count--;
        entry->priority = value;
        object++;
    } while (count >= 0);
}

void Ui_FillGridColumnFromMetadata(s32 slot, s32 value)
{
    s32 index;
    s32 count;
    s32 column;
    struct AnimationMetadata *metadata;
    struct AnimationEntry *entry;
    struct AnimationObject *object;

    object = gSpriteObjects;
    count = 0;
    column = 3 & slot;
    index = 0;
    do {
        entry = object->entries[column];
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
    s32 column;
    s32 count;

    Resource_GetMetadataRecordFar(no);
    column = slot & 3;
    count = 9;
    do {
        struct AnimationEntry *entry = object->entries[column];

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
    /* The monitor's 160-byte position block occupies heap slot9. */
    table = ((union HeapState *)gWorkSlot)->slots[9];

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
