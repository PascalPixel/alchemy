#include "TYPES.H"

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
