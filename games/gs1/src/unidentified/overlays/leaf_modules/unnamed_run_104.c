/* Contiguous unnamed leaf-owner run for resource_38f. */

#include "types.h"

void Func_02002e3a(s32, s32, s32);

void Func_02000274(void) {
    Func_02002e3a(27, 0, 1);
}
#include "resource_38f.h"

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

