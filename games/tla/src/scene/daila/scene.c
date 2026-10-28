#include "types.h"
#include "field_event.h"
#include "adventure.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "actor.h"

extern s16 gData_02000d80[];

void Func_02000c48();
void Func_02000d18(void);
void Func_02000d50(s16 *data);
void Func_02000d68(s32 a, s32 b);
void Actor_SetWideSprite(s32 actor, s32 flag);

static inline void MapCopyAttr(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: as in the scene layer of The Broken Seal, the inline parameters
   * keep the native registers (direct: the second stack constant reuses r3
   * after the first store) */
  Func_02000c48(a, b, c, d, e, f);
}

void Func_02000d58(s16 *data);

void Func_02000a9c(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;
  s32 flags;

  Func_02000d58(gData_02000d80);
  object = *(struct Actor **)(event + 0xcbc);
  flags = 1;
  flags |= object->priorityFlags;
  flags &= 0xfd;
  object->priorityFlags = flags;
}

s32 Func_02000ac8(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  if (gAdventure.scene == (s32)&SceneId_Daila1)
    Func_02000d68(12, 0);
  else if (gAdventure.scene == (s32)&SceneId_Daila2)
    Func_02000d50(gData_02000d80);
  Import_ActorSetSpritePriority(21, 1);
  if (gAdventure.scene == (s32)&SceneId_Daila2 &&
      Import_GameFlagIsSet(0x300) == 0)
    Actor_SetWideSprite(66, 0);
  return 0;
}

s32 Func_02000b44(void) {
  if (gAdventure.scene == (s32)&SceneId_Daila2) {
    if (Import_GameFlagIsSet(0x210))
      MapCopyAttr(10, 19, 1, 1, 11, 19);
    if (Import_GameFlagIsSet(0x211))
      MapCopyAttr(25, 20, 1, 1, 26, 20);
    if (Import_GameFlagIsSet(0x212))
      MapCopyAttr(28, 23, 1, 1, 29, 23);
    if (Import_GameFlagIsSet(0x213))
      MapCopyAttr(17, 11, 1, 1, 18, 11);
    if (Import_GameFlagIsSet(0x214))
      MapCopyAttr(30, 17, 1, 1, 31, 17);
    if (Import_GameFlagIsSet(0x215))
      MapCopyAttr(16, 23, 1, 1, 17, 23);
  }
  return 0;
}

void Func_02000c30(void) { Func_02000d18(); }
