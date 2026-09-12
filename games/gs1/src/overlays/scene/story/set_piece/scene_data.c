#include "types.h"

#define SceneData_SelectTableByWord224 Func_020000e4
#define SceneData_SelectTableC80cOrC83c Func_020001c8
#define SceneData_GetTableC85c Func_02000204
#define SceneData_SelectDataByRuntimeSelector Func_0200020c
extern s16 Data_02000240[];
extern u8 Value_0000004d;
extern u8 Value_0000004e;
extern u8 Value_0000004f;
extern u8 Value_00000050;
extern u8 Value_00000051;
extern u8 Value_00000052;
extern u8 Value_00000053;
extern u8 Value_00000054;
extern u8 Value_00000055;
extern u8 Value_00000056;
extern u8 Value_00000057;
extern u8 Data_0200c194[];
extern u8 Data_0200c20c[];
extern u8 Data_0200c26c[];
extern u8 Data_0200c314[];
extern u8 Data_0200c3ec[];
extern u8 Data_0200c464[];
extern u8 Data_0200c524[];
extern u8 Data_0200c59c[];
extern u8 Data_0200c644[];
extern u8 Data_0200c704[];
extern u8 Data_0200c77c[];
extern u8 Data_0200c164[];
extern u8 Data_0200c80c[];
extern u8 Data_0200c83c[];
extern u8 Data_0200c85c[];   /* Image offset 0x485c. */
extern u8 Data_0200c940[];
extern u8 Data_0200c9a0[];
extern u8 Data_0200ca00[];
extern u8 Data_0200ca60[];
extern u8 Data_0200caa8[];
extern u8 Data_0200cb68[];
extern u8 Data_0200cb98[];
extern u8 Data_0200cc40[];
extern u8 Data_0200ccd0[];
extern u8 Data_0200c928[];

/*
 * Table getter for resource_3a4, published from the overlay's header as an
 * entry point.
 *
 * The eight-byte owner at 0x02000204 includes its one pool word at
 * 0x02000208; the load reads that word and returns it as an address,
 * without dereferencing it.
 */

s32 SceneData_SelectTableByWord224(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004d) {
        return (s32)Data_0200c194;
    }
    if (v == (s32)&Value_0000004e) {
        return (s32)Data_0200c20c;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)Data_0200c26c;
    }
    if (v == (s32)&Value_00000050) {
        return (s32)Data_0200c314;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)Data_0200c3ec;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)Data_0200c464;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)Data_0200c524;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)Data_0200c59c;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200c644;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200c704;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)Data_0200c77c;
    }
    return (s32)Data_0200c164;
}

s32 SceneData_SelectTableC80cOrC83c(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200c80c;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200c83c;
    }
    return 0;
}

u8 *SceneData_GetTableC85c(void)
{
    return Data_0200c85c;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004d) {
        return (s32)Data_0200c940;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)Data_0200c9a0;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)Data_0200ca00;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)Data_0200ca60;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)Data_0200caa8;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)Data_0200cb68;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200cb98;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200cc40;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)Data_0200ccd0;
    }
    return (s32)Data_0200c928;
}
