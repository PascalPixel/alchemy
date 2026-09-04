/* Contiguous unnamed leaf-owner run for resource_3b8. */

#include "types.h"

extern void Func_020083f0(void);
extern void Func_02008398(s32, s32);
extern void Func_020083a0(s32, s32);
extern void Func_0200840c(void);

#define FieldScene_RunStepWithValue29e0 Func_0200400c

void FieldScene_RunStepWithValue29e0(void)
{
    Func_020083f0();
    Func_02008398(0x947, 1);
    Func_020083a0(0x29e0, 1);
    Func_0200840c();
}

#include "types.h"

#define SceneData_SelectTableD004ByStateAndFlags Func_02004034

extern s16 Data_02000240[];
extern u8 Value_0000008b;
extern u8 Data_0200dad8[];
extern u8 Data_0200da48[];
extern u8 Data_0200d9e8[];
extern u8 Data_0200d688[];
extern u8 Data_0200d394[];
extern u8 Data_0200d004[];

extern s32 Func_020083ee(s32);
extern s32 Func_020083fc(s32);
extern s32 Func_02008410(s32);
extern s32 Func_0200841e(s32);

s32 SceneData_SelectTableD004ByStateAndFlags(void) {
    if (Data_02000240[224] == (s32)&Value_0000008b) {
        if (Func_020083ee(0x950) != 0) {
            return (s32)Data_0200dad8;
        }
        if (Func_020083fc(0x962) != 0) {
            return (s32)Data_0200da48;
        }
        return (s32)Data_0200d9e8;
    }
    if (Func_02008410(0x950) != 0) {
        return (s32)Data_0200d688;
    }
    if (Func_0200841e(0x962) != 0) {
        return (s32)Data_0200d394;
    }
    return (s32)Data_0200d004;
}
