#include "types.h"
#include "motion_object.h"
#include "global_cells.h"

struct BattleObjectSlot *GetBattleObjectSlot(s32 object_id) {
    u8 *base = *(u8 **)ADDR_03001E74;
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

struct MotionRecordState {
    u8 unknown_00[0x10];
    s32 field10;
};

void ResetMotionRecordGroup(void *owner)
{
    s32 remaining;
    s32 zero;
    struct MotionRecordState **items;

    if (owner != NULL) {
        zero = 0;
        items = (struct MotionRecordState **)((u8 *)owner + 0x28);
        for (remaining = 3; remaining >= 0; remaining--) {
            struct MotionRecordState *item = *items++;
            if (item != NULL) {
                item->field10 = zero;
            }
        }
    }
}

void ResetBattleObjectRecordGroups(struct MotionObject *object)
{
    s32 storage_kind;
    s32 remaining;
    void **record_groups;
    void *record_group;

    if (object != NULL) {
        storage_kind = object->record_storage_kind & 0xF;
        switch (storage_kind) {
        case 1:
            ResetMotionRecordGroup(object->records);
            return;
        case 2:
            record_groups = object->records;
            remaining = 3;
            do {
                record_group = *record_groups++;
                if (record_group != NULL) {
                    ResetMotionRecordGroup(record_group);
                }
                remaining -= 1;
            } while (remaining >= 0);
            break;
        }
    }
}

s32 Func_080b6e30();
s32 ActivateBattleObjectSlot(s32 object_id)
{
  struct BattleObjectSlot *slot;
  Func_080b6e30();
  slot = GetBattleObjectSlot(object_id);
  slot->active = 1;
  return 0;
}


s32 Object_Destroy(s32);


void *BattleMotion_DestroyAllSlotObjects(void)
{
    s32 no;
    s32 i;
    struct BattleObjectSlot *slot;

    i = 0;
    do {
        no = i + 0x78;
        if (i <= 7) {
            no = i;
        }
        slot = GetBattleObjectSlot(no);
        if ((slot != NULL) && (slot->active != 0)) {
            Object_Destroy((s32)slot->object);
            slot->object = NULL;
            slot->active = 0;
        }
        i += 1;
    } while (i <= 0xD);
}
