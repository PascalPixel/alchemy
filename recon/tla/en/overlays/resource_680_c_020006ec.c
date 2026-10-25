// Draft, differ 173 of 360: Gaia Rock's entry setup: records the scene, starts
// the overlay's frame callback in the first room, places its people and opens
// the cells of what the story flags have moved; in the first room after the
// flags for Kushinada's rescue, the man by the entrance speaks once.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

typedef void (*SchedulerCallback)(void);

struct ObjectRuntime *Object_GetById(s32 id);
s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 order);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Object_SetModeById(u32 id, s32 mode);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void ObjectMotion_SetVariantCallback(u32 objectId, s32 variant);
void FieldEvent_WaitFrames(s32 frames);
s32 Func_02000f0c(void);
void Func_02000f14(s32 id, s32 flags);
void Func_02000e44(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000e8c(s32 id, s32 x, s32 z);
void Func_02000ed4(s32 id, s32 flags);
void Func_02000ea4(const u8 *message);
void Func_02000eac(s32 id, s32 flags);
void Func_02000e7c(void);
void Func_02000dd0(void);
void Func_02000b2c(s32 id, s32 x, s32 y, s32 z);
void Func_02000670(void);

s32 Func_020006ec(void) {
  struct ObjectRuntime *object;

  FIELD_AT(Ram_HeapSlots->eventWork, s32, 428) = 0x259;
  FIELD_AT(&gPartyState, u16, 608) = 0xae;
  FIELD_AT(&gPartyState, u16, 792) = 5;
  Object_GetById(Func_02000f0c())->priorityFlags |= 32;
  if (gPartyState.scene == 0xae) {
    Scheduler_AddOrUpdateCallback(Func_02000670, 0x480);
    Func_02000f14(8, 1);
    Func_02000f14(9, 1);
    Func_02000f14(10, 1);
    Func_02000f14(11, 1);
    Func_02000f14(12, 1);
    Func_02000f14(13, 1);
    Func_02000f14(14, 1);
    Func_02000f14(15, 1);
    if (GameFlag_Test(0x920)) {
      object = Object_GetById(16);
      object->motionFlags = 0;
      object->x = 0x580000;
      object->y = 0x800000;
      ObjectDispatch_SetSingleChildFlags(object, 0);
      Func_02000e44(6, 27, 1, 1, 5, 27);
    }
    object = Object_GetById(19);
    object->priorityFlags = 0;
    if (GameFlag_Test(0x921)) {
      object->motionFlags = 0;
      object->x = 0x2880000;
      object->y = 0x400000;
      ObjectDispatch_SetSingleChildFlags(object, 0);
      Func_02000e44(33, 102, 1, 2, 37, 102);
      Func_02000e44(27, 44, 1, 1, 40, 46);
    } else if (GameFlag_Test(0x109)) {
      if (object->x >> 19 != 75)
        Func_02000e44(33, 102, 1, 2, 37, 102);
      if (object->x >> 19 == 79) {
        object->motionFlags = 0;
        Func_02000e44(38, 46, 1, 1, 39, 46);
      }
    }
    if (GameFlag_Test(0x927) && !GameFlag_Test(0x9ab)) {
      Func_02000e8c(20, 0x31e0000, 0x3200000);
      Func_02000e44(53, 52, 1, 1, 49, 49);
      Func_02000e44(53, 52, 1, 1, 50, 49);
      Object_SetModeById(20, 5);
      if (!GameFlag_Test(0xa61)) {
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        ObjectMotion_SetVariantCallback(20, 3);
        Func_02000ed4(20, 1);
        FieldEvent_WaitFrames(30);
        Func_02000ea4(MsgIMNot);
        Func_02000eac(20, 0);
        ObjectMotion_SetVariantCallback(20, 2);
        FieldEvent_WaitFrames(30);
        GameFlag_SetBit(0xa61);
        Func_02000e7c();
      }
    }
  } else if (gPartyState.scene == 0xaf) {
    Func_02000f14(8, 1);
    Func_02000f14(9, 1);
    Func_02000f14(10, 1);
    Func_02000f14(11, 1);
    Func_02000f14(12, 1);
    Func_02000f14(13, 1);
    Func_02000f14(14, 1);
    if ((u16)(gPartyState.entrance - 3) <= 1)
      Func_02000dd0();
    if (GameFlag_Test(0x300))
      Func_02000b2c(17, 0x2fc0000, -0x1870000, 0x1a80000);
    if (GameFlag_Test(0x301))
      Func_02000b2c(18, 0x3150000, -0xf80000, 0x1a80000);
    if (GameFlag_Test(0x302))
      Func_02000b2c(19, 0x30a0000, -0x770000, 0x1a80000);
  } else if (gPartyState.scene == 0xb0) {
    if (Object_GetById(64) != 0)
      Func_02000e8c(64, 0, 0);
    FIELD_AT(Ram_HeapSlots->eventWork, s32, 276) =
        FIELD_AT(Ram_HeapSlots->eventWork, s32, 84);
    object = Object_GetById(8);
    object->motionFlags = 0;
    Func_02000e44(11, 14, 1, 1, 13, object->z >> 20);
  }
  return 0;
}
