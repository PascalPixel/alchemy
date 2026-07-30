/*
 * resource_3c4 @ 0x02001500 (14 bytes).
 *
 * `movs r0,#0x97 ; lsls r0,r0,#4` materialises 0x970, the sole argument.
 * The epilogue is `pop {r0} ; bx r0`: r0 holds the popped return address at
 * the `bx`, so no value is returned to the caller and the owner is void.
 */
typedef signed int s32;

void Func_020045e4(s32 id);

void Func_02001500(void)
{
    Func_020045e4(0x970);
}
