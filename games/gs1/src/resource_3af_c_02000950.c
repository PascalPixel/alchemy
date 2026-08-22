#include "types.h"

extern s32 Func_02004c22(s32);
extern s32 Func_02004c30(s32);
extern s32 Func_02004c3e(s32);
extern s32 Func_02004c4e(s32);
extern s32 Func_02004c58(s32);
extern s32 Func_02004c76(s32);

extern u8 Value_0000093e;
extern u8 Value_00000927;
extern u8 Value_00000928;
extern u8 Value_00000911;
extern u8 Value_00000925;
extern u8 Value_00000922;

extern u8 Data_0200d508[];
extern u8 Data_0200cef0[];
extern u8 Data_0200d028[];
extern u8 Data_0200ccf8[];
extern u8 Data_0200cba8[];

u8 *Func_02000950(void)
{
    s32 v;

    if (Func_02004c22((s32)&Value_0000093e))
        return Data_0200d508;
    if (Func_02004c30((s32)&Value_00000927))
        return Data_0200cef0;
    v = Func_02004c3e((s32)&Value_00000928);
    if (v != 0)
        return Data_0200d028;
    if (Func_02004c4e((s32)&Value_00000911)) {
        if (Func_02004c58((s32)&Value_00000925)) {
            Data_0200ccf8[0x14E] = v;
            Data_0200ccf8[0x1AE] = 2;
            Data_0200ccf8[0x1C6] = 2;
        } else if (Func_02004c76((s32)&Value_00000922)) {
            Data_0200ccf8[0x1AE] = 1;
            Data_0200ccf8[0x1C6] = 1;
        }
        return Data_0200ccf8;
    }
    return Data_0200cba8;
}
