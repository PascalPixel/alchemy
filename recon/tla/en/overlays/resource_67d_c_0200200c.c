// Draft, differ 20 of 163: Izumo interiors' entry setup: places the party and
// the people by the story flags for entrances 5 and 7.
#include "types.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_0200231c(s32 entrance);
void Func_02002214(s32 actor, s32 x, s32 z);
void Object_SetModeById(u32 id, s32 mode);
void Func_02002274(s32 actor, s32 animation);
struct ObjectRuntime *Object_GetById(s32 id);

s32 Func_0200200c(void) {
  FIELD_AT(Ram_HeapSlots->eventWork, s32, 428) = 0x201;
  if (gAdventure.entrance != 7)
    Call1((void (*)())GameFlag_ClearBit, 0x350);
  if (GameFlag_Test(0x927) && !GameFlag_Test(0x928) &&
      gAdventure.entrance == 7) {
    Call1((void (*)())Func_0200231c, 7);
    Call3((void (*)())Func_02002214, 22, 0x900000, 0x2380000);
    Call3((void (*)())Func_02002214, 21, 0, 0);
    Call3((void (*)())Func_02002214, 16, 0, 0);
    Call3((void (*)())Func_02002214, 17, 0, 0);
    Call3((void (*)())Func_02002214, 18, 0, 0);
    Call3((void (*)())Func_02002214, 19, 0, 0);
  }
  if (GameFlag_Test(0x927) && GameFlag_Test(0x928) &&
      gAdventure.entrance == 7 && GameFlag_Test(0x350)) {
    Call1((void (*)())Func_0200231c, 7);
    Call3((void (*)())Func_02002214, 16, 0, 0);
    Call3((void (*)())Func_02002214, 17, 0, 0);
    Call3((void (*)())Func_02002214, 18, 0, 0);
    Call3((void (*)())Func_02002214, 19, 0, 0);
  }
  if (gAdventure.entrance == 5 && !GameFlag_Test(0x927)) {
    Call2((void (*)())Object_SetModeById, 17, 5);
    Call2((void (*)())Func_02002274, 17, 0x101);
  }
  if (gAdventure.entrance == 7 && !GameFlag_Test(0x927)) {
    struct ObjectRuntime *object;

    Call2((void (*)())Object_SetModeById, 9, 5);
    Call2((void (*)())Object_SetModeById, 10, 5);
    Call2((void (*)())Object_SetModeById, 11, 5);
    Call2((void (*)())Object_SetModeById, 12, 5);
    object = Object_GetById(11);
    object->scaleX = -Object_GetById(11)->scaleX;
  }
  return 0;
}
