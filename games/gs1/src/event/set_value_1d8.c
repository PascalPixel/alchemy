#include "event_runtime.h"

#define Event_SetValue1d8 Func_08092b94

struct EventRuntime1d8 {
    u8 unknown_000[0x1d8];
    s16 value;
};

void Event_SetValue1d8(s16 value)
{
    ((struct EventRuntime1d8 *)Data_03001ebc)->value = value;
}
