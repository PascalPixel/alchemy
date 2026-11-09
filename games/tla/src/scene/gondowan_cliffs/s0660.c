#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_0200120c(void);
void Func_02001214(void);

void Func_02000660(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  struct {
    s32 x, y, z;
  } position;
  s32 *out = (s32 *)&position;

  out[0] = object->x;
  out[1] = object->y;
  out[2] = object->z;
  FieldVector_AddPolarOffset(0x100000, object->angle, out);
  if ((position.x >> 20) == 119 && (position.z >> 20) == 45) {
    FIELD_AT(Ram_HeapSlots->eventWork, u16, 346) = 1;
  } else {
    Func_0200120c();
    Event_ResolvePendingActions(0);
    Actor_TurnToFaceEachOther(16, gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId((s32)MsgRrruffRrruff2);
    Event_ShowMessageAndWait(16, 0);
    Actor_FaceDirection(16, FACING_WEST, 0);
    Func_02001214();
  }
}
