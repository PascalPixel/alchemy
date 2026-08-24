#include "types.h"

struct Obj {
    u8 padding[4];
    s8 kind;
    u8 padding2;
    u16 index : 9;
    u16 rest : 7;
};
void Runtime_PushSlotEntry(s32 *arg0, s32 arg1);
void TextResource_SetPosition(struct Obj *obj, s32 arg1, s32 arg2) {
    obj->index = arg1;
    obj->kind = arg2;
    Runtime_PushSlotEntry((s32 *) obj, 0xFC);
}
