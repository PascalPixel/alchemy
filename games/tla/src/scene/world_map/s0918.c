#include "types.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "flag_call.h"
#include "text/msg_ids.h"

struct EventDestination {
  u8 unk000[0x154];
  s16 scene;
};

void WaitFrames(s32 frames);
void ObjectMotion_SetPositionAndReset(s32 actor, s32 x, s32 z);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 actor);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02004c20(void);
void Func_02004c28(void);
void Func_02004c88(s32 actor, s32 x, s32 z);
void Func_02004c98(s32 actor, s32 target);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 actor, s32 flags);
void Func_02004d00(s32 actor, s32 angle);
s32 Func_02004d08(s32 actor, s32 value, s32 frames);
void Func_02004d10(s32 actor, s32 value);
void Func_02004d40(s32 value);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);
void Func_02004e30(s32 cue);
struct ObjectRuntime *Object_GetById(s32 id);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200050c(void);

/// Back to the map: actor 8's position is kept in the adventure.
void Func_02000918(void) {
  struct EventDestination *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(8);

  Func_02004c20();
  Func_02004db8(0);
  if (gAdventure.renderMode == 8)
    Func_0200050c();
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  gAdventure.savedX = leader->x;
  gAdventure.savedZ = leader->z;
  gAdventure.unk280 = 0;
  gAdventure.ownerOverride = 0;
  GameFlag_ClearBit(0x176);
  Func_02004e30(123);
  Func_02004d40(work->scene);
}
