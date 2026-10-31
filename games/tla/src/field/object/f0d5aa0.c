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
  /* FAKEMATCH(barrier): an empty asm between the angle's store and the
     priority flag; as the plain statements, 2 of 145 instructions differ in
     tla-en. */
  asm("");
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
