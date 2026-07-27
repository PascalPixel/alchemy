#include "types.h"

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

struct Region_0808bde0 *Func_080072e4(RegionProvider_0808bde0 callback);
s32 Func_0808d428(s32 condition);
void Func_080f9010(s32 sound_id);
void Func_08091660(void);

#define FIXED_0808BDE0(value) ((s32)((u32)(s32)(value) << 16))

void Func_0808bde0(s32 arg0, s32 arg1, s32 arg2)
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

    x = arg0;
    y = arg1;
    z = arg2;
    region = Func_080072e4(Data_02008000.region_provider);
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
        if (Func_0808d428(condition) != 0 &&
            y >= FIXED_0808BDE0(min_y) &&
            y < FIXED_0808BDE0(max_y) &&
            x >= FIXED_0808BDE0(min_x) &&
            x < FIXED_0808BDE0(max_x) &&
            z >= FIXED_0808BDE0(min_z) &&
            z < FIXED_0808BDE0(max_z)) {
            runtime->selected_region = (u16)selected_value;
            Func_080f9010(123);
            Func_08091660();
            return;
        }
        runtime = Data_03001ebc;
        region++;
        min_x = region->min_x;
        if (min_x != -1)
            goto loop;
    }
}
