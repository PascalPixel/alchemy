/*
 * resource_3c4 @ 0x020013a4 (16 bytes).
 *
 * Three immediate arguments and one call.  `pop {r0} ; bx r0` return, so the
 * owner is void.
 */
typedef signed int s32;

void Func_02004502(s32 arg0, s32 arg1, s32 arg2);

void Func_020013a4(void)
{
    Func_02004502(0, 6, 0);
}
