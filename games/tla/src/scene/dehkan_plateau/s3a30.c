/// @file
/// Scene code of overlay 0x655, in ROM order, from s3a30's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "scene_tables.h"

extern struct Actor *gData_02004420;

void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);

void Func_02003a30(void) {
  struct Actor **slot = &gData_02004420;
  /* FAKEMATCH(temporary): one zero held in a local, kept in r7 for the three
   * byte stores below as the native does; as plain zeros, built in r2 where
   * they are stored, 22 of 48 instructions differ in tla-en. */
  s32 zero = 0;
  struct Actor *object = ((struct Actor *)Import_ActorCreate(489, 0, 0, 0));

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->unk5c[0] = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(199);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

extern const u8 gData_02003f92[];
extern const u8 gData_02003fb8[];

void Func_02000038(const void *data);
void Func_020001d0(const void *data);
void Func_02001ff4(void);
void Func_02002140(void);
void Func_02003764(void);

void Func_02003aa0(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (GameFlag_Test(95) != 0)
    GameFlag_SetBit(521);
  Func_02000038(gData_02003fb8);
  Func_020001d0(gData_02003f92);
  if (GameFlag_Test(519) == 0) {
    Func_02002140();
    wait(10);
  }
  Func_02001ff4();
  if (GameFlag_Test(265) == 0) {
    if (gMainRoutineWork.entrance > 5) {
      if (gMainRoutineWork.entrance <= 39)
        Func_02003764();
    }
  }
}

s32 Scene_PrepareMap(void) { return 0; }
