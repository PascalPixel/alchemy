// Draft, differ 2 of 266 (tla-en, 5 November), no device: one tie, the same as 664's Func_02000d48: the counter's address (add r2, r8) ahead of the sprite's strb in the native. Func_02000d48's twin, with another ending.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"

extern u8 gData_02003de0[], gData_02003dcc[];
struct Actor *Actor_GetById(u32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_02000348(s32 layer, s32 x, s32 z);
void Func_02003cd0(struct Actor *object, s32 cue);
void Func_02003b38(struct Actor *object, const u8 *script);
s32 Func_020018d8(s32 *a, s32 *b);
s32 Func_020018ac(s32 *a, s32 *b);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_02001908(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  s32 speed = 0x18000;
  s32 cue = 539;
  struct Actor *other, *launcher;
  u8 *hit;
  s32 near, i;
  struct Actor **objects;
  u32 angle;

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
    {
      s32 none = 0;

      ((u8 *)shot)[0x63] = none;
      shot->update = (void *)none;
    }
    if (shot->angle == 0xc000)
      Func_02003b38(shot, gData_02003de0);
    else
      Func_02003b38(shot, gData_02003dcc);
    return;
  }
  hit = (u8 *)shot + 0x62;
  if (*hit == 0) {
    {
      s32 *leaderPosition = &leader->x, *shotPosition = &shot->x;

      near = 0;
      /* FAKEMATCH(barrier): the empty asm on near changes its allocation rank; as no
       * asm, 15 of 266 instructions differ in tla-en. */
      asm("" : "=r"(near) : "0"(near));
      if (gMainRoutineWork.renderMode == 2) {
        if (Func_020018d8(leaderPosition, shotPosition) <= 16 &&
            leader->z >> 20 == shot->z >> 20)
          near = 1;
      } else {
        if (Func_020018ac(leaderPosition, shotPosition) <= 8 &&
            leader->y >> 20 == shot->y >> 20)
          near = 1;
      }
    }
    if (near && !GameFlag_Test(0x104)) {
      leader->angle = shot->angle;
      leader->watchedObject = shot;
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
