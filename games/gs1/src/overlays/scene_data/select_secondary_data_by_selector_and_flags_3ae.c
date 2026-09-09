#include "types.h"

/*
 * All four symbols name the loader-relocated call word each site holds; they
 * reach one event-flag test.
 */
s32 Func_02001704();
s32 Func_0200171e();
s32 Func_02001738();
s32 Func_02001746();

extern s16 Data_02000240[];
extern u8 Value_0000006b;
extern u8 Value_00000070;
extern u8 Value_0000006c;

extern u8 Data_02009e74[];
extern u8 Data_0200a018[];
extern u8 Data_0200a120[];
extern u8 Data_02009e80[];
extern u8 Data_02009fa0[];
extern u8 Data_0200a24c[];
extern u8 Data_0200a30c[];
extern u8 Data_0200a390[];

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
