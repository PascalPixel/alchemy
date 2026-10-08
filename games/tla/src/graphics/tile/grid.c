#include "types.h"
#include "metadata_lookup.h"
#include "ram_buffer.h"
#include "scene.h"

extern u8 Data_03001e60[];

void Ui_SetGridColumnParameter(s32 slot, s32 value)
{
    u8 *base = Ram_HeapSlots->unknown_0c[1];
    s32 offset = (slot & 3) * 4 + 40;
    s32 count = 9;

    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        entry[5] = value;
        base += 56;
    } while (count >= 0);
}

void Ui_SetGridColumnPriority(s32 slot, s32 value)
{
    u8 *base = Ram_HeapSlots->unknown_0c[1];
    s32 offset = (slot & 3) * 4 + 40;
    s32 count = 9;

    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        entry[6] = value;
        base += 56;
    } while (count >= 0);
}

struct UiGridEntry {
    s16 no;
    u16 x;
    u8 value_04;
    u8 unknown_05[7];
    s32 *table_0c;
    s32 value_10;
    u8 value_14;
    u8 value_15;
    u8 value_16;
    u8 value_17;
};

void Ui_FillGridColumnFromMetadata(s32 slot, s32 value)
{
    s32 index;
    s32 count;
    s32 offset;
    struct AnimationMetadata *metadata;
    struct UiGridEntry *entry;
    u8 *work;

    work = Ram_HeapSlots->unknown_0c[1];
    count = 0;
    offset = ((3 & slot) * 4) + 0x28;
    index = 0;
    do {
        entry = *(struct UiGridEntry **)(work + offset);
        if (entry->table_0c != 0) {
            metadata = Resource_GetMetadataRecordFar(entry->no);
            if (value < metadata->animation_count) {
                entry->value_04 = metadata->draw_kind;
                entry->value_10 = entry->table_0c[value];
                entry->x = count * 0x10;
                entry->value_15 = 0x10;
                entry->value_14 = index;
                entry->value_17 = index;
                entry->value_16 = 0xFF;
            }
            work[23] = (u8)metadata->adjust_y;
            *(s16 *)(work + 18) = index;
        }
        count += 1;
        work += 0x38;
    } while (count <= 9);
}

void Ui_SetGridColumnNumber(s32 slot, s32 no)
{
    u8 *base = Ram_HeapSlots->unknown_0c[1];
    s32 offset;
    s32 count;

    Resource_GetMetadataRecordFar(no);
    offset = (slot & 3) * 4 + 40;
    count = 9;
    do {
        u8 *entry = *(u8 **)(base + offset);

        count--;
        *(u16 *)entry = no;
        Animation_InitWorkFromMetadata(entry);
        base += 56;
    } while (count >= 0);
}

void Render_ApplyProjectedPlacement(void *object, s32 *position, s32 *scale, u32 mode);

struct BattleCells {
    u8 *actors;
    u8 *unk_04;
    u8 *unk_08;
    u8 *unk_0c;
    u8 *unk_10;
    u8 *work;
};

struct Vec2 {
    s32 x;
    s32 y;
};

extern struct BattleCells gSpriteObjects;

extern struct Vec2 Battle_FormationPlacementScale;

void Battle_PlaceActorsByFormationKind(void)
{
    u8 *actor = Ram_HeapSlots->unknown_0c[1];
    u32 kind = (*(u8 **)(actor + 40))[4];
    struct Vec2 scale;
    u8 *tbl;
    u16 angle;
    u16 step;
    u16 odd = 0;
    u32 cnt;
    u32 i;

    scale = Battle_FormationPlacementScale;
    tbl = (u8 *)Ram_HeapSlots->battle_work;

    switch (kind) {
    case 3:
        angle = 0;
        step = 0x2aaa;
        cnt = 6;
        break;
    case 5:
    case 8:
    case 44:
    case 88:
        angle = 0;
        step = 0x2000;
        cnt = 8;
        break;
    case 4:
    case 6:
        angle = 0;
        step = 0x1999;
        cnt = 10;
        break;
    case 20:
        angle = 0;
        step = 0;
        odd = 0x8000;
        cnt = 4;
        break;
    default:
        angle = 0x2000;
        step = 0x4000;
        cnt = 4;
        break;
    }

    for (i = 0; i < cnt; i++) {
        Render_ApplyProjectedPlacement(actor, (s32 *)(tbl + i * 16), (s32 *)&scale, angle);
        actor += 56;
        angle += step;
        if (i & 1)
            angle += odd;
    }
}
