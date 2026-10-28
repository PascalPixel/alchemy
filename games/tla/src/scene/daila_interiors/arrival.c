#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "scene_call.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"
#include "daila.h"

void Func_02000d9c(void);
void Func_02000ca4(s32 mode);

struct DailaStartActor {
  u8 unk00[0x18];
  s32 speed18;
  s32 speed1c;
  u8 unk20[3];
  u8 priorityFlags;
  u8 unk24[0x35];
  u8 collisionFlags;
};

struct DailaMapWork {
  u8 unk000[0x17];
  u8 low17 : 2;
  u8 mode17 : 2;
  u8 high17 : 4;
};

extern const u8 gData_02000e04[];

struct DailaStartActor *Actor_GetById(s32 id);
void Actor_SetModeById(s32 actor, s32 mode);
void Actor_SetSingleChildFlags(struct DailaStartActor *object,
                                        s32 value);
void Func_02000c58(void);
void Func_02000cbc(struct DailaStartActor *object, s32 item);
void Func_02000dac(s32 mode);
void Func_02000dc4(const void *script);

s32 DailaInteriors_Start(void) {
  struct DailaMapWork *map;
  s32 *effect = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  s32 value = 0x209;

  /* FAKEMATCH(barrier): an empty asm after the address and the value, so hegSequenceParentWork's address loads just before the store; as one assignment, 2
   * of 174 instructions differ in tla-en. */
  __asm__("");
  *effect = value;
  if (gSequenceParentWork.scene == (s32)&SceneId_DailaInteriors1) {
    if (gSequenceParentWork.entrance == 7) {
      Func_02000c58();
    } else {
      if (GameFlag_Test(0x873)) {
        Actor_SetModeById(24, 5);
        Actor_SetSingleChildFlags(Actor_GetById(24), 0);
        Actor_SetModeById(25, 5);
        Actor_SetSingleChildFlags(Actor_GetById(25), 0);
      }
      Actor_GetById(gSequenceParentWork.currentOwner)->priorityFlags |= 32;
      Actor_GetById(13)->priorityFlags |= 32;
      Actor_GetById(19)->priorityFlags |= 32;
      Import_ActorSetSpritePriority(19, 2);
      Actor_GetById(19)->collisionFlags |= 4;
    }
  } else if (gSequenceParentWork.scene == (s32)&SceneId_DailaInteriors2) {
    map = (struct DailaMapWork *)Ram_HeapSlots->mapWork;
    Import_EventStart();
    Import_EventResolvePendingActions(0);
    Actor_SetSingleChildFlags(Actor_GetById(13), 0);
    Func_02000dac(0);
    if (!GameFlag_Test(0xa20)) {
      Func_02000cbc(Actor_GetById(9), 458);
      Actor_SetSingleChildFlags(Actor_GetById(9), 0);
      Import_ActorSetSpritePriority(9, 1);
      Actor_GetById(9)->speed1c = 0x13333;
      Actor_GetById(9)->speed18 = 0x13333;
      Actor_GetById(9)->collisionFlags |= 4;
      Actor_GetById(13)->collisionFlags |= 4;
    } else {
      Actor_GetById(9)->collisionFlags |= 4;
      Actor_SetSingleChildFlags(Actor_GetById(9), 0);
    }
    if (!GameFlag_Test(0x873)) {
      Func_02000d4c(10, 0, 0);
      Func_02000d4c(11, 0, 0);
      Func_02000d4c(12, 0, 0);
      map->mode17 = 1;
    } else {
      Func_02000dc4(gData_02000e04);
    }
    Import_EventEnd();
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_DailaInteriors2 &&
      GameFlag_Test(0x873)) {
    Func_02000ca4(0);
    Daila_CopyCells(29, 33, 10, 6, 10, 12);
  }
  return 0;
}

void Func_02000c58(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x209;
  GameFlag_ClearBit(0x12f);
}

void Func_02000c74(void) { Func_02000d9c(); }
