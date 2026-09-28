/* resource_3b9 0x02008030 Scene_GetEntrances (as SceneData_SelectTableBySceneId),
 * 0x02008238 Scene_GetEvents (as SceneData_SelectDataBySelectorAndSubstate)
 * and 0x02009a14 Scene_Initialize (as SceneState_DispatchByStatus448): all
 * three compile exactly with GCC 2.96 against FIELD/KORASHIAMU_IRIGUCHI/STATUS.H.
 * Remaining difference: each compares the scene number with 0x8c, 0x8d or
 * 0x8e loaded from its literal pool, which only a link-time symbol explains;
 * the main image names none of them (the Value_ spellings were equates), so
 * the rows stay in the listing. The Data_ tables are the listing's rows at
 * those addresses and need labels before adopting. */
#include "STATUS.H"

extern u8 Value_0000008c;
extern u8 Value_0000008d;
extern u8 Value_0000008e;
extern u8 Data_0200b094[];
extern u8 Data_0200b274[];
extern u8 Data_0200b034[];
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
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000008c) {
        return (s32)Data_0200b094;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)Data_0200b274;
    }
    return (s32)Data_0200b034;
}

s32 SceneData_SelectDataBySelectorAndSubstate(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000008d) {
        return (s32)Data_0200be70;
    }
    if (v == (s32)&Value_0000008c) {
        if (gGameState.entrance == 12) {
            return (s32)Data_0200c110;
        }
        return (s32)Data_0200be94;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)Data_0200bf60;
    }
    return (s32)Data_0200be64;
}

s32 SceneState_DispatchByStatus448(void)
{
    extern u8 Data_02000240[];
    s32 off = 448;
    s16 status = *(s16 *)(Data_02000240 + off);

    if (status == (s32)&Value_0000008c) {
        FieldScene_DispatchBySelector();
    } else if (status == (s32)&Value_0000008e) {
        SceneState_ApplyRectsByFlatla384And962();
    }
    return 0;
}
