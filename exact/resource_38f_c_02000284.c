#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000027;
extern u8 Data_0200b590[];
extern u8 Data_0200b3b0[];

s32 Func_02000284(void) {
    if (Data_02000240[224] == (s32)&Value_00000027) {
        return (s32)Data_0200b590;
    }
    return (s32)Data_0200b3b0;
}
#include "resource_38f.h"
