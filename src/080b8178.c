#include "types.h"
#include "motion_object.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08009140(void *);
s32 Func_08009150(void *, s32, s32, s32);
struct BattleObjectSlot *Func_080b7dd0(s32 object_id);

void Func_080b8178(s32 object_id) {
    u32 scaled_x;
    void *slot;
    void *object;

    slot = Func_080b7dd0(object_id);
    object = M2C_FIELD(slot, void **, 0);
    M2C_FIELD(object, s32 *, 0x34) = 0x10000;
    M2C_FIELD(object, s32 *, 0x30) = 0x40000;
    M2C_FIELD(object, s32 *, 0x28) = 0x30000;
    M2C_FIELD(object, s32 *, 0x48) = 0x9999;
    M2C_FIELD(object, s32 *, 0x44) = 0;
    M2C_FIELD(object, s8 *, 0x5A) = 0;
    Func_08009140(object);
    scaled_x = M2C_FIELD(slot, s32 *, 0xC) * 3;
    Func_08009150(object, (s32) (scaled_x + (scaled_x >> 0x1F)) >> 1, 0, M2C_FIELD(slot, s32 *, 0x10));
}
