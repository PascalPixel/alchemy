#include "types.h"

extern u8 *Func_02004ba4(s32);
extern void Func_02004b92(void);
extern void Func_02002c9e(s32, s32);
extern void Func_02002b96(s32, s32, s32, s32);
extern void Func_02004ba8(s32);
extern void Func_02002912(s32, s32, s32, s32, s32, s32, s32, s32);
extern void Func_02004c86(s32, s32);
extern void Func_02004c58(s32, s32, s32);
extern void Func_02004bde(s32);
extern void Func_02004c4e(s32, s32);
extern void Func_02004ca0(s32, s32);
extern void Func_02004bf8(s32);
extern void Func_02002bfc(s32, s32, s32, s32);
extern void Func_02004c8c(s32, s32, s32);
extern void Func_02004c1c(s32);
extern void Func_02002c1c(s32, s32, s32, s32);
extern void Func_02004caa(s32, s32, s32);
extern void Func_02004c38(s32);
extern void Func_02002c38(s32, s32, s32, s32);
extern void Func_02004cc6(s32, s32, s32);
extern void Func_02004c54(s32);
extern void Func_02004d14(s32, s32);
extern void Func_02004cbe(s32, s32, s32);
extern void Func_02004c6c(s32);
extern void Func_02004cd6(s32, s32, s32);
extern void Func_02004c92(void);
void Func_02001de0(void)
{
    u8 *slot;

    slot = Func_02004ba4(16);

    /* r0 still holds the record returned above. */
    Func_02004b92();

    Func_02002c9e(16, 1);
    Func_02002b96(16, 456, 152, 0x60000);       /* 228 << 1, 192 << 11 */
    Func_02004ba8(10);

    Func_02002912(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004c86(16, 1);
    Func_02004c58(16, 0, 0);
    Func_02004bde(20);
    Func_02004c4e(16, 2);
    Func_02004ca0(16, 258);                     /* 129 << 1 */
    Func_02004bf8(60);

    /* Three waypoints at height 0x30000 (192 << 10). */
    Func_02002bfc(16, 448, 192, 0x30000);       /* 224 << 1 */
    Func_02004c8c(0, 16, 0);
    Func_02004c1c(6);

    Func_02002c1c(16, 424, 208, 0x30000);       /* 212 << 1 */
    Func_02004caa(0, 16, 0);
    Func_02004c38(6);

    Func_02002c38(16, 424, 224, 0x30000);
    Func_02004cc6(0, 16, 0);
    Func_02004c54(6);

    Func_02004d14(0, 1);
    Func_02004cbe(16, 0, 0);
    Func_02004c6c(30);
    Func_02004c6c(776);                         /* 194 << 2 */
    Func_02004cd6(20, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02004c92();
}
