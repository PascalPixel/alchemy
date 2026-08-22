#include "event_runtime.h"

extern struct EventValidationWork Data_02000240;
s32 Func_08091814(u32 packed_id);

void Func_08091858(void)
{
    if (Func_08091814(Data_02000240.values[0]) != 0) {
        Data_02000240.values[0] = 0;
    }
    if (Func_08091814(Data_02000240.values[1]) != 0) {
        Data_02000240.values[1] = 0;
    }
}
