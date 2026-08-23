#include "types.h"

extern s32 Data_0200f754[];

void Func_0200641c(s32, s32, s32, s32, s32, s32);
void Func_0200642e(s32, s32, s32, s32, s32, s32);
void Func_02006442(s32, s32, s32, s32, s32, s32);

#define PlaceSceneObjectPairFromTableB Func_02000e30

void PlaceSceneObjectPairFromTableB(s32 table_index)
{
    s32 position_x = Data_0200f754[table_index * 2];
    s32 position_z = Data_0200f754[table_index * 2 + 1];

    Func_0200641c(0x37, 0x79, 1, 3, position_x, position_z);
    Func_0200642e(0x38, 0x79, 1, 1, position_x + 1, position_z);
    Func_02006442(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}
