#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

extern void *Func_08077008(s32);
extern s32 Func_080c2454(s32 index);
extern s32 Func_080c23c0(s32 index);

u32 Func_080b8530(s32 object_id) {
    u32 palette_value;
    u8 character_id;

    character_id = M2C_FIELD(Func_08077008(object_id), u8 *, 0x128);
    palette_value = (u32) (Func_080c2454((s32) character_id) << 0x18) >> 8;
    if (palette_value == 0) {
        character_id = M2C_FIELD(Func_08077008(object_id), u8 *, 0x128);
        if (Func_080c23c0((s32) character_id) != 0) {
            palette_value = 0x180000;
        } else {
            palette_value = 0x300000;
        }
    }
    return palette_value;
}
