#include "types.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "scene_ids.h"

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

s32 GameFlag_Test(s32 flag);
void Func_02001e78(s32 id);
void Object_SetModeById(u32 id, s32 mode);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void Func_02003d24(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003d1c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003ddc(s32 id, s32 x, s32 z);
void Func_02002ffc(void);
void Func_02000b5c(void);
// Returns what its closing Scheduler_AddOrUpdateCallback call leaves in r0.
s32 Func_02003ab0(const u8 *a, const u8 *b, void (*handler)(void));
void Func_02003ecc(s32 value);
void Func_0200107c(s32 a, s32 b);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02001f04(void);
void Func_02001fcc(void);
void Func_02001ea0(struct ObjectRuntime *object);

/// Aqua Rock's entry setup: in each room it shows the statues already filled by
/// their flags (or dims those still empty), opens what the story has opened and
/// arms the room's water and cell handlers by entrance.
s32 Func_02002020(void) {
  s32 scene = gAdventure.scene;
  s32 entrance = gAdventure.entrance;
  struct ObjectRuntime *object;

  FIELD_AT(Ram_HeapSlots->eventWork, s32, 428) = 0x204;
  if (scene == (s32)&SceneId_Ov67cRoom1) {
    if (GameFlag_Test(0x230)) {
      Call1((void (*)())Func_02001e78, 8);
      Call2((void (*)())Object_SetModeById, 8, 3);
    } else {
      Call2((void (*)())Object_SetPartAttribute, Object_GetById(8), 6);
    }
    if (GameFlag_Test(0x231)) {
      Call1((void (*)())Func_02001e78, 9);
      Call2((void (*)())Object_SetModeById, 9, 3);
    } else {
      Call2((void (*)())Object_SetPartAttribute, Object_GetById(9), 6);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom2) {
    if (GameFlag_Test(0x240)) {
      Call6((void (*)())Func_02003d24, 118, 15, 5, 32, 110, 15);
      Call6((void (*)())Func_02003d1c, 59, 16, 5, 32, 46, 16);
      Call2((void (*)())Object_SetPartAttribute, Object_GetById(8), 6);
    } else {
      Call1((void (*)())Func_02001e78, 8);
      Call2((void (*)())Object_SetModeById, 8, 3);
    }
    object = Object_GetById(9);
    object->motionFlags = 0;
    if (GameFlag_Test(0x95c)) {
      Call3((void (*)())Func_02003ddc, 9, 0x2780000, 0x980000);
      Call6((void (*)())Func_02003d1c, 41, 9, 1, 1, 39, 9);
      object->y = 0;
      object->terrainHeight = 0;
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom3) {
    if (entrance == 7)
      Func_02002ffc();
  } else if (scene == (s32)&SceneId_Ov67cRoom4) {
    Call1((void (*)())Func_02001e78, 10);
    Call2((void (*)())Object_SetModeById, 10, 3);
    Call1((void (*)())Func_02001e78, 11);
    Call2((void (*)())Object_SetModeById, 11, 3);
    if ((u32)(entrance - 6) <= 1) {
      if (GameFlag_Test(0x249)) {
        Call2((void (*)())Object_SetPartAttribute, Object_GetById(9), 5);
        Call1((void (*)())Func_02001e78, 9);
        Call2((void (*)())Object_SetModeById, 9, 3);
        Func_02000b5c();
      } else {
        Call2((void (*)())Object_SetPartAttribute, Object_GetById(9), 6);
      }
    }
    FIELD_AT(Object_GetById(13), u8, 98) = 1;
    FIELD_AT(Object_GetById(14), u8, 98) = 1;
    object = Object_GetById(64);
    if (object != 0) {
      object->motionFlags = 0;
      object->terrainHeight = -0x200000;
      object->y = -0x200000;
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom5) {
    if ((u32)(entrance - 5) <= 1) {
      Func_02003ab0(gData_020040f4, gData_02004102, Func_02001fcc);
      Call1((void (*)())Func_02003ecc, 170);
    }
  } else if (scene == (s32)&SceneId_Ov67cRoom6) {
    if (entrance == 5 || entrance == 7) {
      Func_02003ab0(gData_02004106, gData_02004114, Func_02001fcc);
      Call1((void (*)())Func_02003ecc, 170);
      Call2((void (*)())Func_0200107c, 0, 10);
    }
    if (!GameFlag_Test(0x238)) {
      Call1((void (*)())Func_02001e78, 9);
      Call2((void (*)())Object_SetModeById, 9, 3);
    }
    object = Object_GetById(12);
    Call2((void (*)())ObjectDispatch_SetSingleChildFlags, object, 0);
    /* FAKEMATCH(one-pass): the collision byte stored in a once-only block, so
     * the priority byte is addressed after the store; as a plain store, 2 of
     * 308 instructions differ in tla-en. */
    do {
      object->collisionFlags = 0;
    } while (0);
    object->priorityFlags |= 2;
    object->update = (void (*)(void))Func_02001ea0;
    if (!GameFlag_Test(0x109) && entrance == 13)
      Func_02001f04();
  } else if (scene == (s32)&SceneId_Ov67cRoom9) {
    if (GameFlag_Test(0x24b)) {
      Call2((void (*)())Object_SetPartAttribute, Object_GetById(9), 5);
      Call1((void (*)())Func_02001e78, 9);
      Call2((void (*)())Object_SetModeById, 9, 3);
    } else {
      Call2((void (*)())Object_SetPartAttribute, Object_GetById(9), 6);
    }
    FIELD_AT(Object_GetById(10), u8, 98) = 1;
    FIELD_AT(Object_GetById(11), u8, 98) = 1;
    FIELD_AT(Object_GetById(14), u8, 98) = 1;
    FIELD_AT(Object_GetById(16), u8, 98) = 1;
    FIELD_AT(Object_GetById(17), u8, 98) = 1;
    FIELD_AT(Object_GetById(18), u8, 98) = 1;
  }
  return 0;
}
