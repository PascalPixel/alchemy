typedef unsigned char u8;
typedef signed int s32;

/* 0x02004d98 serves two imports: the two-argument setter at site 7 and the
 * three-argument one at site 14. */
extern u8 *Func_02004cb4(s32);
extern void Func_02004ca4(void);
extern void Func_02002db0(s32, s32);
extern void Func_02002ca8(s32, s32, s32, s32);
extern void Func_02004cba(s32);
extern void Func_02002a24(s32, s32, s32, s32, s32, s32, s32, s32);
extern void Func_02004d98(s32, s32);
extern void Func_02004d6a(s32, s32, s32);
extern void Func_02004cf0(s32);
extern void Func_02004d60(s32, s32);
extern void Func_02004db2(s32, s32);
extern void Func_02004d08(s32);
extern void Func_02002d0a(s32, s32, s32, s32);
extern void Func_02004d98_b(s32, s32, s32);
extern void Func_02004d28(s32);
extern void Func_02002d2c(s32, s32, s32, s32);
extern void Func_02004dba(s32, s32, s32);
extern void Func_02004d48(s32);
extern void Func_02002d4a(s32, s32, s32, s32);
extern void Func_02004dd8(s32, s32, s32);
extern void Func_02004d66(s32);
extern void Func_02004e26(s32, s32);
extern void Func_02004dd0(s32, s32, s32);
extern void Func_02004d7e(s32);
extern void Func_02004d7c(s32);
extern void Func_02004de6(s32, s32, s32);
extern void Func_02004da2(void);
void Func_02001ef0(void)
{
    u8 *slot;

    slot = Func_02004cb4(17);

    /* r0 still holds the record returned above. */
    Func_02004ca4();

    Func_02002db0(17, 1);
    Func_02002ca8(17, 392, 104, 0x60000);       /* 196 << 1, 192 << 11 */
    Func_02004cba(10);

    Func_02002a24(*(s32 *)(slot + 8), *(s32 *)(slot + 12),
                  *(s32 *)(slot + 16) + 0x40000,    /* 128 << 11 */
                  0, 0, 0, 1, 0);

    Func_02004d98(17, 1);
    Func_02004d6a(17, 0, 0);
    Func_02004cf0(20);
    Func_02004d60(17, 2);
    Func_02004db2(17, 258);                     /* 129 << 1 */
    Func_02004d08(60);

    Func_02002d0a(17, 376, 152, 0x60000);       /* 188 << 1 */
    Func_02004d98_b(0, 17, 0);
    Func_02004d28(10);

    Func_02002d2c(17, 328, 160, 0x30000);       /* 164 << 1, 192 << 10 */
    Func_02004dba(0, 17, 0);
    Func_02004d48(6);

    Func_02002d4a(17, 296, 160, 0x30000);       /* 148 << 1 */
    Func_02004dd8(0, 17, 0);
    Func_02004d66(6);

    Func_02004e26(0, 1);
    Func_02004dd0(17, 0, 0);
    Func_02004d7e(30);
    Func_02004d7c(0x309);
    Func_02004de6(21, 0, 0);

    /* Common exit; no argument registers are set. */
    Func_02004da2();
}
