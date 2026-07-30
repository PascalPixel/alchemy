typedef signed int s32;

/*
 * resource_3a7 owner at 0x02001554, 16 bytes.
 *
 * TRANSPOSED from the byte-exact assets/code/resource_3a7_c_0200143c.c, a twin
 * inside this same overlay.  The two owners are BYTE-IDENTICAL: all 8 halfwords
 * match, no calls, no pool word.  Only the entry symbol was renamed.
 */
s32 Func_02001554(s32 *state)
{
    return *state == 4;
}
