#include "types.h"
#include "motion_object.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Object_SetMode(void *, s32);
s32 Object_ResetMotion(void *);
s32 Object_SetPosition(void *, s32, s32, s32);
struct BattleObjectSlot *GetBattleObjectSlot(s32 object_id);

void BattleMotion_SetupEscapeObject(s32 object_id) {
    void *slot;
    void *object;

    slot = GetBattleObjectSlot(object_id);
    object = FIELD_AT_OFFSET(slot, void **, 0);
    FIELD_AT_OFFSET(object, s32 *, 0x34) = 0x20000;
    FIELD_AT_OFFSET(object, s32 *, 0x30) = 0x80000;
    FIELD_AT_OFFSET(object, s32 *, 0x28) = 0x50000;
    FIELD_AT_OFFSET(object, s32 *, 0x48) = 0x7851;
    FIELD_AT_OFFSET(object, s32 *, 0x44) = 0;
    FIELD_AT_OFFSET(object, s8 *, 0x5A) = 0;
    Object_ResetMotion(object);
    Object_SetPosition(object, FIELD_AT_OFFSET(slot, s32 *, 0xC) * 3, 0, FIELD_AT_OFFSET(slot, s32 *, 0x10));
    Object_SetMode(object, 1);
}
