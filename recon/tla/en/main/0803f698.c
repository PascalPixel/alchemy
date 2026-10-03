#include "CALLBACK_SCHEDULER.H"
/*
 * Draft: UiTextResource_SetPosition does not yet match; 5 halfwords differ from ☀️'s C, first at +0xa (strb r2, [r0, #4]).
 * Links as recon/tla/raw/0803f698.s.
 */
#include "TYPES.H"

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
    Runtime_PushSlotEntry((s32 *)obj, 0xFC);
}
