// Draft, differ 131 of 217 (was 143): Func_020010b4's twin for another shot:
// it also stops while the leader's movement is 5, and on a cell of kind 212 it
// ends with one of two end scripts by its angle; otherwise, the first time it
// reaches the leader (within 16 pixels on the same height when the leader
// climbs, else within 8 on the same level) and flag 0x104 is clear, it
// carries the leader along (counter 0x16a 200, movement 0); it moves 1.5
// pixels a frame along its angle, upward when its angle is 0xc000. The
// counter's 200 goes through an s32 (as a halfword constant it comes from the
// pool). Left: as Func_020010b4's draft, the shot and the zero swap r6 and
// r7, which moves most rows.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"

extern u8 gData_020061ac[], gData_02006194[];
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
  u8 *hit;
  s32 near;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      gMainRoutineWork.renderMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000690(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
    ((u8 *)shot)[0x63] = 0;
    shot->update = 0;
    Func_02005f8c(shot, 539);
    if (shot->angle == 0xc000)
      Func_02005d6c(shot, gData_020061ac);
    else
      Func_02005d6c(shot, gData_02006194);
    return;
  }
  hit = (u8 *)shot + 0x62;
  if (*hit == 0) {
    near = 0;
    if (gMainRoutineWork.renderMode == 2) {
      if (Func_02000d18(&leader->x, &shot->x) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Func_02000cec(&leader->x, &shot->x) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Test(0x104)) {
      leader->watchedObject = shot;
      leader->angle = shot->angle;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->animation)->priority =
          ((struct FieldSprite *)shot->animation)->priority;
      {
        s16 *counter = (s16 *)(work + 0x16a);
        s32 value = 200;

        *counter = value;
      }
      gMainRoutineWork.renderMode = 0;
      *hit = 1;
      ((u8 *)shot)[0x63] = 1;
    }
  }
  if (shot->angle == 0xc000) {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), 0x18000);
    shot->y -= Iwram_MulQ16(Math_Sine(shot->angle), 0x18000);
  } else {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), 0x18000);
    shot->z += Iwram_MulQ16(Math_Sine(shot->angle), 0x18000);
  }
}
