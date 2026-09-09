#include "types.h"

/*
 * Data selector for resource_3ae.  The 164-byte owner includes its 13-word
 * literal pool.
 */

/* Per-call-site import labels.  All four sites reach the same event-flag test,
 * but each keeps its own name because the relocated call word differs. */
s32 Func_02001608();
s32 Func_02001622();
s32 Func_0200163c();
s32 Func_0200164a();

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
