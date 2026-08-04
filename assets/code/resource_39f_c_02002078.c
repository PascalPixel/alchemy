#include "types.h"

/* 0x02004f28 serves two imports: the two-argument setter at site 7 and the
 * three-argument step at site 14. */
extern u8 *Func_02004e40(s32);
extern void Func_02004e30(void);
extern void Func_02002f3c(s32, s32);
extern void Func_02002e38(s32, s32, s32, s32);
extern void Func_02004e4a(s32);
extern void Func_02002bb4(s32, s32, s32, s32, s32, s32, s32, s32);
extern void Func_02004f28(s32, s32);
extern void Func_02004efa(s32, s32, s32);
extern void Func_02004e80(s32);
extern void Func_02004ef0(s32, s32);
extern void Func_02004f42(s32, s32);
extern void Func_02004e98(s32);
extern void Func_02002e9a(s32, s32, s32, s32);
extern void Func_02004f28_b(s32, s32, s32);
extern void Func_02004eb8(s32);
extern void Func_02002ebc(s32, s32, s32, s32);
extern void Func_02004f4a(s32, s32, s32);
extern void Func_02004ed8(s32);
extern void Func_02002edc(s32, s32, s32, s32);
extern void Func_02004f6a(s32, s32, s32);
extern void Func_02004ef8(s32);
extern void Func_02002efa(s32, s32, s32, s32);
extern void Func_02004f88(s32, s32, s32);
extern void Func_02004f16(s32);
extern void Func_02004fd6(s32, s32);
extern void Func_02004f80(s32, s32, s32);
extern void Func_02004f2e(s32);
extern void Func_02004f2c(s32);
extern void Func_02004f48(void);
void Func_02002078(void)
{
    u8 *slot;

    slot = Func_02004e40(18);

    /* r0 still holds the record returned above. */
    Func_02004e30();

    Func_02002f3c(18, 1);
    Func_02002e38(18, 712, 536, 0x60000);       /* 178 << 2, 134 << 2, 192 << 11 */
    Func_02004e4a(10);

    Func_02002bb4(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004f28(18, 1);
    Func_02004efa(18, 0, 0);
    Func_02004e80(20);
    Func_02004ef0(18, 2);
    Func_02004f42(18, 258);                     /* 129 << 1 */
    Func_02004e98(60);

    Func_02002e9a(18, 712, 568, 0x60000);       /* 142 << 2 */
    Func_02004f28_b(0, 18, 0);
    Func_02004eb8(10);

    Func_02002ebc(18, 712, 600, 0x30000);       /* 150 << 2, 192 << 10 */
    Func_02004f4a(0, 18, 0);
    Func_02004ed8(6);

    Func_02002edc(18, 736, 640, 0x30000);       /* X += 24, 160 << 2 */
    Func_02004f6a(0, 18, 0);
    Func_02004ef8(6);

    Func_02002efa(18, 736, 704, 0x30000);       /* 176 << 2 */
    Func_02004f88(0, 18, 0);
    Func_02004f16(6);

    Func_02004fd6(0, 1);
    Func_02004f80(18, 0, 0);
    Func_02004f2e(30);
    Func_02004f2c(0x30b);

    /* Common exit; no argument registers are set. */
    Func_02004f48();
}
