#include "types.h"
/* resource_387 owner at 0x02000d04, 100 bytes. */
struct Rec_387 { u8 pad00[8]; s32 f8; };
void Func_02001e36();
struct Rec_387 *Func_02001e54();
void Func_02001af6();
u8 *Func_02001e68();
void Func_02001dfe();
void Func_02001e0e();
void Func_02001e66();
void Func_02001e92();
void Func_02000d04(void)
{
    s32 depth;
    s32 span = 12;

    Func_02001e36();
    depth = Func_02001e54(8)->f8 >> 20;
    if (depth == 11) {
        Func_02001af6(8);
        Func_02001e68(8)[35] |= 2;
        Func_02001dfe(39, 12, 3, 1, 8, span);
        Func_02001e0e(43, 11, 3, 1, span, depth);
        Func_02001e66(2144);
    }
    Func_02001e92();
}
