#include "types.h"
#include "map.h"

struct Region_0808bde0 {
    s16 min_x;
    s16 min_y;
    s16 min_z;
    s16 max_x;
    s16 max_y;
    s16 max_z;
    s16 condition;
    s16 value;
};

struct Runtime_0808bde0 {
    u8 padding000[0x170];
    u16 selected_region;
};

typedef struct Region_0808bde0 *(*RegionProvider_0808bde0)(void);

struct RuntimeServices_0808bde0 {
    u8 padding000[0x2c];
    RegionProvider_0808bde0 region_provider;
};

extern struct RuntimeServices_0808bde0 Data_02008000;
extern struct Runtime_0808bde0 *Data_03001ebc;

s32 GameFlag_IsConditionActive(s32 condition);
void Audio_PlayCue(s32 sound_id);
void BattleRuntime_InitializeRenderObject(void);

#define FIXED_0808BDE0(value) ((s32)((u32)(s32)(value) << 16))

void UpdateMapRegionAtPosition(s32 position_x, s32 position_y, s32 position_z)
{
    s32 selected_value;
    struct Runtime_0808bde0 *runtime;
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
    struct Region_0808bde0 *region;

    x = position_x;
    y = position_y;
    z = position_z;
    region = Data_02008000.region_provider();
    if (region != 0 &&
        (runtime = Data_03001ebc, min_x = region->min_x, min_x != -1)) {
loop:
        min_y = region->min_y;
        min_z = region->min_z;
        max_x = region->max_x;
        max_y = region->max_y;
        max_z = region->max_z;
        condition = region->condition;
        selected_value = region->value;
        if (GameFlag_IsConditionActive(condition) != 0 &&
            y >= FIXED_0808BDE0(min_y) &&
            y < FIXED_0808BDE0(max_y) &&
            x >= FIXED_0808BDE0(min_x) &&
            x < FIXED_0808BDE0(max_x) &&
            z >= FIXED_0808BDE0(min_z) &&
            z < FIXED_0808BDE0(max_z)) {
            runtime->selected_region = (u16)selected_value;
            Audio_PlayCue(123);
            BattleRuntime_InitializeRenderObject();
            return;
        }
        runtime = Data_03001ebc;
        region++;
        min_x = region->min_x;
        if (min_x != -1)
            goto loop;
    }
}
