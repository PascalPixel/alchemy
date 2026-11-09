#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "scene_ids.h"
#include "event_inline.h"

void Func_020005c4();
void Func_020005cc();

s32 Func_02000078(void) {
  Func_020005c4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors18);
  Event_ShowMessage(8, 0);
  Func_020005cc();
  return 0;
}

s32 Func_020000a0(void) {
  Func_020005c4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors32);
  Event_ShowMessage(9, 0);
  Func_020005cc();
  return 0;
}

extern const u8 gData_02000a20[];
extern const u8 gData_02000bac[];

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom2)
    return (const struct Trigger *)gData_02000bac;
  return (const struct Trigger *)gData_02000a20;
}
