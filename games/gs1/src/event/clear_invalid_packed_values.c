#include "scene.h"
#include "abi/event/clear_invalid_packed_values.h"
#include "event_runtime.h"

extern struct EventValidationWork gCell;

s32 Event_ValidatePackedId(u32 packed_id);

void Event_ClearInvalidPackedValues(void)
{
    if (Event_ValidatePackedId(gCell.values[0]) != 0)
        gCell.values[0] = 0;
    if (Event_ValidatePackedId(gCell.values[1]) != 0)
        gCell.values[1] = 0;
}
