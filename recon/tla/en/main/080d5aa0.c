// Under -mthumb-slow-loads (2026-10-26): differ 8 of 145 in tla-en, instruction
// order only. An empty asm before `leader->priorityFlags |= 1;` lowers it to 6.
// Near miss, differ 14 of 145: Func_080d57e4 without the scale reset, the
// motion values stored in the order the native shows (acceleration first):
// the leader lets go of a grabbed node (Func_080d59dc) toward the held
// direction, moving onto the free cell ahead with its saved motion flags,
// frame and priority restored and the render mode back to 0; returns 1 when
// it moved, 0 otherwise. Left, as in 080d57e4: instruction order only (the
// vector's y and z copies interleaved with the polar offset's arguments, the
// table loaded before the 15 mask, an address add one slot later).
#include "types.h"
#include "field_sprite.h"
#include "input.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

void Sound_PlayFar(s32 cue);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Func_080201d0(s32 layer, s32 x, s32 z);
s32 Func_08020210(struct Actor *object, s32 *position);

extern const u16 Data_080f0890[];

struct EventWorkGrab {
  u8 unk000[388];
  s16 grabState;
};

s32 Func_080d5aa0(void) {
  struct Actor *leader = ObjectTable_Get(gMainRoutineWork.currentOwner);
  struct FieldSprite *sprite = leader->animation;
  s32 position[3];
  u32 angle;

  ((struct EventWorkGrab *)Ram_HeapSlots->eventWork)->grabState = 0;
  angle = Data_080f0890[(gInput.held >> 4) & 15];
  if (angle == 0xffff)
    return 0;
  leader->x = (leader->x & 0xfff00000) + 0x80000;
  leader->z = (leader->z & 0xfff00000) + 0x80000;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x100000, angle, position);
  leader->terrainHeight = leader->y - 0x80000;
  if (Func_080201d0(leader->terrainId, position[0], position[2]) ||
      Func_08020210(leader, position))
    return 0;
  leader->angle = angle;
  leader->priorityFlags |= 1;
  leader->motionFlags = leader->action;
  sprite->nextFrame = leader->unk66;
  sprite->rotation = 0;
  Sound_PlayFar(152);
  leader->acceleration = 0x10000;
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  Actor_SetMode(leader, 2);
  Actor_SetTargetPosition(leader, position[0], position[1], position[2]);
  Actor_CommitPosition(leader);
  sprite->priority = ((u8 *)leader)[98];
  gMainRoutineWork.renderMode = 0;
  return 1;
}
