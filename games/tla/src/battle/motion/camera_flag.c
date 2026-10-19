#include "types.h"
#include "motobj.h"
#include "battle_session.h"
#include "battle_display.h"
#include "fixed_point_position.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_work.h"

void Object_Destroy(void *object);
void Render_ResetTransformState(void);
s32 GameFlag_Test(s32 flag);
s32 Transform_ApplyLookAt(void *, void *);
s32 Transform_SetLookAt(void *, void *);
s32 Render_ProjectPoint(const void *, s32 *);
void *GetMotionRecord(struct MotionObject *object, s32 record_index);
extern const s32 gData_08128868[];

struct MotionRecordScale {
  u8 unk00[0xc];
  s32 scale;
};

s32 Camera_ApplyTransformByFlag(void) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;

  Render_ResetTransformState();
  if (GameFlag_Test(0x16B) != 0) {
    Iwram_TransformMatrix(gData_08128868);
    return Transform_ApplyLookAt(state, state->pos);
  } else {
    return Transform_SetLookAt(state, state->pos);
  }
}

/// BattleObjGet, as the native expands it here.
static __inline__ struct BattleObj *BattleObject_FindSlot(s32 id) {
  u8 *work = (u8 *)Ram_HeapSlots->battleWork;
  struct BattleObj *slots = ((struct BattleWork *)work)->slots;

  if (id > 7)
    id -= 0x78;
  id += 0x74;
  if (work[id] == 0xff)
    return NULL;
  return &slots[work[id]];
}

/// Projects a battle object's position to the screen, in projected.
s32 BattleMotion_ProjectPosition(s32 id, s32 *projected) {
  struct MotionObject *object = BattleObject_FindSlot(id)->object;
  struct MotionRecordScale *record = GetMotionRecord(object, 0);
  s32 position[3];
  s32 scaled;

  Camera_ApplyTransformByFlag();
  {
    s32 *pos = position;
    /* FAKEMATCH: the native copies projected to its argument register r1
     * between the first store of the position and the second load; plainly the
     * load comes first (one reordered row, differ 2). The pointer is pinned to
     * r1 and the barrier, which uses it and the position, holds the rest of the
     * position after the copy. Measured against it: the barrier alone (differ
     * 2), the pin alone (2), the barrier on the pin alone (4, the position's
     * base moves). */
    register s32 *out __asm__("r1");

    pos[0] = object->x;
    out = projected;
    /* FAKEMATCH: the barrier of the pin above. */
    __asm__ volatile("" : : "l"(out), "l"(pos));
    pos[1] = object->y;
    pos[2] = object->z;
    scaled = Render_ProjectPoint(pos, out);
  }
  (void)Iwram_MulQ16(scaled, record->scale);
  return 0;
}
