#include "types.h"
#include "scene.h"
#include "map.h"

/* map/shared/get_focused_object_collision.c */
/* 注目オブジェクトの座標を補正してマップセルを引き、衝突コードを返す。
   オブジェクト枠が空なら0。 */

struct Object_0808bd24 {
    u8 padding00[6];
    u16 kind;
    s32 x;
    s32 unknown_0c;
    s32 y;
};

struct Runtime_0808bd24 {
    u8 padding000[0x19e];
    s16 mode;
};

struct Global_0808bd24 {
    u8 padding000[0x1f4];
    u32 object_id;
};

/* 補正呼出しへ渡す一時座標。中央の語は用途未確定。 */
struct Position_0808bd24 {
    s32 x;
    s32 unknown_04;
    s32 y;
};

void RotateVectorByMagnitude(s32, u32, struct Position_0808bd24 *);

extern struct Global_0808bd24 gCell;
extern struct Runtime_0808bd24 *gWork;

u8 GetFocusedObjectCollision(void)
{
    u32 runtime_slot_address;
    struct Runtime_0808bd24 *runtime;
    struct Object_0808bd24 *object;
    struct MapState *map;
    struct Position_0808bd24 position;
    u8 *tile;
    u32 offset;
    s32 x;
    s32 y;

    runtime_slot_address = (u32)&gWork;
    runtime = gWork;
    offset = (gCell.object_id * 4) + 0x14;
    object = *(struct Object_0808bd24 **)((u8 *)runtime + offset);
    /* マップ状態のポインタ枠は実行時ポインタ枠の19語手前。 */
    map = *(struct MapState **)(runtime_slot_address - 76);

    /* 早期復帰の形が必須。累算用の局所変数にするとレジスタ割付が1つずれる。 */
    if (object == 0)
        return 0;

    position.x = object->x;
    position.unknown_04 = object->unknown_0c;
    position.y = object->y;
    RotateVectorByMagnitude(0x100000, object->kind, &position);

    if (runtime->mode == 3) {
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

/* map/shared/update_region_at_position.c */
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

extern struct RuntimeServices_0808bde0 gOv;
extern struct Runtime_0808bde0 *gWork;

s32 GameFlag_IsConditionActive(s32 condition);
void Audio_PlayCue(s32 sound_id);
void Battle_InitializeRenderObject(void);

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
    region = gOv.region_provider();
    if (region != 0 &&
        (runtime = gWork, min_x = region->min_x, min_x != -1)) {
loop:
        min_y = region->min_y;
        min_z = region->min_z;
        max_x = region->max_x;
        max_y = region->max_y;
        max_z = region->max_z;
        condition = region->condition;
        selected_value = region->value;
        if (GameFlag_IsConditionActive(condition)!= 0 &&
            y >= FIXED_0808BDE0(min_y)&&
            y < FIXED_0808BDE0(max_y)&&
            x >= FIXED_0808BDE0(min_x)&&
            x < FIXED_0808BDE0(max_x)&&
            z >= FIXED_0808BDE0(min_z)&&
            z < FIXED_0808BDE0(max_z)) {
            runtime->selected_region = (u16)selected_value;
            Audio_PlayCue(123);
            Battle_InitializeRenderObject();
            return;
        }
        runtime = gWork;
        region++;
        min_x = region->min_x;
        if (min_x != -1)
            goto loop;
    }
}
