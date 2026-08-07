#include "types.h"



/* Complete actor-20 presentation-bit update through interworking return. */
extern u8 * Func_020016fa();
extern u8 * Func_02001708();
extern u8 * Func_02001718();
/*
 * 3 halfwords out at 52 bytes: the merged flag value lands in r2 and the
 * loaded byte in r3, the reverse of the reference.  Every source shape that
 * moves the pair (mask local, byte local, single variable, either operand
 * order) leaves the same join-register choice.
 */
void Func_02000324(void)
{
    u8 *entity = Func_020016fa(0);
    u8 *slot;
    u32 flag;
    u32 dummy;

    if (*(s16 *)(entity + 14) > 31) {
        slot = Func_02001708(20) + 35;
        dummy = *slot;
        flag = dummy | 2;
    } else {
        slot = Func_02001718(20) + 35;
        dummy = *slot;
        flag = dummy & 0xfd;
    }
    *slot = (u8)flag;
}
