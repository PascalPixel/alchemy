#include "scene.h"
#include "object_runtime.h"

void Func_02000e78(void) {
  Func_02004040();
  Func_020041b8(0);
  Func_020021d4();
  Object_LinkObjectAndSetCallback(gPartyState.currentOwner, 21);
  Func_02004118(0x26666, 0x4ccc);
  Scene_CamBounds(0xe80000, -1, 0x1480000, 1);
  Scene_SetPositionAndCommit(21, 232, 328);
  Scene_CamBounds(0xe80000, -1, 0xc80000, 1);
  Scene_SetPositionAndCommit(21, 232, 280);
  Scene_SetPositionAndCommit(21, 232, 168);
  Scheduler_RemoveCallbackFar(Func_02002180);
  Scene_ArmCallback(21, 0x4000, 0);
  ObjectMotion_EnableActionAndResetMotion(gPartyState.currentOwner);
  FieldEvent_WaitFrames(20);
  GameFlag_SetBit(0x88b);
  Func_02004048();
}

extern u32 gData_020042b4[];
extern u32 gData_02005828[];

u32 Random16Far(void);
struct ObjectRuntime *Func_02003fc0(s32 kind, s32 x, s32 y, s32 z);
void Func_02003fb8(struct ObjectRuntime *object, const void *script);

/// Spawns an object of kind 285 at each of the fifteen cells of
/// gData_020042b4, twice, each moved by -2 to +1 pixels at random.
void Func_02000f24(void) {
  u32 pass;
  u32 i;

  for (pass = 0; pass <= 1; pass++) {
    u16 *cell;

    /* FAKEMATCH: the one-pass block keeps the table address loaded before the
     * outer loop's counter is zeroed; as no block, 2 instructions differ in
     * tla-en. */
    do {
      cell = (u16 *)gData_020042b4;
    } while (0);
    for (i = 0; i <= 14; i++) {
      s32 x = *cell++ + ((Random16Far() << 2) >> 16);
      s32 z = *cell++ + ((Random16Far() << 2) >> 16);
      struct ObjectRuntime *object =
          Func_02003fc0(285, (x - 2) << 16, 0, (z - 2) << 16);

      if (object != NULL) {
        object->speedLimit = 0x12000;
        object->acceleration = 0x8000;
        ObjectDispatch_SetSingleChildFlags((struct FieldActor *)object, 0);
        Func_02003fa8((struct FieldActor *)object, 1);
        Func_02003fb8(object, gData_02005828);
      }
    }
  }
  Func_02003ff0(74, 71, 77, 9, 3, 3);
}
