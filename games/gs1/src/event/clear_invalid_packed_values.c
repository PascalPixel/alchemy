#include "event_runtime.h"

extern struct EventValidationWork Data_02000240;

s32 Event_ValidatePackedId(u32 packed_id);

void Event_ClearInvalidPackedValues(void)
{
    if (Event_ValidatePackedId(Data_02000240.values[0]) != 0)
        Data_02000240.values[0] = 0;
    if (Event_ValidatePackedId(Data_02000240.values[1]) != 0)
        Data_02000240.values[1] = 0;
}
