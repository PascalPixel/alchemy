#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "motobj.h"
#include "battle_session.h"
#include "btlwork.h"
#include "ram_buffer.h"

#define Object_Destroy ((void (*)(void*))Object_Destroy)



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

static __inline__ struct BtlObj *BattleObject_FindSlot(s32 id)
{
    u8 *work = (u8 *)Ram_HeapSlots->battle_work;
    struct BtlObj *slots = ((struct BtlWork *)work)->slots;

    if (id > 7)
        id -= 0x78;
    id += 0x74;
    if (work[id] == 0xff)
        return NULL;
    return &slots[work[id]];
}

s32 ActivateBattleObjectSlot(s32 object_id)
{
    struct BtlObj *slot;

    BattleMotion_ReleaseObjectSlotByValue(object_id);
    slot = BattleObject_FindSlot(object_id);
    slot->active = 1;
    return 0;
}

void BattleMotion_DestroyAllSlotObjects(void)
{
    s32 no;
    s32 i;
    struct BtlObj *slot;

    i = 0;
    do {
        no = i + 0x78;
        if (i <= 7) {
            no = i;
        }
        slot = BattleObject_FindSlot(no);
        if ((slot != NULL) && (slot->active != 0)) {
            Object_Destroy(slot->object);
            slot->object = NULL;
            slot->active = 0;
        }
        i += 1;
    } while (i <= 0xD);
}
