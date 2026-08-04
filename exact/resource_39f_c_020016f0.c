#include "types.h"

extern u8 *Func_020044b4(s32);
extern void Func_020044a2(void);
extern void Func_020025ae(s32, s32);
extern void Func_020024a6(s32, s32, s32, s32);
extern void Func_020044b8(s32);
extern void Func_02002222(s32, s32, s32, s32, s32, s32, s32, s32);
extern void Func_02004596(s32, s32);
extern void Func_02004568(s32, s32, s32);
extern void Func_020044ee(s32);
extern void Func_0200455e(s32, s32);
extern void Func_020045b0(s32, s32);
extern void Func_02004506(s32);
extern void Func_0200250a(s32, s32, s32, s32);
extern void Func_02004598(s32, s32, s32);
extern void Func_02004526(s32);
extern void Func_02002528(s32, s32, s32, s32);
extern void Func_020045b6(s32, s32, s32);
extern void Func_02004546(s32);
extern void Func_0200254a(s32, s32, s32, s32);
extern void Func_020045d8(s32, s32, s32);
extern void Func_02004566(s32);
extern void Func_02002568(s32, s32, s32, s32);
extern void Func_020045f6(s32, s32, s32);
extern void Func_02004584(s32);
extern void Func_020045e6(s32, s32, s32);
extern void Func_0200458e(s32);
extern void Func_020045f8(s32, s32, s32);
extern void Func_020045b4(void);
void Func_020016f0(void)
{
    u8 *slot;

    slot = Func_020044b4(13);

    /* r0 still holds the record returned above. */
    Func_020044a2();

    Func_020025ae(13, 1);
    Func_020024a6(13, 456, 104, 0x70000);       /* 228 << 1, 224 << 11 */
    Func_020044b8(10);

    Func_02002222(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004596(13, 1);
    Func_02004568(13, 0, 0);
    Func_020044ee(20);
    Func_0200455e(13, 2);
    Func_020045b0(13, 258);                     /* 129 << 1 */
    Func_02004506(60);

    Func_0200250a(13, 472, 136, 0x30000);       /* 236 << 1, 192 << 10 */
    Func_02004598(0, 13, 0);
    Func_02004526(6);

    Func_02002528(13, 504, 136, 0x33333);       /* 252 << 1, pooled height */
    Func_020045b6(0, 13, 0);
    Func_02004546(6);

    Func_0200254a(13, 552, 136, 0x38000);       /* 138 << 2, 224 << 10 */
    Func_020045d8(0, 13, 0);
    Func_02004566(6);

    Func_02002568(13, 584, 136, 0x38000);       /* 146 << 2 */
    Func_020045f6(0, 13, 0);
    Func_02004584(6);

    Func_020045e6(13, 0, 0);
    Func_0200458e(772);                         /* 193 << 2 */
    Func_020045f8(16, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_020045b4();
}
