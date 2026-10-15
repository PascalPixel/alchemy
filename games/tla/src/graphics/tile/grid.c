#include "types.h"
#include "metadata_lookup.h"
#include "ram_buffer.h"
#include "scene.h"

extern u8 gData_03001e60[];

void Ui_SetGridColumnParameter(s32 slot, s32 value)
{
    u8 *base = Ram_HeapSlots->unk0c[1];
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
    u8 *base = Ram_HeapSlots->unk0c[1];
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
    u8 value04;
    u8 unk05[7];
    s32 *table0c;
    s32 value10;
    u8 value14;
    u8 value15;
    u8 value16;
    u8 value17;
};

void Ui_FillGridColumnFromMetadata(s32 slot, s32 value)
{
    s32 index;
    s32 count;
    s32 offset;
    struct AnimationMetadata *metadata;
    struct UiGridEntry *entry;
    u8 *work;

    work = Ram_HeapSlots->unk0c[1];
    count = 0;
    offset = ((3 & slot) * 4) + 0x28;
    index = 0;
    do {
        entry = *(struct UiGridEntry **)(work + offset);
        if (entry->table0c != 0) {
            metadata = Resource_GetMetadataRecordFar(entry->no);
            if (value < metadata->animationCount) {
                entry->value04 = metadata->drawKind;
                entry->value10 = entry->table0c[value];
                entry->x = count * 0x10;
                entry->value15 = 0x10;
                entry->value14 = index;
                entry->value17 = index;
                entry->value16 = 0xFF;
            }
            work[23] = (u8)metadata->adjustY;
            *(s16 *)(work + 18) = index;
        }
        count += 1;
        work += 0x38;
    } while (count <= 9);
}

void Ui_SetGridColumnNumber(s32 slot, s32 no)
{
    u8 *base = Ram_HeapSlots->unk0c[1];
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
    u8 *unk04;
    u8 *unk08;
    u8 *unk0c;
    u8 *unk10;
    u8 *work;
};

struct Vec2 {
    s32 x;
    s32 y;
};

extern struct BattleCells gSpriteObjects;

extern struct Vec2 gBattle_FormationPlacementScale;

void Battle_PlaceActorsByFormationKind(void)
{
    u8 *actor = Ram_HeapSlots->unk0c[1];
    u32 kind = (*(u8 **)(actor + 40))[4];
    struct Vec2 scale;
    u8 *table;
    u16 angle;
    u16 step;
    u16 odd = 0;
    u32 cnt;
    u32 i;

    scale = gBattle_FormationPlacementScale;
    table = (u8 *)Ram_HeapSlots->battleWork;

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
        Render_ApplyProjectedPlacement(actor, (s32 *)(table + i * 16), (s32 *)&scale, angle);
        actor += 56;
        angle += step;
        if (i & 1)
            angle += odd;
    }
}
