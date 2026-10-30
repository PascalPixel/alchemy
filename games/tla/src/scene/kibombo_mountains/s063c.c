#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "call.h"
#include "edition.h"

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020004f4(s32 id, s32 x, s32 z);
void Func_02000af0(void);
void Func_0200106c(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
void Func_020010d4(s32 actor, s32 x, s32 z);
void Func_02001124(s32 actor, s32 value);
void Func_02001174(void);
void Func_0200117c(s32 a, s32 b, s32 c, s32 d);
s32 Func_02001184(void);
void Func_0200118c(s32 actor, s32 value);

// The JA edition has its own form of this function; it stays on its listing in
// recon/tla/raw/overlays (S4).
#if (LANGUAGE != LANGUAGE_JAPANESE)
s32 Func_0200063c(void) {
  struct Actor *object;
  s16 scene;
  s32 flag;
  s32 height;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x204;
  Actor_GetById(Func_02001184())->priorityFlags |= 0x20;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov66fRoom1) {
    Actor_SetSpritePriority(18, 1);
    Actor_SetSpritePriority(19, 1);
    Actor_SetSingleChildFlags(Actor_GetById(19), 0);
    Actor_GetById(19)->collisionFlags = 0;
    Actor_GetById(19)->priorityFlags |= 2;
    Actor_GetById(12)->collisionFlags = 0;
    if (GameFlag_Test(0x222)) {
      Call6((void (*)())Func_0200106c, 30, 35, 1, 1, 28, 34);
      Call6((void (*)())Func_0200106c, 30, 34, 1, 1, 28, 33);
    }
    if (GameFlag_Test(0x221))
      Call6((void (*)())Func_0200106c, 24, 32, 1, 1, 22, 32);
    if (GameFlag_Test(0x223))
      Call6((void (*)())Func_0200106c, 49, 28, 1, 1, 47, 28);
    else
      Actor_SetSpritePriority(14, 1);
    if (GameFlag_Test(0x224))
      Call6((void (*)())Func_0200106c, 53, 35, 1, 1, 53, 36);
    flag = GameFlag_Test(0x202);
    if (flag == 0) {
      struct Actor *lift;

      Actor_GetById(16)->priorityFlags |= 0x80;
      lift = Actor_GetById(17);
      lift->priorityFlags |= 0x40;
      lift->y = 0xe00000;
      lift->terrainHeight = 0xe00000;
      lift->collisionFlags = flag;
    } else {
      Call2((void (*)())Func_02001124, 10, 257);
    }
    Func_0200118c(17, 231);
    if (GameFlag_Test(0x882))
      Func_020010d4(13, 0, 0);
    else
      Actor_GetById(13)->scaleY = 0x14000;
  } else if (scene == (s32)&SceneId_Ov66fRoom2) {
    Actor_SetSpritePriority(8, 1);
    Actor_SetSpritePriority(9, 1);
    Func_02001174();
    Func_0200117c(0, 0x9e8, 8, 9);
    if (GameFlag_Test(0x210))
      Call6((void (*)())Func_0200106c, 6, 16, 1, 1, 4, 16);
    if (GameFlag_Test(0x211))
      Call6((void (*)())Func_0200106c, 8, 30, 1, 1, 10, 30);
    if (GameFlag_Test(0x8e7)) {
      object = Actor_GetById(20);
      Call3((void (*)())Func_020010d4, 20, 0x480000, 0x1c80000);
      object->motionFlags = 0;
      height = Map_GetTerrainHeight(0, object->x, object->z);
      object->terrainHeight = height;
      object->y = height;
      Call6((void (*)())Func_0200106c, 3, 28, 1, 1, 4, 28);
    }
    if (GameFlag_Test(0x8e8)) {
      object = Actor_GetById(13);
      Call3((void (*)())Func_020010d4, 13, 0x1c80000, 0x1e80000);
      object->motionFlags = 0;
      height = Map_GetTerrainHeight(0, object->x, object->z);
      object->terrainHeight = height;
      object->y = height;
      Call6((void (*)())Func_0200106c, 32, 30, 1, 2, 28, 30);
    }
    Call6((void (*)())Func_0200106c, 8, 15, 16, 4, 112, 15);
    Call6((void (*)())Func_0200106c, 20, 33, 8, 2, 120, 33);
    Func_020004f4(14, 8, 17);
    Func_020004f4(15, 8, 17);
    Func_020004f4(16, 22, 33);
    Func_020004f4(17, 22, 33);
    if (!GameFlag_Test(0x109) && gMainRoutineWork.entrance == 8)
      Func_02000af0();
  }
  return 0;
}
#endif
