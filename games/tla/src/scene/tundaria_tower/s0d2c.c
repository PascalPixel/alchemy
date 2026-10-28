#include "tundaria_tower.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020031a8(s32 cue);
void Func_02003178(void);

/// Off layer 2's terrain height, the event's object runs cue 188 on terrain 2
/// with motion flags 3. In room 6, column 27 sets flag 0x947, and column 25
/// with that flag set sets 0x946.
void Func_02000d2c(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *object;

  /* FAKEMATCH(one-pass): the block loads the event work before the 0xcbc offset
   * is built; as plain statements, 2 of 76 instructions differ in tla-en. */
  do {
  } while (0);
  object = *(struct Actor **)(event + 0xcbc);

  Func_02003048();
  Func_02003148(0);
  if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
    object->terrainId = 2;
    object->motionFlags = 3;
    OverlayObject_WaitUntilIdle(object);
    Func_020031a8(188);
    OverlayObject_WaitUntilIdle(object);
    object->motionFlags = 0;
    object->terrainId = 0;
  }
  Func_02003178();
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov68fRoom6) {
    /* FAKEMATCH(temporary): the copy makes each column test reload X; as
     * object->x, 10 of 76 instructions differ in tla-en. */
    struct Actor *o = object;

    if (o->x >> 19 == 27)
      GameFlag_SetBit(0x947);
    if (o->x >> 19 == 25 && GameFlag_Test(0x947))
      GameFlag_SetBit(0x946);
  }
  Func_02003050();
}
