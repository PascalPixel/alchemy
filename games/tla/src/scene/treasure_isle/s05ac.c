#include "types.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "field_event.h"
#include "event_inline.h"

// Import stubs: Event_Start and Event_End, and the pending
// actions' resolution.
void Func_02000fcc(void);
void Func_02000fd4(void);
void Func_0200043c(void *cells, s32 index);
extern u32 gData_0200117c[];
extern u32 gData_020011cc[];

/// For each of cells 0 to 3 of gData_0200117c whose flag (0x303 to 0x306) is
/// still clear, calls Func_0200043c and sets the flag; the event is
/// begun before the first and ended after the last.
void Func_020005ac(void) {
  s16 count = 0;

  if (!Event_GetFlag(0x303)) {
    Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_0200117c, 0);
    Event_SetFlag(0x303);
    count = 1;
  }
  if (!Event_GetFlag(0x304)) {
    if (count == 0)
      Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_0200117c, 1);
    Event_SetFlag(0x304);
    count++;
  }
  if (!Event_GetFlag(0x305)) {
    if (count == 0)
      Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_0200117c, 2);
    Event_SetFlag(0x305);
    count++;
  }
  if (!Event_GetFlag(0x306)) {
    if (count == 0)
      Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_0200117c, 3);
    Event_SetFlag(0x306);
    count++;
  }
  if (count != 0)
    Func_02000fd4();
}

/// For each of cells 0 to 2 of gData_020011cc whose flag (0x30a to 0x30c) is
/// still clear, calls Func_0200043c and sets the flag; the event is
/// begun before the first and ended after the last.
void Func_02000684(void) {
  s16 count = 0;

  if (!Event_GetFlag(0x30a)) {
    Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_020011cc, 0);
    Event_SetFlag(0x30a);
    count = 1;
  }
  if (!Event_GetFlag(0x30b)) {
    if (count == 0)
      Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_020011cc, 1);
    Event_SetFlag(0x30b);
    count++;
  }
  if (!Event_GetFlag(0x30c)) {
    if (count == 0)
      Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_020011cc, 2);
    Event_SetFlag(0x30c);
    count++;
  }
  if (count != 0)
    Func_02000fd4();
}

void Func_02000728(void) {
  if (!Event_GetFlag(0x307)) {
    Func_02000fcc();
    FieldEvent_ResolvePendingActions(0);
    Func_0200043c(gData_0200117c, 4);
    Event_SetFlag(0x307);
    Func_02000fd4();
  }
}

/// Called through the scene table with an object id: in each of the first
/// four rooms, sets a flag when object 8 to 11 is at a given x; object 11 at
/// a given x and z in the second room runs Func_02000728 instead.
void Func_02000760(s32 unused, s32 id) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom1) {
    if (id == 8 && object->x >> 20 == 16)
      Event_SetFlag(0x300);
    if (id == 9 && object->x >> 20 == 20)
      Event_SetFlag(0x301);
    if (id == 10 && object->x >> 20 == 27)
      Event_SetFlag(0x302);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom2) {
    if (id == 10 && object->x >> 19 == 33)
      Event_SetFlag(0x308);
    if (id == 11 && object->x >> 19 == 85 && object->z >> 19 == 83)
      Func_02000728();
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom3) {
    if (id == 10 && object->x >> 19 == 83)
      Event_SetFlag(0x309);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom4) {
    if (id == 8 && object->x >> 19 == 87)
      Event_SetFlag(0x30d);
  }
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000864(void) { Field_RequestDjinnBattle(0x17, 0x4d); }

void Func_02000870(void) { Djinn_JoinParty(9, 0, 0x13); }
