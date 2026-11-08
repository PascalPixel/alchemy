/// @file
/// Func_020010b4's twin for another shot: it also stops while the render mode
/// is 5, and on a cell of kind 212 it ends with one of two end scripts by its
/// angle; otherwise, the first time it reaches the leader (within 16 pixels on
/// the same height when the leader climbs, else within 8 on the same level) and
/// flag 0x104 is clear, it carries the leader along (counter 0x16a 200, render
/// mode 0); it moves 1.5 pixels a frame along its angle, upward when its angle
/// is 0xc000.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "call.h"
#include "field/event_work.h"

extern u8 gData_020061ac[], gData_02006194[];
struct Shot {
  u8 unk00[0x62];
  u8 unk62; ///< set once it has hit the leader
  u8 unk63; ///< set with unk62, cleared when it ends
};

struct Actor *Actor_GetById(u32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_02000690(s32 layer, s32 x, s32 z);
void Func_02005f8c(struct Actor *object, s32 cue);
void Func_02005d6c(struct Actor *object, const u8 *script);
s32 Func_02000d18(s32 *a, s32 *b);
s32 Func_02000cec(s32 *a, s32 *b);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_02000d48(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  s32 near;
  s32 *leaderPosition;
  s32 *shotPosition;
  s32 speed = 0x18000;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      gMainRoutineWork.renderMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000690(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
    ((struct Shot *)shot)->unk63 = 0;
    shot->update = 0;
    Call2((void (*)())Func_02005f8c, (s32)shot, 539);
    if (shot->angle == 0xc000)
      Func_02005d6c(shot, gData_020061ac);
    else
      Func_02005d6c(shot, gData_02006194);
    return;
  }
  if (((struct Shot *)shot)->unk62 == 0) {
    near = 0;
    leaderPosition = &leader->x;
    shotPosition = &shot->x;
    if (gMainRoutineWork.renderMode == 2) {
      if (Func_02000d18(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Func_02000cec(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Test(0x104)) {
      leader->angle = shot->angle;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->animation)->priority =
          ((struct FieldSprite *)shot->animation)->priority;
      ((struct EventWork *)work)->unk16a = 200;
      gMainRoutineWork.renderMode = 0;
      ((struct Shot *)shot)->unk62 = 1;
      ((struct Shot *)shot)->unk63 = 1;
    }
  }
  if (shot->angle == 0xc000) {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->y -= Iwram_MulQ16(Math_Sine(shot->angle), speed);
  } else {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->z += Iwram_MulQ16(Math_Sine(shot->angle), speed);
  }
}
