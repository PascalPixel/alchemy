#include "types.h"



/* Complete actor-20 presentation-bit update through interworking return. */
extern u8 * Func_020016fa();
extern u8 * Func_02001708();
extern u8 * Func_02001718();
void Func_02000324(void)
{
    u8 *entity = Func_020016fa(0);
    u8 *actor;

    if (*(s16 *)(entity + 14) > 31) {
        actor = Func_02001708(20);
        actor[35] |= 2;
    } else {
        actor = Func_02001718(20);
        actor[35] &= 0xfd;
    }
}
