#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000059;
extern u8 Value_0000005a;
extern u8 Value_0000005b;
extern u8 Data_0200a3c8[];
extern u8 Data_0200a410[];
extern u8 Data_0200a4b8[];
extern u8 Data_0200a3b0[];
void Func_0200218e(s32);

s32 SceneData_SelectTableByScene59To5b(void)
{
    s16 *work = Data_02000240;
    s16 v;

    if (work[224] == (s32)&Value_0000005b) {
        if (work[225] == 5)
            Func_0200218e(0x90a);
    }
    v = work[224];
    if (v == (s32)&Value_00000059)
        return (s32)Data_0200a3c8;
    if (v == (s32)&Value_0000005a)
        return (s32)Data_0200a410;
    if (v == (s32)&Value_0000005b)
        return (s32)Data_0200a4b8;
    return (s32)Data_0200a3b0;
}
