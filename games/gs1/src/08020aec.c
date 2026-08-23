#include "types.h"

s32 Resource_CopyData(s32, s32, s32);
extern u8 Data_080310a4[];

void Func_08020aec(s32 arg0) {
    Resource_CopyData(arg0, 0x80, Data_080310a4);
}
