#include "types.h"
#include "scene.h"
#include "abi/ui/get_table_word_zero.h"

extern s32 gRom[];

s32 Ui_GetTableWordZero(s32 index)
{
    if (index != 0)
        index = 0;
    return gRom[index];
}
