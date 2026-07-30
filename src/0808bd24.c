#include "types.h"
#include "map.h"

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

void Func_0800447c(s32, u32, struct Position_0808bd24 *);

extern struct Global_0808bd24 Data_02000240;
extern struct Runtime_0808bd24 *Data_03001ebc;

u8 Func_0808bd24(void)
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

    runtime_slot_address = (u32)&Data_03001ebc;
    runtime = Data_03001ebc;
    offset = (Data_02000240.object_id * 4) + 0x14;
    object = *(struct Object_0808bd24 **)((u8 *)runtime + offset);
    /* マップ状態のポインタ枠は実行時ポインタ枠の19語手前。 */
    map = *(struct MapState **)(runtime_slot_address - 76);

    /* 早期復帰の形が必須。累算用の局所変数にするとレジスタ割付が1つずれる。 */
    if (object == 0)
        return 0;

    position.x = object->x;
    position.unknown_04 = object->unknown_0c;
    position.y = object->y;
    Func_0800447c(0x100000, object->kind, &position);

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
