#include "types.h"

extern u8 Data_00000008[];
extern u8 Data_080fc624[];
void Func_080fab3c(u8 *entry);

void Func_080fa458(void)
{
    u32 value = (u16)(u32)Data_00000008;

    if (value != 0) {
        u8 *entry = Data_080fc624;
        u32 count = value;

        do {
            Func_080fab3c(*(void **)entry);
            entry += 12;
            count--;
        } while (count != 0);
    }
}
