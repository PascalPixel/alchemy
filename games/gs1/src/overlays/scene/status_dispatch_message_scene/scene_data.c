#include "types.h"

#define SceneData_SelectTableBySceneId Func_02000030
#define SceneData_GetTableB2bc Func_02000074
#define SceneData_SelectDataBySelectorAndSubstate Func_02000238
extern s16 Data_02000240[];
extern u8 Value_0000008c;
extern u8 Value_0000008e;
extern u8 Data_0200b094[];
extern u8 Data_0200b274[];
extern u8 Data_0200b034[];
extern u8 Data_0200b2bc[];   /* image offset 0x32bc */
extern u8 Value_0000008d;
extern u8 Data_0200be70[];
extern u8 Data_0200c110[];
extern u8 Data_0200be94[];
extern u8 Data_0200bf60[];
extern u8 Data_0200be64[];

/*
 * resource_3b9 owner at 0x02000074: a leaf that loads its literal pool word
 * and returns it. The eight-byte owner includes that one pool word at
 * 0x02000078, holding the address 0x0200b2bc -- image offset 0x32bc --
 * which is returned without being dereferenced.
 */

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000008c) {
        return (s32)Data_0200b094;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)Data_0200b274;
    }
    return (s32)Data_0200b034;
}

u8 *SceneData_GetTableB2bc(void)
{
    return Data_0200b2bc;
}

s32 SceneData_SelectDataBySelectorAndSubstate(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000008d) {
        return (s32)Data_0200be70;
    }
    if (v == (s32)&Value_0000008c) {
        if (Data_02000240[225] == 12) {
            return (s32)Data_0200c110;
        }
        return (s32)Data_0200be94;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)Data_0200bf60;
    }
    return (s32)Data_0200be64;
}
