#include "types.h"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000006b
#define SecondaryRuntimeSelector Value_00000070
#define TertiaryRuntimeSelector Value_0000006c
#define PrimaryOverlayData Data_02009738
#define SecondaryOverlayData Data_020097c8
#define TertiaryOverlayData Data_02009840
#define DefaultOverlayData Data_02009708
#define SceneData_SelectByRuntimeSelector Func_02000040
#define SceneData_GetTable98a0 Func_02000098
#define SceneData_SelectDataBySelectorAndFlags Func_020000a0
#define SceneData_SelectSecondaryDataBySelectorAndFlags Func_0200019c

#include "select_overlay_data_by_runtime_selector.h"

extern s16 Data_02000240[];
extern u8 Value_0000006b;
extern u8 Value_00000070;
extern u8 Value_0000006c;
extern u8 Data_020098d4[];
extern u8 Data_020098ec[];
extern u8 Data_020099c4[];
extern u8 Data_02009acc[];
extern u8 Data_02009ba4[];
extern u8 Data_02009c7c[];
extern u8 Data_02009d24[];
extern u8 Data_02009dcc[];
extern u8 Data_02009e74[];
extern u8 Data_0200a018[];
extern u8 Data_0200a120[];
extern u8 Data_02009e80[];
extern u8 Data_02009fa0[];
extern u8 Data_0200a24c[];
extern u8 Data_0200a30c[];
extern u8 Data_0200a390[];

s32 Func_02001608();
s32 Func_02001622();
s32 Func_0200163c();
s32 Func_0200164a();
s32 Func_02001704();
s32 Func_0200171e();
s32 Func_02001738();
s32 Func_02001746();

/*
 * Data selector for resource_3ae.  The 164-byte owner includes its 13-word
 * literal pool.
 */

/* Per-call-site import labels.  All four sites reach the same event-flag test,
 * but each keeps its own name because the relocated call word differs. */

/*
 * All four symbols name the loader-relocated call word each site holds; they
 * reach one event-flag test.
 */

s32 SceneData_SelectByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

/*
 * Table getter for resource_3ae. The owner at 0x02000098 is eight bytes and
 * includes its one pool word at 0x0200009c: the pc-relative load reads that
 * word, so the word is part of the owner and not a following gap.
 *
 * The pool word is an address that is returned without being dereferenced.
 * Many getters share this body but each returns a different address, so the
 * constant here is not interchangeable with theirs.
 */
u8 *SceneData_GetTable98a0(void)
{
    return (u8 *)0x020098a0;
}

/*
 * The selector is the signed halfword at byte offset 448 of the resident
 * workspace, Data_02000240[224].  It is compared as a signed 32-bit value
 * against full words held in the pool, which is why the three constants are
 * spelled as the addresses of Value_ symbols rather than as immediates.
 * 0x93e and 0x950 are read as event-flag ids only from their use as the sole
 * argument of the flag test.
 */
s32 SceneData_SelectDataBySelectorAndFlags(void)
{
    s16 room = Data_02000240[224];

    if (room == (s32)&Value_0000006b) {
        if (Func_02001608(0x93e) != 0) {
            return (s32)Data_02009ba4;
        }
        return (s32)Data_02009acc;
    }

    if (room == (s32)&Value_00000070) {
        /* 0x950 is built from an immediate and a shift. */
        if (Func_02001622(0x950) != 0) {
            return (s32)Data_020099c4;
        }
        return (s32)Data_020098ec;
    }

    if (room == (s32)&Value_0000006c) {
        if (Func_0200163c(0x950) != 0) {
            return (s32)Data_02009dcc;
        }
        if (Func_0200164a(0x93e) != 0) {
            return (s32)Data_02009d24;
        }
        return (s32)Data_02009c7c;
    }

    return (s32)Data_020098d4;
}

/*
 * Pick one of eight in-image data tables from the scene selector and two
 * event flags. The selector is the signed halfword at offset 448 of the
 * shared workspace, and each arm compares it as a signed 32-bit value
 * against a full word, which is why the selector constants are spelled as
 * addresses. 0x93e and 0x950 are taken to be event-flag ids only from their
 * use here. The 164-byte owner includes its thirteen-word literal pool.
 */
s32 SceneData_SelectSecondaryDataBySelectorAndFlags(void)
{
    s16 scene = Data_02000240[224];

    if (scene == (s32)&Value_0000006b) {
        if (Func_02001704(0x93e) != 0) {
            return (s32)Data_02009fa0;
        }
        return (s32)Data_02009e80;
    }

    if (scene == (s32)&Value_00000070) {
        /* 0x950 is built by shifting a small constant, not loaded. */
        if (Func_0200171e(0x950) != 0) {
            return (s32)Data_0200a120;
        }
        return (s32)Data_0200a018;
    }

    if (scene == (s32)&Value_0000006c) {
        if (Func_02001738(0x950) != 0) {
            return (s32)Data_0200a390;
        }
        if (Func_02001746(0x93e) != 0) {
            return (s32)Data_0200a30c;
        }
        return (s32)Data_0200a24c;
    }

    return (s32)Data_02009e74;
}
