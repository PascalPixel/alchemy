#include "types.h"
#include "motobj.h"
#include "battle_session.h"
#include "ram_buffer.h"
#include "iwram_call.h"

s32 Camera_ApplyTransformByFlag(void);
s32 Render_ProjectPoint(const s32 *, s32 *);
void *GetMotionRecord(struct MotionObject *object, s32 record_index);

struct MotionRecordScale {
    u8 unknown_00[0xc];
    s32 scale;
};

static __inline__ struct BtlObj *BattleObject_FindSlot(s32 id)
{
    u8 *work = (u8 *)Ram_HeapSlots->battle_work;
    struct BtlObj *slots = (struct BtlObj *)(work + 0x84);

    if (id > 7)
        id -= 0x78;
    id += 0x74;
    if (work[id] == 0xff)
        return NULL;
    return &slots[work[id]];
}

s32 BattleMotion_ProjectPosition(s32 id, s32 *projected)
{
    struct MotionObject *object = BattleObject_FindSlot(id)->object;
    struct MotionRecordScale *record = GetMotionRecord(object, 0);
    s32 position[3];
    s32 scaled;

    Camera_ApplyTransformByFlag();
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z;
    scaled = Render_ProjectPoint(position, projected);
    (void)Iwram_MulQ16(scaled, record->scale);
    return 0;
}
