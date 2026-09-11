#include "scene.h"
#include "abi/event/set_value_1d8.h"
#include "event_runtime.h"

struct EventRuntime1d8 {
    u8 unknown_000[0x1d8];
    s16 value;
};

void Event_SetValue1d8(s16 value)
{
    ((struct EventRuntime1d8 *)gWork)->value = value;
}
