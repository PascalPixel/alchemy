#include "EVENT_RUNTIME.H"

extern struct EventValidationWork gGameState;

s32 Event_ValidatePackedId(u32 packed_id);

void Event_ClearInvalidPackedValues(void)
{
    if (Event_ValidatePackedId(gGameState.values[0]) != 0)
        gGameState.values[0] = 0;
    if (Event_ValidatePackedId(gGameState.values[1]) != 0)
        gGameState.values[1] = 0;
}
