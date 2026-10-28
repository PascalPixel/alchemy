// Under -mthumb-slow-loads (2026-10-26): differ 10 of 145; left: argument
// constants of the first palette fade and the actor 9 placement are built in
// another order (Call3 and Call4 forms measured, 6 at best with the two
// placements through them).
// Draft, differ 12 of 145 (the English listing; the Japanese one has no case
// for entrance 5): the Islets' setup: sets BG layer priority, clears flag
// 303 on entrance 2, queues two palette fades, places actors 9 and 10 when
// flag 0xaa5 is set and 265 clear, frees actors 9, 11 and 12, schedules
// Func_02001c4c, sets flag 0x217 and places 9 and 10 again for entrances 4
// and 5, then runs entrance 3's (Func_02002830) or 4's (Func_020029c4)
// arrival, or a screen transition and Func_020034d8. Left: four rows where the native schedules an argument constant otherwise (the first palette fade builds 3 last, the gAdventure load comes before the 241 offset, two position pairs split their shifts).
#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"

struct MapLayerBits {
  u8 unk00[23];
  u8 unk17Low : 2;
  u8 priority : 2;
  u8 unk17High : 4;
};

void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003fa4(s32 skip);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void Func_02003410(void);
void PaletteQueue_Add(s32 a, s32 b, s32 c, s32 d);
void Func_02003edc(s32 id, s32 x, s32 z);
void Func_02003ee4(s32 id, s32 x, s32 z, s32 angle);
struct ObjectRuntime *Object_GetById(u32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Func_02001c4c(void);
void WaitFrames(s32 frames);
void Func_02002830(void);
void Func_020029c4(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020034d8(void);

void Func_0200329c(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 done;

  Func_02003e64();
  Func_02003fa4(0);
  ((struct MapLayerBits *)map)->priority = 1;
  if (gAdventure.entrance == 2)
    GameFlag_ClearBit(303);
  Func_02003410();
  PaletteQueue_Add(3, 12, 5, 4);
  PaletteQueue_Add(4, 12, 5, 4);
  if (GameFlag_Test(0xaa5) && (done = GameFlag_Test(265)) == 0) {
    Func_02003edc(10, 0x1180000, 0x1d40000);
    Object_GetById(10)->motionFlags = done;
    Func_02003edc(9, 0x1280000, 0x1c40000);
  }
  ObjectDispatch_SetSingleChildFlags(Object_GetById(9), 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(11), 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(12), 0);
  Scheduler_AddOrUpdateCallback(Func_02001c4c, 0x480);
  if ((u16)(gAdventure.entrance - 4) <= 1) {
    GameFlag_SetBit(0x217);
    if (!GameFlag_Test(265)) {
      Func_02003ee4(9, 0x980000, 0x3000000, 0x4000);
      Func_02003edc(10, 0xb00000, 0x3090000);
      WaitFrames(1);
    }
  }
  if (!GameFlag_Test(265)) {
    s32 entrance = gAdventure.entrance;

    if (entrance == 3) {
      Func_02002830();
    } else if (entrance == 4) {
      Func_020029c4();
    } else if (entrance != 5) {
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Func_020034d8();
    }
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_020034d8();
  }
  Func_02003e6c();
}
