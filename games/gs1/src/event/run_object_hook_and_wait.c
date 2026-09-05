#include "event_runtime.h"
#include "object_lookup.h"

extern struct EventValueWork Data_02000240;

void WaitFrames(s32);
void Func_0808b674(s32);
void Func_0809177c(void);

void Event_RunObjectHookAndWait(s32 object_id)
{
    Func_0809177c();
    Func_0808b674(object_id);
    WaitFrames(1);
    ObjectTable_Get(Data_02000240.value);
}
