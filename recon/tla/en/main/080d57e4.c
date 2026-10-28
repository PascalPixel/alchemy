// Near miss, differ 14 of 146 (16 before; 2026-10-26, the scale reset before
// the priority flag): the leader lets go of a grabbed item
// (Func_080d5720) toward the held direction: with no direction it returns 0; it
// snaps to its cell's centre, and if the cell one step ahead is free it faces
// that way, its saved motion flags, frame and priority come back, cue 152 plays
// and it moves onto that cell, the render mode returning to 0. Returns 1 when
// it moved. No ☀️ twin found. Left: instruction order only: the native
// interleaves the vector's y and z copies with the polar offset's argument
// setup, loads the table before starting the 15 mask, and moves three
// address adds one slot. The event work halfword is a struct field: through
// a cast pointer its zero comes from the pool and is shared with the render
// mode's (differ 58).
#include "types.h"
#include "field_sprite.h"
#include "input.h"
#include "actor.h"
#include "adventure.h"
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

s32 Func_080d57e4(void) {
  struct Actor *leader = ObjectTable_Get(gAdventure.currentOwner);
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
  leader->scaleX = 0x10000;
  leader->priorityFlags |= 1;
  leader->motionFlags = leader->action;
  sprite->nextFrame = leader->unk66;
  sprite->rotation = 0;
  Sound_PlayFar(152);
  leader->speedLimit = 0x20000;
  leader->velocityY = 0x40000;
  leader->acceleration = 0x10000;
  Actor_SetMode(leader, 2);
  Actor_SetTargetPosition(leader, position[0], position[1], position[2]);
  Actor_CommitPosition(leader);
  sprite->priority = ((u8 *)leader)[98];
  gAdventure.renderMode = 0;
  return 1;
}
