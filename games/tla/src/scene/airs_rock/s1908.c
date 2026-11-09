#include "types.h"
#include "actor.h"
#include "field/event_work.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "event_inline.h"

extern u8 gData_02003de0[], gData_02003dcc[];
struct Shot {
  u8 unk00[0x62];
  u8 unk62; ///< set once it has hit the leader
  u8 unk63; ///< set with unk62, cleared when it ends
};

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_02000348(s32 layer, s32 x, s32 z);
void Func_02003cd0(struct Actor *object, s32 cue);
void Func_02003b38(struct Actor *object, const u8 *script);
s32 Func_020018d8(s32 *a, s32 *b);
s32 Func_020018ac(s32 *a, s32 *b);
s32 Math_Cosine(s32 angle);

void Func_02001908(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  s32 speed = 0x18000;
  s32 cue = 539;
  struct Actor *other, *launcher;
  s32 i;
  s32 *leaderPosition;
  s32 *shotPosition;
  struct Actor **objects;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      gMainRoutineWork.renderMode == 5) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000348(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
  end:
    Func_02003cd0(shot, cue);
    ((struct Shot *)shot)->unk63 = 0;
    shot->update = 0;
    if (shot->angle == 0xc000)
      Func_02003b38(shot, gData_02003de0);
    else
      Func_02003b38(shot, gData_02003dcc);
    return;
  }
  if (((struct Shot *)shot)->unk62 == 0) {
    // i first says whether the shot is beside the leader; the loop below
    // reuses it as the object index.
    i = 0;
    leaderPosition = &leader->x;
    shotPosition = &shot->x;
    if (gMainRoutineWork.renderMode == 2) {
      if (Func_020018d8(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        i = 1;
    } else {
      if (Func_020018ac(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        i = 1;
    }
    if (i && !GameFlag_Test(0x104)) {
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
  objects = (struct Actor **)(work + 0x34);
  for (i = 8; i != 64; i++) {
    other = *objects++;
    if (other->script != 0) {
      launcher = shot->watchedObject;
      if (launcher->x >> 20 != other->x >> 20 ||
          launcher->y >> 20 != other->y >> 20 ||
          launcher->z >> 20 != other->z >> 20) {
        if (Func_020018ac(&other->x, &shot->x) <= 8 &&
            other->y >> 20 == shot->y >> 20)
          goto end;
      }
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
