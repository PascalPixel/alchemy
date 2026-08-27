#include "types.h"

#define TILE_HI(ptr, offset) (*(s16 *)((u8 *)(ptr) + (offset) + 2))

struct GridEffectObject_08093e28 {
    u8 unknown_00[6];
    u16 field_06;
    s32 x;
    s32 y;
    s32 z;
    s32 field_20;
    u8 unknown_24[16];
    s32 field_28;
    u8 unknown_2c[4];
    s32 field_30;
    u8 unknown_34[33];
    u8 value_55;
    u8 unknown_56[4];
    u8 value_5a;
};

struct GridTileCell_08093e28 {
    u8 unknown_00[2];
    u8 kind;
    u8 unknown_03;
};

extern s32 Data_02000240[];
extern struct GridTileCell_08093e28 Data_02010000[];
extern struct GridTileCell_08093e28 Data_02010200[];

#define ACTIVE_FLAG (((u8 *)Data_02000240)[498])

struct GridEffectObject_08093e28 *Func_08092054(s32);
void Func_080916b0(void);
s32 Func_08009220(const s32 *position);
void Func_08092158(s32, s32, s32);
void Func_08092adc(s32, s32, s32);
void Func_080920e8(s32);
void Func_080091e0(struct GridEffectObject_08093e28 *, s32);
void Func_08009080(struct GridEffectObject_08093e28 *, s32);
void Func_08009150(struct GridEffectObject_08093e28 *, s32, s32, s32);
void Func_080923c4(s32);
void Func_0809163c(s32);
void Func_08091750(void);

s32 Func_08093e28(void)
{
    struct GridEffectObject_08093e28 *object = Func_08092054(Data_02000240[125]);
    s32 tile_x = TILE_HI(object, 8) & 0xfff0;
    s32 tile_z = TILE_HI(object, 16) & 0xfff0;
    s32 grid_x = 8 + tile_x;
    s32 grid_z = 8 + tile_z;
    s32 result;

    Func_080916b0();

    if (ACTIVE_FLAG == 0) {
        s32 index_x = grid_x;
        s32 index_z;
        s32 index;

        if (index_x < 0)
            index_x = tile_x + 23;
        index_x >>= 4;

        index_z = grid_z;
        if (index_z < 0)
            index_z = tile_z + 23;
        index_z >>= 4;

        index = index_x + (index_z << 7);

        if (Data_02010000[index].kind != Data_02010200[index].kind) {
            Func_08091750();
            return -1;
        } else {
            s32 position[6];

            position[0] = object->x;
            position[1] = object->y + (s32)0xfff00000;
            position[2] = object->z;
            result = Func_08009220(position);
            if (result != 0) {
                Func_08091750();
                return -1;
            }

            Func_08092158(Data_02000240[125], grid_x, grid_z);
            object->field_30 = 0x10000;
            Func_08092adc(Data_02000240[125], 0xc000, 0);
            Func_080920e8(Data_02000240[125]);
            object->value_5a = 1;
            object->value_55 = 0;
            Func_080091e0(object, 0);
            Func_08009080(object, 13);
            Func_08009150(object, grid_x << 16,
                object->y + (s32)0xfff00000, (grid_z << 16) + 0x100000);
            Func_080923c4(Data_02000240[125]);
            ACTIVE_FLAG = 1;
        }
    } else {
        Func_08009080(object, 10);
        object->value_55 = 3;
        object->field_28 = 0x40000;
        object->field_20 = object->y;
        Func_080091e0(object, 1);
        Func_0809163c(6);
        ACTIVE_FLAG = 0;
        object->value_5a = 1;
        object->field_06 = 0xc000;
    }

    Func_08091750();
    return 0;
}
