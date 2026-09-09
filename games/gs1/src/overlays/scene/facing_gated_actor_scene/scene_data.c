#include "types.h"

#define SceneData_GetTableBaa8 Func_0200005c
#define SceneData_ReturnZero Func_02000064
#define SceneData_GetTableBbc8 Func_02000068
#define SceneData_SelectPlacementTableBySubstate Func_02000070
#define SceneData_SelectTableBySceneIdAndFlags Func_0200014c

extern u8 Data_0200baa8;
extern u8 Data_0200bbc8;
extern u8 Data_0200bbf4[];   /* Empty table: place nothing. */
extern u8 Data_0200bc0c[];
extern u8 Data_0200bccc[];
extern u8 Data_0200bd2c[];
extern u8 Data_0200be04[];
extern u8 Data_0200be34[];
extern u8 Data_0200be70[];
extern u8 Data_0200bec4[];
extern u8 Data_0200bf0c[];
extern u8 Data_0200bf78[];
extern u8 Data_0200bfd8[];
extern u8 Data_0200c038[];
extern u8 Data_0200c080[];
extern u8 Data_0200c0ec[];

s32 Func_020037f8();
s32 Func_02003806();

/* One symbol per call site, named at the site's decoded address. */

void *SceneData_GetTableBaa8(void)
{
    return &Data_0200baa8;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTableBbc8(void)
{
    return &Data_0200bbc8;
}

/*
 * Select this scene's placement script from its stored sub-state.  The
 * 220-byte owner at 0x02000070 includes the 35-entry jump table at
 * 0x0200008c-0x02000117 and the literal pool at 0x02000130-0x0200014b.  The
 * selector is the signed halfword at offset 450 of the scene-record block, and
 * its address is built rather than folded: the `s32 off = 450;` local in its
 * own block is what forces that.  The out-of-range arm is also the arm for
 * most of the in-range entries, so it is a genuine default.
 */

/*
 * In-image script tables: runs of 24-byte records terminated by 0x0000ffff, in
 * the shape {0xffff0000 | selector, count, x, 0, z, value} with coordinates in
 * 16.16 fixed point.  The layout is read off the data, so the fields at +12
 * and +20 are named by position only, and the selector's return type stays an
 * opaque pointer.  The tables live in EWRAM, which is writable and used as
 * save state, so they are deliberately not const.
 */

/* Shared cross-overlay scene-record block; +450 is the scene sub-state. */

/*
 * Select a table from the scene id and two flags.  The 304-byte owner at
 * 0x0200014c decomposes as dispatcher, the 41-entry jump table at
 * 0x02000168-0x0200020b, the case bodies, an alignment halfword at 0x02000246
 * and the literal pool.  Case-arm order follows the table -- entries grouped
 * by value, distinct values ascending -- so the 20/21/50 arm comes third and
 * the 29 arm after the 32 arm, not in selector order.
 */
void *SceneData_SelectPlacementTableBySubstate(void)
{
    extern u8 Data_02000240[];

    s16 sub;

    {
        s32 off = 450;

        sub = *(s16 *)(Data_02000240 + off);
    }
    switch ((s32)sub) {
    case 1:
    case 2:
        return Data_0200bc0c;

    case 10:
    case 11:
    case 12:
    case 35:
        return Data_0200bccc;

    case 20:
    case 21:
        return Data_0200bd2c;

    case 29:
    case 32:
        return Data_0200be04;

    default:
        return Data_0200bbf4;
    }
}

u8 *SceneData_SelectTableBySceneIdAndFlags(void)
{
    extern s16 Data_02000240[];

    s16 *tbl = Data_02000240;
    s32 no = tbl[225];

    switch (no) {
    case 10:
    case 12:
        return Data_0200be70;
    case 11:
        return Data_0200bec4;
    case 20:
    case 21:
    case 50:
        return Data_0200bf0c;
    case 32:
        return Data_0200c0ec;
    case 29:
        return Data_0200c038;
    case 35:
        return Data_0200c080;
    default:
        break;
    }

    if (Func_020037f8(0x87a) != 0) {
        return Data_0200bfd8;
    }
    if (Func_02003806(0x815) != 0) {
        return Data_0200bf78;
    }
    return Data_0200be34;
}
