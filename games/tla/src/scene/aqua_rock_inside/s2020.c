#include "types.h"
#include "scene_tables.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u8 gData_020040f4[];
extern const u8 gData_02004102[];
extern const u8 gData_02004106[];
extern const u8 gData_02004114[];

void Func_02001e78(s32 id);
void Func_02002ffc(void);
void Func_02000b5c(void);
// Returns what its closing Task_Add call leaves in r0.
s32 Func_02003ab0(const u8 *a, const u8 *b, void (*handler)(void));
void Func_02003ecc(s32 value);
void Func_0200107c(s32 a, s32 b);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02001f04(void);
void Func_02001fcc(void);
void Func_02001ea0(struct Actor *object);

/// Aqua Rock's entry setup: in each room it shows the statues already filled by
/// their flags (or dims those still empty), opens what the story has opened and
/// arms the room's water and cell handlers by entrance.
s32 Func_02002020(void) {
  s32 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  FIELD_AT(Ram_HeapSlots->eventWork, s32, 428) = 0x204;
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if (GameFlag_Test(0x230)) {
      Call1((void (*)())Func_02001e78, 8);
      Actor_SetAnimation(8, 3);
    } else {
      Actor_SetSpriteColor(Actor_GetById(8), 6);
    }
    if (GameFlag_Test(0x231)) {
      Call1((void (*)())Func_02001e78, 9);
      Actor_SetAnimation(9, 3);
    } else {
      Actor_SetSpriteColor(Actor_GetById(9), 6);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if (GameFlag_Test(0x240)) {
      MapGrid_CopyCellsLayered(118, 15, 5, 32, 110, 15);
      MapGrid_CopyCellAttributes(59, 16, 5, 32, 46, 16);
      Actor_SetSpriteColor(Actor_GetById(8), 6);
    } else {
      Call1((void (*)())Func_02001e78, 8);
      Actor_SetAnimation(8, 3);
    }
    object = Actor_GetById(9);
    object->motionFlags = 0;
    if (GameFlag_Test(0x95c)) {
      Actor_SetPosition(9, PIXELS(632), PIXELS(152));
      MapGrid_CopyCellAttributes(41, 9, 1, 1, 39, 9);
      object->y = 0;
      object->terrainHeight = 0;
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom3) {
    if (entrance == 7)
      Func_02002ffc();
  } else if (scene == (s32)&SceneId_Ov67cRoom4) {
    Call1((void (*)())Func_02001e78, 10);
    Actor_SetAnimation(10, 3);
    Call1((void (*)())Func_02001e78, 11);
    Actor_SetAnimation(11, 3);
    if ((u32)(entrance - 6) <= 1) {
      if (GameFlag_Test(0x249)) {
        Actor_SetSpriteColor(Actor_GetById(9), 5);
        Call1((void (*)())Func_02001e78, 9);
        Actor_SetAnimation(9, 3);
        Func_02000b5c();
      } else {
        Actor_SetSpriteColor(Actor_GetById(9), 6);
      }
    }
    FIELD_AT(Actor_GetById(13), u8, 98) = 1;
    FIELD_AT(Actor_GetById(14), u8, 98) = 1;
    object = Actor_GetById(64);
    if (object != 0) {
      object->motionFlags = 0;
      object->terrainHeight = -0x200000;
      object->y = -0x200000;
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom5) {
    if ((u32)(entrance - 5) <= 1) {
      Func_02003ab0(gData_020040f4, gData_02004102, Func_02001fcc);
      Func_02003ecc(170);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom6) {
    if (entrance == 5 || entrance == 7) {
      Func_02003ab0(gData_02004106, gData_02004114, Func_02001fcc);
      Func_02003ecc(170);
      Func_0200107c(0, 10);
    }
    if (!GameFlag_Test(0x238)) {
      Func_02001e78(9);
      Actor_SetAnimation(9, 3);
    }
    object = Actor_GetById(12);
    Actor_SetSingleChildFlags(object, 0);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    object->update = (void (*)(void))Func_02001ea0;
    if (!GameFlag_Test(0x109) && entrance == 13)
      Func_02001f04();
  } else if (scene == (s32)&SceneId_Ov67cRoom9) {
    if (GameFlag_Test(0x24b)) {
      Actor_SetSpriteColor(Actor_GetById(9), 5);
      Func_02001e78(9);
      Actor_SetAnimation(9, 3);
    } else {
      Actor_SetSpriteColor(Actor_GetById(9), 6);
    }
    FIELD_AT(Actor_GetById(10), u8, 98) = 1;
    FIELD_AT(Actor_GetById(11), u8, 98) = 1;
    FIELD_AT(Actor_GetById(14), u8, 98) = 1;
    FIELD_AT(Actor_GetById(16), u8, 98) = 1;
    FIELD_AT(Actor_GetById(17), u8, 98) = 1;
    FIELD_AT(Actor_GetById(18), u8, 98) = 1;
  }
  return 0;
}
