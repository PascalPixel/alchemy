#include "types.h"

/* Create one scene handle and bind its three adjacent resource records. */
extern u8 Data_02008920;
extern u8 Data_0200893c;
extern u8 Data_02008958;




extern s32 Func_02000f6a(s32, s32, s32, s32, s32);
extern void Func_02000fa0(const void *, s32, s32, s32);
extern void Func_02000fac(const void *, s32, s32, s32);
extern void Func_02000fb8(const void *, s32, s32, s32);
void Func_02000768(void)
{
    /* The reference frame is `sub sp, #36`: 4 bytes for Func_02000f6a's
     * stacked fifth argument plus 32 local bytes no shipped instruction
     * reads or writes -- a local record the original still declared. Its
     * element type is not recoverable from the bytes; only its 32-byte
     * size is. */
    u8 workspace[32];
    s32 handle = Func_02000f6a(0, 13, 30, 6, 2);

    Func_02000fa0(&Data_02008920, handle, 0, 0);
    Func_02000fac(&Data_0200893c, handle, 0, 8);
    Func_02000fb8(&Data_02008958, handle, 0, 16);
}
