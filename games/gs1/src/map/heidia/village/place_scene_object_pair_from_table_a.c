#include "types.h"

extern s32 Data_0200f714[];

void Func_0200627c(s32, s32, s32, s32, s32, s32);
void Func_0200628e(s32, s32, s32, s32, s32, s32);
void Func_020062a2(s32, s32, s32, s32, s32, s32);

#define PlaceSceneObjectPairFromTableA Func_02000c90

void PlaceSceneObjectPairFromTableA(s32 table_index)
{
    s32 position_x = Data_0200f714[table_index * 2];
    s32 position_z = Data_0200f714[table_index * 2 + 1];

    Func_0200627c(0, 0x4d, 1, 3, position_x, position_z);
    Func_0200628e(1, 0x4d, 1, 1, position_x + 1, position_z);
    Func_020062a2(position_x, position_z - 0x30, 1, 1, position_x, position_z - 0x2e);
}
