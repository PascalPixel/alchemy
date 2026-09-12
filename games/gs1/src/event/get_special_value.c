#include "types.h"
#include "scene.h"
#include "object_lookup.h"

/* event/get_special_value.c */
/* event/get_special_value.c */
/* event/get_special_value.c */
struct Fields_0808b248 {
    u8 filler[0x1d6];
    s16 value;
};

extern s16 gCell[];

s16 Event_GetSpecialValue(void)
{
    /* 作業領域0x1d6の半語を返す。 */
    return gCell.value;
}

/* battle/effects/fx_set_special_from_table.c */
/* battle/effects/set/set_special_from_table.c */
extern s32 gRom[];

void BattleFx_SetSpecialFromTable(s32 arg0, s32 arg1)
{
    s32 target = gCell[224];
    s32 *table = gRom;
    s32 entry = *table++;
    s32 result = arg1;

    if (entry != 0 && entry != target) {
        do {
            if (entry & 0x80000000) {
                result = entry & 0xFFFF;
            }
            entry = *table++;
        } while (entry != 0 && entry != target);
    }
    gCell[235] = (s16)result;
}

/* battle/effects/data/select_result_pointer.c */
extern u8 Value_00000038;
extern u8 Value_0000003a;
extern u8 Value_0000003c;
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000039;

void BattleFx_SelectResultPointer(s32 arg0)
{
    u16 value;

    switch (arg0 - 1) {
    case 0:
        value = (u16)(u32)&Value_00000038;
        break;
    case 1:
        value = (u16)(u32)&Value_0000003a;
        break;
    case 2:
        value = (u16)(u32)&Value_0000003c;
        break;
    case 3:
    case 6:
        value = (u16)(u32)&Value_00000036;
        break;
    case 4:
    case 5:
        value = (u16)(u32)&Value_00000037;
        break;
    default:
        value = (u16)(u32)&Value_00000039;
        break;
    }
    gCell[235] = value;
}

/* party/remap_character_id_by_flags.c */
/* party/remap_character_id_by_flags.c */
s32 GameFlag_IsSet(s32);

s32 Party_RemapCharacterIdByFlags(s32 arg0)
{
    s32 result;

    result = arg0;
    if (result <= 8) {
        if (GameFlag_IsSet(0x20) != 0) {
            if (result == 0) {
                result = 0x12;
            }
            if (result == 1) {
                result = 0x13;
            }
        } else if ((GameFlag_IsSet(0x21) != 0) && (result == 0)) {
            result = 0x11;
        }
    }
    return result;
}

/* party/remap_character_id_if_enabled.c */
s32 Party_RemapCharacterIdIfEnabled(s32 arg0, s32 arg1)
{
    s32 result;

    result = arg0;
    if ((result <= 8) && (arg1 != 0)) {
        if (result == 0) {
            result = 0x12;
        }
        if (result == 1) {
            result = 0x13;
        }
    }
    return result;
}

/* object/table/destroy_at_index.c */
struct Fields_0808b64c {
    u8 filler[20];
    void *objects[1];
};

extern struct Fields_0808b64c *gWork;

void Object_Destroy(void *);

void ObjectTable_DestroyAtIndex(s32 arg0)
{
    struct Fields_0808b64c *state = gWork;
    void *object = ObjectTable_Get(arg0);
    if (object != 0) {
        Object_Destroy(object);
        state->objects[arg0] = 0;
    }
}
