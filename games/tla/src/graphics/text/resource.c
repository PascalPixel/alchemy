#include "types.h"
#include "resource.h"
#include "callback_scheduler.h"

struct TextResourcePosition {
    u8 padding[4];
    s8 kind;
    u8 padding2;
    u16 index : 9;
    u16 rest : 7;
};

void UiTextResource_SetPosition(struct TextResourcePosition *obj, s32 arg1, s32 arg2)
{
    obj->index = arg1;
    obj->kind = arg2;
    /* FAKEMATCH: the native stores the index before it loads the slot kind. */
    __asm__("");
    Runtime_PushSlotEntry((s32 *)obj, 0xFC);
}

void UiTextResource_Release(s32 value)
{
    Resource_ResetEntry(value);
}
