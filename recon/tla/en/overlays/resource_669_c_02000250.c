// 26 October 2026, under -mthumb-slow-loads: differ 2 of 170: the 0x100000 step
// after the priority byte store, where the native issues a load one slot later
// after a store, where alchemy-gcc issues it at once; the step is read plainly
// again, since reading it as long no longer changes anything; alchemy permute (5 to 6 minutes) and an empty asm or
// one-pass block after any one statement do not move it.
// Draft, differ 2 of 170: the scene setup of overlay 669, written from the
// listing (its ☀️ twin by shape, CottageRoomsA_Initialize in
// scene/cottage_rooms_a.c, is a different scene). Left: one scheduling
// swap at the start: the native loads gAdventure's address before it stores
// the screen effect (ldr r5, =gAdventure; adds r2, #93; str r2, [r3]); the
// draft stores first. In sched2 the 0x209 add ranks 7 (its store anti-depends
// into the next constant) and the pool load 6, one below. Measured: one-pass
// blocks around the y step or the priority bit (4), a party pointer (4), an
// "+l" barrier on the step (4), alchemy permute 3 minutes (4). Round two: the y
// step read and written as long (another alias set than the priority byte store
// before it) fixes the end, where the native builds the 0x100000 step before it
// loads y (4 to 2).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "call.h"

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02003198();
void Func_02003298(s32 actor, s32 value);
void Func_02000c5c(void);

static __inline__ void EasternAlhafraShip_ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the engine through one inline layer, as
   * cutscene.c's Call3. */
  Call3((void (*)())Func_02003198, actor, x, z);
}

s32 Func_02000250(void) {
  struct ObjectRuntime *object;

  ((struct EventWork1ac *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  if (gAdventure.entrance == 99) {
    GameFlag_SetBit(0x8aa);
    GameFlag_SetBit(0x8ab);
    gAdventure.secondary = 3;
    gAdventure.entrance = 3;
    Func_02000c5c();
  }
  if (GameFlag_Test(0x8a7)) {
    EasternAlhafraShip_ObjPlace(10, 0, 0);
    EasternAlhafraShip_ObjPlace(11, 0, 0);
    EasternAlhafraShip_ObjPlace(8, 0, 0);
    EasternAlhafraShip_ObjPlace(9, 0, 0);
  } else if (GameFlag_Test(0x8ab)) {
    EasternAlhafraShip_ObjPlace(8, 0, 0);
    EasternAlhafraShip_ObjPlace(9, 0, 0);
    if (!GameFlag_Test(0x8bd)) {
      EasternAlhafraShip_ObjPlace(10, 0, 0);
      EasternAlhafraShip_ObjPlace(11, 0, 0);
    }
  } else {
    EasternAlhafraShip_ObjPlace(10, 0, 0);
    EasternAlhafraShip_ObjPlace(11, 0, 0);
  }
  if (GameFlag_Test(0x900))
    EasternAlhafraShip_ObjPlace(13, 0x1880000, 0x2880000);
  Object_GetById(13)->priorityFlags |= 32;
  Func_02003298(15, 457);
  if (!GameFlag_Test(0x901)) {
    if (!GameFlag_Test(0x109)) {
      Object_GetById(14)->priorityFlags |= 128;
      object = Object_GetById(15);
      object->priorityFlags |= 64;
      object->y += 0x100000;
      object->terrainHeight += 0x100000;
    }
  } else {
    EasternAlhafraShip_ObjPlace(14, 0x880000, 0x1db0000);
    if (!GameFlag_Test(0x902))
      EasternAlhafraShip_ObjPlace(15, 0x780000, 0x1d80000);
    else
      EasternAlhafraShip_ObjPlace(15, 0, 0);
  }
  return 0;
}
