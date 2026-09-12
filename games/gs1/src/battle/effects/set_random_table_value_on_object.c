#include "fixed_math.h"
#include "types.h"
#include "scene.h"
#include "object_lookup.h"

/* battle/effects/objects/set_random_table_value.c */
extern s8 gRom[];

u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16

void BattleFx_SetRandomTableValueOnObject(s32 arg0)
{
    s8 *table = gRom;
    s32 index = Rand();
    Battle_Apply(arg0, table[(u32)(index * 8) >> 16]);
}

/* battle/effects/object_control/pause_object.c */
void Object_SetAction(void *, s32);

struct GlobalState {
    u8 unknown_000[0x249];
    u8 saved_byte;
    u8 unknown_24a[6];
    u32 saved_callback;
};

extern struct GlobalState gCell;
void Battle_Run(void);

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
