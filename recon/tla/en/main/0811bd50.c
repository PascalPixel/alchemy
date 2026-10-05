/* Near miss: differ 2, one scheduling step. ☀️'s
   BattleMotion_ProjectPosition with the slot found inline and the record's
   scale at 0xc. The listing passes the projected pointer (adds r1, r7, #0)
   before loading the object's y (ldr r3, [r5, #12]); this build loads
   first. Goes after Camera_ApplyTransformByFlag in
   RESET_BATTLE_OBJECT_RECORD_GROUPS.C. */
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_SESSION.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"

s32 Camera_ApplyTransformByFlag(void);
s32 Render_ProjectPoint(const s32 *, s32 *);
void *GetMotionRecord(struct MotionObject *object, s32 record_index);

struct MotionRecordScale {
    u8 unknown_00[0xc];
    s32 scale;
};

static __inline__ struct BattleObjectSlot *BattleObject_FindSlot(s32 id)
{
    u8 *work = (u8 *)Ram_HeapSlots->battle_work;
    struct BattleObjectSlot *slots = (struct BattleObjectSlot *)(work + 0x84);

    if (id > 7)
        id -= 0x78;
    id += 0x74;
    if (work[id] == 0xff)
        return NULL;
    return &slots[work[id]];
}

/* ☀️'s: projects a unit's object onto the screen. */
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
