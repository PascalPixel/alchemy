#include "types.h"
#include "motion_object.h"

struct BattleObjectSlot *Func_080b7dd0(s32 object_id) {
    u8 *base = *(u8 **)0x03001E74;
    u8 *result_base = base + 0x74;
    s32 offset;
    if (object_id > 7) {
        object_id -= 0x78;
    }
    offset = object_id + 0x2DC;
    if (base[offset] == 0xFF) {
        return NULL;
    }
    return (struct BattleObjectSlot *)(result_base + base[offset] * 0x2C);
}
