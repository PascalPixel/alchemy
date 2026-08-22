#include "types.h"

extern void Func_08004278(void (*)(void));
extern void Func_08002dd8(s32);

typedef void (*Transfer)(void *, s32);
extern u8 Data_080cc961;
extern u8 Data_080cd261;
extern u8 Data_080cd4b5;

void Func_080ccbdc(void) {
    Func_08004278((void (*)(void))&Data_080cc961);
    Func_08004278((void (*)(void))&Data_080cd261);
    {
        Transfer transfer = (Transfer)0x03000164;

        transfer((void *)0x06004000, 0x4000);
    }
    Func_08004278((void (*)(void))&Data_080cd4b5);
    Func_08002dd8(40);
    Func_08002dd8(39);
}
