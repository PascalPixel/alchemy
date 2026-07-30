/*
 * resource_3c4 @ 0x02001374 (46 bytes).
 *
 * One six-argument placement (four register arguments plus [sp,#0] = 29 and
 * [sp,#4] = 30) bracketed by service calls.
 *
 * Func_02001456 is a genuine entry: 0x02001456 holds an alignment `nop` and
 * falls into the `push {lr}` prologue at 0x02001458.
 *
 * `add sp,#8 ; pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;

void Func_0200446e(void);
void Func_0200442a(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001456(void);
void Func_020037a6(void);
void Func_02004496(void);

void Func_02001374(void)
{
    Func_0200446e();
    Func_0200442a(93, 30, 6, 5, 29, 30);
    Func_02001456();
    Func_020037a6();
    Func_02004496();
}
