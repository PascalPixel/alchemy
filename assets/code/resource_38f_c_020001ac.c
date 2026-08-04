#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000026;
extern u8 Data_0200b010[];

s32 Func_020001ac(void) {
    if (Data_02000240[224] == (s32)&Value_00000026) {
        return (s32)Data_0200b010;
    }
    return 0;
}
#include "resource_38f.h"
