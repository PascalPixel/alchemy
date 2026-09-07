#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000004b;
extern u8 Value_0000004c;
extern u8 Data_02009120[];
extern u8 Data_02009288[];
extern u8 Data_020090f0[];

s32 SceneData_SelectTableByWord224(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004b) {
        return (s32)Data_02009120;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)Data_02009288;
    }
    return (s32)Data_020090f0;
}

extern u8 Data_0200940c[], Data_020095bc[], Data_020093f4[];
s32 Func_02000f1c(s32);
s32 Func_02000f3e(s32);
s32 Func_02000f50(s32);
s32 Func_02000f5a(s32);
void Func_02000f9e(void *);

u8 *SceneData_SelectFlaggedTable(void)
{
    s32 id = Data_02000240[224];
    if (id == (s32)&Value_0000004b) {
        if (Func_02000f1c(0x909)) {
            Data_0200940c[142] = 0;
            Data_0200940c[166] = 0;
        }
        return Data_0200940c;
    }
    if (id == (s32)&Value_0000004c) {
        if (Func_02000f3e(0x8fd))
            Data_020095bc[46] = 1;
        if (Func_02000f50(0x8fe) || Func_02000f5a(0x907))
            Data_020095bc[94] = 1;
        Func_02000f9e(Data_020095bc);
        return Data_020095bc;
    }
    return Data_020093f4;
}
