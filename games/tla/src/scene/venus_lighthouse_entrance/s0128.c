#include "types.h"
#include "ram_buffer.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"
s32 Func_020028b4();
s32 Func_0200030c();
s32 Func_020028bc();
s32 Func_020028a4();
s32 Func_02001c30();
s32 Party_RemoveActiveOwner();
s32 Func_0200299c();
s32 Func_02002a04();
s32 Actor_SetActionCallback();
extern u8 gMainRoutineWork[];
extern char SceneId_VenusLighthouseEntrance;

s32 Func_02000128(void) {
  s32 t37;
  s32 t0;
  s32 t7;
  s32 t10;
  s32 t21;
  s32 t33;

  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x100;
  t0 = ((s32)Actor_Get(FIELD_AT(gMainRoutineWork, s32, 532)));
  FIELD_AT((t0 + 0x23), u8, 0) = (0x20 | FIELD_AT((t0 + 0x23), u8, 0));
  Actor_SetAnimation(10, 5);
  FieldActor_SetSpritePriority(10, 1);
  Actor_SetAnimation(11, 5);
  FieldActor_SetSpritePriority(11, 1);
  FieldActor_SetSpritePriority(14, 2);
  FieldActor_SetSpritePriority(19, 2);
  t7 = Event_GetFlag(0x892);
  if (t7 != 0) {
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
  }
  t10 = Event_GetFlag(0x890);
  if (t10 != 0) {
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(15, 0, 0);
    Actor_SetPosition(16, 0, 0);
    Actor_SetPosition(17, 0, 0);
    Actor_SetPosition(18, 0, 0);
    Actor_SetPosition(19, 0, 0);
    Actor_SetPosition(20, 0, 0);
    Actor_SetPosition(21, 0, 0);
  }
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 9) {
    t21 = Event_GetFlag(0x891);
    if (t21 == 0) {
      Func_020028b4();
      FieldEvent_ResolvePendingActions(0);
      Func_0200030c();
      Func_020028bc();
    }
  }
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 0x62) {
    FIELD_AT(gMainRoutineWork, u16, 490) = 1;
    *(u16 *)(gMainRoutineWork + 482) = 1;
    Func_020028a4(1);
    Func_02001c30();
  }
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 0x63) {
    Event_SetFlag(0x893);
  }
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 0x5a) {
    FIELD_AT(gMainRoutineWork, s32, 532) = 5;
    Party_RemoveActiveOwner(4);
    Party_RemoveActiveOwner(6);
    Party_RemoveActiveOwner(7);
    Func_0200299c((s32)&SceneId_VenusLighthouseEntrance, 1);
  }
  t33 = Event_GetFlag(0x893);
  if (t33 == 0) {
    Actor_SetAnimation(22, 5);
    FieldActor_SetSpritePriority(22, 1);
  } else {
    Actor_SetPosition(22, 40 << 16, 248 << 16);
    t37 = ((s32)Actor_Get(22));
    FIELD_AT(t37, u16, 6) = 0x4000;
    Func_02002a04(0x16, 0x4000, 0);
    FieldActor_SetSpritePriority(22, 3);
    Actor_SetActionCallback(((s32)Actor_Get(22)), 8);
  }
  return 0;
}
