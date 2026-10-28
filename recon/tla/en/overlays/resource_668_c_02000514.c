// Draft, differ 77 of 198: a push in Eastern Alhafra: while object 16 stands
// unpushed (flag 0x8a4 clear) its cells are blocked; a measured push
// (Func_02004240) is carried out (Func_020044c4) at once unless flag 0x8a4
// is set and the leader faces other than left; otherwise, inside an event,
// flag 0x8a6 is set, the camera moves, the push is carried out, object 15's
// six cells are cleared and it falls (up to 60 frames) into place
// (Func_02000fa4, cue 240, mode 8) before the camera follows the leader
// again. Left: the native keeps object 16 in r10 and the address of the push state's stacked half in r8, and builds 0xc000 in r2; the falling loop and the six cell clears are close.
#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"

struct PushState {
  s32 x;
  s32 y;
  s32 z;
  s32 kind;
  s32 number;
  s32 unk14;
};

struct Actor *Actor_GetById(u32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02004c7c(s32 x, s32 z, s32 layer, s32 value);
s32 Func_02004240(struct PushState *push);
void Func_020044c4(struct PushState push);
void Func_02004a94(void);
void Func_02004a9c(void);
void Func_02004be4(s32 skip);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Actor_SetModeById(u32 id, s32 mode);
void Actor_OffsetPositionAndResetMotion(u32 id, s32 dx, s32 dz);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
void Func_02000fa4(void);
void Func_02004c9c(s32 cue);
void Camera_FollowActor(u32 id, s32 mode);

void Func_02000514(void) {
  struct Actor *gate = Actor_GetById(15);
  struct Actor *block = Actor_GetById(16);
  struct PushState push;
  u32 facing = (Actor_GetById(gSequenceParentWork.currentOwner)->angle + 0x2000) &
               0xc000;
  s32 i;

  if (block != 0 && !GameFlag_Test(0x8a4))
    Func_02004c7c(block->x, block->z, 2, 255);
  if (Func_02004240(&push)) {
    if (!GameFlag_Test(0x8a4) || facing == 0xc000) {
      Func_020044c4(push);
    } else {
      GameFlag_SetBit(0x8a6);
      Func_02004a94();
      Func_02004be4(0);
      Camera_MoveWithinMapBounds(0x1500000, -1, 0x2300000, 1);
      Func_020044c4(push);
      Func_02004c7c(gate->x - 0x180000, gate->z, 0, 0);
      Func_02004c7c(gate->x + 0x80000, gate->z, 0, 0);
      Func_02004c7c(gate->x - 0x180000, gate->z, 2, 0);
      Func_02004c7c(gate->x - 0x80000, gate->z, 2, 0);
      Func_02004c7c(gate->x + 0x80000, gate->z, 2, 0);
      Func_02004c7c(gate->x + 0x180000, gate->z, 2, 0);
      Actor_SetModeById(15, 3);
      Actor_OffsetPositionAndResetMotion(15, 0, 16);
      FieldEvent_Wait(20);
      gate->gravity = 0xccc;
      gate->motionFlags = 3;
      i = 0;
      while (gate->y > 0) {
        i++;
        wait(1);
        if (i > 59)
          break;
      }
      Func_02000fa4();
      Func_02004c9c(240);
      Actor_SetModeById(15, 8);
      FieldEvent_Wait(60);
      Camera_FollowActor(gSequenceParentWork.currentOwner, 1);
      Func_02004a9c();
    }
  }
  if (block != 0 && !GameFlag_Test(0x8a4))
    Func_02004c7c(block->x, block->z, 2, 0);
}
