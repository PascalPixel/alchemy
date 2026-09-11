#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "abi/battle/effects/object_control/pause_object.h"

void Object_SetAction(u8 *, s32);

struct GlobalState {
    u8 unknown_000[0x249];
    u8 saved_byte;
    u8 unknown_24a[6];
    u32 saved_callback;
};

extern struct GlobalState gCell;

void BattleFx_PauseObject(s32 arg0)
{
    u8 *object;
    u8 *entry;

    object = ObjectTable_Get(arg0);
    if (object != NULL) {
        gCell.saved_callback = *(u32 *)(object + 0x6C);
        gCell.saved_byte = 0;
        if (object[0x54] == 1) {
            entry = *(u8 **)(*(u8 **)(object + 0x50) + 0x28);
            if (entry != NULL) {
                gCell.saved_byte = entry[5];
            }
        }
        *(u32 *)(object + 0x6C) = (u32)Battle_Run;
        object[0x5B] = 1;
        Object_SetAction(object, 0);
    }
}
