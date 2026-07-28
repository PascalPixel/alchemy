#include "event_runtime.h"
#include "object_lookup.h"

extern struct EventValueWork Data_02000240;

void Func_080030f8(s32);
void Func_0808b674(s32);
void Func_0809177c(void);

void Func_08091780(s32 object_id)
{
    Func_0809177c();
    Func_0808b674(object_id);
    Func_080030f8(1);
    Func_0808ba1c(Data_02000240.value);
}
