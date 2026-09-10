#include "types.h"

#define SceneData_GetTablec994 Func_02000938
#define SceneData_GetTablecb44 Func_02000940
#define SceneData_GetTablecb64 Func_02000948
#define SceneData_SelectTableByFlags Func_02000950
#define SceneData_SelectTableByThreeFlags Func_020009fc
#define SceneState_FindFirstSetFlagOfGroup Func_02003150
#define SceneData_SelectTableEntryByFlagGroup Func_02003464
extern u8 Data_0200c994[];
extern u8 Data_0200cb44[];
extern u8 Data_0200cb64[];
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
extern u8 Data_0200d9d0[];
extern u8 Data_0200da54[];
extern u8 Data_0200d958[];
extern u8 Data_0200d778[];
extern s32 Data_0200db08[];

s32 Func_02004c22(s32);
s32 Func_02004c30(s32);
s32 Func_02004c3e(s32);
s32 Func_02004c4e(s32);
s32 Func_02004c58(s32);
s32 Func_02004c76(s32);
s32 Func_02004cce(s32);
s32 Func_02004cde(s32);
s32 Func_02004cec(s32);
s32 Func_0200744e(s32);
s32 Func_02007764(s32);

/* Contiguous unnamed leaf-owner run for resource_3af. */

u8 *SceneData_GetTablec994(void)
{
    return Data_0200c994;
}

u8 *SceneData_GetTablecb44(void)
{
    return Data_0200cb44;
}

u8 *SceneData_GetTablecb64(void)
{
    return Data_0200cb64;
}

u8 *SceneData_SelectTableByFlags(void)
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

u8 *SceneData_SelectTableByThreeFlags(void)
{
    if (Func_02004cce((s32)&Value_0000093e))
        return Data_0200d9d0;
    if (Func_02004cde(0x8A0))
        return Data_0200da54;
    if (Func_02004cec((s32)&Value_00000928))
        return Data_0200d958;
    return Data_0200d778;
}

s32 SceneState_FindFirstSetFlagOfGroup(u32 sel)
{
    s32 v = 0;
    s32 id = 23;
    u32 i;

    switch (sel) {
    case 0:
        v = 0x92C;
        break;
    case 1:
        v = 0x935;
        break;
    case 2:
        v = 0x917;
        break;
    case 3:
        v = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (Func_0200744e(v)!= 0) return id;
        v++;
        id++;
    }
    return 0;
}

s32 SceneData_SelectTableEntryByFlagGroup(u32 sel)
{
    s32 base = 0;
    u32 i;

    switch (sel) {
    case 0:
        base = 0x92C;
        break;
    case 1:
        base = 0x935;
        break;
    case 2:
        base = 0x917;
        break;
    case 3:
        base = 0x990;
        break;
    }
    for (i = 0; i < 9; i++) {
        if (Func_02007764(base + i) != 0) return Data_0200db08[i];
    }
    return 0;
}
