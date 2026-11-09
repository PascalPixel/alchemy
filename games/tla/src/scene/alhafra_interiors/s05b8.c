#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "call.h"
#include "version.h"
#include "event_inline.h"

// The Japanese edition sets no flag 0x200 by the leader's position and places
// no actors 31 and 32.
#if TLA_PATCH_1
extern u8 gData_02001c90[], gData_02001c60[];
void Func_02000bac(s32 mode);
void Func_02000bb4();
void Func_02000578(void);
void Func_02000598(void);

s32 Func_020005b8(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_Get(16)->collisionFlags |= 4;
  Actor_Get(16)->collisionFlags |= 16;
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(31, 3);
  FieldActor_SetSpritePriority(14, 3);
  FieldActor_SetSpritePriority(15, 3);
  Func_02000bac(0);
  Call4(Func_02000bb4, (s32)gData_02001c90, 8, 15, 0x40000000);
  Call4(Func_02000bb4, (s32)gData_02001c60, 8, 15, 0x40000000);
  *(s32 *)(gData_02001c60 + 24) = 0x18000;
  Actor_Get(14)->update = Func_02000578;
  Actor_Get(15)->update = Func_02000598;
  if (Event_GetFlag(0x8a7)) {
    if (Actor_Get(gMainRoutineWork.currentOwner)->x >= 0x2000000)
      Event_SetFlag(0x200);
    else
      Event_ClearFlag(0x200);
    if (Event_GetFlag(0x200) && Event_GetFlag(0x823) && !Event_GetFlag(0x903)) {
      Actor_SetPosition(31, PIXELS(376), PIXELS(316));
      Actor_SetPosition(32, PIXELS(392), PIXELS(344));
    } else {
      Actor_SetPosition(31, 0, 0);
      Actor_SetPosition(32, 0, 0);
    }
  } else if (Event_GetFlag(0x8ab)) {
    if (Event_GetFlag(0x971))
      Actor_SetPosition(31, 0, 0);
  } else {
    FieldActor_SetSpritePriority(24, 2);
    FieldActor_SetSpritePriority(19, 3);
    FieldActor_SetSpritePriority(28, 2);
    FieldActor_SetSpritePriority(25, 3);
  }
  return 0;
}
#else
extern u8 gData_02001c90[], gData_02001c60[];
void Func_02000bac(s32 mode);
void Func_02000bb4();
void Func_02000578(void);
void Func_02000598(void);

s32 Func_020005b8(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 521;
  Actor_Get(16)->collisionFlags |= 4;
  Actor_Get(16)->collisionFlags |= 16;
  FieldActor_SetSpritePriority(9, 3);
  FieldActor_SetSpritePriority(31, 3);
  FieldActor_SetSpritePriority(14, 3);
  FieldActor_SetSpritePriority(15, 3);
  Func_02000bac(0);
  Call4(Func_02000bb4, (s32)gData_02001c90, 8, 15, 0x40000000);
  Call4(Func_02000bb4, (s32)gData_02001c60, 8, 15, 0x40000000);
  *(s32 *)(gData_02001c60 + 24) = 0x18000;
  Actor_Get(14)->update = Func_02000578;
  Actor_Get(15)->update = Func_02000598;
  if (Event_GetFlag(0x8a7)) {
    if (!Event_GetFlag(0x823) || Event_GetFlag(0x903)) {
      Actor_SetPosition(31, 0, 0);
      Actor_SetPosition(32, 0, 0);
    }
  } else if (Event_GetFlag(0x8ab)) {
    if (Event_GetFlag(0x971))
      Actor_SetPosition(31, 0, 0);
  } else {
    FieldActor_SetSpritePriority(24, 2);
    FieldActor_SetSpritePriority(19, 3);
    FieldActor_SetSpritePriority(28, 2);
    FieldActor_SetSpritePriority(25, 3);
  }
  return 0;
}
#endif
