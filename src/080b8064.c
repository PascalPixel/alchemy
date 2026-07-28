#include "types.h"
#include "motion_object.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08009080(void *, s32);
s32 Func_08009140(void *);
s32 Func_08009150(void *, s32, s32, s32);
struct BattleObjectSlot *Func_080b7dd0(s32 object_id);

void Func_080b8064(s32 object_id) {
    void *slot;
    void *object;

    slot = Func_080b7dd0(object_id);
    object = M2C_FIELD(slot, void **, 0);
    M2C_FIELD(object, s32 *, 0x34) = 0x20000;
    M2C_FIELD(object, s32 *, 0x30) = 0x80000;
    M2C_FIELD(object, s32 *, 0x28) = 0x50000;
    M2C_FIELD(object, s32 *, 0x48) = 0x7851;
    M2C_FIELD(object, s32 *, 0x44) = 0;
    M2C_FIELD(object, s8 *, 0x5A) = 0;
    Func_08009140(object);
    Func_08009150(object, M2C_FIELD(slot, s32 *, 0xC) * 3, 0, M2C_FIELD(slot, s32 *, 0x10));
    Func_08009080(object, 1);
}
