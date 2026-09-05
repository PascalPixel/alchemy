#include "types.h"

extern s32 Data_0200f764[];

void Func_020064cc(s32, s32, s32, s32, s32, s32);
void Func_020064de(s32, s32, s32, s32, s32, s32);
void Func_020064fa(s32, s32, s32, s32, s32, s32);

void PlaceSceneObjectPairFromTableC(s32 table_index)
{
    s32 position_x = Data_0200f764[table_index * 2];
    s32 position_z = Data_0200f764[table_index * 2 + 1];

    Func_020064cc(1, 0x50, 1, 3, position_x, position_z);
    Func_020064de(2, 0x50, 1, 1, position_x + 1, position_z);
    Func_020064fa(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}
