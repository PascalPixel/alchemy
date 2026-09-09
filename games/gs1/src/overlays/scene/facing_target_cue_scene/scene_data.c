#include "types.h"

#define SceneData_SelectTableA1DCOrA198 Func_02000094
#define SceneData_SelectTableByStoryFlags Func_020000b8
#define SceneData_SelectTableByStoryFlagsB Func_02000160
extern s32 Data_0200a518[];
extern s32 Data_0200a410[];
extern s32 Data_0200a338[];
extern s32 Data_0200a218[];

s32 Func_02001986(s32);
s32 Func_020019be(s32);
s32 Func_020019cc(s32);
s32 Func_02001a0c(s32);
/* One import, four call sites, four distinct veneer addresses. */
s32 Func_02001a64();
s32 Func_02001a76();
s32 Func_02001a84();
s32 Func_02001a92();

s32 SceneData_SelectTableA1DCOrA198(void)
{
    if (Func_02001986(0x834) != 0) {
        return 0x0200A1DC;
    }
    return 0x0200A198;
}

s32 SceneData_SelectTableByStoryFlags(void)
{
    extern u8 Data_02000240[];

    u8 *b = Data_02000240;
    s32 *tbl;

    if (*(s16 *)(b + 0x1c2) == 19)
        return (s32)Data_0200a518;
    if (Func_020019be(0x87a) != 0)
        tbl = Data_0200a410;
    else if (Func_020019cc(0x815) != 0)
        tbl = Data_0200a338;
    else
        tbl = Data_0200a218;
    Func_02001a0c((s32)tbl);
    return (s32)tbl;
}

s32 SceneData_SelectTableByStoryFlagsB(void)
{
    extern s16 Data_02000240[];

    if (Data_02000240[225] == 19) {
        if (Func_02001a64(0x950) != 0) {
            return 0x0200AC5C;
        }
        return 0x0200AB9C;
    }

    if (Func_02001a76(0x834) != 0) {
        return 0x0200A5A8;
    }
    if (Func_02001a84(0x87A) != 0) {
        return 0x0200A980;
    }
    if (Func_02001a92(0x815) != 0) {
        return 0x0200A800;
    }
    return 0x0200A6B0;
}
