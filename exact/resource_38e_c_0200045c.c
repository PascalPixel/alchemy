#include "types.h"

extern u8 *Data_03001ebc;

extern void Func_02000f34(void);
extern s32 Func_02000f1c(s32);
extern void Func_02000d58(void);
extern void Func_02000f7a(s32, s32, s32);
extern void Func_02000fc0(s32, s32);
extern void Func_02000fb4(s32, s32, s32);
extern void Func_02000f72(s32);
extern void Func_02001050(s32);
extern void Func_02000f8c(void);

void Func_0200045c(void)
{
    u8 *workspace;

    Func_02000f34();

    if (Func_02000f1c(512) == 0) {   /* 128 << 2 */
        Func_02000d58();
    }

    Func_02000f7a(0, 0x00008000, 0x00004000);   /* 0.5, 0.25 in 16.16 */

    /* 448 is the offset and 256 is the value stored there. The reference
     * derives 256 by subtracting 192 from the offset register it has just
     * built, which only works because 256 will not fit a Thumb `movs`. */
    workspace = Data_03001ebc;
    *(s32 *) (workspace + 448) = 256;

    Func_02000fc0(0, 2);
    Func_02000fb4(0, 2, -16);
    Func_02000f72(16);
    Func_02001050(2);
    Func_02000f8c();
}
