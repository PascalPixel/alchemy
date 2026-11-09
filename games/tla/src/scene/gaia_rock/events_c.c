#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "call.h"
#include "event_work.h"
#include "scene_tables.h"
#include "event_inline.h"

struct SceneEntrance;

void Func_02000e74(void);
void Func_02000e7c(void);
void Func_02000edc(s32 a);
void Func_02000f04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);

extern const u8 MsgCheckedTheAltar[];
extern const u8 MsgIMSensing[];
extern const u8 MsgICanT[];
extern const u8 MsgYouReGoing[];
extern const u8 MsgIfYouRe[];
extern const u8 MsgKushinadaSSafe[];
extern const u8 MsgIReallyGot[];

extern const u8 gData_020013c8[];
extern const u8 gData_020014b8[];
extern const u8 gData_02001554[];

void Func_02000258(void) {
  MapGrid_CopyCellAttributes(13, 18, 1, 1, 13, Actor_Get(8)->z >> 20);
}

void Func_0200027c(void) {
  MapGrid_CopyCellAttributes(11, 14, 1, 1, 13, Actor_Get(8)->z >> 20);
}

void Func_020002a0(void) {
  Call7((void (*)())Func_02000f04, 110, 64, 14, 12, 110, 47, 1);
}

void Func_020002c0(void) {
  struct Actor *object = Actor_Get(64);
  s32 done;

  if (object == NULL)
    return;
  done = Event_GetFlag(0xf74);
  if (done == 0) {
    Actor_SetPosition(64, PIXELS(657), PIXELS(246));
    object->motionFlags = done;
    object->y = PIXELS(80);
  }
}

void Func_020002fc(void) {
  if (Actor_Get(64) != NULL)
    Actor_SetPosition(64, 0, 0);
}

void Func_02000314(void) {
  struct Actor *object = Actor_Get(64);

  Message_ShowCentered(MsgCheckedTheAltar, 1);
  if (object != NULL && object->x != 0) {
    Func_02000edc(0x1040);
  } else if (Event_GetFlag(0xf74) == 0) {
    struct EventWork *work = Ram_HeapSlots->eventWork;


    Message_ShowCentered(MsgIMSensing, 1);
    work->unk15a = 1;
  } else {
    Message_ShowCentered(MsgICanT, 1);
  }
}

void Func_02000378(void) {
  if (Event_GetFlag(0x927)) {
    Func_02000e74();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId(MsgYouReGoing);
    FieldActor_StartRepeatedMotion(20, 1);
    FieldEvent_Wait(15);
    Event_AskYesNo(20, 0);
    Event_SetFlag(0x9ab);
    Func_02000e7c();
  }
}

void Func_020003c4(void) {
  if (Event_GetFlag(0x9ab)) {
    FieldEvent_SetMessageId(MsgYouReGoing);
    Event_AskYesNo(20, 0);
  } else {
    FieldEvent_SetMessageId(MsgIfYouRe);
    Event_ShowMessage(20, 0);
  }
}

void Func_020003fc(void) {
  if (Event_GetFlag(0x9ab)) {
    FieldEvent_SetMessageId(MsgKushinadaSSafe);
    Event_ShowMessage(20, 0);
  } else {
    FieldEvent_SetMessageId(MsgIReallyGot);
    Event_ShowMessage(20, 0);
  }
}

const struct SceneEntrance *Func_02000434(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room1)
    return (const struct SceneEntrance *)gData_020013c8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov680Room2)
    return (const struct SceneEntrance *)gData_020014b8;
  return (const struct SceneEntrance *)gData_02001554;
}
