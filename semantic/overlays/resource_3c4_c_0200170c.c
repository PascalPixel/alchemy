/*
 * resource_3c4 @ 0x0200170c (54 bytes).
 *
 * Shuts slot 20 down: three service writes, then bit 1 of the byte at +35 of
 * the slot descriptor is cleared in place (`movs r3,#0xfd ; ands`), then a
 * 0x200 selector is published.  `pop {r0} ; bx r0` return: void.
 */
typedef signed int s32;
typedef unsigned char u8;

void Func_02004858(s32 slot, s32 arg1);
void Func_02004878(s32 slot, s32 arg1);
void Func_02004868(s32 slot, s32 arg1);
u8 *Func_02004836(s32 slot);
void Func_02004818(s32 selector);

void Func_0200170c(void)
{
    u8 *descriptor;

    Func_02004858(20, 1);
    Func_02004878(20, 0);
    Func_02004868(20, 2);
    descriptor = Func_02004836(20);
    descriptor[35] &= (u8)0xfd;
    Func_02004818(0x200);
}
