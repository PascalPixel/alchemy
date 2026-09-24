#include "TYPES.H"
#include "MAP.H"
#include "EVENT_RUNTIME.H"

struct MapFocusObject {
    u8 padding00[6];
    u16 kind;
    s32 x;
    s32 unknown_0c;
    s32 y;
};

struct MapFocusPosition {
    s32 x;
    s32 unknown_04;
    s32 y;
};

void Vector_AddPolarOffset(s32, u32, struct MapFocusPosition *);

extern struct EventPairWork1d6 gGameState;
extern struct EventRuntime *gEventWork;

u8 GetFocusedObjectCollision(void)
{
    u32 runtime_slot_address;
    struct EventRuntime *runtime;
    struct MapFocusObject *object;
    struct MapState *map;
    struct MapFocusPosition position;
    u8 *tile;
    u32 offset;
    s32 x;
    s32 y;

    runtime_slot_address = (u32)&gEventWork;
    runtime = gEventWork;
    offset = (gGameState.object_id * 4) + 0x14;
    object = *(struct MapFocusObject **)((u8 *)runtime + offset);

    map = *(struct MapState **)(runtime_slot_address - 76);

    if (object == 0)
        return 0;

    position.x = object->x;
    position.unknown_04 = object->unknown_0c;
    position.y = object->y;
    Vector_AddPolarOffset(0x100000, object->kind, &position);

    if (runtime->mode_19e == 3) {
        u32 tile_x;
        u32 tile_y;

        x = position.x;
        if (x < 0)
            x += 0x1fffff;
        tile_x = (x >> 21) & 31;

        y = position.y;
        if (y < 0)
            y += 0x1fffff;
        tile_y = (y >> 21) & 31;

        tile = (u8 *)(0x02020000 + ((tile_x + (tile_y << 5)) << 2));
    } else {
        tile = (u8 *)map->layers[0].cells;

        x = position.x;
        if (x < 0)
            x += 0xfffff;
        {
            u32 tile_x = x >> 20;

            y = position.y;
            if (y < 0)
                y += 0xfffff;

            tile = (u8 *)((u32)tile +
                ((tile_x + ((u32)(y >> 20) << 7)) << 2));
        }
    }

    return tile[2];
}

struct MapRegion {
    s16 min_x;
    s16 min_y;
    s16 min_z;
    s16 max_x;
    s16 max_y;
    s16 max_z;
    s16 condition;
    s16 value;
};

typedef struct MapRegion *(*MapRegionProvider)(void);

struct MapRegionServices {
    u8 padding000[0x2c];
    MapRegionProvider region_provider;
};

extern struct MapRegionServices gOverlayArea;

s32 GameFlag_IsConditionActive(s32 condition);
void Audio_PlayCue(s32 sound_id);
void Battle_InitializeRenderObject(void);

#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))

void UpdateMapRegionAtPosition(s32 position_x, s32 position_y, s32 position_z)
{
    s32 selected_value;
    struct EventRuntime *runtime;
    s32 z;
    s32 y;
    s32 x;
    s16 condition;
    s16 min_z;
    s16 max_y;
    s16 min_y;
    s32 max_z;
    s16 max_x;
    s16 min_x;
    struct MapRegion *region;

    x = position_x;
    y = position_y;
    z = position_z;
    region = gOverlayArea.region_provider();
    if (region != 0 &&
        (runtime = gEventWork, min_x = region->min_x, min_x != -1)) {
loop:
        min_y = region->min_y;
        min_z = region->min_z;
        max_x = region->max_x;
        max_y = region->max_y;
        max_z = region->max_z;
        condition = region->condition;
        selected_value = region->value;
        if (GameFlag_IsConditionActive(condition)!= 0 &&
            y >= REGION_FIXED(min_y)&&
            y < REGION_FIXED(max_y)&&
            x >= REGION_FIXED(min_x)&&
            x < REGION_FIXED(max_x)&&
            z >= REGION_FIXED(min_z)&&
            z < REGION_FIXED(max_z)) {
            runtime->value_170 = (u16)selected_value;
            Audio_PlayCue(123);
            Battle_InitializeRenderObject();
            return;
        }
        runtime = gEventWork;
        region++;
        min_x = region->min_x;
        if (min_x != -1)
            goto loop;
    }
}
