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

/* battle/effects/object_control/resume_object.c */


void BattleFx_ResumeObject(void)
{
    u8 *object = ObjectTable_Get();
    if (object != 0) {
        if (*(void (**)(void))(object + 0x6C) == Battle_Run) {
            *(s32 *)(object + 0x6C) = gCell.saved_callback;
            gCell.saved_callback = 0;
            Battle_Apply(object, gCell.saved_byte);
        }
        object[0x5B] = 0;
        Object_SetAction(object, 16);
    }
}

/* battle/effects/obj/filter_object_id.c */
s32 BattleFx_GetFlags();

s32 BattleFx_FilterObjectIdByFlags(s32 arg0)
{
    if (BattleFx_GetFlags() == 0xFF) {
        return -1;
    }
    return arg0;
}
